/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.helpshift;

import java.io.File;
import java.util.HashSet;
import java.util.Arrays;
import com.helpshift.support.Support;
import com.helpshift.util.HSLogger;
import com.helpshift.delegate.AuthenticationFailureReason;
import com.helpshift.HelpshiftUser;

class HelpshiftCocos2dxDelegates implements Support.Delegate {

  private static final String TAG = "Helpshift_CocosDeleg";
  static HashSet<String> supportedFileFormats;

  public static void setSupportedFileFormats(String[] fileFormats) {
    HSLogger.d(TAG, "Setting supported file formats to : " + Arrays.asList(fileFormats));
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
   * Optional delegate method that gets called when the user authentication fails.
   * Whenever you receive this call, You should be calling the login API for the given user with valid authToken.
   * <pre>
   *   void authenticationFailed(HelpshiftUser user, AuthenticationFailureReason reason) {
   *      String newValidAuthToken= @"fetch-new-valid-auth-token"; // fetch the valid auth token from your server
   *      HelpshiftUser helpshiftUser = new HelpshiftUser.Builder(user.getIdentifier(), user.getEmail())
   *                                                     .name(user.getName())
   *                                                     .authToken(newValidAuthToken)
   *                                                     .build();
   *      Core.login(helpshiftUser);
   *   }
   * </pre>
   *
   * @param user   The logged-in user for which the authentication failed
   * @param reason The reason for authentication failure
   */
   @Override
   public void authenticationFailed(HelpshiftUser user, AuthenticationFailureReason reason) {
       HelpshiftBridge.authenticationFailed(user, reason.getValue());
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

  /**
   * Delegate method which is called when a new helpshift notification is received on device.
   *
   * @param count of new messages
   */
  @Override
  public void didReceiveNotification(int newMessagesCount) {
    HelpshiftBridge.didReceiveNotification(newMessagesCount);
  }

  /**
   * Delegate method which is called when a conversation is ended.
   */
  @Override
  public void conversationEnded() {
    HelpshiftBridge.conversationEnded();
  }

}
