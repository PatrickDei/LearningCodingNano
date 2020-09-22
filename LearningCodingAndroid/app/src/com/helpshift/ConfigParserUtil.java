package com.helpshift;

import android.content.Context;
import android.util.Log;

import com.helpshift.InstallConfig;
import com.helpshift.support.ApiConfig;
import com.helpshift.support.FaqTagFilter;
import com.helpshift.support.Metadata;
import com.helpshift.support.Support;
import com.helpshift.support.flows.ConversationFlow;
import com.helpshift.support.flows.DynamicFormFlow;
import com.helpshift.support.flows.FAQSectionFlow;
import com.helpshift.support.flows.FAQsFlow;
import com.helpshift.support.flows.Flow;
import com.helpshift.support.flows.SingleFAQFlow;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * Config parser.
 */
class ConfigParserUtil {

  private static final String TAG = ConfigParserUtil.class.getSimpleName();

  public static InstallConfig buildInstallConfig(HashMap<String, Object> config) {
    InstallConfig.Builder installConfigBuilder = new InstallConfig.Builder();
    if (config == null || config.isEmpty()) {
      return installConfigBuilder.build();
    }

    Map<String, Object> extraMap = new HashMap<String, Object>();
    for (String key : config.keySet()) {
      if ("enableInAppNotification".equals(key)) {
        Boolean enableInAppNotification = convertToBoolean(config.get(key));
        if (enableInAppNotification != null) {
          installConfigBuilder.setEnableInAppNotification(enableInAppNotification);
        }
      } else if ("enableDefaultFallbackLanguage".equals(key)) {
        Boolean enableDefaultFallbackLanguage = convertToBoolean(config.get(key));
        if (enableDefaultFallbackLanguage != null) {
          installConfigBuilder.setEnableDefaultFallbackLanguage(enableDefaultFallbackLanguage);
        }
      } else if ("enableInboxPolling".equals(key)) {
        Boolean enableInboxPolling = convertToBoolean(config.get(key));
        if (enableInboxPolling != null) {
          installConfigBuilder.setEnableInboxPolling(enableInboxPolling);
        }
      } else if ("enableLogging".equals(key)) {
        Boolean enableLogging = convertToBoolean(config.get(key));
        if (enableLogging != null) {
          installConfigBuilder.setEnableLogging(enableLogging);
        }
      } else if ("notificationIcon".equals(key)) {
        Integer notificationIcon = convertToInteger(config.get(key));
        if (notificationIcon != null) {
          installConfigBuilder.setNotificationIcon(notificationIcon);
        }
      } else if ("largeNotificationIcon".equals(key)) {
        Integer largeNotificationIcon = convertToInteger(config.get(key));
        if (largeNotificationIcon != null) {
          installConfigBuilder.setLargeNotificationIcon(largeNotificationIcon);
        }
      } else if ("notificationSound".equals(key)) {
        Integer notificationSound = convertToInteger(config.get(key));
        if (notificationSound != null) {
          installConfigBuilder.setNotificationSound(notificationSound);
        }
      } else if ("screenOrientation".equals(key)) {
        Integer screenOrientation = convertToInteger(config.get(key));
        if (screenOrientation != null) {
          installConfigBuilder.setScreenOrientation(screenOrientation);
        }
      } else if ("font".equals(key)) {
        String fontPath = convertToString(config.get(key));
        installConfigBuilder.setFont(fontPath);
      } else if ("supportNotificationChannelId".equals(key)) {
        String supportChannelId = convertToString(config.get(key));
        installConfigBuilder.setSupportNotificationChannelId(supportChannelId);
      } else if ("campaignsNotificationChannelId".equals(key)) {
        String campaignChannelId = convertToString(config.get(key));
        installConfigBuilder.setCampaignsNotificationChannelId(campaignChannelId);
      } else {
        extraMap.put(key, config.get(key));
      }

      if (!extraMap.isEmpty()) {
        installConfigBuilder.setExtras(parseMap(extraMap));
      }
    }

    return installConfigBuilder.build();
  }

  public static ApiConfig buildApiConfig(Context context, HashMap<String, Object> config) {
    ApiConfig.Builder apiConfigBuilder = new ApiConfig.Builder();
    if (config == null || config.isEmpty()) {
      return apiConfigBuilder.build();
    }

    Map<String, Object> extraMap = new HashMap<String, Object>();
    for (String key : config.keySet()) {
      if ("enableContactUs".equals(key)) {
        Integer enableContactUs = parseEnableContactUs(config.get(key));
        if (enableContactUs != null) {
          apiConfigBuilder.setEnableContactUs(enableContactUs);
        }
      } else if ("gotoConversationAfterContactUs".equals(key)) {
        Boolean gotoConversationAfterContactUs = convertToBoolean(config.get(key));
        if (gotoConversationAfterContactUs != null) {
          apiConfigBuilder.setGotoConversationAfterContactUs(gotoConversationAfterContactUs);
        }
      } else if ("requireEmail".equals(key)) {
        Boolean requireEmail = convertToBoolean(config.get(key));
        if (requireEmail != null) {
          apiConfigBuilder.setRequireEmail(requireEmail);
        }
      } else if ("hideNameAndEmail".equals(key)) {
        Boolean hideNameAndEmail = convertToBoolean(config.get(key));
        if (hideNameAndEmail != null) {
          apiConfigBuilder.setHideNameAndEmail(hideNameAndEmail);
        }
      } else if ("enableFullPrivacy".equals(key)) {
        Boolean enableFullPrivacy = convertToBoolean(config.get(key));
        if (enableFullPrivacy != null) {
          apiConfigBuilder.setEnableFullPrivacy(enableFullPrivacy);
        }
      } else if ("showSearchOnNewConversation".equals(key)) {
        Boolean showSearchOnNewConversation = convertToBoolean(config.get(key));
        if (showSearchOnNewConversation != null) {
          apiConfigBuilder.setShowSearchOnNewConversation(showSearchOnNewConversation);
        }
      } else if ("showConversationResolutionQuestion".equals(key)) {
        Boolean showConversationResolutionQuestion = convertToBoolean(config.get(key));
        if (showConversationResolutionQuestion != null) {
          apiConfigBuilder.setShowConversationResolutionQuestion(showConversationResolutionQuestion);
        }
      } else if ("showConversationInfoScreen".equals(key)) {
        Boolean showConversationInfoScreen = convertToBoolean(config.get(key));
        if (showConversationInfoScreen != null) {
          apiConfigBuilder.setShowConversationInfoScreen(showConversationInfoScreen);
        }
      } else if ("enableTypingIndicator".equals(key)) {
        Boolean enableTypingIndicator = convertToBoolean(config.get(key));
        if (enableTypingIndicator != null) {
          apiConfigBuilder.setEnableTypingIndicator(enableTypingIndicator);
        }
      } else if ("conversationPrefillText".equals(key)) {
        String conversationPrefillText = convertToString(config.get(key));
        apiConfigBuilder.setConversationPrefillText(conversationPrefillText);
      } else if ("customContactUsFlows".equals(key)) {
        List<Flow> customContactUsFlows = convertToFlowList(context, config.get(key));
        apiConfigBuilder.setCustomContactUsFlows(customContactUsFlows);
      } else if ("withTagsMatching".equals(key)) {
        FaqTagFilter faqTagFilter = convertToFaqTagFilter(config.get(key));
        apiConfigBuilder.setWithTagsMatching(faqTagFilter);
      } else if (Support.CustomIssueFieldKey.equals(key)) {
        Object cifDataObject = config.get(key);
        //Fix: Check for iOS native key "customIssueFields".
        if (cifDataObject == null) {
          cifDataObject = config.get("customIssueFields");
        }
        HashMap<String, String[]> customIssueFields = parseCustomIssueFields(cifDataObject);
        apiConfigBuilder.setCustomIssueFields(customIssueFields);
      } else if (Support.CustomMetadataKey.equals(key)) {
	Metadata metadata = parseMetaData(config.get(key));
        apiConfigBuilder.setCustomMetadata(metadata);
      } else {
        extraMap.put(key, config.get(key));
      }
    }

    if (!extraMap.isEmpty()) {
      apiConfigBuilder.setExtras(parseMap(extraMap));
    }

    return apiConfigBuilder.build();
  }

  public static String[] convertToStringArray(Object value) {
    if (value instanceof ArrayList) {
      ArrayList<String> arrayList = (ArrayList<String>) value;
      return arrayList.toArray(new String[arrayList.size()]);
    } else if (value instanceof String[]) {
      return (String[]) value;
    }
    return null;
  }

  private static Integer parseEnableContactUs(Object value) {
    String valueEnableContactUs = convertToString(value);
    if (valueEnableContactUs == null) {
      return null;
    }

    Integer enableContactUs = null;
    if ("yes".equals(valueEnableContactUs) ||
         "always".equals(valueEnableContactUs)) {
      enableContactUs = Support.EnableContactUs.ALWAYS;
    } else if ("no".equals(valueEnableContactUs) ||
                "never".equals(valueEnableContactUs)) {
      enableContactUs = Support.EnableContactUs.NEVER;
    } else if ("after_viewing_faqs".equals(valueEnableContactUs)) {
      enableContactUs = Support.EnableContactUs.AFTER_VIEWING_FAQS;
    } else if ("after_marking_answer_unhelpful".equals(valueEnableContactUs)) {
      enableContactUs = Support.EnableContactUs.AFTER_MARKING_ANSWER_UNHELPFUL;
    }
    return enableContactUs;
  }

  private static Map<String, Object> parseMap(Object value) {
    Map<String, Object> extraMap = (Map<String, Object>) value;
    if (value == null) {
      return null;
    }

    for (String key : extraMap.keySet()) {
      Object data = extraMap.get(key);
      Boolean isBoolean = convertToBoolean(data);
      if (isBoolean != null) {
        extraMap.put(key, isBoolean);
      }
      //Convert internal maps
      if (data instanceof HashMap) {
        extraMap.put(key, parseMap(data));
      }
    }
    return extraMap;
  }

  private static String convertToString(Object value) {
    if (value instanceof String) {
      return (String) value;
    }

    //Default value
    return null;
  }

  private static Integer convertToInteger(Object value) {
    if (value instanceof Integer) {
      return (Integer) value;
    } else if (value instanceof String) {
      try {
        return Integer.valueOf((String) value);
      } catch (NumberFormatException e) {
        Log.i(TAG, "convertToInteger", e);
      }
    }
    return null;
  }

  private static Boolean convertToBoolean(Object value) {
    if (value instanceof String) {
      if ("yes".equalsIgnoreCase(value.toString())) {
        return true;
      } else if ("no".equalsIgnoreCase(value.toString())) {
        return false;
      }
    } else if (value instanceof Boolean) {
      return (Boolean) value;
    }

    //Default value
    return null;
  }

  private static Metadata parseMetaData(Object value) {
    HashMap<String, Object> valueMap = (HashMap<String, Object>) value;
    if (value == null) {
      return null;
    }

    String[] tagArray = convertToStringArray(valueMap.get(Support.TagsKey));
    valueMap.remove(Support.TagsKey);
    return new Metadata(valueMap, tagArray);
  }

  private static HashMap<String, String[]> parseCustomIssueFields(Object value) {
    HashMap<String, Object> cifData = (HashMap<String, Object>) value;
    if (value == null) {
      return null;
    }

    HashMap<String, String[]> resultCIFDataMap = new HashMap();
    for (String key : cifData.keySet()) {
      String[] dataArray = convertToStringArray(cifData.get(key));
      if (dataArray != null) {
        resultCIFDataMap.put(key, dataArray);
      }
    }
    return resultCIFDataMap;
  }

  private static FaqTagFilter convertToFaqTagFilter(Object value) {
    if (value instanceof HashMap) {
      HashMap valueMap = (HashMap) value;
      String operator = convertToString(valueMap.get("operator"));
      String[] tags = convertToStringArray(valueMap.get("tags"));
      if (operator != null && tags != null) {
        return new FaqTagFilter(operator, tags);
      }
    }
    return null;
  }

  public static List<Flow> convertToFlowList(Context context, Object value) {
    List<HashMap<String, Object>> flowListMap = null;
    try {
      flowListMap = (List<HashMap<String, Object>>) value;
    } catch (ClassCastException e) {
      Log.i(TAG, "convertToFlowList", e);
    }

    if (flowListMap == null) {
      return null;
    }

    List<Flow> flowList = new ArrayList<Flow>();
    for (HashMap<String, Object> flowMap : flowListMap) {
      Flow flow = convertToFlow(context, flowMap);
      if (flow != null) {
        flowList.add(flow);
      }
    }
    return flowList;
  }

  private static Flow convertToFlow(Context context, HashMap<String, Object> flowMap) {
    if (flowMap == null || flowMap.isEmpty()) {
      return null;
    }
    String type = (String) flowMap.get("type");
    if (type == null) {
      return null;
    }

    //Parse config
    ApiConfig apiConfig;
    if (flowMap.containsKey("config")) {
      apiConfig = buildApiConfig(context, (HashMap) flowMap.get("config"));
    } else {
      apiConfig = new ApiConfig.Builder().build();
    }

    //Parse title
    String titleResName = (String) flowMap.get("titleResourceName");
    int resId = 0;
    if (titleResName != null) {
      resId = context.getResources().getIdentifier(titleResName, "string", context.getPackageName());
    }
    String title = "";
    if (resId == 0) {
      title = (String) flowMap.get("title");
    }

    //Build flow
    Flow flow = null;
    if (Support.FAQS_FLOW.equals(type)) {
      if (resId != 0) {
        flow = new FAQsFlow(resId, apiConfig);
      } else {
        flow = new FAQsFlow(title, apiConfig);
      }
    } else if (Support.CONVERSATION_FLOW.equals(type)) {
      if (resId != 0) {
        flow = new ConversationFlow(resId, apiConfig);
      } else {
        flow = new ConversationFlow(title, apiConfig);
      }
    } else if (Support.FAQ_SECTION_FLOW.equals(type)) {
      String sectionId = (String) flowMap.get("data");
      if (resId != 0) {
        flow = new FAQSectionFlow(resId, sectionId, apiConfig);
      } else {
        flow = new FAQSectionFlow(title, sectionId, apiConfig);
      }
    } else if (Support.SINGLE_FAQ_FLOW.equals(type)) {
      String faqId = (String) flowMap.get("data");
      if (resId != 0) {
        flow = new SingleFAQFlow(title, faqId, apiConfig);
      } else {
        flow = new SingleFAQFlow(title, faqId, apiConfig);
      }
    } else if (Support.DYNAMIC_FORM_FLOW.equals(type)) {
      List<Flow> flowList = convertToFlowList(context, flowMap.get("data"));
      if (resId != 0) {
        flow = new DynamicFormFlow(resId, flowList);
      } else {
        flow = new DynamicFormFlow(title, flowList);
      }
    }
    return flow;
  }
}
