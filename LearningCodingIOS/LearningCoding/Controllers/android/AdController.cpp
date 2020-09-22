//
//  AdController.mm
//  Hollywood
//
//  Created by Marko Vrljičak on 31/08/15.
//
//

#include "AdController.h"
#include "AnalyticsController.h"
#include "AppDelegate.h"
#include "ChartboostController.h"
#include "CrossPromotionController.h"
#include "CrossPromotion.h"
#include "CrossPromotionProfile.h"
#include "FyberWrapper.h"
#include "GameState.h"
#include "HudLayer.h"
#include "InAppPurchaseManager.h"
#include "IronSourceWrapper.h"
#include "NGSTime.h"
#include "Parameters.h"
#include "RewardPopup.h"
#include "TicketController.h"

using namespace cocos2d;

AdController::RewardType AdController::rewardType = SPEEDUP;
void AdController::init ()
{
    CCLog ("AdController::init");

    FyberWrapper::init ();
    IronSourceWrapper::init();
}

void AdController::onGameStarted ()
{
    FyberWrapper::get()->start();
    checkUncollectedRewards ();
}

void AdController::checkUncollectedRewards ()
{
    checkUncollectedVideoRewards ();
    checkUncollectedOfferwallRewards ();
}

void AdController::checkUncollectedVideoRewards ()
{
    int uncollectedReward = cocos2d::CCUserDefault::sharedUserDefault ()->getIntegerForKey (
            kUserDefaultUncollectedFyberRewards, AdController::RewardType::NONE);

    if (uncollectedReward > AdController::RewardType::NONE) {
        addVideoRewardForType (static_cast<AdController::RewardType> (uncollectedReward), "");
        cocos2d::CCUserDefault::sharedUserDefault ()->setIntegerForKey (kUserDefaultUncollectedFyberRewards,
                                                                        AdController::RewardType::NONE);
    }
}

void AdController::checkUncollectedOfferwallRewards ()
{
    int uncollectedOfferWallReward =
            CCUserDefault::sharedUserDefault ()->getIntegerForKey (kUserDefaultUncollectedOfferWallRewards, 0);
    if (uncollectedOfferWallReward > 0) {
        AdController::offerWallReward (uncollectedOfferWallReward);
        cocos2d::CCUserDefault::sharedUserDefault ()->setIntegerForKey (kUserDefaultUncollectedOfferWallRewards, 0);
    }
}

void AdController::addVideoRewardForType (AdController::RewardType type, std::string source)
{
    int gemsAmount = 0;

    if (type == SPEEDUP) {
        TicketController::get ()->rewardVideoWatchedSpeedup (source);
    } else if (type == DIAMONDS) {
        gemsAmount = Parameters::getParameters ()->dictForKey("IAP")->intForKey ("iappVideoReward");
        GameState::get ()->addPlayerGems (gemsAmount);

        // update lastWatchedTimeGems
        auto nowStamp = NGSTime::currentTimeServer ();
        cocos2d::CCUserDefault::sharedUserDefault ()->setDoubleForKey (kUserDefaultGemsVideoLastWatchedTime, nowStamp);
        AnalyticsController::videoWatched ("DailyReward", std::to_string (gemsAmount), source, "Gems");
    } else if (type == ENDCHAPTER) {
        gemsAmount = 1;
        GameState::get ()->addPlayerGems (gemsAmount);
        AnalyticsController::videoWatched ("EndChapterReward", std::to_string (gemsAmount), source, "Gems");
    }

    if (gemsAmount > 0) {
        if(AppDelegate::getInstance()->isGlReady())
        {
            HudLayer::get()->showAdCollectPopupForAmount(gemsAmount);
        } else {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(kFyberGemRewardAmount, gemsAmount);
            CCNotificationCenter::sharedNotificationCenter()->addObserver(
                    HudLayer::get() , callfuncO_selector(HudLayer::showAdCollectPopup), GL_CONTEXT_READY, nullptr);
        }
    };
}

void AdController::prepareInterstitial ()
{
#if DISABLE_ADS == 0
    FyberWrapper::requestInterstitial ();
#endif
}

bool AdController::isRewardedVideoAvailableGems ()
{
    bool isAvailable = false;
#if DISABLE_ADS == 0
    isAvailable = FyberWrapper::isVideoAvailable ();

    auto lastWatched =
        cocos2d::CCUserDefault::sharedUserDefault ()->getDoubleForKey (kUserDefaultGemsVideoLastWatchedTime, 0);
    double secondsUntilNextVideo = Parameters::get ()->getParameters ()->intForKey ("gemsVideoDelayMinutes", 480) * 60;
    auto nowStamp = NGSTime::currentTimeServer ();
    bool timeLimitPassed = (lastWatched + secondsUntilNextVideo < nowStamp);

    isAvailable = isAvailable && timeLimitPassed;
#endif
    return isAvailable;
}

bool AdController::isRewardedVideoAvailable ()
{
    bool isAvailable = false;
#if DISABLE_ADS == 0
    isAvailable = FyberWrapper::isVideoAvailable ();
#endif
    return isAvailable;
}

bool AdController::isInterstitialAvailable ()
{
    bool isAvailable;
#if DISABLE_ADS == 0
    isAvailable = FyberWrapper::isInterstitialAvailable ();
#endif
    return isAvailable;
}

void AdController::showRewardedVideo (RewardType _rewardType)
{
    AdController::rewardType = _rewardType;

    if (CrossPromotionController::get()->isCrossPromotionActive()) {

        CrossPromotionController::get()->refreshDisplayCount();
        CrossPromotion *activeCrossPromotion = GameState::get()->getActiveCrossPromotion();
        int displayCount = activeCrossPromotion->getVideoDisplayCount();

        //check display count
        if (displayCount < activeCrossPromotion->getLevel()->getVideoDisplayCount()) {
            showChartboostCrossPromoVideo();
            activeCrossPromotion->setVideoDisplayCount(++displayCount);
        }
        //check frequency
        int frequency = activeCrossPromotion->getLevel()->getVideoFrequency();
        if (frequency != 0) {
            if (displayCount % frequency == 0) {
                showChartboostCrossPromoVideo();
                activeCrossPromotion->setVideoDisplayCount(++displayCount);
            }
            else {
                if (isRewardedVideoAvailable ()) {
                    FyberWrapper::playVideo ();
                    activeCrossPromotion->setVideoDisplayCount(++displayCount);
                }
            }
        }
        else {
            if (isRewardedVideoAvailable ()) {
                FyberWrapper::playVideo ();
                activeCrossPromotion->setVideoDisplayCount(++displayCount);
            }
        }
    }
    else {
        if (isRewardedVideoAvailable ()) {
            FyberWrapper::playVideo ();
        }
    }

}

void AdController::showInterstitial ()
{
    if (CrossPromotionController::get()->isCrossPromotionActive()) {

        CrossPromotionController::get()->refreshDisplayCount();
        CrossPromotion *activeCrossPromotion = GameState::get()->getActiveCrossPromotion();
        int displayCount = activeCrossPromotion->getInterstitialDisplayCount();

        //check display count
        if (displayCount < activeCrossPromotion->getLevel()->getInterstitialDisplayCount()) {
            showChartboostCrossPromoInterstitial();
            activeCrossPromotion->setInterstitialDisplayCount(++displayCount);
        }
        //check frequency
        else {
            int frequency = activeCrossPromotion->getLevel()->getInterstitalFrequency();
            if (frequency != 0) {
                if (displayCount % frequency == 0) {
                    showChartboostCrossPromoInterstitial();
                    activeCrossPromotion->setInterstitialDisplayCount(++displayCount);
                }
                else if (isInterstitialAvailable ()) {
                    AnalyticsController::interstitialAdWatched ("Fyber");
                    FyberWrapper::showInterstitial ();
                    activeCrossPromotion->setInterstitialDisplayCount(++displayCount);
                }
            }
            else if (isInterstitialAvailable ()) {
                AnalyticsController::interstitialAdWatched ("Fyber");
                FyberWrapper::showInterstitial ();
                activeCrossPromotion->setInterstitialDisplayCount(++displayCount);
            }
        }
    }
    else if (isInterstitialAvailable ()) {
        AnalyticsController::interstitialAdWatched ("Fyber");
        FyberWrapper::showInterstitial ();
    }

}

std::set<int> AdController::offerWallUserCategories ()
{
    CCDictionary *offerWall = Parameters::get ()->getParameters ()->dictForKey ("offerWall");
    CCArray *userCategories = offerWall->arrayForKey ("userCategories");
    std::set<int> categories;
    CCARRAY_FOREACH_TYPE (userCategories, cocos2d::CCString, userCategory)
    {
        int category = InAppPurchaseManager::userCategoryForString (userCategory->getCString ());
        categories.insert (category);
    }
    return categories;
}

bool AdController::isOfferWallAvailable ()
{
    bool isAvailable = false;

    if (GameState::get () && Parameters::get ()) {
        CCDictionary *offerWall = Parameters::get()->getParameters()->dictForKey("offerWall");

        std::set<int> userCategories = AdController::offerWallUserCategories();
        bool correctUserCategory = userCategories.find(InAppPurchaseManager::getUserCategory()) !=
                                   userCategories.end();
        int neededDaysPlayed = ((CCString *) offerWall->objectForKey("daysPlayed"))->intValue();
        bool enoughDaysPlayed = GameState::get()->getDaysPlayed() >= neededDaysPlayed;
        int chaptersRead = ((CCString *) offerWall->objectForKey("chaptersRead"))->intValue();
        bool enoughChaptersRead = GameState::get()->getChaptersRead() >= chaptersRead;
        int sessions = ((CCString *) offerWall->objectForKey("sessions"))->intValue();
        bool enoughSessions = GameState::get()->getSessionNumAll() >= sessions;

        bool conditionsSatisfied =
                correctUserCategory && enoughDaysPlayed && enoughChaptersRead && enoughSessions;

        int alwaysAfterDays = ((CCString *) offerWall->objectForKey("alwaysAfterDays"))->intValue();
        bool enoughDaysPlayedForever = GameState::get()->getDaysPlayed() >= alwaysAfterDays;

        bool isIronSourceAvaiable = IronSourceWrapper::isOfferWallAvailable();

        isAvailable = isIronSourceAvaiable && (conditionsSatisfied || enoughDaysPlayedForever) && !AppDelegate::getInstance ()->isGameOffline ();

        //    CCLog("AdController - correctUserCategory %d", correctUserCategory);
        //    CCLog("AdController - daysPlayed %d", GameState::get ()->getDaysPlayed ());
        //    CCLog("AdController - neededDaysPlayed %d", neededDaysPlayed);
        //    CCLog("AdController - enoughDaysPlayed %d", enoughDaysPlayed);
        //    CCLog("AdController - enoughChaptersRead %d", enoughChaptersRead);
        //    CCLog("AdController - enoughSessions %d", enoughSessions);
        //    CCLog("AdController - alwaysAfterDays %d", enoughDaysPlayedForever);
        //    CCLog("AdController - isAvailable %d", isAvailable);
    }

    return isAvailable;
}

void AdController::showOfferWall ()
{
    if (isOfferWallAvailable ()) {
        AnalyticsController::offerWallClicked ();
        IronSourceWrapper::showOfferWall ();
    }
}

void AdController::rewardedVideoWatched (std::string source)
{
    int oldDisplayedAds =
        cocos2d::CCUserDefault::sharedUserDefault ()->getIntegerForKey (kUserDefaultTotalAdsWatched, 0);
    ++oldDisplayedAds;
    cocos2d::CCUserDefault::sharedUserDefault ()->setIntegerForKey (kUserDefaultTotalAdsWatched, oldDisplayedAds);

    if (GameState::get())
    {
        addVideoRewardForType (AdController::rewardType, source);
    } else {
        cocos2d::CCUserDefault::sharedUserDefault ()->setIntegerForKey (kUserDefaultUncollectedFyberRewards,
                                                                        AdController::RewardType::SPEEDUP);
    }

    cocos2d::CCNotificationCenter::sharedNotificationCenter ()->postNotification (kRewardedVideoWatched);
}

void AdController::interstitialWatched ()
{
    cocos2d::CCNotificationCenter::sharedNotificationCenter ()->postNotification (kAdFinished);
}

bool AdController::isRewardVideoShowing () { return FyberWrapper::isRewardVideoShowing; }

bool AdController::isInterstitialShowing () { return FyberWrapper::isInterstitialShowing; }

void AdController::offerWallReward (int amount)
{
    if(amount == 0)
        return;
   if(GameState::get()) {
       GameState::get()->addPlayerGems(amount);
       AnalyticsController::offerWallRewardReceived (amount);
   }
    else{
       int uncollectedRewards =  cocos2d::CCUserDefault::sharedUserDefault ()->getIntegerForKey (kUserDefaultUncollectedOfferWallRewards, 0);
       cocos2d::CCUserDefault::sharedUserDefault ()->setIntegerForKey (kUserDefaultUncollectedOfferWallRewards, uncollectedRewards + amount);
   }
}

void AdController::showChartboostCrossPromoVideo()
{
    CrossPromotion *activeCrossPromotion = GameState::get()->getActiveCrossPromotion();
    if (activeCrossPromotion->getFirstShownTimestamp() == 0) {
        activeCrossPromotion->setFirstShownTimestamp(NGSTime::currentTimeServer());
    }

    ChartboostController::showCrossPromo();
}

void AdController::showChartboostCrossPromoInterstitial()
{
    CrossPromotion *activeCrossPromotion = GameState::get()->getActiveCrossPromotion();
    if (activeCrossPromotion->getFirstShownTimestamp() == 0) {
        activeCrossPromotion->setFirstShownTimestamp(NGSTime::currentTimeServer());
    }
    ChartboostController::showInterstitial();
}