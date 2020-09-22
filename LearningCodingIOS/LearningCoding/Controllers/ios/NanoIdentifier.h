//
//  NanoIdentifier.h
//  NanoIdentifier
//
//  Created by Vladimir Kovač on 29/04/15.
//
//

#import <Foundation/Foundation.h>
#import "FCUUID.h"
#import "OpenUDID.h"

@interface NanoIdentifier : NSObject

+ (NSString *)uuidForDevice;

@end

