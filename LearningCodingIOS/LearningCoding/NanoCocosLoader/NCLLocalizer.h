//
//  NCLLocalizer.h
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#ifndef __KnightKingdom__NCLLocalizer__
#define __KnightKingdom__NCLLocalizer__

#include <string>

#pragma mark - NCLLocalizer
class NCLLocalizer {
public:
	NCLLocalizer();
	virtual ~NCLLocalizer();
	
	virtual std::string getLocalizedString( const std::string& input );
	
#pragma mark Localizer statics
private:
	static NCLLocalizer* globalLocalizer;
public:
	static std::string localizedString( const std::string& input );
	
	static  NCLLocalizer* getGlobalLocalizer();
	static  void setGlobalLocalizer(NCLLocalizer* l);
};

#endif /* defined(__KnightKingdom__NCLLocalizer__) */
