//
//  NCLDeviceInfo.h
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#ifndef __KnightKingdom__NCLDeviceInfo__
#define __KnightKingdom__NCLDeviceInfo__

#include "CCGeometry.h"
#include <array>

enum kNCLDeviceType{
    kNCLDeviceIPhone = 0,
    kNCLDeviceIPhone5 = 1,
    kNCLDeviceIPad = 2,
    kNCLDeviceCustom = 3
};

#pragma mark -
class NCLDeviceInfo
{
#pragma mark Device
private:
    class NCLDevice{
    public:
        float width;
        float height;
        /**
         * Ideal image size. 0.5 == non hd, 1.0 == hd, 2.0 == ipad retina.
         */
        float deviceDensity;
        std::string name;
        std::string deviceExtension;
        
        NCLDevice( float w, float h, float density, std::string n, std::string ext );
        float distance( float w, float h );
        float adjustedDensity( float w, float h );
        
        static std::array<NCLDevice, 19> devices;
        static NCLDevice* getDeviceNamed( std::string name );
        static NCLDevice* getNearestDevice( float w, float h );
    };
    
#pragma mark Device Info
private:
    static void setInfo();
    
    static bool isLandscape;
    static cocos2d::CCSize deviceSize;
    static cocos2d::CCSize deviceResolution;
    
    static std::string name;
    static std::string extension;
    
    static float deviceScale;
    
public:
    static  bool getIsLandscape();
    static  cocos2d::CCSize getDeviceSize();
    static  cocos2d::CCSize getDeviceResolution();
    
    static float getDoubleOnIpadRetina() ;
    template<class T>
    static T doubleOnIpadRetina( T value )
    {
        setInfo();
        return deviceScale * value;
    }
    static float getContentScaleFactor() ;
    
    static bool isRetina() ;
    static bool isIpad() ;
    static bool isIpadRetina() ;
    static bool isIphoneAspect ();

    static float getDeviceScale();
    static std::string getExtension();
    static bool isIphoneX();
    static bool isIphone5();
    
    static bool hasTopNotch();
    
#pragma mark Device Extensions
private:
    
    static void recheckExtension();
    static void setExtension( std::string deviceName, std::string extension );
    
public:
    static void setupExtensions( std::string iPhone, std::string iPhoneRetina, std::string iPhone5, std::string iPad, std::string iPadRetina );
    
    static std::string getExtensionForDevice(std::string deviceName, bool isRetina = false);
    
    static std::string appendExtension( const std::string& original, const std::string& extension );
    static std::string appendExtensionToPath( const std::string& path );
};


#endif /* defined(__KnightKingdom__NCLDeviceInfo__) */
