//
//  NCLUtils.cpp
//  KnightKingdom
//
//  Created by Bruno on 2/27/14.
//
//

#include "NCLUtils.h"

using namespace cocos2d;
using namespace std;

namespace NCLUtils {

CCPoint pointFromString( CCString *pointString, CCPoint newPoint ){
	if( pointString ){
		sscanf(pointString->getCString(), "%f,%f", &newPoint.x, &newPoint.y);
	}
	return newPoint;
}

CCSize sizeFromString( CCString *sizeString, CCSize newSize  ){
	if( sizeString ){
		const char* found = strstr( sizeString->getCString(), "," );
		if( found )
		{
			sscanf(sizeString->getCString(), "%f,%f", &newSize.width, &newSize.height);
		}
		else
		{
			sscanf(sizeString->getCString(), "%f", &newSize.width);
			newSize.height = newSize.width;
		}
	}
	return newSize;
}

bool boolFromString( CCString *boolString ){
	return boolString->getCString()[0] != '0';
}

float floatFromString( CCString *floatString ){
	return atof(floatString->getCString());
}

int intFromString( CCString *intString ){
	return atoi(intString->getCString());
}

void normalizeNewline( std::string& content )
{
	while (true)
	{
		auto slashslash = content.find("\\n");
		if( slashslash == std::string::npos ) break;
		content.replace(slashslash, 2, "\n");
	}
}

CCTextAlignment horizontalAlignFromString( CCString* alignString ){
	if( alignString->compare("right") == 0 ){
		return kCCTextAlignmentRight;
	}else if( alignString->compare("center") == 0 ){
		return kCCTextAlignmentCenter;
	}else{
		return kCCTextAlignmentLeft;
	}
}

CCVerticalTextAlignment verticalAlignFromString( CCString* alignString ){
	if( alignString->compare("bottom") == 0 ){
		return kCCVerticalTextAlignmentBottom;
	}else if( alignString->compare("center") == 0 ){
		return kCCVerticalTextAlignmentCenter;
	}else{
		return kCCVerticalTextAlignmentTop;
	}
}

ccColor3B color3FromString( CCString *colorString ){
	int r,g,b;
	sscanf(colorString->getCString(), "%d,%d,%d", &r, &g, &b);
	return ccc3(r, g, b);
}

std::string stripExtensionAndPath( const std::string& fileName )
{
	std::string name = fileName;
	auto foundIt = name.rfind(".");
	if( foundIt != std::string::npos )
	{
		name.erase(foundIt);
	}
	foundIt = name.rfind("/");
	if( foundIt != std::string::npos )
	{
		name.erase(0, foundIt);
	}
	return name;
}

void qsortByZ( CCArray* array, int first, int last )
{
	int i = first;
	int j = last;
	CCObject* tmp = nullptr;
	CCObject* pivot = array->data->arr[ (first + last)/2 ];
	int pivotZ = static_cast<CCNode*>(pivot)->getZOrder();
	
	while( i <= j )
	{
		while( static_cast<CCNode*>( array->data->arr[i] )->getZOrder() < pivotZ )
			i++;
		
		while( static_cast<CCNode*>( array->data->arr[j] )->getZOrder() > pivotZ )
			j--;
		
		if( i <= j )
		{
			tmp = array->data->arr[i];
			array->data->arr[i] = array->data->arr[j];
			array->data->arr[j] = tmp;
			i++;
			j--;
		}
	}
	
	if( first < j )
		qsortByZ(array, first, j);
	if( i < last )
		qsortByZ(array, i, last);
}

void sortNodesByZ( CCArray* array )
{
	qsortByZ( array, 0, array->data->num-1 );
}


CCMenu* createMenuByStrippingMenuItems( CCNode* firstItem, ... )
{
	va_list vl;
	va_start(vl, firstItem);
	CCArray* collectChildren = CCArray::create();
	while( firstItem )
	{
		CCArray* children = firstItem->getChildren();
		for (int i = 0; i<children->data->num; i++) {
			if( dynamic_cast<CCMenuItem*>( children->data->arr[i] ) )
				collectChildren->addObject( children->data->arr[i] );
		}
		firstItem = va_arg(vl,CCNode*);
	}
	va_end(vl);
	
	sortNodesByZ(collectChildren);
	
	for( int i = 0; i<collectChildren->data->num; i++){
		CCNode* child =	static_cast<CCNode*>( collectChildren->data->arr[i] );
		child->setPosition( child->convertToWorldSpaceAR( CCPoint(0, 0) ) );
		child->removeFromParent();
	}
	
	CCMenu* menu = CCMenu::createWithArray(collectChildren);
	menu->setAnchorPoint( CCPoint(0, 0) );
	menu->setPosition( CCPoint(0, 0) );
	
	return menu;
}

}
