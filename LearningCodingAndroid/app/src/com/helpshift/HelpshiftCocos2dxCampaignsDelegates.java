/**
 * Copyright (c) 2015-2017 by Helpshift, Inc. All Rights Reserved.
 *
 */
package com.helpshift;

import com.helpshift.campaigns.Campaigns;

class HelpshiftCocos2dxCampaignsDelegates implements Campaigns.Delegate {

  private static final String TAG = "Helpshift_CocosCampaignsDeleg";

  /**
   * Delegate method which is called when Helpshift campaigns session begins.
   */
  @Override
  public void sessionBegan () {
    HelpshiftBridge.helpshiftCampaignsSessionBegan();
  }

  /**
   * Delegate method which is called when Helpshift campaigns session ends.
   */
  @Override
  public void sessionEnded () {
    HelpshiftBridge.helpshiftCampaignsSessionEnded();
  }
}
