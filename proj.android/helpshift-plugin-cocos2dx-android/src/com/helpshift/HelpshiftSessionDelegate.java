/**
 * Copyright (c) 2015-2016 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.helpshift;

import java.io.File;
import java.util.HashSet;
import java.util.Arrays;
import com.helpshift.support.Support;

class HelpshiftCocos2dxDelegates implements Support.Delegate {

  static HashSet<String> supportedFileFormats;

  public static void setSupportedFileFormats(String[] fileFormats) {
    if(fileFormats != null && fileFormats.length > 0) {
      supportedFileFormats = new HashSet<String>(Arrays.asList(fileFormats));
    }
  }
  /**
   * Delegate method which is called when Helpshift session begins.
   */
  @Override
  public void sessionBegan () {
    HelpshiftBridge.helpshiftSessionBegan();
  }

  /**
   * Delegate method which is called when Helpshift session ends.
   */
  @Override
  public void sessionEnded () {
    HelpshiftBridge.helpshiftSessionEnded();
  }

  /**
   * Delegate method which is called when user starts a new conversation
   *
   * @param newConversationMessage
   */
  @Override
  public void newConversationStarted (String newConversationMessage) {
    HelpshiftBridge.newConversationStarted(newConversationMessage);
  }

  /**
   * Delegate method which is called when user sends a new message
   *
   * @param newMessage
   */
  @Override
  public void userRepliedToConversation (String newMessage) {
    HelpshiftBridge.userRepliedToConversation(newMessage);
  }

  /**
   * Delegate method which is called when user completes the customer
   * satisfaction survey
   *
   * @param rating
   * @param feedback
   */
  @Override
  public void userCompletedCustomerSatisfactionSurvey (int rating, String feedback) {
    HelpshiftBridge.userCompletedCustomerSatisfactionSurvey(rating, feedback);
  }

  /**
   * Optional delegate method that is called when the user taps an downloaded attachment file to view it.
   *
   * @param attachmentFile
   */
  @Override
  public void displayAttachmentFile (File attachmentFile) {
    String path = attachmentFile.getAbsolutePath();
    String fileName = attachmentFile.getName();
    String extension = null;
    if(fileName.lastIndexOf(".") != -1 && fileName.lastIndexOf(".") != 0) {
      extension =  fileName.substring(fileName.lastIndexOf(".") + 1);
    }
    if(extension != null && supportedFileFormats.contains(extension)) {
      HelpshiftBridge.displayAttachmentFile(path);
    }
  }

  @Override
  public void didReceiveNotification(int newMessagesCount) {
    HelpshiftBridge.didReceiveNotification(newMessagesCount);
  }

}
