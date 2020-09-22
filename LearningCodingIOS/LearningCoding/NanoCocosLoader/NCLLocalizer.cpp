//
//  NCLLocalizer.cpp
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#include "NCLLocalizer.h"

#pragma mark NCLLocalizer

NCLLocalizer* NCLLocalizer::globalLocalizer = nullptr;

NCLLocalizer::NCLLocalizer()
{
	if( globalLocalizer == nullptr )
		setGlobalLocalizer(this);
}

NCLLocalizer::~NCLLocalizer()
{
	if( globalLocalizer == this )
		setGlobalLocalizer(nullptr);
}

std::string NCLLocalizer::getLocalizedString(const std::string &input)
{
	return std::string(input);
}

#pragma mark Localizer statics

std::string NCLLocalizer::localizedString(const std::string& input)
{
	if( globalLocalizer )
	{
		return globalLocalizer->getLocalizedString(input);
	}
	return std::string( input );
}

NCLLocalizer* NCLLocalizer::getGlobalLocalizer()
{
	return globalLocalizer;
}

void NCLLocalizer::setGlobalLocalizer(NCLLocalizer *l)
{
	globalLocalizer = l;
}