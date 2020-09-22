
#include "CCNetwork_objc.h"

#pragma mark -
#pragma mark Reachability

@implementation CCNetwork_objc

+(BOOL) isInternetConnectionAvailable
{
    return [[Reachability reachabilityForInternetConnection] currentReachabilityStatus] != NotReachable;
}

+(BOOL) isHostNameReachable:(NSString *)hostName
{
    return [[Reachability reachabilityWithHostName:hostName] currentReachabilityStatus] != NotReachable;
}

+(NetworkStatus) getInternetConnectionStatus
{
    NetworkStatus status = [[Reachability reachabilityForInternetConnection] currentReachabilityStatus];
    if (status == ReachableViaWiFi) return NetworkStatus::ReachableViaWiFi;
    if (status == ReachableViaWWAN) return NetworkStatus::ReachableViaWWAN;
    return NetworkStatus::NotReachable;
}

@end
