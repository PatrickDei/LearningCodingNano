//
//  NCLDeviceNodeManager.h
//  NCLCpp
//
//  Created by Bruno on 6/24/13.
//
//

#ifndef __NCLCpp__NCLDeviceNodeManager__
#define __NCLCpp__NCLDeviceNodeManager__

#include "NCLWidget.h"

// Comment this to remove the warning of deprecation
// #define NOTIFY_DEPRECATED

class NCLDeviceNodeManager : public NCLWidget
{
public:
	
	NCLDeviceNodeManager( const char* plistFile, const NCLLoadingSettings& settings = NCLLoadingSettings() );
	NCLDeviceNodeManager( const char* plistFile, NCLResponder* responder, CEFlowLayoutDataSource* tableDataSource, CEFlowLayoutDelegate* tableDelegate );
	~NCLDeviceNodeManager();
	
};

#endif /* defined(__NCLCpp__NCLDeviceNodeManager__) */
