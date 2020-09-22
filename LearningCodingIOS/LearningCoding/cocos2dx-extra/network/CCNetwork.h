
#ifndef __CC_EXTENSION_CCNETWORK_H_
#define __CC_EXTENSION_CCNETWORK_H_

#include "cocos2dx_extensions.h"

NS_CC_EXT_BEGIN

typedef enum {
    CCNetworkStatusNotReachable = 0,
    CCNetworkStatusReachableViaWiFi,
    CCNetworkStatusReachableViaWWAN
} CCNetworkStatus;

class CCNetwork
{
public:
#pragma mark -
#pragma mark reachability
    
#ifndef CC_UNDER_WIN32
        
    /** @brief Checks whether the default route is available */
    static bool isInternetConnectionAvailable(void);
    
    /** @brief Checks the reachability of a particular host name */
    static bool isHostNameReachable(const char* hostName);
    
    /** @brief Checks Internet connection reachability status */
    static CCNetworkStatus getInternetConnectionStatus(void);
#endif
    
    
private:
    CCNetwork(void) {}
};

NS_CC_EXT_END

#endif // __CC_EXTENSION_CCNETWORK_H_
