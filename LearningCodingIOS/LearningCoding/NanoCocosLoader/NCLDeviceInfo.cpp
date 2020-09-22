//
//  NCLDeviceInfo.cpp
//  KnightKingdom
//
//  Created by Bruno Mikus on 26/09/14.
//
//

#include "NCLDeviceInfo.h"
#include "CCDirector.h"
#include "CCFileUtils.h"
#include <array>
#include "IosFunctions.h"

using namespace cocos2d;
using namespace std;

#define DEVICE_COUNT 17

#pragma mark Device

//  width(w), height(h), deviceDensity(density), name(n), deviceExtension

std::array<NCLDeviceInfo::NCLDevice, 19> NCLDeviceInfo::NCLDevice::devices = {
    {NCLDeviceInfo::NCLDevice(480, 320, 0.5, "iPhone", ""),
     NCLDeviceInfo::NCLDevice(960, 640, 1.0, "iPhone Retina", ""),
     NCLDeviceInfo::NCLDevice(1136, 640, 0.94, "iPhoneSE", ""),
     NCLDeviceInfo::NCLDevice(1334, 750, 1.0, "iPhone6s", ""),
     NCLDeviceInfo::NCLDevice(1792, 828, 1.104, "iPhone XS", ""),
     NCLDeviceInfo::NCLDevice(2208, 1242, 1.6, "iPhone7Plus", ""),
     NCLDeviceInfo::NCLDevice(2436, 1125, 1.6, "iPhone X", ""),
     NCLDeviceInfo::NCLDevice(2688, 1242, 1.6, "iPhone XS Max", ""),
     NCLDeviceInfo::NCLDevice(1024, 768, 0.9375, "iPad", ""),
     NCLDeviceInfo::NCLDevice(2048, 1536, 1.4, "iPad Retina", ""),

     NCLDeviceInfo::NCLDevice(480, 320, 0.5, "HTC Desire C", ""),
     NCLDeviceInfo::NCLDevice(800, 480, 0.75, "Galaxy S", ""),
     NCLDeviceInfo::NCLDevice(960, 540, 0.84375, "HTC One S", ""),
     NCLDeviceInfo::NCLDevice(1024, 600, 0.9375, "Galaxy Tab", ""),
     NCLDeviceInfo::NCLDevice(1280, 720, 1.0, "Galaxy S3", ""), NCLDeviceInfo::NCLDevice(1280, 768, 1.0, "Nexus 4", ""),
     NCLDeviceInfo::NCLDevice(1280, 800, 1.0416666, "Nexus 7", ""),
     NCLDeviceInfo::NCLDevice(1920, 1080, 1.40625, "Nexus 5", ""),
     NCLDeviceInfo::NCLDevice(2560, 1600, 2.0833333, "Nexus 10", "")}};

NCLDeviceInfo::NCLDevice::NCLDevice(float w, float h, float density, std::string n, std::string ext)
    : width(w), height(h), deviceDensity(density), name(n), deviceExtension(ext)
{
}

float NCLDeviceInfo::NCLDevice::distance(float w, float h) { return CCPoint(width - w, height - h).getLength(); }

float NCLDeviceInfo::NCLDevice::adjustedDensity(float w, float h)
{
    return deviceDensity * fminf(w / width, h / height);
}

NCLDeviceInfo::NCLDevice *NCLDeviceInfo::NCLDevice::getDeviceNamed(std::string name)
{
    for(auto &device : devices) {
        if(device.name.compare(name) == 0) {
            return &device;
        }
    }
    return NULL;
}

NCLDeviceInfo::NCLDevice *NCLDeviceInfo::NCLDevice::getNearestDevice(float w, float h)
{
    auto nearest = devices.end();
    float nearestDist = INFINITY;
    for(auto deviceIt = devices.begin(); deviceIt != devices.end() && nearestDist > 0; deviceIt++) {
        float distance = fabsf(deviceIt->distance(w, h));
        if(distance < nearestDist) {
            nearestDist = distance;
            nearest = deviceIt;
        }
    }
    return nearest;
}

#pragma mark Device Info

bool NCLDeviceInfo::isLandscape;
cocos2d::CCSize NCLDeviceInfo::deviceSize;
cocos2d::CCSize NCLDeviceInfo::deviceResolution;

string NCLDeviceInfo::name = "";
string NCLDeviceInfo::extension = "";

float NCLDeviceInfo::deviceScale = 1.0;

void NCLDeviceInfo::setInfo()
{
    static bool infoSet = false;
    if(!infoSet) {
        infoSet = true;

        CCDirector *director = CCDirector::sharedDirector();

        deviceSize = director->getWinSize();
        isLandscape = deviceSize.width > deviceSize.height;
        deviceResolution = director->getWinSizeInPixels();

        if(!isLandscape)
            deviceSize = CCSize(deviceSize.height, deviceSize.width);

        auto nearest = NCLDevice::getNearestDevice(deviceSize.width, deviceSize.height);
        name = nearest->name;
        extension = nearest->deviceExtension;

        deviceScale = nearest->adjustedDensity(deviceSize.width, deviceSize.height);

        CCLOG("--- NCLDEVICEINFO ---");
        CCLOG("--- Content scale factor: %f ", deviceScale);
        CCLOG("--- Size: %d, %d ", (int)deviceSize.width, (int)deviceSize.height);
        CCLOG("--- Px size: %d, %d ", (int)deviceResolution.width, (int)deviceResolution.height);
        CCLOG("--- NCLDEVICEINFO ---");

        if(!isLandscape)
            deviceSize = CCSize(deviceSize.height, deviceSize.width);

        recheckExtension();
    }
}

bool NCLDeviceInfo::getIsLandscape()
{
    setInfo();
    return isLandscape;
}

CCSize NCLDeviceInfo::getDeviceSize()
{
    setInfo();
    return deviceSize;
}

CCSize NCLDeviceInfo::getDeviceResolution()
{
    setInfo();
    return deviceResolution;
}

float NCLDeviceInfo::getDoubleOnIpadRetina()
{
    setInfo();
    return deviceScale;
}

float NCLDeviceInfo::getContentScaleFactor()
{
    setInfo();
    return deviceScale;
}

float NCLDeviceInfo::getDeviceScale()
{
    setInfo();
    return deviceScale;
}

std::string NCLDeviceInfo::getExtension()
{
    setInfo();
    return getExtension();
}

bool NCLDeviceInfo::isIphoneAspect()
{
    bool isIphoneAspect = true;

    CCSize winSize = NCLDeviceInfo::getDeviceSize();
    double aspectRatioDevice = winSize.height / winSize.width;
    double iphoneAspectRatio = 16.f / 9.f;
    double ipadAspectRatio = 4.f / 3.f;

    auto iphoneDist = fabs(aspectRatioDevice - iphoneAspectRatio);
    auto ipadDist = fabs(aspectRatioDevice - ipadAspectRatio);

    if(ipadDist < iphoneDist) {
        isIphoneAspect = false;
    }
    return isIphoneAspect;
}

#pragma mark Device Extensions

// CP this somwhere before any loading and modify it as needed
// NCLDeviceInfo::setup("", "-hd", "-iphone5", "-ipad", "-ipad");

void NCLDeviceInfo::setupExtensions(string iPhone, string iPhoneRetina, string iPhone5, string iPad, string iPadRetina)
{
    setExtension("iPhone", iPhone);
    setExtension("iPhone Retina", iPadRetina);
    setExtension("iPhone5", iPhone5);
    setExtension("iPad", iPad);
    setExtension("iPad Retina", iPadRetina);
    recheckExtension();
}

void NCLDeviceInfo::setExtension(std::string deviceName, std::string newExtension)
{
    NCLDevice *device = NCLDevice::getDeviceNamed(deviceName);
    if(device) {
        device->deviceExtension = newExtension;
    }
}

void NCLDeviceInfo::recheckExtension()
{
    setInfo();
    NCLDevice *device = NCLDevice::getDeviceNamed(name);
    extension = device ? device->deviceExtension : "";
}

std::string NCLDeviceInfo::getExtensionForDevice(std::string deviceName, bool isRetina)
{
    //    if( isRetina ){
    //        deviceName = deviceName + " Retina";
    //    }

    NCLDevice *device = NCLDevice::getDeviceNamed(deviceName);
    return device ? device->deviceExtension : "";
}

string NCLDeviceInfo::appendExtension(const string &original, const string &extension)
{
    int period = original.find_last_of(".");
    if(period < original.length()) {
        string result = original;
        return result.insert(period, extension.c_str(), extension.length());
    }
    return original;
}

string NCLDeviceInfo::appendExtensionToPath(const string &path)
{
    string newFileName = NCLDeviceInfo::appendExtension(path, extension);
    string foundPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(newFileName.c_str());
    if(foundPath.compare(newFileName) != 0) {
        return newFileName;
    }
    return path;
}

bool NCLDeviceInfo::isIphoneX()
{
    CCDirector *director = CCDirector::sharedDirector();
    deviceSize = director->getWinSize();
    return deviceSize.width == 1125 && deviceSize.height == 2436;
}

bool NCLDeviceInfo::hasTopNotch() { return IosFunctions::hasTopNotch(); }

bool NCLDeviceInfo::isIphone5()
{
    CCDirector *director = CCDirector::sharedDirector();
    deviceSize = director->getWinSize();
    return deviceSize.height == 1136 && deviceSize.width == 640;
}

bool NCLDeviceInfo::isIpad()
{
    bool returnVal = false;
    if(extension.find("-ipad") != std::string::npos) {
        returnVal = true;
    }
    return returnVal;
}
