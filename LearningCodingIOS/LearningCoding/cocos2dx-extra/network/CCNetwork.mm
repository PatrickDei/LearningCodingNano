
#include "CCNetwork.h"
#include "CCNetwork_objc.h"


NS_CC_EXT_BEGIN

#pragma mark -
#pragma mark Reachability

bool CCNetwork::isInternetConnectionAvailable(void)
{
    return [CCNetwork_objc isInternetConnectionAvailable];
}

CCNetworkStatus CCNetwork::getInternetConnectionStatus(void)
{
    return (CCNetworkStatus)[CCNetwork_objc getInternetConnectionStatus];
}

bool CCNetwork::isHostNameReachable(const char* hostName)
{
    return [CCNetwork_objc isHostNameReachable:[NSString stringWithUTF8String:hostName]];
}


NS_CC_EXT_END
