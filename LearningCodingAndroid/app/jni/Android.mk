LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared
APP_ALLOW_MISSING_DEPS=true

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \

LOCAL_C_INCLUDES :=	$(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx-extra/platform \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx-extra \
					$(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx/include \
				    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/Serialization \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/CocosExtensions \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer/Common \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer/Connection \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer/Nodes \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base/Shaders \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Layer \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Base \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Conversations \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/DataHolders \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/LevelObjects \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Profiles \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/States \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Menu \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/Loading \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Classes/GeneralControllers \
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx/layers_scenes_transitions_nodes\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx/sprite_nodes\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx/base_nodes\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx/cocoa\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx/actions\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/extensions\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/extensions/spine\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/cocos2dx/support\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/Controllers/include\
                    $(LOCAL_PATH)/../../../StoryGameIOS/StoryGame/NanoCocosLoader\


LOCAL_WHOLE_STATIC_LIBRARIES := com_nanobit_storygame_StoryGame_common
LOCAL_STATIC_LIBRARIES += libc++abi

include $(BUILD_SHARED_LIBRARY)

LOCAL_CPPFLAGS += -std=c++11
APP_CXXFLAGS += -std=gnu++11
APP_STL := c++_static
APP_STL += libandroid_support

IMPORT_PATH := $(LOCAL_PATH)/../../..

$(call import-add-path, $(IMPORT_PATH))
$(call import-add-path, $(IMPORT_PATH)/StoryGameAndroid)
$(call import-add-path, $(IMPORT_PATH)/StoryGameIOS/StoryGame/cocos2dx/platform/third_party/android/prebuilt)
$(call import-add-path, $(IMPORT_PATH)/StoryGameIOS/StoryGame/cocos2dx)

$(call import-module,app)
