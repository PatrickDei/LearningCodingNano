#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "../../../../StoryGameIOS/StoryGame/Classes/AppDelegate.h"

#define  LOG_TAG    "main"

using namespace cocos2d;

extern "C"
{
    
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	CCLOG("PROBA");
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        CCApplication::sharedApplication()->run();
    }
    else
    {
        ccGLInvalidateStateCache();
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        ccDrawInit();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues();
    }
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeReload(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    // here would be a good place to reload the menus that need reloading
    CCNotificationCenter::sharedNotificationCenter()->postNotification(GL_CONTEXT_READY, NULL);
}

}
