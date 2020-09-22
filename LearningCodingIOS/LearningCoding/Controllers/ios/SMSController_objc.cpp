//
//  SMSController_objc.m
//  NanoGirl
//
//  Created by Matko Juribasic on 19/03/14.
//
//

#import "SMSController_objc.h"
//#import "SynthesizeSingleton.h"

//#include "SMSController.h"

//@implementation SMSController_objc

void sendSMS(const char * body , SMSController* handler)
{
//    smsController = handler;
//    
//    if ([MFMessageComposeViewController canSendText])
//    {
//        smsViewController = [[[MFMessageComposeViewController alloc] init] autorelease];
//        smsViewController.messageComposeDelegate = self;
//        smsViewController.body = body;
//        
//        [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:smsViewController animated:YES completion:nil];
//    }
//    else
//    {
//        smsController->smsSent(false);
//    }
}

bool canSendSMS()
{
//    return [MFMessageComposeViewController canSendText];
    return false;
}

//- (void)messageComposeViewController:(MFMessageComposeViewController *)controller didFinishWithResult:(MessageComposeResult)result
//{
//    switch (result)
//    {
//        case MessageComposeResultSent:
//            smsController->smsSent(true);
//            break;
//        case MessageComposeResultCancelled:
//        case MessageComposeResultFailed:
//        default:
//            smsController->smsSent(false);
//            break;
//    }
//    
//    [smsViewController dismissViewControllerAnimated:YES completion:nil];
//}

#pragma mark -
#pragma mark                        S I N G L E T O N • B E H A V I O U R

//SYNTHESIZE_SINGLETON_FOR_CLASS(SMSController_objc);

/*┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
 ┃ the SYNTHESIZE_SINGLETON_FOR_CLASS macro expands thus:                                           ┃
 ┃──────────────────────────────────────────────────────────────────────────────────────────────────┃
 ┃                                                                                                  ┃
 ┃         static Singleton *sharedSingleton = ((void*)0);                                          ┃
 ┃                                                                                                  ┃
 ┃         + (Singleton *)sharedSingleton {                                                         ┃
 ┃             @synchronized(self) {                                                                ┃
 ┃                 if (sharedSingleton == ((void*)0)) {                                             ┃
 ┃                     sharedSingleton = [[self alloc] init];                                       ┃
 ┃                 }                                                                                ┃
 ┃             }                                                                                    ┃
 ┃             return sharedSingleton;                                                              ┃
 ┃         }                                                                                        ┃
 ┃                                                                                                  ┃
 ┃         + (id)allocWithZone:(NSZone *)zone {                                                     ┃
 ┃             @synchronized(self) {                                                                ┃
 ┃                 if (sharedSingleton == ((void*)0)) {                                             ┃
 ┃                     sharedSingleton = [super allocWithZone:zone];                                ┃
 ┃                     return sharedSingleton;                                                      ┃
 ┃                 }                                                                                ┃
 ┃             }                                                                                    ┃
 ┃             return ((void*)0);                                                                   ┃
 ┃         }                                                                                        ┃
 ┃                                                                                                  ┃
 ┃         - (id)copyWithZone:(NSZone *)zone {                                                      ┃
 ┃             return self;                                                                         ┃
 ┃         }                                                                                        ┃
 ┃                                                                                                  ┃
 ┃         - (id)retain {                                                                           ┃
 ┃             return self;                                                                         ┃
 ┃         }                                                                                        ┃
 ┃                                                                                                  ┃
 ┃         - (NSUInteger)retainCount {                                                              ┃
 ┃             return (2147483647L *2UL +1UL);                                                      ┃
 ┃         }                                                                                        ┃
 ┃                                                                                                  ┃
 ┃         - (void)release {                                                                        ┃
 ┃         }                                                                                        ┃
 ┃                                                                                                  ┃
 ┃         - (id)autorelease {                                                                      ┃
 ┃             return self;                                                                         ┃
 ┃         };                                                                                       ┃
 ┃                                                                                                  ┃
 ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛*/

//@end