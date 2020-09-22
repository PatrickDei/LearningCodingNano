//
//  NCLDeviceNodeManager.cpp
//  NCLCpp
//
//  Created by Bruno on 6/24/13.
//
//

#include "NCLDeviceNodeManager.h"
#include "NCLDeviceInfo.h"

using namespace std;
using namespace cocos2d;


#pragma mark - NCLDeviceNodeManager

NCLDeviceNodeManager::NCLDeviceNodeManager( const char* plistFile, const NCLLoadingSettings& settings ) :
	NCLWidget( plistFile, settings )
{	
}

NCLDeviceNodeManager::NCLDeviceNodeManager( const char* plistFile, NCLResponder* responder, CEFlowLayoutDataSource* tableDataSource, CEFlowLayoutDelegate* tableDelegate ) :
	NCLWidget( plistFile, responder, tableDataSource, tableDelegate )
{
	
}

NCLDeviceNodeManager::~NCLDeviceNodeManager()
{
}
