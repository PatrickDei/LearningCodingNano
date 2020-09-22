LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := com_nanobit_storygame_StoryGame_common

LOCAL_MODULE_FILENAME := libstorygamecommon

APP_PLATFORM := android-14

FILE_LIST := $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/**/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/**/**/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/**/**/**/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/*.c)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/**/**/*.c)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx/**/**/**/*.c)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx-extra/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx-extra/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx-extra/**/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/cocos2dx-extra/**/**/*.c)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/extensions/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/extensions/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/extensions/**/*.c)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/extensions/**/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/extensions/**/**/*.c)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/CocosDenshion/android/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/CocosDenshion/android/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/Controllers/android/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/Controllers/include/*.cpp)
FILE_LIST += $(wildcard ../libraries/OpenUDID/Android/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/LZ4/lz4.c)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/NanoCocosLoader/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/json/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/goodform-master/src/*.cpp)

FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/Classes/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/Classes/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/Classes/**/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/Classes/**/**/**/*.cpp)
FILE_LIST += $(wildcard ../../StoryGameIOS/StoryGame/Classes/**/**/**/**/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += libwebsockets_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libpng_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_jpeg_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libtiff_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_libwebp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_zlib_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_crypto_static
LOCAL_STATIC_LIBRARIES += libzip
LOCAL_STATIC_LIBRARIES += libc++abi
LOCAL_CPPFLAGS += -std=c++11

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)\
						   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx\
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/include \
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/kazmath/include\
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/platform/android \
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/support/zip_support \
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/support \
                    	   $(LOCAL_PATH)/../../StoryGameIOSStoryGame/Classes/Base/Serialization \
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base \
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes \
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/GeneralControllers \
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/cocoa\
                    	   $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/json\
                    	   $(LOCAL_PATH)/../libraries/libzip/jni \
                           $(LOCAL_PATH)/../libraries/libzip \
                           $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra/platform\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/CocosDenshion/include \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/touch_dispatcher\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/base_nodes\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/layers_scenes_transitions_nodes\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/sprite_nodes\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/support\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra/network\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/support/user_default\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/cocoa\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/include \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/actions \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/menu_nodes \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/misc_nodes \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/kazmath/include\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/platform \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/support/zip_support \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra/crypto/base64\
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra/crypto/md5\
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra/crypto/sha1\
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra/crypto\
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx-extra/platform\
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/goodform-master/include \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Controllers/include\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions\
                    $(LOCAL_PATH)/../libraries/OpenUDID/Android \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/Json/lib_json\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/spine\
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/GUI/CCControlExtension \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/GUI/CCScrollView \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/network \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/LocalStorage \
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/cocos2d-x-jsonconverter \
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/cocos2d-x-jsonconverter/cJSON \
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/extensions/GUI/CCEditBox \
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/cocos2dx/platform/android/jni \
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/NanoCocosLoader\
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/json\
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/LZ4\
				    $(LOCAL_PATH)/../libraries/libzip/jni \
				    $(LOCAL_PATH)/../libraries/libzip \
				    $(LOCAL_PATH)/../libraries \
				    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/Serialization \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/CocosExtensions \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer/Common \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer/Connection \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/NanoGameServer/Nodes \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base/Shaders \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Layer \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Base \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Conversations \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/DataHolders \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/LevelObjects \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Profiles \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/States \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Menu \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/Loading \
                    $(LOCAL_PATH)/../../StoryGameIOS/StoryGame/Classes/GeneralControllers



APP_CPPFLAGS += -frtti -g -fexceptions

APP_STL := c++_static
APP_STL += libandroid_support
LOCAL_CPPFLAGS += -std=c++11
LOCAL_CFLAGS += -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS += -DUSE_FILE32API

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog

include $(BUILD_STATIC_LIBRARY)

$(call import-module, libraries/libzip/jni)
$(call import-module, ../StoryGameIOS/StoryGame/libs/libwebsockets/android)
$(call import-module, ../StoryGameIOS/StoryGame/libs/openssl/prebuilt/android)
$(call import-module, ../StoryGameIOS/StoryGame/libs/curl/prebuilt/android)
$(call import-module, ../StoryGameIOS/StoryGame/cocos2dx/platform/third_party/android/prebuilt/libjpeg)
$(call import-module, ../StoryGameIOS/StoryGame/cocos2dx/platform/third_party/android/prebuilt/libpng)
$(call import-module, ../StoryGameIOS/StoryGame/cocos2dx/platform/third_party/android/prebuilt/libtiff)
$(call import-module, ../StoryGameIOS/StoryGame/cocos2dx/platform/third_party/android/prebuilt/libwebp)
$(call import-module, ../StoryGameIOS/StoryGame/cocos2dx/platform/third_party/android/prebuilt/zlib)