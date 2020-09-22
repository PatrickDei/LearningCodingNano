/**
 * Copyright (c) 2014-2015 by Helpshift, Inc. All Rights Reserved.
 *
 */

#include "HelpshiftCocos2dx.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <string>


using namespace std;

static void (*alertToRateAppListener) (int result);
static void (*didReceiveNotificationCount) (int result);

static void (*helpshiftSessionBeganListener) () = NULL;
static void (*helpshiftSessionEndedListener) () = NULL;
static void (*helpshiftCampaignsSessionBeganListener) () = NULL;
static void (*helpshiftCampaignsSessionEndedListener) () = NULL;
static void (*newConversationStartedListener) (const char *message) = NULL;
static void (*userRepliedToConversationListener) (const char *message) = NULL;
static void (*userCompletedCustomerSatisfactionSurveyListener) (int rating, const char *message) = NULL;
static void (*didReceiveNotificationListener) (int newMessageCount) = NULL;
static void (*displayAttachmentListener)(const char *filePath) = NULL;
static void (*conversationEndedListener)() = NULL;
static void (*authenticationFailedListener)(HelpshiftCocosUser *user, HSAuthenticationFailureReason reason) = NULL;

jobject parseArrayOfDictionaries (JNIEnv *env, cocos2d::CCArray* data);
jobject parseCCArrayToArrayList (JNIEnv *env, cocos2d::CCArray *data) {
    if(data == NULL) {
        return NULL;
    }
    const char* arraylist_class_name = "java/util/ArrayList";
    jclass clsArrayList = env->FindClass(arraylist_class_name);
    jmethodID arrayConstructorID = env->GetMethodID (clsArrayList, "<init>", "()V");
    jobject jarrayobj = env->NewObject(clsArrayList, arrayConstructorID);
    jmethodID array_add_method = 0;
    array_add_method = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");

    int j = 0;
    for (j = 0; j < data->count(); j++) {
        cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (data->objectAtIndex(j));
        if (valStr != NULL && valStr->length() > 0) {
            jstring value = env->NewStringUTF(valStr->getCString());
            env->CallBooleanMethod(jarrayobj, array_add_method, value);
        }
    }
    return jarrayobj;
}

/*! \brief Parses the tags array
 *         from the metaData and creates a Java ArrayList
 *
 *  \param env The JNIEnv
 *  \param metaData The metaData dictionary which contains the tags array
 */
jobject parseTagsArray (JNIEnv *env, cocos2d::CCDictionary* metaData) {
    if(metaData == NULL) {
        return NULL;
    }
    return parseCCArrayToArrayList(env, dynamic_cast<cocos2d::CCArray*> (metaData->objectForKey(HS_TAGS_KEY)));
}

jobject parseCustomIssueFields(JNIEnv *env, cocos2d::CCDictionary* cifData) {
    if(cifData == NULL) {
        return NULL;
    }

    const char* hashmap_class_name = "java/util/HashMap";
    jclass clsHashMap = env->FindClass(hashmap_class_name);
    jmethodID constructorID = env->GetMethodID (clsHashMap, "<init>", "()V");
    jmethodID map_put_method = env->GetMethodID(clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    jobject cifMap = env->NewObject(clsHashMap, constructorID);
    cocos2d::CCArray *keys = cifData->allKeys();
    int i = 0;
    for(i = 0; i < keys->count(); i++) {
        cocos2d::CCString* keyStr = dynamic_cast<cocos2d::CCString*> (keys->objectAtIndex(i));
        jstring keyString = env->NewStringUTF(keyStr->getCString());
        cocos2d::CCArray *valueArray = dynamic_cast<cocos2d::CCArray*> (cifData->objectForKey(keyStr->getCString()));
        jobject valueList = parseCCArrayToArrayList(env, valueArray);
        if (valueList != NULL) {
            env->CallObjectMethod(cifMap, map_put_method, keyString, valueList);
        }
    }
    return cifMap;
}

/*! \brief Parses the config dictionary
 *         and created a Java HashMap to pass to the native layer
 *
 *  \param env The JNIEnv
 *  \param config The config dictionary which contains various config options which accepted by the SDK
 */
jobject parseConfigDictionary (JNIEnv* env, cocos2d::CCDictionary *config) {
    if(config == NULL) {
        return NULL;
    }
    const char* hashmap_class_name = "java/util/HashMap";
    jclass clsHashMap = env->FindClass(hashmap_class_name);
    jmethodID constructorID = env->GetMethodID (clsHashMap, "<init>", "()V");

    jobject jmapobj = env->NewObject(clsHashMap, constructorID);

    jmethodID map_put_method = 0;
    map_put_method = env->GetMethodID(clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    jstring key = env->NewStringUTF(HS_ENABLE_IN_APP_NOTIFICATION);
    jstring data;

    cocos2d::CCString* enableInAppStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_IN_APP_NOTIFICATION));
    if (enableInAppStr != NULL && enableInAppStr->length() > 0) {
        data = env->NewStringUTF(enableInAppStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_ENABLE_CONTACT_US);
    cocos2d::CCString* enableContactUsStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_CONTACT_US));
    if (enableContactUsStr != NULL && enableContactUsStr->length() > 0) {
        data = env->NewStringUTF(enableContactUsStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_GOTO_CONVERSATION_AFTER_CONTACT_US);
    cocos2d::CCString* gotoConvStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_GOTO_CONVERSATION_AFTER_CONTACT_US));
    if (gotoConvStr != NULL && gotoConvStr->length() > 0) {
        data = env->NewStringUTF(gotoConvStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_REQUIRE_EMAIL);
    cocos2d::CCString* requireEmailStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_REQUIRE_EMAIL));
    if (requireEmailStr != NULL && requireEmailStr->length() > 0) {
        data = env->NewStringUTF(requireEmailStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_ENABLE_TYPING_INDICATOR);
    cocos2d::CCString* enableTypingIndicatorStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_TYPING_INDICATOR));
    if (enableTypingIndicatorStr != NULL && enableTypingIndicatorStr->length() > 0) {
        data = env->NewStringUTF(enableTypingIndicatorStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_SHOW_CONVERSATION_INFO_SCREEN);
    cocos2d::CCString* showConversationInfoScreen = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_SHOW_CONVERSATION_INFO_SCREEN));
    if (showConversationInfoScreen != NULL && showConversationInfoScreen->length() > 0) {
        data = env->NewStringUTF(showConversationInfoScreen->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_HIDE_NAME_AND_EMAIL);
    cocos2d::CCString* hideNameEmailStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_HIDE_NAME_AND_EMAIL));
    if (hideNameEmailStr != NULL && hideNameEmailStr->length() > 0) {
        data = env->NewStringUTF(hideNameEmailStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_META_DATA_KEY);
    cocos2d::CCDictionary* metaData = dynamic_cast<cocos2d::CCDictionary*>(config->objectForKey(HS_META_DATA_KEY));
    if (metaData!= NULL) {
        jobject metaMap = env->NewObject(clsHashMap, constructorID);
        cocos2d::CCArray *keys = metaData->allKeys();
        int i = 0;
        for(i = 0; i < keys->count(); i++) {
            cocos2d::CCString* keyStr = dynamic_cast<cocos2d::CCString*> (keys->objectAtIndex(i));
            if (keyStr != NULL && keyStr->length() > 0) {
                jstring key = env->NewStringUTF(keyStr->getCString());
                cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (metaData->objectForKey(keyStr->getCString()));
                if(valStr != NULL && valStr->length() > 0) {
                    jstring value = env->NewStringUTF(valStr->getCString());
                    env->CallObjectMethod(metaMap, map_put_method, key, value);
                } else if (valStr == NULL) {
                    env->CallObjectMethod(metaMap, map_put_method, key, parseTagsArray(env, metaData));
                }
            }
        }
        env->CallObjectMethod(jmapobj, map_put_method, key, metaMap);
    }

    key = env->NewStringUTF(HS_CONVERSATION_PREFILL_TEXT);
    cocos2d::CCString* prefillStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_CONVERSATION_PREFILL_TEXT));
    if (prefillStr != NULL && prefillStr->length() > 0) {
        data = env->NewStringUTF(prefillStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_SHOW_SEARCH_ON_NEW_CONVERSATION);
    cocos2d::CCString* showSearchOnConversationStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_SHOW_SEARCH_ON_NEW_CONVERSATION));
    if (showSearchOnConversationStr != NULL && showSearchOnConversationStr->length() > 0) {
        data = env->NewStringUTF(showSearchOnConversationStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_ENABLE_FULL_PRIVACY);
    cocos2d::CCString* fullPrivacyStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_FULL_PRIVACY));
    if (fullPrivacyStr != NULL && fullPrivacyStr->length() > 0) {
        data = env->NewStringUTF(fullPrivacyStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_ENABLE_DEFAULT_FALL_BACK_LANGUAGE);
    cocos2d::CCString* fallBackStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_DEFAULT_FALL_BACK_LANGUAGE));
    if (fallBackStr != NULL && fallBackStr->length() > 0) {
        data = env->NewStringUTF(fallBackStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_SHOW_CONVERSATION_RESOLUTION_QUESTION);
    cocos2d::CCString* convResolutionStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_SHOW_CONVERSATION_RESOLUTION_QUESTION));
    if (convResolutionStr != NULL && convResolutionStr->length() > 0) {
        data = env->NewStringUTF(convResolutionStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_WITH_TAGS_MATCHING);
    cocos2d::CCDictionary* tagsData = dynamic_cast<cocos2d::CCDictionary*>(config->objectForKey(HS_WITH_TAGS_MATCHING));
    if (tagsData!= NULL) {
        jobject tagsMap = env->NewObject(clsHashMap, constructorID);
        cocos2d::CCArray *keys = tagsData->allKeys();
        int i = 0;
        for(i = 0; i < keys->count(); i++) {
            cocos2d::CCString* keyStr = dynamic_cast<cocos2d::CCString*> (keys->objectAtIndex(i));
            if (keyStr != NULL && keyStr->length() > 0) {
                jstring keyString = env->NewStringUTF(keyStr->getCString());
                if(strcmp(keyStr->getCString(), HS_FILTER_TAGS) == 0) {
                    env->CallObjectMethod(tagsMap,
                                          map_put_method,
                                          keyString,
                                          parseCCArrayToArrayList(env,
                                                                  dynamic_cast<cocos2d::CCArray*> (tagsData->objectForKey(keyStr->getCString()))));
                } else {
                    cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (tagsData->objectForKey(keyStr->getCString()));
                    if(valStr != NULL && valStr->length() > 0) {
                        jstring value = env->NewStringUTF(valStr->getCString());
                        env->CallObjectMethod(tagsMap, map_put_method, keyString, value);
                    }
                }
            }
        }
        env->CallObjectMethod(jmapobj, map_put_method, key, tagsMap);
    }

    key = env->NewStringUTF(HS_CUSTOM_CONTACT_US_FLOWS);
    cocos2d::CCArray *flowsData = dynamic_cast<cocos2d::CCArray*>(config->objectForKey(HS_CUSTOM_CONTACT_US_FLOWS));
    if (flowsData!= NULL) {
        env->CallObjectMethod(jmapobj, map_put_method, key, parseArrayOfDictionaries(env, flowsData));
    }

    key = env->NewStringUTF(HS_CUSTOM_ISSUE_FIELDS);
    cocos2d::CCDictionary* cifData = dynamic_cast<cocos2d::CCDictionary*>(config->objectForKey(HS_CUSTOM_ISSUE_FIELDS));
    //Check for iOS native key : "customIssueFields"
    if (cifData == NULL) {
        key = env->NewStringUTF("customIssueFields");
        cifData = dynamic_cast<cocos2d::CCDictionary*>(config->objectForKey("customIssueFields"));
    }
    if(cifData != NULL) {
        jobject cifMap = parseCustomIssueFields(env, cifData);
        env->CallObjectMethod(jmapobj, map_put_method, key, cifMap);
    }
    return jmapobj;
}

jobject parseDictionaryToHashMap (JNIEnv* env, cocos2d::CCDictionary *input) {
    if(input == NULL) {
        return NULL;
    }

    const char* hashmap_class_name = "java/util/HashMap";
    jclass clsHashMap = env->FindClass(hashmap_class_name);
    jmethodID constructorID = env->GetMethodID (clsHashMap, "<init>", "()V");

    jobject jmapobj = env->NewObject(clsHashMap, constructorID);

    jmethodID map_put_method = 0;
    map_put_method = env->GetMethodID(clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    cocos2d::CCArray *keys = input->allKeys();
    int i = 0;
    for(i = 0; i < keys->count(); i++) {
        cocos2d::CCString* keyStr = dynamic_cast<cocos2d::CCString*> (keys->objectAtIndex(i));
        if (keyStr != NULL && keyStr->length() > 0) {
            jstring key = env->NewStringUTF(keyStr->getCString());
            cocos2d::CCArray *arrayValue = dynamic_cast<cocos2d::CCArray*> (input->objectForKey(keyStr->getCString()));
            if (arrayValue != NULL) {
                env->CallObjectMethod(jmapobj,
                                      map_put_method,
                                      key,
                                      parseArrayOfDictionaries(env, arrayValue));
            } else {
                if(strcmp(keyStr->getCString(), HS_FLOW_CONFIG) == 0) {
                    env->CallObjectMethod(jmapobj,
                                          map_put_method,
                                          key,
                                          parseDictionaryToHashMap(env, dynamic_cast<cocos2d::CCDictionary *>(input->objectForKey(keyStr->getCString()))));
                } else {
                    cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (input->objectForKey(keyStr->getCString()));
                    if(valStr != NULL && valStr->length() > 0) {
                        jstring value = env->NewStringUTF(valStr->getCString());
                        env->CallObjectMethod(jmapobj, map_put_method, key, value);
                    }
                }
            }
        }
    }

    return jmapobj;
}

jobject parseArrayOfDictionaries (JNIEnv *env, cocos2d::CCArray* data) {
    if(data == NULL) {
        return NULL;
    }
    const char* arraylist_class_name = "java/util/ArrayList";
    jclass clsArrayList = env->FindClass(arraylist_class_name);
    jmethodID arrayConstructorID = env->GetMethodID (clsArrayList, "<init>", "()V");
    jobject jarrayobj = env->NewObject(clsArrayList, arrayConstructorID);
    jmethodID array_add_method = 0;
    array_add_method = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");

    int j = 0;
    for (j = 0; j < data->count(); j++) {
        jobject value = parseDictionaryToHashMap(env, (cocos2d::CCDictionary* )data->objectAtIndex(j));
        env->CallBooleanMethod(jarrayobj, array_add_method, value);
    }
    return jarrayobj;
}

/*! \brief You can use this api call to provide a way
 *         for the user to send feedback or start a new conversation with you.
 *
 */
void HelpshiftCocos2dx::showConversation(void) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showConversation",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}


/*! \brief You can use this api call to provide a way
 *         for the user to send feedback or start a new conversation with you.
 * There is one flag supported in this config -
 * gotoConversationAfterContactUs (Default : "no")
 * If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 *
 * \param config Extra config
 */
void HelpshiftCocos2dx::showConversation(cocos2d::CCDictionary *config) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showConversation",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, hashMap);
    }
}


/*! \brief Shows FAQ section activity.
 * This will show a FAQ section view with list of questions in that section. The search inside this view will be limited to the specified section. You can specify a section using publish ID of that section.
 *
 * \param sectionPublishId id specifying a section
 */
void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId) {
    if(sectionPublishId == NULL || strlen(sectionPublishId) == 0) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQSection",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring sectionPubId = minfo.env->NewStringUTF(sectionPublishId);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        sectionPubId);
    }
}


/*! \brief Shows FAQ section activity.
 * This will show a FAQ section view with list of questions in that section. The search inside this view will be limited to the specified section. You can specify a section using publish ID of that section.
 *
 * There are two flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 * requireEmail (Default : "no" ) If requireEmail flag is set to "yes", an e-mail address is required while starting a new conversation.
 * \param sectionPublishId id specifying a section
 * \param config Additional config
 */
void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId, cocos2d::CCDictionary *config) {
    if(sectionPublishId == NULL || strlen(sectionPublishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQSection",
                                                             "(Ljava/lang/String;Ljava/util/HashMap;)V");
    if (hasMethod) {
        jstring sectionPubId = minfo.env->NewStringUTF(sectionPublishId);
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        sectionPubId,
                                        hashMap);
    }
}


/*! \brief Shows question activity.
 *         This provides show question view provided a publish id of that question.
 *
 * \param publishId id specifying a question
 *
 */
void HelpshiftCocos2dx::showSingleFAQ(const char *publishId) {
    if(publishId == NULL || strlen(publishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showSingleFAQ",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring pubId = minfo.env->NewStringUTF(publishId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, pubId);
    }
}


/*! \brief Shows question activity.
 *         This provides show question view provided a publish id of that question.
 *
 * There are two flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 * requireEmail (Default : "no" ) If requireEmail flag is set to "yes", an e-mail address is required while starting a new conversation.
 *
 * \param publishId id specifying a question
 * \param config Additional config
 */
void HelpshiftCocos2dx::showSingleFAQ(const char *publishId, cocos2d::CCDictionary *config) {
    if(publishId == NULL || strlen(publishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showSingleFAQ",
                                                             "(Ljava/lang/String;Ljava/util/HashMap;)V");
    if (hasMethod) {
        jstring pubId = minfo.env->NewStringUTF(publishId);
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        pubId,
                                        hashMap);
    }
}


/*! \brief Shows faqs activity. This will show list of sections with search.
 *
 */
void HelpshiftCocos2dx::showFAQs() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQs",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

/*! \brief Shows Inbox activity. This will show list of campaigns.
 *
 */
void HelpshiftCocos2dx::showInbox() {
        cocos2d::JniMethodInfo minfo;
        bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                                 "com/helpshift/HelpshiftBridge",
                                                                 "showInbox",
                                                                 "()V");
        if (hasMethod) {
                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
        }
}

//Internal method to Gets count of unread campaign messages.
static int getCountOfUnreadMessagesInternal() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "getCountOfUnreadMessages",
                                                             "()I");
    if(hasMethod) {
        return minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
    }
    return 0;
}

/*! \brief Gets count of unread campaign messages.
 *
 * \return The count of unread campaign messages in the user's inbox.
 *
 */
int HelpshiftCocos2dx::getCountOfUnreadMessages() {
   return getCountOfUnreadMessagesInternal();
}

void HelpshiftCocos2dx::requestUnreadCampaignsMessagesCount(void (*unreadCampaignsMessagesCountHandler)(int count)) {
    if (unreadCampaignsMessagesCountHandler != NULL) {
        unreadCampaignsMessagesCountHandler(getCountOfUnreadMessagesInternal());
    }
}

/*! \brief Shows faqs activity. This will show list of sections with search.
 *
 * There are two flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 * requireEmail (Default : "no" ) If requireEmail flag is set to "yes", an e-mail address is required while starting a new conversation.
 *
 * \param config Additional config
 */
void HelpshiftCocos2dx::showFAQs(cocos2d::CCDictionary *config) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQs",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, hashMap);
    }
}


void HelpshiftCocos2dx::showDynamicForm(cocos2d::CCArray *data) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showDynamicForm",
                                                             "(Ljava/util/List;)V");
    if (hasMethod) {
        jobject listOfHashMaps = parseArrayOfDictionaries(minfo.env, data);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, listOfHashMaps);
    }
}

void HelpshiftCocos2dx::showDynamicForm(const char *title, cocos2d::CCArray *data) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showDynamicForm",
                                                             "(Ljava/lang/String;Ljava/util/List;)V");
    if (hasMethod) {
        jstring titleStr = minfo.env->NewStringUTF(title);
        jobject listOfHashMaps = parseArrayOfDictionaries(minfo.env, data);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, titleStr, listOfHashMaps);
    }
}

/*! \brief API to login a user to the Help section.
 * \param identifier The unique id for the login
 * \param name User name
 * \param email User email
 */
void HelpshiftCocos2dx::login(const char *identifier, const char *name, const char *email) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "login",
                                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(identifier);
        jstring nameStr = minfo.env->NewStringUTF(name);
        jstring emailStr = minfo.env->NewStringUTF(email);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        idStr,
                                        nameStr,
                                        emailStr);
    }
}

void HelpshiftCocos2dx::login(HelpshiftCocosUser *user)
{
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "login",
                                                             "(Lcom/helpshift/HelpshiftUser;)V");

    if(hasMethod)
    {
      minfo.env->CallStaticVoidMethod(minfo.classID,
                                      minfo.methodID,
                                      user->userObject);
    }

}

/*! \brief API to logout the current user
 */
void HelpshiftCocos2dx::logout() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "logout",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
    }
}

/*! \brief API to clear anonymous user
 * NOTE: anonymous user can only be reset when an user is logged in.
 */
void HelpshiftCocos2dx::clearAnonymousUser() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "clearAnonymousUser",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
    }
}

/*! \brief You can specify the name and email for your User.
 * If you want to reset previously set values, please provide NULL for both params
 * \param name User name
 * \param email User email
 */
void HelpshiftCocos2dx::setNameAndEmail(const char *name, const char *email) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "setNameAndEmail",
                                                             "(Ljava/lang/String;Ljava/lang/String;)V");
    if(hasMethod) {
        jstring nameStr = minfo.env->NewStringUTF(name);
        jstring emailStr = minfo.env->NewStringUTF(email);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        nameStr,
                                        emailStr);
    }
}


/*! \brief If you already have indentification for your users, you can specify that as well.
 *
 * \param userIdentifier A custom user identifier
 *
 */
void HelpshiftCocos2dx::setUserIdentifier(const char *userIdentifier) {
    if(userIdentifier == NULL || strlen(userIdentifier) == 0) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "setUserIdentifier",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(userIdentifier);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, idStr);
    }
}


/*! \brief If you are using GCM push or Urban Airship and if you want to enable Push Notification in the Helpshift Android SDK, set the Android Push ID (APID) using this method
 *
 * \param deviceToken The Android Push Id
 */
void HelpshiftCocos2dx::registerDeviceToken(const char *deviceToken) {
    if(deviceToken == NULL || strlen(deviceToken) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "registerDeviceToken",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring devTokenStr = minfo.env->NewStringUTF(deviceToken);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        devTokenStr);
    }
}


/*! \brief Add additonal debugging information in your code.
 * You can add additional debugging statements to your code,
 * and see exactly what the user was doing right before they started a new conversation.
 *
 * \param breadCrumb Action/Message to add to bread-crumbs list.
 */
void HelpshiftCocos2dx::leaveBreadCrumb(const char *breadCrumb) {
    if(breadCrumb == NULL || strlen(breadCrumb) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "leaveBreadCrumb",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring breadCrumbStr = minfo.env->NewStringUTF(breadCrumb);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        breadCrumbStr);
    }
}


/*! \brief Show an alert to let the user rate your app
 *
 * \param url The url of your app on the play store
 * \param action A callback function which takes a HSAlertToRateAppAction param
 * and returns void. This callback function will be called to let the developer
 * know what the user's response was to the Alert.
 */
void HelpshiftCocos2dx::showAlertToRateApp(const char *url,
                                           void (*action) (int result)) {
    if(url == NULL || strlen(url) == 0) {
        return;
    }
    if (action) {
        alertToRateAppListener = action;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showAlertToRateApp",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(url);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, idStr);
    }
}


/*! \brief Clears Breadcrumbs list. Breadcrumbs list stores upto 100 latest actions. You'll receive those in every Issue. But if for reason you want to clear previous messages (On app load, for eg), you can do that by calling this api.
 *
 */
void HelpshiftCocos2dx::clearBreadCrumbs() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "clearBreadCrumbs",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}


/*! \brief Gets notification count
 *
 * \return The count of new notifications for updates to the conversation
 *
 */
int HelpshiftCocos2dx::getNotificationCount(bool isAsync, void (*receiver) (int result)) {
    cocos2d::JniMethodInfo minfo;
    if(receiver) {
        didReceiveNotificationCount = receiver;
    }

    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "getNotificationCount",
                                                             "(Z)I");
    if(hasMethod) {
        return minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID, isAsync);
    }
}

//Internal method to check isConversationActive
static bool isConversationActiveInternal() {
    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "isConversationActive",
                                                             "()Z");
    if(hasMethod) {
        retVal = minfo.env->CallStaticBooleanMethod(minfo.classID,
                                                minfo.methodID);
    }
    return retVal;
}

bool HelpshiftCocos2dx::isConversationActive() {
    return isConversationActiveInternal();
}

void HelpshiftCocos2dx::checkIfConversationActive(void (*checkIfConversationActiveHandler)(bool isActive)) {
    if (checkIfConversationActiveHandler != NULL) {
        checkIfConversationActiveHandler(isConversationActiveInternal());
    }
}

/*! \brief If you receive a push notification from the Helpshift server, the "origin" field of the notification will be set to "helpshift". In such a case, you can forward the notification to Helpshift so that the relevant issue chat screen is opened. To forward the push notification to the Helpshift sdk, please use this api.
 *
 * \param issueId The "issue_id" field of the received notification dictionary
 */
void HelpshiftCocos2dx::handlePush(cocos2d::CCDictionary *notification) {
    if(notification == NULL) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "handlePush",
                                                             "(Ljava/util/HashMap;)V");
    if(hasMethod) {
        jobject notificationMap = parseDictionaryToHashMap(minfo.env, notification);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        notificationMap);
    }
}

void HelpshiftCocos2dx::registerSessionDelegates (void (*sessionBeganListener)(),
                                                  void (*sessionEndedListener)()) {
    if (sessionBeganListener != NULL) {
        helpshiftSessionBeganListener = sessionBeganListener;
    }
    if (sessionEndedListener != NULL) {
        helpshiftSessionEndedListener = sessionEndedListener;
    }
}

void HelpshiftCocos2dx::registerCampaignsSessionDelegates (void (*campaignsSessionBeganListener) (),
                                                           void (*campaignsSessionEndedListener) ()) {
  if (campaignsSessionBeganListener != NULL) {
    helpshiftCampaignsSessionBeganListener = campaignsSessionBeganListener;
  }
  if (campaignsSessionEndedListener != NULL) {
    helpshiftCampaignsSessionEndedListener = campaignsSessionEndedListener;
  }
}

void HelpshiftCocos2dx::registerConversationDelegates (void (*newConversationStartedListenerArg)(const char *message),
                                                       void (*userRepliedToConversationListenerArg)(const char *message),
                                                       void (*userCompletedCustomerSatisfactionSurveyListenerArg)(int rating, const char *feedback),
                                                       void (*didReceiveNotificationListenerArg)(int newMessageCount),
                                                       void (*conversationEndedListenerArg)()) {
    if(newConversationStartedListenerArg != NULL) {
        newConversationStartedListener = newConversationStartedListenerArg;
    }

    if(userRepliedToConversationListenerArg != NULL) {
        userRepliedToConversationListener = userRepliedToConversationListenerArg;
    }

    if(userCompletedCustomerSatisfactionSurveyListenerArg != NULL) {
        userCompletedCustomerSatisfactionSurveyListener = userCompletedCustomerSatisfactionSurveyListenerArg;
    }

    if(didReceiveNotificationListenerArg != NULL) {
        didReceiveNotificationListener = didReceiveNotificationListenerArg;
    }

    if (conversationEndedListenerArg != NULL) {
        conversationEndedListener = conversationEndedListenerArg;
    }
}

void HelpshiftCocos2dx::registerDisplayAttachmentDelegate (void (*displayAttachmentListenerArg)(const char *filePath)) {
    if(displayAttachmentListenerArg != NULL) {
        displayAttachmentListener = displayAttachmentListenerArg;
    }
}

void HelpshiftCocos2dx::registerAuthenticationFailureDelegate (void (*authenticationFailedListenerArg)(HelpshiftCocosUser *user, HSAuthenticationFailureReason reason))
{
    if(authenticationFailedListenerArg != NULL) {
        authenticationFailedListener = authenticationFailedListenerArg;
    }
}


//Internal method to update SDK language
static void setLanguageInternal(const char *locale) {
    if(locale == NULL || strlen(locale) == 0) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "setSDKLanguage",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(locale);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, idStr);
    }
}

void HelpshiftCocos2dx::setSDKLanguage(const char *locale) {
    setLanguageInternal(locale);
}

void HelpshiftCocos2dx::setLanguage(const char *locale) {
    setLanguageInternal(locale);
}

bool HelpshiftCocos2dx::addStringProperty(const char *key, const char *value) {
    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "addProperty",
                                                             "(Ljava/lang/String;Ljava/lang/String;)Z");
    if(hasMethod) {
        jstring keyStr = minfo.env->NewStringUTF(key);
        jstring valueStr = minfo.env->NewStringUTF(value);
        retVal = minfo.env->CallStaticBooleanMethod(minfo.classID,
                                                minfo.methodID,
                                                keyStr,
                                                valueStr);
    }
    return retVal;
}

bool HelpshiftCocos2dx::addIntegerProperty(const char *key, int value) {
    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "addProperty",
                                                             "(Ljava/lang/String;I)Z");
    if(hasMethod) {
        jstring keyStr = minfo.env->NewStringUTF(key);
        retVal = minfo.env->CallStaticBooleanMethod(minfo.classID,
                                                minfo.methodID,
                                                keyStr,
                                                value);
    }
    return retVal;
}

bool HelpshiftCocos2dx::addBooleanProperty(const char *key, bool value) {
    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "addProperty",
                                                             "(Ljava/lang/String;Z)Z");
    if(hasMethod) {
        jstring keyStr = minfo.env->NewStringUTF(key);
        retVal = minfo.env->CallStaticBooleanMethod(minfo.classID,
                                                minfo.methodID,
                                                keyStr,
                                                value);
    }
    return retVal;
}

bool HelpshiftCocos2dx::addDateProperty(const char *key, double value) {
    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "addProperty",
                                                             "(Ljava/lang/String;D)Z");
    if(hasMethod) {
        jstring keyStr = minfo.env->NewStringUTF(key);
        retVal = minfo.env->CallStaticBooleanMethod(minfo.classID,
                                                minfo.methodID,
                                                keyStr,
                                                value);
    }
    return retVal;
}

bool HelpshiftCocos2dx::addLongLongProperty(const char *key, long long value) {
    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "addProperty",
                                                             "(Ljava/lang/String;J)Z");
    if(hasMethod) {
        jstring keyStr = minfo.env->NewStringUTF(key);
        retVal = minfo.env->CallStaticBooleanMethod(minfo.classID,
                                                minfo.methodID,
                                                keyStr,
                                                value);
    }
    return retVal;
}

void HelpshiftCocos2dx::addProperties(cocos2d::CCDictionary *properties) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "addProperties",
                                                             "(Ljava/util/HashMap;)V");
    if(hasMethod) {
        jobject propertiesObject = parseDictionaryToHashMap(minfo.env, properties);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        propertiesObject);
    }
}


/*! \brief Calls the Helpshift log class with provided arguments
 *
 * \param logFunction corresponds to the log level
 * \param tagString the debug tag to use
 * \param logString the log message
 */
int logger (const char* logFunction, const char *tagString, const char* logString) {
    if(tagString == NULL || logString == NULL ||
       strlen(logString) == 0) {
        return -1;
    }

    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/support/Log",
                                                             logFunction,
                                                             "(Ljava/lang/String;Ljava/lang/String;)I");
    if(hasMethod) {
        jstring logStr = minfo.env->NewStringUTF(logString);
        jstring tagStr = minfo.env->NewStringUTF(tagString);
        retVal = minfo.env->CallStaticIntMethod(minfo.classID,
                                                minfo.methodID,
                                                tagStr,
                                                logStr);
    }
    return retVal;
}


/*! \brief Adds logs with debug level
 */
int HelpshiftCocos2dx::logd(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("d", tag, logString);
}


/*! \brief Adds logs with info level
 */
int HelpshiftCocos2dx::logi(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("i", tag, logString);
}

/*! \brief Adds logs with warn level
 */
int HelpshiftCocos2dx::logw(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("w", tag, logString);
}

/*! \brief Adds logs with verbose level
 */
int HelpshiftCocos2dx::logv(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("v", tag, logString);
}


extern "C" {
    void Java_com_helpshift_HelpshiftBridge_alertToRateAppAction (JNIEnv *env,
                                                                  jobject object,
                                                                  int message) {
        if(alertToRateAppListener) {
            alertToRateAppListener(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_didReceiveNotificationCount (JNIEnv *env,
                                                                         jobject object,
                                                                         int message) {
        if(didReceiveNotificationCount) {
            didReceiveNotificationCount(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_helpshiftSessionBegan (JNIEnv *env,
                                                                   jobject object) {
        if(helpshiftSessionBeganListener) {
            helpshiftSessionBeganListener();
        }
    }

    void Java_com_helpshift_HelpshiftBridge_helpshiftSessionEnded (JNIEnv *env,
                                                                   jobject object) {
        if(helpshiftSessionEndedListener) {
            helpshiftSessionEndedListener();
        }
    }

    void Java_com_helpshift_HelpshiftBridge_helpshiftCampaignsSessionBegan (JNIEnv *env,
                                                                            jobject object) {
        if(helpshiftCampaignsSessionBeganListener) {
            helpshiftCampaignsSessionBeganListener();
        }
    }

    void Java_com_helpshift_HelpshiftBridge_helpshiftCampaignsSessionEnded (JNIEnv *env,
                                                                            jobject object) {
        if(helpshiftCampaignsSessionEndedListener) {
            helpshiftCampaignsSessionEndedListener();
        }
    }


    void Java_com_helpshift_HelpshiftBridge_newConversationStarted (JNIEnv *env,
                                                                    jobject object,
                                                                    jstring messageString) {
        const char *message  = env->GetStringUTFChars(messageString, NULL);
        if(newConversationStartedListener) {
            newConversationStartedListener(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_userRepliedToConversation (JNIEnv *env,
                                                                       jobject object,
                                                                       jstring messageString) {
        const char *message  = env->GetStringUTFChars(messageString, NULL);
        if(userRepliedToConversationListener) {
            userRepliedToConversationListener(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_userCompletedCustomerSatisfactionSurvey (JNIEnv *env,
                                                                                     jobject object,
                                                                                     int rating,
                                                                                     jstring feedbackString) {
        const char *feedback = env->GetStringUTFChars(feedbackString, NULL);
        if(userCompletedCustomerSatisfactionSurveyListener) {
            userCompletedCustomerSatisfactionSurveyListener(rating, feedback);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_didReceiveNotification(JNIEnv *env,
                                                                   jobject object,
                                                                   int newMessageCount) {
        if(didReceiveNotificationListener) {
            didReceiveNotificationListener(newMessageCount);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_displayAttachmentFile(JNIEnv *env,
                                                                  jobject object,
                                                                  jstring filePath) {
        const char *path = env->GetStringUTFChars(filePath, NULL);
        if(displayAttachmentListener) {
            displayAttachmentListener(path);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_conversationEnded (JNIEnv *env,
                                                               jobject object) {
        if(conversationEndedListener) {
            conversationEndedListener();
        }
    }

    void Java_com_helpshift_HelpshiftBridge_authenticationFailed (JNIEnv *env,
                                                                  jobject thisObject,
                                                                  jobject userObject,
                                                                  int reason)
    {
        if(authenticationFailedListener)
        {
            HelpshiftCocosUser *user = new HelpshiftCocosUser(userObject);

            if(reason == 0)
            {
                authenticationFailedListener(user, HSAuthTokenNotProvided);
            }
            else if (reason == 1)
            {
                authenticationFailedListener(user, HSInvalidAuthToken);
            }
        }
    }

    jstring Java_com_helpshift_HelpshiftBridge_getRuntimeVersion(JNIEnv *env,
                                                                 jobject object) {
        return env->NewStringUTF(cocos2d::cocos2dVersion());
    }
}
