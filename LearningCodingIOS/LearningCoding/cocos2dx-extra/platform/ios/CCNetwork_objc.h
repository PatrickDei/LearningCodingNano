//
//  CCNetwork_objc.h
//  FiledCommander
//
//  Created by Darko Ronic on 12/10/13.
//
//

#import <Foundation/Foundation.h>
#import "Reachability.h"

@interface CCNetwork_objc : NSObject

+(BOOL) isInternetConnectionAvailable;
+(NetworkStatus) getInternetConnectionStatus;
+(BOOL) isHostNameReachable:(NSString *)hostName;


@end
