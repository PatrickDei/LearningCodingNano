/*
//
//  SupersonicController.m
//  Hollywood
//
//  Created by Marko Vrljičak on 20/07/15.
//
//

//#import <Foundation/Foundation.h>
#import "SupersonicController.h"
#import "AppKeys.h"
//#import <AdSupport/ASIdentifierManager.h>
#import "SynthesizeSingleton.h"
//#import <Supersonic/SUSupersonicAdsConfiguration.h>

#include "GameState.h"
#include "Parameters.h"
#include "NGSTime.h"
#include "VideoRewardPopup.h"
#include "HudLayer.h"
#include "Constants.h"
#include "SoundManager.h"
#include "InAppPurchaseManager.h"
#include "AdController.h"

//@implementation SupersonicController

//- (id) init
//{
////	if (self = [super init])
////	{
////		NSString* userId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
////		[[Supersonic sharedInstance] setLogDelegate:self];
////		[[Supersonic sharedInstance] setRVDelegate:self];
////		[[Supersonic sharedInstance] initRVWithAppKey:SUPERSONIC_APP_ID withUserId:userId];
////		[[Supersonic sharedInstance] setISDelegate:self];
////		[[Supersonic sharedInstance] initISWithAppKey:SUPERSONIC_APP_ID withUserId:userId];
////		[[Supersonic sharedInstance] setOWDelegate:self];
////		[SUSupersonicAdsConfiguration getConfiguration].useClientSideCallbacks = [NSNumber numberWithInt:1];
////		[[Supersonic sharedInstance] initOWWithAppKey:SUPERSONIC_APP_ID withUserId:userId];
////	}
//	return self;
//}

void startRewardedVideo()
{
//	[[Supersonic sharedInstance] showRV];
}

void startOfferWall()
{
//	[[Supersonic sharedInstance] showOW];
}

void startInterstitial()
{
//	[[Supersonic sharedInstance] showIS];
}

bool SupersonicController::isRewardedVideoAvailable()
{
//	return [[Supersonic sharedInstance] isAdAvailable];
    return false;
}

bool isOfferWallAvailable()
{
//	if (GameState::get() && Parameters::getParameters()->intForKey("minLevelForOfferWall") <= GameState::get()->getPlayerLevel())
//	{
//		if(InAppPurchaseManager::get() &&
//		   InAppPurchaseManager::get()->getUserCategory() == InAppPurchaseManager::UserCategory::kUserCategoryZombie)
//		{
//			return [[Supersonic sharedInstance] isOWAvailable];
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}
    return false;
}

bool SupersonicController::isInterstitialAvailable()
{
//	if(GameState::get() && InAppPurchaseManager::get() &&
//	   InAppPurchaseManager::get()->getUserCategory() == InAppPurchaseManager::UserCategory::kUserCategoryZombie)
//		return [[Supersonic sharedInstance] isISAdAvailable];
//	else
//		return false;
    return false;
}

#pragma mark - Log Delegate

//- (void) sendLog:(NSString *)log level:(LogLevel)level tag:(LogTag)tag
//{
//	NSLog(@"Supersonic: Log: %@", log);
//}

#pragma mark - Offer Wall Delegate

//- (void) supersonicOWInitSuccess
//{
//	NSLog(@"Supersonic: Offer Wall initialized.");
//}
//
//- (void) supersonicOWInitFailedWithError:(NSError *)error
//{
//	NSLog(@"Supersonic: Offer Wall initialization failed.");
//}
//
//- (void) supersonicOWShowSuccess
//{
//	NSLog(@"Supersonic: Offer Wall displayed.");
//}
//
//- (void) supersonicOWShowFailedWithError:(NSError *)error
//{
//	NSLog(@"Supersonic: Offer Wall displaying failed.");
//}
//
//- (void) supersonicOWFailGettingCreditWithError:(NSError *)error
//{
//	NSLog(@"Supersonic: Offer Wall failed to get credit.");
//}
//
//- (void) supersonicOWAdClosed
//{
//	NSLog(@"Supersonic: Offer Wall closed.");
//	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationRefreshInAppMenu);
//}
//
//- (BOOL) supersonicOWDidReceiveCredit:(NSDictionary *)creditInfo
//{
//	if ([[creditInfo valueForKey:@"credits"] intValue] > 0)
//	{
//		if(GameState::get())
//		{
//			GameState::get()->addPlayerGems([[creditInfo valueForKey:@"credits"] intValue]);
//		}
//		else
//		{
//			[[NSUserDefaults standardUserDefaults] setValue:[creditInfo valueForKey:@"credits"] forKey:@kUserDefaultUncollectedOWRewards];
//		}
//	}
//	return TRUE;
//}

#pragma mark - Interstitial Delegate

//- (void) supersonicISInitSuccess
//{
//	NSLog(@"Supersonic: Interstitial initialized.");
//}
//
//- (void) supersonicISInitFailedWithError:(NSError *)error
//{
//	NSLog(@"Supersonic: Interstitial initialization failed.");
//}
//
//- (void) supersonicISShowSuccess
//{
//	NSLog(@"Supersonic: Interstitial displayed.");
//}
//
//- (void) supersonicISShowFailWithError:(NSError *)error
//{
//	NSLog(@"Supersonic: Interstitial displaying failed.");
//}
//
//- (void) supersonicISAdClicked
//{
//	
//}
//
//- (void) supersonicISAdClosed
//{
//	
//}
//
//- (void) supersonicISAdAvailable:(BOOL)available
//{
//	
//}

#pragma mark - Rewarded Video Delegate

//- (void) supersonicRVInitSuccess
//{
//	NSLog(@"Supersonic: Rewarded Video initialized.");
//}
//
//- (void) supersonicRVInitFailedWithError:(NSError *)error
//{
//	NSLog(@"Supersonic: Rewarded Video initialization failed.");
//}
//
//- (void) supersonicRVAdAvailabilityChanged:(BOOL)hasAvailableAds
//{
//	NSLog(@"Supersonic: Rewarded Video availability changed. %s", hasAvailableAds ? "Ads available!" : "Ads not available.");
//}
//
//- (void) supersonicRVAdOpened
//{
//	NSLog(@"Supersonic: Rewarded Video ad opened.");
//	if(GameState::get() && GameState::get()->getProperties()->intForKey(kPropertySettingsMusicOn))
//		SoundManager::get()->fadeOutBackgroundMusic();
//}
//
//- (void) supersonicRVAdStarted
//{
//	NSLog(@"Supersonic: Rewarded Video ad started.");
//}
//
//- (void) supersonicRVAdEnded
//{
//	NSLog(@"Supersonic: Rewarded Video ad ended.");
//}
//
//- (void) supersonicRVAdClosed
//{
//	NSLog(@"Supersonic: Rewarded Video ad closed.");
//	if(GameState::get() && GameState::get()->getProperties()->intForKey(kPropertySettingsMusicOn))
//	{
//		SoundManager::get()->fadeInBackgroundMusic();
//	}
//	cocos2d::CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationRefreshInAppMenu);
//}
//
//- (void) supersonicRVAdRewarded:(SupersonicPlacementInfo *)placementInfo
//{
//	NSNumber * rewardAmount = [placementInfo rewardAmount];
//	NSString * rewardName = [placementInfo rewardName];
//	if (GameState::get())
//	{
//		if (rewardAmount > 0 && [rewardName isEqual: @"gems"])
//		{
//			GameState::get()->addPlayerGems([rewardAmount integerValue]);
//		}
//		else
//		{
//			[[NSUserDefaults standardUserDefaults] setInteger:1 forKey:@kUserDefaultUncollectedAdRewards];
//			AdController::rewardedVideoWatched();
//		}
//	}
//	else
//	{
//		[[NSUserDefaults standardUserDefaults] setInteger:1 forKey:@kUserDefaultUncollectedAdRewards];
//	}
//}
//
//- (void) supersonicRVAdFailedWithError:(NSError *)error
//{
//	
//}
//
//SYNTHESIZE_SINGLETON_FOR_CLASS(SupersonicController);

//@end


*/
