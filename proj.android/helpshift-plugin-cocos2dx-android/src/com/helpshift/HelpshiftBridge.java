/**
 * Copyright (c) 2015-2016 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.helpshift;
import android.os.Looper;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.content.Intent;
import android.app.Activity;
import android.util.Log;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.List;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Date;

import com.helpshift.support.Support;
import com.helpshift.support.AlertToRateAppListener;
import com.helpshift.campaigns.Campaigns;
import com.helpshift.Core;

public class HelpshiftBridge
{
  private static final String TAG = "HSCocosPlugin";
  private static Activity activity;
  private static String gcmRegId;

  public static native void alertToRateAppAction(int result);
  public static native void didReceiveNotificationCount(int result);
  public static native void helpshiftSessionBegan();
  public static native void helpshiftSessionEnded();
  public static native void newConversationStarted(String newConversationMessage);
  public static native void userRepliedToConversation(String newMessage);
  public static native void userCompletedCustomerSatisfactionSurvey(int rating, String feedback);
  public static native void didReceiveNotification(int newMessageCount);
  public static native void displayAttachmentFile(String filePath);

  public static void init (Activity a) {
    HelpshiftBridge.activity = a;
  }

  private static HashMap convertKey(HashSet keys, HashMap config)
  {
    Iterator iterator = keys.iterator();
    while(iterator.hasNext()) {
      String key = (String) iterator.next();
      Object value = config.get(key);
      if (value != null) {
        if(value.toString().equals("yes")) {
          config.put(key, true);
        } else {
          config.put(key, false);
        }
      }
    }
    return config;
  }
  private static HashMap parseConfigDictionary (HashMap config) {
    if(config == null) {
      return new HashMap();
    }

    if (config.get("enableContactUs") != null) {
      if(config.get("enableContactUs").equals("yes") ||
         config.get("enableContactUs").equals("always")) {
        config.put("enableContactUs", Support.EnableContactUs.ALWAYS);
      } else if (config.get("enableContactUs").equals("no") ||
                 config.get("enableContactUs").equals("never")){
        config.put("enableContactUs", Support.EnableContactUs.NEVER);
      } else if (config.get("enableContactUs").equals("after_viewing_faqs")) {
        config.put("enableContactUs", Support.EnableContactUs.AFTER_VIEWING_FAQS);
      } else if (config.get("enableContactUs").equals("after_marking_answer_unhelpful")) {
        config.put("enableContactUs", Support.EnableContactUs.AFTER_MARKING_ANSWER_UNHELPFUL);
      }
    }
    if (config.get("supportedFileFormats") != null) {
      HelpshiftCocos2dxDelegates.setSupportedFileFormats((String[])config.get("supportedFileFormats"));
      config.remove("supportedFileFormats");
    }

    HashSet keySet = new HashSet();
    keySet.add("enableInAppNotification");
    keySet.add("requireEmail");
    keySet.add("hideNameAndEmail");
    keySet.add("enableFullPrivacy");
    keySet.add("showSearchOnNewConversation");
    keySet.add("gotoConversationAfterContactUs");
    keySet.add("showConversationResolutionQuestion");
    keySet.add("enableDefaultFallbackLanguage");
    config = convertKey(keySet, config);
    
    config.put("showSearchOnNewConversation", true);
    config.put("showConversationResolutionQuestion", false);
    config.put("enableContactUs", Support.EnableContactUs.AFTER_VIEWING_FAQS);
    
    HashMap customMeta = (HashMap) (config.get(Support.CustomMetadataKey));
    if(customMeta != null) {
      ArrayList<String> tags = (ArrayList<String>) customMeta.get(Support.TagsKey);
      if(tags != null && tags.size() > 0) {
        String[] tagsArray = (String[])tags.toArray(new String[tags.size()]);
        customMeta.put(Support.TagsKey, tagsArray);
      }
      config.put(Support.CustomMetadataKey,customMeta);
    }

    HashMap tagsMatching = (HashMap) (config.get("withTagsMatching"));
    if(tagsMatching != null) {
      ArrayList<String> tags = (ArrayList<String>) tagsMatching.get("tags");
      if(tags != null && tags.size() > 0) {
        String[] tagsArray = (String[])tags.toArray(new String[tags.size()]);
        tagsMatching.put("tags", tagsArray);
      }
      config.put("withTagsMatching",tagsMatching);
    }

    return config;
  }

  public static void install (Activity a, String apiKey, String domain, String appId) {
    HashMap<String, Object> config = new HashMap<String, Object>();
    config.put ("sdkType", "cocos2dx");
    HelpshiftBridge.install (a, apiKey, domain, appId, config);
  }

  public static void install (Activity a, String apiKey, String domain, String appId, HashMap config) {
    HelpshiftBridge.activity = a;
    HashMap parsedConfig = parseConfigDictionary(config);
    parsedConfig.put("sdkType", "cocos2dx");
    Core.init(All.getInstance());
    Core.install(a.getApplication(),
                 apiKey,
                 domain,
                 appId,
                 config);
    HelpshiftCocos2dxDelegates helpshiftDelegate = new HelpshiftCocos2dxDelegates();
    Support.setDelegate(helpshiftDelegate);
    regGCM();
  }

  public static void showConversation() {
    HelpshiftBridge.showConversation(new HashMap());
  }

  public static void showConversation(HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Support.showConversation(HelpshiftBridge.activity, parseConfigDictionary(config));
  }

  public static void showFAQSection(String sectionPublishId) {
	HelpshiftBridge.showFAQSection(sectionPublishId, new HashMap());
  }

  public static void showFAQSection(String sectionPublishId, HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Support.showFAQSection(HelpshiftBridge.activity, sectionPublishId, parseConfigDictionary(config));
  }

  public static void showSingleFAQ(String questionPublishId) {
    HelpshiftBridge.showSingleFAQ(questionPublishId, new HashMap());
  }

  public static void showSingleFAQ(String questionPublishId, HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Support.showSingleFAQ(HelpshiftBridge.activity, questionPublishId, parseConfigDictionary(config));
  }

  public static void showFAQs() {
    HelpshiftBridge.showFAQs(new HashMap());
  }

  public static void showFAQs(HashMap config) {
	  if(HelpshiftBridge.activity != null)
		  Support.showFAQs(HelpshiftBridge.activity, parseConfigDictionary(config));
  }

  public static void setNameAndEmail(String name, String email) {
    Core.setNameAndEmail(name, email);
  }

  public static void login(String identifier, String name, String email) {
    Core.login(identifier, name, email);
  }

  public static void logout() {
    Core.logout();
  }

  public static void setUserIdentifier(String identifier) {
    Support.setUserIdentifier(identifier);
  }

  public static void registerDeviceToken(String deviceToken) {
	gcmRegId = deviceToken;
	regGCM();
  }
  
  private static void regGCM(){
	if(HelpshiftBridge.activity != null && gcmRegId != null){
		Log.d("COK", "COK Helpshift registerDeviceToken " + gcmRegId);
		Core.registerDeviceToken(HelpshiftBridge.activity, gcmRegId);
	}else{
		Log.d("COK", "COK Helpshift registerDeviceToken delay");
	}
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
      mainThreadHandler.post (new Runnable() {
          public void run () {
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

  public static void handlePush (HashMap notification) {
    Bundle bundle = new Bundle();
    for(Object key:notification.keySet()) {
      bundle.putString((String)key, (String)notification.get((String)key));
    }
    Core.handlePush(HelpshiftBridge.activity, bundle);
  }

  public static void showAlertToRateApp (String url) {
    AlertToRateAppListener actionListener =  new AlertToRateAppListener() {
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

  public static void setSDKLanguage(String locale) {
    Support.setSDKLanguage(locale);
  }

  public static void showDynamicForm(List<HashMap<String,Object>> data) {
    Support.showDynamicFormFromData(HelpshiftBridge.activity, data);
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
    Date dateValue = new Date((long) (value * 1000));
    return Campaigns.addProperty(key, dateValue);
  }

  public static void addProperties(HashMap properties) {
    Campaigns.addProperties(properties);
  }
  
  public static void sendLog(String IP, String uid, String tag) {
//	  Log.d(TAG, strUuid+"===="+strHost);
	  String _tmp = "==customtag=" + tag + "; uid=" + uid;
	  Thread thread = new Thread(new NetworkChecker(_tmp, IP));
		// 网络请求必须不能在主线程中，需要另起线程来做
		thread.start();
  }
}
