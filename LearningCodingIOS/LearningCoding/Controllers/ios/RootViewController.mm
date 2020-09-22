//
//  RootViewController.mm
//  LearningCoding
//
//  Created by Lana Lisjak on 18/05/16.
//  Copyright __MyCompanyName__ 2016. All rights reserved.
//

#import "RootViewController.h"

@implementation RootViewController

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger)supportedInterfaceOrientations
{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskPortrait;
#endif
}

- (BOOL)shouldAutorotate { return YES; }

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidLoad {
    [super viewDidLoad];
    if (@available(iOS 11.0, *)) {
        // This should only be necessary if you changed the return value of the property
        // `BOOL preferredScreenEdgesDeferringSystemGestures`, but in limited testing needed this
        [self setNeedsUpdateOfScreenEdgesDeferringSystemGestures];
    } else {
        // Fallback on earlier versions
    }
}

// NOTE(steve): meant more for hiding in passive use where touches are infrequent
//- (BOOL)prefersHomeIndicatorAutoHidden {
//    // NOTE(steve): setting this to yes causes the defer system edge gestures to not work
//    //return YES;
//    return NO;
//}

//- (UIRectEdge)preferredScreenEdgesDeferringSystemGestures
//{
////    return UIRectEdgeAll;
//    // NOTE(steve): should probably just defer the bottom swipe (home gesture)
//    return UIRectEdgeBottom;
//}

- (void)dealloc { [super dealloc]; }

@end
