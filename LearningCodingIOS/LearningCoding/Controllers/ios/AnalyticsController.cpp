//
//  AnalyticsController.cpp
//  NanoGirl
//
//  Created by Filip Kirsek on 19/09/16.
//
// This class only forwards event calls to Omniata
// It can be used to add other event-tracking frameworks later on, as well as remove the current.
// If new events are needed, add them to this class, and call them at the appropriate moments in the game.

#include "AnalyticsController.h"
#include "OmniataController.h"
#include "NGSAnalyticsController.h"

#define CUSTOM_DIMENSION_AGE 0
#define CUSTOM_DIMENSION_GROUP 1
#define CUSTOM_DIMENSION_BUILD 2

#pragma mark - Wrapper

void AnalyticsController::startSession(bool isTesting)
{
    OmniataController::startSession(isTesting);
    NGSAnalyticsController::startSession(isTesting);
}

void AnalyticsController::closeSession()
{
    //NGSAnalyticsController::closeSession();
}

void AnalyticsController::resumeSession()
{
    //NGSAnalyticsController::resumeSession();
}


#pragma mark - Localytics tagEvent

void AnalyticsController::gameStarted(bool fbConnected, bool devicePushEnabled, bool inGamePushEnabled, std::string build)
{
    NGSAnalyticsController::gameStarted(fbConnected, devicePushEnabled, inGamePushEnabled, build);
    OmniataController::gameStarted(fbConnected, devicePushEnabled, inGamePushEnabled, build);
}

void AnalyticsController::gameEnded()
{
    NGSAnalyticsController::gameEnded();
    OmniataController::gameEnded();
}

#pragma mark - InApps


void AnalyticsController::purchaseCompleted(double price, std::string purchaseType, int offerId, std::string offerName,
                                            std::string currencyType, int amountBought)
{
    AnalyticsController::purchaseCompleted( price,  purchaseType,  offerId,  offerName,
                                           currencyType,  amountBought) ;
    NGSAnalyticsController::purchaseCompleted( price,  purchaseType,  offerId,  offerName,
                                              currencyType,  amountBought) ;
}

void AnalyticsController::purchaseFailed()
{
    OmniataController::purchaseFailed();
    NGSAnalyticsController::purchaseFailed();
}

void AnalyticsController::purchaseVerificationConnectFailed()
{
   // OmniataController::purchaseVerificationConnectFailed();
    NGSAnalyticsController::purchaseVerificationConnectFailed();
}


void AnalyticsController::setPushRetStr(std::string pRet)
{
    OmniataController::pushRetStr = pRet;
    NGSAnalyticsController::pushRetStr = pRet;
}

void AnalyticsController::clearPushRetStr()
{
    OmniataController::pushRetStr.clear();
    NGSAnalyticsController::pushRetStr.clear();
}

void AnalyticsController::pushEnabled()
{
    OmniataController::pushEnabled();
    NGSAnalyticsController::pushEnabled();
}
void AnalyticsController::verificationFailed()
{
    OmniataController::verificationFailed();
    NGSAnalyticsController::verificationFailed();
}


void AnalyticsController::chapterCompleted(std::string storyId, std::string chapterId)
{
    OmniataController::chapterCompleted(storyId, chapterId);
    NGSAnalyticsController::chapterCompleted(storyId, chapterId);
}

void AnalyticsController::storyCompleted(std::string storyId)
{
    OmniataController::storyCompleted(storyId);
    NGSAnalyticsController::storyCompleted(storyId);
}

void AnalyticsController::storyUnlocked(std::string storyId)
{
    OmniataController::storyUnlocked(storyId);
    NGSAnalyticsController::storyUnlocked(storyId);
}

void AnalyticsController::chapterStarted(std::string storyId, std::string chapterId)
{
    OmniataController::chapterStarted(storyId, chapterId);
    NGSAnalyticsController::chapterStarted(storyId, chapterId);
}

void AnalyticsController::chapterUnlocked(std::string storyId, std::string chapterId)
{
    OmniataController::chapterUnlocked(storyId, chapterId);
    NGSAnalyticsController::chapterUnlocked(storyId, chapterId);
}

void AnalyticsController::chapterDropped(std::string storyId, std::string chapterId)
{
    OmniataController::chapterDropped(storyId, chapterId);
    NGSAnalyticsController::chapterDropped(storyId, chapterId);
}

void AnalyticsController::notEnoughResources(std::string currencyType, std::string category, std::string storyId,
                               std::string chapterId, std::string itemId, int currencyPrice)
{
    OmniataController::notEnoughResources(currencyType, category, storyId, chapterId, itemId, currencyPrice);
    NGSAnalyticsController::notEnoughResources(currencyType, category, storyId, chapterId, itemId, currencyPrice);
}

void AnalyticsController::currencySpent(std::string currencyType, std::string category, std::string storyId,
                                             std::string chapterId, std::string itemId, int currencyPrice)
{
    OmniataController::currencySpent(currencyType, category, storyId, chapterId, itemId, currencyPrice);
    NGSAnalyticsController::currencySpent(currencyType, category, storyId, chapterId, itemId, currencyPrice);
}



