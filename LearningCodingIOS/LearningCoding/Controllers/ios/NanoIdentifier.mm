//
//  NanoIdentifier.m
//  NanoIdentifier
//
//  Created by Vladimir Kovač on 29/04/15.
//
//

#import "NanoIdentifier.h"

@implementation NanoIdentifier

static NSString *const kOpenUDIDKey = @"OpenUDID";

+ (NSString *)uuidForDevice
{
  NSString *uuid;

  BOOL userHasFcUuid = [FCUUID checkIfUdidExists];
  BOOL userHasOpenUdid = [OpenUDID checkIfUdidExists];
  if (userHasFcUuid) {
    // Usert has a FCUUID. Return it.
    uuid = [FCUUID uuidForDevice];
    if (!userHasOpenUdid) {
      [OpenUDID saveUdid:uuid];
    }
  } else if (userHasOpenUdid) {
    // User has an OpenUDID but not a FCUUID. Save it using FCUUID.
    uuid = [FCUUID uuidForDeviceMigratingValueForKey:kOpenUDIDKey commitMigration:YES forceMigration:YES];
  } else {
    // User doesn't have a FCUUID or an OpenUDID. Create a new FCUUID and save it as the OpenUDID and FCUUID.
    uuid = [FCUUID uuidForDevice];
    [OpenUDID saveUdid:uuid];
  }
  return uuid;
}

@end
