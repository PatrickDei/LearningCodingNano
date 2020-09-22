#include "platform/CCDevice.h"
#include "jni/DPIJni.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

NS_CC_BEGIN

int CCDevice::getDPI()
{
    static int dpi = -1;
    if (dpi == -1)
    {
        dpi = (int)getDPIJNI();
    }
    return dpi;
}

void CCDevice::vibrate()
{
    vibrateJNI();
 }

NS_CC_END
