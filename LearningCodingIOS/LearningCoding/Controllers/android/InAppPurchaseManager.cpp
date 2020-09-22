
//
//  InAppPurchaseManager.cpp
//  NanoGirl
//
//  Created by Matko Juribasic on 12/12/13.
//
//

#include "InAppPurchaseManager.h"
#include "AnalyticsController.h"
#include "CCJSONConverter.h"
#include "CCNative.h"
#include "CCNotificationCenter.h"
#include "CCUserDefault.h"
#include "Constants.h"
#include "GameProfile.h"
#include "GameState.h"
#include "HudLayer.h"
#include "InAppProfile.h"
#include "IosFunctions.h"
#include "NGSDataManager.h"
#include "NGSManager.h"
#include "NGSNode.h"
#include "NGSSubscription.h"
#include "NGSTime.h"
#include "NanoExtensions.h"
#include "OfferController.h"
#include "Parameters.h"
#include "SaveGameController.h"
#include "SoundManager.h"
#include "TicketController.h"
#include "UserPurchases.h"

#include "Jni.h"
#include "JniHelper.h"

using namespace cocos2d::extension;
using namespace cocos2d;

#define CLASS_NAME "org/nanobit/mystory/StoryGame"

#define ZOMBIE "zombie"
#define SPENDER "spender"
#define MINGLER "mingler"
#define DOLPHIN "dolphin"
#define WHALE "whale"

static bool getJNIStaticMethodInfo (cocos2d::JniMethodInfo &methodinfo, const char *methodName, const char *paramCode)
{
    return cocos2d::JniHelper::getStaticMethodInfo (methodinfo, CLASS_NAME, methodName, paramCode);
}

extern "C" {
// purchaseSetupFailed
JNIEXPORT void JNICALL Java_org_nanobit_mystory_purchase_InappPurchaseManager_purchaseSetupFailed (JNIEnv *env,
                                                                                                   jobject thiz,
                                                                                                   jint jStatus,
                                                                                                   jstring jMessage)
{
    std::string message = JniHelper::jstring2string (jMessage);
    env->DeleteLocalRef (jMessage);
}

// subscriptionReceived
JNIEXPORT void JNICALL Java_org_nanobit_mystory_purchase_InappPurchaseManager_subscriptionReceived (
    JNIEnv *env, jobject thiz, jint jStatus, jstring jProductId, jstring jMessage, jstring jTransactionId,
    jlong purchaseTimestamp, jboolean shouldReport)
{
    std::string productId = JniHelper::jstring2string (jProductId);
    std::string message = JniHelper::jstring2string (jMessage);
    std::string transactionId = JniHelper::jstring2string (jTransactionId);
    CCLOG ("IAP: Subscription received (%s): %s (%d)", productId.c_str (), message.c_str (), jStatus);
    // todo - pass onto purchaseCompleted call?
    // no, new different function, using timestamp
    if (InAppPurchaseManager::get ()) {
        InAppPurchaseManager::get ()->sendSubscriptionInit (InAppPurchaseManager::Status (jStatus), productId.c_str (),
                                                            transactionId.c_str (), purchaseTimestamp, shouldReport);
    }
    env->DeleteLocalRef (jProductId);
    env->DeleteLocalRef (jMessage);
    env->DeleteLocalRef (jTransactionId);
}

// purchaseCompleted
JNIEXPORT void JNICALL Java_org_nanobit_mystory_purchase_InappPurchaseManager_purchaseCompleted (
    JNIEnv *env, jobject thiz, jint jStatus, jstring jProductId, jstring jMessage, jstring jTransactionId)
{
    std::string productId = JniHelper::jstring2string (jProductId);
    std::string message = JniHelper::jstring2string (jMessage);
    std::string transactionId = JniHelper::jstring2string (jTransactionId);
    CCLOG ("IAP: InAppPurchase completed (%s): %s (%d)", productId.c_str (), message.c_str (), jStatus);
    if (InAppPurchaseManager::get ()) {
        InAppPurchaseManager::get()->purchaseCompleted(InAppPurchaseManager::Status(jStatus),
                                                       productId.c_str(),
                                                       transactionId.c_str());
    }
    env->DeleteLocalRef (jProductId);
    env->DeleteLocalRef (jMessage);
    env->DeleteLocalRef (jTransactionId);
}

// purchaseFailed
JNIEXPORT void JNICALL Java_org_nanobit_mystory_purchase_InappPurchaseManager_purchaseFailed (JNIEnv *env, jobject thiz,
                                                                                              jint jStatus,
                                                                                              jstring jProductId,
                                                                                              jstring jMessage)
{
    std::string productId = JniHelper::jstring2string (jProductId);
    std::string message = JniHelper::jstring2string (jMessage);
    CCLOG ("IAP: InAppPurchase failed (%s): %s (%d)", productId.c_str (), message.c_str (), jStatus);
    if (InAppPurchaseManager::get ()) {
        InAppPurchaseManager::get()->purchaseCompleted(InAppPurchaseManager::Status(jStatus),
                                                       productId.c_str());
    }
    env->DeleteLocalRef (jProductId);
    env->DeleteLocalRef (jMessage);
}

// skuDataRecieved
JNIEXPORT void JNICALL Java_org_nanobit_mystory_purchase_InappPurchaseManager_skuDataRecieved (JNIEnv *env,
                                                                                               jobject thiz,
                                                                                               jstring jJSONData)
{
    std::string jsonData = JniHelper::jstring2string (jJSONData);
    CCLOG ("IAP: Recieved product data: %s", jsonData.c_str ());

    CCDictionary *productData = CCJSONConverter::sharedConverter()->dictionaryFrom(jsonData.c_str());
    CCArray *products = productData ? dynamic_cast<CCArray *>(productData->objectForKey("products")) : nullptr;

    if (products && InAppPurchaseManager::get ()) {
        //                auto& allInApps = GameProfile::get()->getAllInApps();
        //                CCARRAY_FOREACH_TYPE(allInapps, InAppProfile, profile)
        //                {
        //                    profile->setStoreValid(false);
        //                }

        InAppPurchaseManager *manager = InAppPurchaseManager::get ();
        CCARRAY_FOREACH_TYPE (products, CCDictionary, productDict)
        {
            CCString *productId = static_cast<CCString *> (productDict->objectForKey ("productId"));
            CCString *price = static_cast<CCString *> (productDict->objectForKey ("price"));
            CCString *title = static_cast<CCString *> (productDict->objectForKey ("title"));
            CCString *currency = static_cast<CCString *> (productDict->objectForKey ("price_currency_code"));
            manager->updateProductData (productId->getCString (), price->getCString (), title->getCString ());

            if (currency != nullptr && currency->length () > 0) {
                manager->setCurrencyCode (currency->m_sString);
            }
        }

        manager->productsReceived (true);
        env->DeleteLocalRef (jJSONData);
    } else {
        CCLOG ("IAP: Missing product data array!");
    }
}

// skuQuerryFailed
JNIEXPORT void JNICALL Java_org_nanobit_mystory_purchase_InappPurchaseManager_skuQuerryFailed (JNIEnv *env,
                                                                                               jobject thiz,
                                                                                               jint jStatus,
                                                                                               jstring jMessage)
{
    std::string message = JniHelper::jstring2string (jMessage);
    env->DeleteLocalRef (jMessage);
}

// productRevenue
// if its a subscription, returns revenue
JNIEXPORT jdouble JNICALL Java_org_nanobit_mystory_purchase_InappPurchaseManager_productRevenue (JNIEnv *env,
                                                                                                 jobject thiz,
                                                                                                 jstring jProductId)
{
    double retVal = -1;
    std::string productId = JniHelper::jstring2string (jProductId);

    if (GameProfile::get ()) {
        auto profile = GameProfile::get ()->getSubscriptionsWithProductId (productId.c_str ());
        if (profile) {
            retVal = profile->getPriceInDollars ();
            retVal *= 0.7f;
        }
    }

    env->DeleteLocalRef (jProductId);
    return retVal;
}
}

#pragma mark - Public

void InAppPurchaseManager::initInAppPurchaseManager () {}

InAppPurchaseManager::~InAppPurchaseManager ()
{
    CCNotificationCenter::sharedNotificationCenter ()->removeAllObservers (this);
}

void InAppPurchaseManager::requestProductData ()
{
    // Creating a vector of all product IDs
    std::vector<std::string> productIds;

    auto &allInApps = GameProfile::get ()->getAllInApps ();
    for (auto &p : allInApps) {
        InAppProfile *inProfile = static_cast<InAppProfile *> (p);
        productIds.push_back (inProfile->getProductId ()->getCString ());
    }
    // tickets are inapps
    auto &allTickets = GameProfile::get ()->getAllTickets ();
    for (auto &p : allTickets) {
        InAppProfile *inProfile = static_cast<InAppProfile *> (p);
        productIds.push_back (inProfile->getProductId ()->getCString ());
    }

    // subscriptions are inapps
    auto &allBasicSubscriptions = GameProfile::get ()->getAllBasicSubscriptions ();
    for (auto &p : allBasicSubscriptions) {
        InAppProfile *inProfile = static_cast<InAppProfile *> (p);
        productIds.push_back (inProfile->getProductId ()->getCString ());
    }

    std::string products = "";
    bool ignoreSpace = true;
    for (std::string part : productIds) {
        if (ignoreSpace) {
            ignoreSpace = false;
        } else {
            products += " ";
        }
        products += part;
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/purchase/InappPurchaseManager",
                                        "sQuerryInventoryAndSKUs", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring productsArg = t.env->NewStringUTF (products.c_str ());
        jstring username = t.env->NewStringUTF (NGSDataManager::getUsername ().c_str ());
        t.env->CallStaticVoidMethod (t.classID, t.methodID, productsArg, username);

        t.env->DeleteLocalRef (productsArg);
        t.env->DeleteLocalRef (username);
        t.env->DeleteLocalRef (t.classID);
    }
}

void InAppPurchaseManager::purchase (const char *productId)
{
    // check if it's a subscription or regular inapp
    auto profile = GameProfile::get ()->getSubscriptionsWithProductId (productId);
    bool isSub = profile != nullptr;

    OfferController::get()->purchaseStarted(productId);

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/purchase/InappPurchaseManager", "sInitiatePurchase",
                                        "(Ljava/lang/String;Ljava/lang/String;Z)V")) {
        jstring productArg = t.env->NewStringUTF (productId);
        jstring username = t.env->NewStringUTF (NGSDataManager::getUsername ().c_str ());
        jboolean jIsSub = isSub;
        t.env->CallStaticVoidMethod (t.classID, t.methodID, productArg, username, jIsSub);

        t.env->DeleteLocalRef (productArg);
        t.env->DeleteLocalRef (username);
        t.env->DeleteLocalRef (t.classID);
    }
}

void InAppPurchaseManager::processPendingTransactions () {}

void InAppPurchaseManager::restorePurchases () {}

std::string InAppPurchaseManager::getCountryCode ()
{
    std::string countryCode;
    if (Parameters::get ()) {
        CCDictionary *params = Parameters::getParameters ();
        CCDictionary *currencyToCountryDict = params->dictForKey ("currencyToCountry");
        if (currencyToCountryDict) {
            countryCode = currencyToCountryDict->valueForKey (currencyCode)->m_sString;
        }
    }

    return countryCode.empty () ? IosFunctions::getLocaleCountryCode () : countryCode;
}

InAppPurchaseManager::UserCategory InAppPurchaseManager::getUserCategory(InAppType inAppType)
{
    double moneySpent = 0;
    if (GameState::get() != nullptr) {
        switch (inAppType) {
            case InAppType::None:
                moneySpent = GameState::get()->getUserPurchases()->getTotalMoneySpent();
                break;

            case InAppType::GemsPackage:
                moneySpent = GameState::get()->getUserPurchases()->getTotalMoneySpentGems();
                break;

            case InAppType::TicketsPackage:
                moneySpent = GameState::get()->getUserPurchases()->getTotalMoneySpentTickets();
                break;

            default:
                break;
        }
    }

    if (moneySpent > 0 && moneySpent < 10)
        return kUserCategoryMingler;
    else if (moneySpent >= 10 && moneySpent < 30)
        return kUserCategorySpender;
    else if (moneySpent >= 30 && moneySpent < 50)
        return kUserCategoryDolphin;
    else if (moneySpent >= 50)
        return kUserCategoryWhale;

    return kUserCategoryZombie;
}

InAppPurchaseManager::UserCategory InAppPurchaseManager::userCategoryForString (std::string category)
{
    UserCategory userCategory = kUserCategoryZombie;
    if (category == SPENDER) {
        userCategory = kUserCategorySpender;
    } else if (category == MINGLER) {
        userCategory = kUserCategoryMingler;
    } else if (category == WHALE) {
        userCategory = kUserCategoryWhale;
    } else if (category == DOLPHIN) {
        userCategory = kUserCategoryDolphin;
    }
    return userCategory;
}

std::set<int> InAppPurchaseManager::getUserCategories(CCArray *userCategories)
{
    std::set<int> categories;
    CCARRAY_FOREACH_TYPE(userCategories, cocos2d::CCString, userCategory)
                {
                    int category = InAppPurchaseManager::userCategoryForString(userCategory->getCString());
                    categories.insert(category);
                }
    return categories;
}

int InAppPurchaseManager::getRecommendedInAppId ()
{
    /*    InAppPurchaseManager::UserCategory category = InAppPurchaseManager::getUserCategory();

        switch (category) {
            case kUserCategoryZombie:
                if (sale) return 94006;
                else return 94000;
                break;
            case kUserCategoryMingler:
                if (sale) return 94007;
                else return 94001;
                break;
            case kUserCategorySpender:
                if (sale) return 94008;
                else return 94002;
                break;
            case kUserCategoryDolphin:
                if (sale) return 94009;
                else return 94003;
                break;
            case kUserCategoryWhale:
                if (sale) return 94010;
                else return 94004;
                break;
        }*/
    return 94000;
}

#pragma mark - InAppPurchaseManager_objc

void InAppPurchaseManager::updateProductData (const char *productId, const char *price, const char *title)
{
    if (!productId || !price)
        return;

    InAppProfile *inProfile = GameProfile::get ()->getGemsInAppWithProductId (productId);

    if (inProfile == nullptr) {
        inProfile = GameProfile::get ()->getTicketsWithProductId (productId);
    }
    if (inProfile == nullptr) {
        inProfile = GameProfile::get ()->getSubscriptionsWithProductId (productId);
    }

    if (inProfile) {
        CCLOG ("IAP: Valid product %s with price %s", productId, price);

        inProfile->setPrice (price);
    }
}

void InAppPurchaseManager::productsReceived (bool success)
{
    inappsReceived = success;

    CCNotificationCenter::sharedNotificationCenter ()->postNotification (
        kNotificationInAppPurchaseManagerProductsReceived, CCBool::create (success));
}

bool InAppPurchaseManager::purchaseCompleted (InAppPurchaseManager::Status status, const char *productId,
                                              const char *transactionId, bool originalTransaction,
                                              double originalTransactionTimestamp, double purchaseTimestamp,
                                              const char *expirationTimestamp, const char *cancellationTs,
                                              int verificationsInProgressCount)
{
    std::cout << "Inapp purchase completed" << std::endl;
    bool processed = true;
    InAppProfile *profile;
    if (productId && GameProfile::get ()) {
        profile = GameProfile::get ()->getInAppWithProductId (productId);
    }

    if (status == kStatusSuccess) {

        bool saveAsPending = true;
        GameState *gs = GameState::get ();
        if (profile && gs && GameProfile::get ()) {

            // testing!
            if (profile->getIsSubscription ())
                TicketController::get ()->addPlayerTickets (123456);

            // fix for MS-447
            saveAsPending = false;

            if (transactionId && strlen (transactionId) != 0 && lastFinishedTransactionId == transactionId) {
                return true;
            }

            bool shouldReportAnalytics = true;

            // add gems, tickets or subscription
            std::string currencyType = "Gems";
            if (profile->getInAppType () == GemsPackage) {
                // Gems
                gs->addPlayerGems (profile->getGain ());
                IosFunctions::adjustTrackRevenue (profile->getPriceInDollars ());
                AnalyticsController::purchaseCompleted (profile->getPriceInDollars (), profile->getType (),
                                                        profile->getProfileId (), productId, currencyType,
                                                        profile->getGain (), std::string (transactionId));
            } else if (profile->getInAppType () == InAppType::TicketsPackage) {
                // Tickets
                currencyType = "Tickets";
                TicketController::get ()->addPlayerTickets (profile->getGain ());
                IosFunctions::adjustTrackRevenue (profile->getPriceInDollars ());
                AnalyticsController::purchaseCompleted (profile->getPriceInDollars (), profile->getType (),
                                                        profile->getProfileId (), productId, currencyType,
                                                        profile->getGain (), std::string (transactionId));
            } else {
                // Subscriptions
                currencyType = "Subscription";
            }
            // bool isFlowOffer = OffersController::get()->purchaseFinishedSuccessfully();

            // update stats
            double moneySpent = gs->getUserPurchases()->getTotalMoneySpent();
            gs->getUserPurchases()->setTotalMoneySpent(moneySpent + profile->getPriceInDollars());

            if (profile->getInAppType() == InAppType::TicketsPackage) {
                double moneySpent = gs->getUserPurchases()->getTotalMoneySpentTickets();
                gs->getUserPurchases()->setTotalMoneySpentTickets(moneySpent + profile->getPriceInDollars());
            } else {
                double moneySpent = gs->getUserPurchases()->getTotalMoneySpentGems();
                gs->getUserPurchases()->setTotalMoneySpentGems(moneySpent + profile->getPriceInDollars());
            }

            int nowTimestamp = NGSTime::currentTimeServer ();

            gs->getUserPurchases()->setLastPurchaseTime(nowTimestamp);

            // int daysPlaying = gs->getDaysPlayed();

            int firstPurchaseTime = gs->getUserPurchases()->getFirstPurchaseTime();
            bool firstPurchaseB = false;
            if (firstPurchaseTime == 0) {
                firstPurchaseB = true;
                gs->getUserPurchases()->setFirstPurchaseTime(nowTimestamp);

                IosFunctions::adjustFirstPurchase ();
                // AnalyticsController::firstPurchase(profile->getPriceInDollars(), gs->getPlayerLevel(), daysPlaying);
            }

            SoundManager::get ()->playEffect (kEffectInAppPurchase);
            auto profileId = profile->getProfileId ();

            SaveGameController::get ()->save ();

            if (transactionId) {
                lastFinishedTransactionId = std::string (transactionId);
            }

            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationInAppPurchaseManagerPurchaseUSDSpent, CCDouble::create (profile->getPriceInDollars ()));

            if (profile->getIsSubscription ()) {
                CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                    kNotificationInAppPurchaseManagerSubscriptionPurchaseComplete, CCInteger::create (status));
            }

            sendCurrencyBoughtSuccessfullyNotification (currencyType, productId);
        }
        // GameState not initialized, save product id, LoadingScene will call purchaseCompleted when GameState
        // initialized
        if (saveAsPending) {
            std::string pendingInAppPurchaseProductId =
                CCUserDefault::sharedUserDefault ()->getStringForKey (kUserDefaultPendingInAppPurchaseProductId);

            if (pendingInAppPurchaseProductId.length () != 0)
                pendingInAppPurchaseProductId += " ";

            pendingInAppPurchaseProductId += productId;

            CCUserDefault::sharedUserDefault ()->setStringForKey (kUserDefaultPendingInAppPurchaseProductId, productId);
            CCLOG ("IAP Saved purchase! (%s)", pendingInAppPurchaseProductId.c_str ());
        }
    } else if (status == kStatusFail) {
        AnalyticsController::purchaseFailed ();
    } else if (status == kStatusVerificationFailed) {
        AnalyticsController::verificationFailed ();
    } else if (status == kStatusVerificationConnectFailed) {
        AnalyticsController::verificationFailed ();
    }

    if (status == kStatusUnknownProduct)
        // try request again
        requestProductData ();

    showAlert (status);

    CCDictionary *data = CCDictionary::create ();
    if (profile) {
        data->setObject (profile->getProductId (), "productId");
    }
    data->setObject (CCString::create (std::to_string((int)status)), "status");
    data->setObject (CCString::create (std::to_string(verificationsInProgressCount)),
                     "verificationsInProgressCount");
    CCNotificationCenter::sharedNotificationCenter ()->postNotification (
        kNotificationInAppPurchaseManagerPurchaseComplete, data);

    return processed;
}

void InAppPurchaseManager::checkAndResolvePendingPurchases ()
{
    std::string pendingInAppPurchaseProductId =
        CCUserDefault::sharedUserDefault ()->getStringForKey (kUserDefaultPendingInAppPurchaseProductId);
    std::vector<std::string> productIds = split (pendingInAppPurchaseProductId, ' ');
    // clear pending in app purchase
    CCUserDefault::sharedUserDefault ()->setStringForKey (kUserDefaultPendingInAppPurchaseProductId, "");

    for (auto product : productIds) {
        // TODO - add for subscriptions
        purchaseCompleted (kStatusSuccess, product.c_str (), "");
    }
}
#pragma mark - Helper methods

void InAppPurchaseManager::showAlert (InAppPurchaseManager::Status status)
{
    switch (status) {
        case kStatusFail:
            CCNative::createAlert ("Error", "There is a problem with the Google Play store. Please try again later.",
                                   "OK");
            CCNative::showAlert ();
            break;
        case kStatusVerificationFailedOnce:
            CCNative::createAlert (
                "Server problem",
                "There was a problem during this In-App purchase. It will be completed next time you launch game.",
                "OK");
            CCNative::showAlert ();
            break;
        case kStatusVerificationConnectFailed:
            CCNative::createAlert ("Server problem", "There was a server problem during this In-App purchase. It will "
                                                     "be completed next time you launch game.",
                                   "OK");
            CCNative::showAlert ();
            break;
        case kStatusVerificationFailed:
            CCNative::createAlert (
                "IAP Error", "There was a problem during this In-App purchase. Receipt verification failed.", "OK");
            CCNative::showAlert ();
            break;
        case kStatusUnknownProduct:
            CCNative::createAlert ("Error", "There is a problem with the Google Play store. Please try again later.",
                                   "OK");
            CCNative::showAlert ();
            break;
        case kStatusSuccess:
        default:
            break;
    }
}

void InAppPurchaseManager::sendCurrencyBoughtSuccessfullyNotification (std::string type, const char *productId)
{
    CCDictionary *dict = CCDictionary::create ();

    std::transform (type.begin (), type.end (), type.begin (), ::tolower);

    dict->setObject (CCString::create (type), EVENT_FILTER);
    dict->setObject (CCString::create (productId), EVENT_NUMBER_FILTER);
    dict->setObject (1, EVENT_AMOUNT);

    CCNotificationCenter::sharedNotificationCenter ()->postNotification (kNotificationCurrencyBoughtSuccessfully, dict);
}

void InAppPurchaseManager::sendSubscriptionInit (Status status, std::string productId, std::string transactionId,
                                                 long long int timestamp, bool shouldReportAdjust,
                                                 int verificationsInProgressCount)
{
    if (status == kStatusSuccess) {

        // init subscription so that the user doesn't have to wait
        auto profile = GameProfile::get ()->getSubscriptionsWithProductId (productId.c_str ());
        if (profile == nullptr) {
            showAlert (kStatusUnknownProduct);
            return;
        } // it's not a subscription, or we don't have the up-to-date profiles

        // set the expiration timestamp either a month, or a week later
        auto gs = GameState::get ();
            if (gs) {
            auto subDays = profile->getDaysDuration ();
#ifdef DEBUG
            subDays = 1;
#endif

            // timestamp is (probably) in miliseconds, not seconds
            double endTimestampServer = (timestamp) + subDays * 3600 * 24 * 1000;
            // give one day subscription on every entry
            double endTimestampLocal = (timestamp) + 1 * 3600 * 24 * 1000;
            CCLOG ("updated end timestamp local %f server %f gs %f", endTimestampLocal, endTimestampServer,
                   gs->getSubscriptionExpirationTimestamp ());

            if (NGSTime::currentTimeServer () > gs->getSubscriptionExpirationTimestamp ()) {
                TicketController::get ()->setSubscriptionExpirationTimestamp (endTimestampLocal / 1000);
            }

            // log the info, since we're unsure if it's logged
            NGSManager::get ()->getSubscriptionNode ()->sendInit (transactionId, endTimestampServer, productId);

            // server reports everything to adjust - remove the shouldReportAdjust in future commits. - quickfix
                //
                int firstPurchaseTime = gs->getUserPurchases()->getFirstPurchaseTime();
            bool firstPurchaseB = false;
            if (firstPurchaseTime == 0) {
                firstPurchaseB = true;
                gs->getUserPurchases()->setFirstPurchaseTime(NGSTime::currentTimeServer ());

                IosFunctions::adjustFirstPurchase ();
                // AnalyticsController::firstPurchase(profile->getPriceInDollars(), gs->getPlayerLevel(), daysPlaying);
            }

            CCDictionary *data = CCDictionary::create ();
            if (profile) {
                data->setObject (profile->getProductId (), "productId");
            }
            data->setObject (CCInteger::create (status), "status");
            data->setObject (CCInteger::create (verificationsInProgressCount), "verificationsInProgressCount");
            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                kNotificationInAppPurchaseManagerSubscriptionPurchaseComplete, data);

            CCNotificationCenter::sharedNotificationCenter ()->postNotification (
                    kNotificationInAppPurchaseManagerPurchaseComplete, data);
        }
    }
    // showAlert(status);
}

void InAppPurchaseManager::setCurrencyCode (std::string currency) { currencyCode = currency; }

bool InAppPurchaseManager::showIncreasedGain(std::string productId)
{
    return OfferController::get()->isOfferActive() && OfferController::get()->isAnOfferPurchase(productId);
}

#pragma mark - One Time Offer

bool InAppPurchaseManager::showOneTimeOffer()
{
    CCDictionary *oneTimeOffer = Parameters::getParameters()->dictForKey("oneTimeOffer");
    bool showOneTimeOffer = false;
    if(oneTimeOffer) {
        bool enabled = oneTimeOffer->boolForKey("enabled");
        int minChaptersRead = oneTimeOffer->intForKey("minChaptersRead");
        std::string userCategory = oneTimeOffer->valueForKey("userCategory")->getCString();
        std::string inAppType = oneTimeOffer->valueForKey("categoryInAppType")->getCString();

        showOneTimeOffer = enabled && GameState::get()->getChaptersRead() >= minChaptersRead &&
                getUserCategory(InAppProfile::inAppTypeForString(inAppType)) == userCategoryForString(userCategory);
    }

    return showOneTimeOffer;
}

std::string InAppPurchaseManager::getOneTimeOfferPackageId()
{
    CCDictionary *oneTimeOffer = Parameters::getParameters()->dictForKey("oneTimeOffer");
    std::string packageId = "";
    if (oneTimeOffer) {
        packageId = oneTimeOffer->valueForKey("packageId")->getCString();
    }
    return packageId;
}


