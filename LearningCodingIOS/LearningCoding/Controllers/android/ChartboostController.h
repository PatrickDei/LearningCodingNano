//
// Created by Dominik Baričević on 18/10/2018.
//

#define CROSS_PROMO_VIDEO "chartboost_reward_video"
#define CROSS_PROMO_INTERSTITIAL "chartboost_interstitial"

class ChartboostController
{
public:
    static void showCrossPromo();
    static bool hasVideo();
    static void cacheVideo();

    static void resumeMusic();
    static void checkHasVideo();

    static void hasInterstitial();
    static void showInterstitial();

    static bool isVideoAvailable;
};
