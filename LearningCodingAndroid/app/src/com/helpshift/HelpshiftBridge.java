/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 */
package com.helpshift;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import com.helpshift.campaigns.Campaigns;
import com.helpshift.exceptions.InstallException;
import com.helpshift.support.AlertToRateAppListener;
import com.helpshift.support.ApiConfig;
import com.helpshift.support.Support;
import com.helpshift.util.HSLogger;
import com.helpshift.HelpshiftUser;

import java.util.Date;
import java.util.HashMap;
import java.util.List;

public class HelpshiftBridge {
  private static final String TAG = "Helpshift_CocosBridge";
  private static Activity activity;

  public static native void alertToRateAppAction(int result);

  public static native void didReceiveNotificationCount(int result);

  public static native void helpshiftSessionBegan();

  public static native void helpshiftSessionEnded();

  public static native void helpshiftCampaignsSessionBegan();

  public static native void helpshiftCampaignsSessionEnded();

  public static native void newConversationStarted(String newConversationMessage);

  public static native void userRepliedToConversation(String newMessage);

  public static native void userCompletedCustomerSatisfactionSurvey(int rating, String feedback);

  public static native void didReceiveNotification(int newMessageCount);

  public static native void displayAttachmentFile(String filePath);

  public static native void authenticationFailed(HelpshiftUser user, int reason);

  public static native void conversationEnded();

  public static native String getRuntimeVersion();

  private static final String pluginVersion = "4.1.0";

  public static void init(Activity a) {
    HelpshiftBridge.activity = a;
  }

  private static void checkAndFilterSupportedFileFormats(HashMap config) {
    if (config == null) {
      return;
    }

    Object value = config.get("supportedFileFormats");
    if (value != null) {
      String[] supportedFileFormats = ConfigParserUtil.convertToStringArray(value);
      if (supportedFileFormats != null) {
        HelpshiftCocos2dxDelegates.setSupportedFileFormats(supportedFileFormats);
      }
      config.remove("supportedFileFormats");
    }
  }

  private static InstallConfig buildInstallConfig(HashMap config) {
    checkAndFilterSupportedFileFormats(config);
    return ConfigParserUtil.buildInstallConfig((HashMap<String, Object>) config);
  }

  private static ApiConfig buildApiConfig(HashMap config) {
    checkAndFilterSupportedFileFormats(config);
    return ConfigParserUtil.buildApiConfig(activity, (HashMap<String, Object>) config);
  }

  public static void install(Activity a, String apiKey, String domain, String appId) throws InstallException {
    HashMap<String, Object> config = new HashMap<String, Object>();
    HelpshiftBridge.install(a, apiKey, domain, appId, config);
  }

  public static void install(Activity a, String apiKey, String domain, String appId, HashMap config) throws InstallException {
    HelpshiftBridge.activity = a;
    config.put("sdkType", "cocos2dx");
    config.put("pluginVersion", pluginVersion);
    config.put("runtimeVersion", getRuntimeVersion());
    InstallConfig installConfig = buildInstallConfig(config);
    Core.init(All.getInstance());
    Core.install(a.getApplication(),
                 apiKey,
                 domain,
                 appId,
                 installConfig);
    HSLogger.d(TAG, "install - config : " + config);
    HelpshiftCocos2dxDelegates helpshiftDelegate = new HelpshiftCocos2dxDelegates();
    Support.setDelegate(helpshiftDelegate);
    Campaigns.setDelegate(new HelpshiftCocos2dxCampaignsDelegates());
  }

  public static void showConversation() {
    HelpshiftBridge.showConversation(new HashMap());
  }

  public static void showConversation(HashMap config) {
    HSLogger.d(TAG, "showConversation - config : " + config);
    Support.showConversation(HelpshiftBridge.activity, buildApiConfig(config));
  }

  public static void showFAQSection(String sectionPublishId) {
    HelpshiftBridge.showFAQSection(sectionPublishId, new HashMap());
  }

  public static void showFAQSection(String sectionPublishId, HashMap config) {
    HSLogger.d(TAG, "showFAQSection - sectionPublishId : " + sectionPublishId + " config : " + config);
    Support.showFAQSection(HelpshiftBridge.activity, sectionPublishId, buildApiConfig(config));
  }

  public static void showSingleFAQ(String questionPublishId) {
    HelpshiftBridge.showSingleFAQ(questionPublishId, new HashMap());
  }

  public static void showSingleFAQ(String questionPublishId, HashMap config) {
    HSLogger.d(TAG, "showSingleFAQ - questionPublishId : " + questionPublishId + " config : " + config);
    Support.showSingleFAQ(HelpshiftBridge.activity, questionPublishId, buildApiConfig(config));
  }

  public static void showFAQs() {
    HelpshiftBridge.showFAQs(new HashMap());
  }

  public static void showFAQs(HashMap config) {
    HSLogger.d(TAG, "showFAQs - config : " + config);
    Support.showFAQs(HelpshiftBridge.activity, buildApiConfig(config));
  }

  public static void showInbox() {
    Campaigns.showInbox(HelpshiftBridge.activity);
  }

  public static int getCountOfUnreadMessages() {
    return Campaigns.getCountOfUnreadMessages();
  }

  public static void setNameAndEmail(String name, String email) {
    Core.setNameAndEmail(name, email);
  }

  public static void login(String identifier, String name, String email) {
    Core.login(identifier, name, email);
  }

  public static void login(HelpshiftUser user) {
    Core.login(user);
  }

  public static void logout() {
    Core.logout();
  }

  public static void clearAnonymousUser() {
    Core.clearAnonymousUser();
  }

  public static void setUserIdentifier(String identifier) {
    Support.setUserIdentifier(identifier);
  }

  public static void registerDeviceToken(String deviceToken) {
    Core.registerDeviceToken(HelpshiftBridge.activity, deviceToken);
  }

  public static void leaveBreadCrumb(String breadCrumb) {
    Support.leaveBreadCrumb(breadCrumb);
  }

  public static void clearBreadCrumbs() {
    Support.clearBreadCrumbs();
  }

  public static int getNotificationCount(boolean isAsync) {
    int count = -1;
    if (!isAsync) {
      count = Support.getNotificationCount();
    } else {
      Handler mainThreadHandler = new Handler(Looper.getMainLooper());
      mainThreadHandler.post(new Runnable() {
        public void run() {
          Handler countHandler = new Handler() {
            public void handleMessage(Message msg) {
              super.handleMessage(msg);
              Bundle countData = (Bundle) msg.obj;
              Integer count = countData.getInt("value");
              didReceiveNotificationCount(count);
            }
          };
          Support.getNotificationCount(countHandler, new Handler());
        }
      });
    }
    return count;
  }

  public static void handlePush(HashMap notification) {
    Bundle bundle = new Bundle();
    HSLogger.d(TAG, "handlePush - notification : " + notification);
    for (Object key : notification.keySet()) {
      bundle.putString((String) key, (String) notification.get((String) key));
    }
    Core.handlePush(HelpshiftBridge.activity, bundle);
  }

  public static void showAlertToRateApp(String url) {
    AlertToRateAppListener actionListener = new AlertToRateAppListener() {
      @Override
      public void onAction(int action) {
        int msg = 0;
        switch (action) {
          case Support.RateAlert.CLOSE:
            msg = 0;
            break;
          case Support.RateAlert.FEEDBACK:
            msg = 1;
            break;
          case Support.RateAlert.SUCCESS:
            msg = 2;
            break;
          case Support.RateAlert.FAIL:
            msg = 3;
            break;
        }
        alertToRateAppAction(msg);
      }
    };
    Support.showAlertToRateApp(url, actionListener);
  }

  public static boolean isConversationActive() {
    return Support.isConversationActive();
  }

  public static void setSDKLanguage(String locale) {
    Support.setSDKLanguage(locale);
  }

  public static void showDynamicForm(List<HashMap<String, Object>> data) {
    showDynamicForm("", data);
  }

  public static void showDynamicForm(String title, List<HashMap<String, Object>> data) {
    HSLogger.d(TAG, "showDynamicForm - title : " + title + ", data : " + data);
    Support.showDynamicForm(HelpshiftBridge.activity, title, ConfigParserUtil.convertToFlowList(activity, data));
  }

  public static boolean addProperty(String key, String value) {
    return Campaigns.addProperty(key, value);
  }

  public static boolean addProperty(String key, int value) {
    return Campaigns.addProperty(key, value);
  }

  public static boolean addProperty(String key, boolean value) {
    return Campaigns.addProperty(key, value);
  }

  public static boolean addProperty(String key, double value) {
    HSLogger.d(TAG, "addProperty of date : " + value);
    Date dateValue = new Date((long) (value * 1000));
    return Campaigns.addProperty(key, dateValue);
  }

  public static boolean addProperty(String key, long value) {
    return Campaigns.addProperty(key, value);
  }

  public static void addProperties(HashMap properties) {
    Campaigns.addProperties(properties);
  }
}
