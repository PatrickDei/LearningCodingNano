
#include "CCNetwork.h"

#include "JniHelper.h"
#include <jni.h>

//#import "Reachability_objc.h"

NS_CC_EXT_BEGIN

#pragma mark -
#pragma mark Reachability

bool CCNetwork::isInternetConnectionAvailable(void)
{
	//TODO
	return false;
   // return [[Reachability_objc reachabilityForInternetConnection] currentReachabilityStatus] != NotReachable;
}

bool CCNetwork::isHostNameReachable(const char* hostName)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/nanobit/mystory/NanoAndroidFunctions", "isWifiAvailable", "()Z")) {

		jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return ret;
	}
}

CCNetworkStatus CCNetwork::getInternetConnectionStatus(void)
{
//    NetworkStatus status = [[Reachability_objc reachabilityForInternetConnection] currentReachabilityStatus];
//    if (status == ReachableViaWiFi) return CCNetworkStatusReachableViaWiFi;
//    if (status == ReachableViaWWAN) return CCNetworkStatusReachableViaWWAN;
//    return CCNetworkStatusNotReachable;
}

NS_CC_EXT_END
