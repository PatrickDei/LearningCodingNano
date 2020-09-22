
#include "NanoDownloadWrapper.h"
#include "AppDelegate.h"
#include <string>
#include "JniHelper.h"
#include <jni.h>

using namespace cocos2d;

std::function<void(std::string, std::string)> NanoDownloadWrapper::onSuccess = [](std::string fn, std::string fileHash) {};
std::function<void(std::string, std::string)> NanoDownloadWrapper::onFail = [](std::string fn, std::string fileHash) {};

extern "C" {
    JNIEXPORT void JNICALL Java_org_nanobit_mystory_FileDownloader_onDownloadSuccess (JNIEnv *env, jobject thiz,
                                                                                      jstring fileName, jstring fileHash)
    {
        std::string fName = JniHelper::jstring2string (fileName);
        std::string fHash = JniHelper::jstring2string (fileHash);
        NanoDownloadWrapper::onSuccess (fName, fHash);
    }
    
    JNIEXPORT void JNICALL Java_org_nanobit_mystory_FileDownloader_onDownloadFailed (JNIEnv *env, jobject thiz,
                                                                                     jstring fileName, jstring fileHash)
    {
        std::string fName = JniHelper::jstring2string (fileName);
        std::string fHash = JniHelper::jstring2string (fileHash);
        NanoDownloadWrapper::onFail (fName, fHash);
    }
}

NanoDownloadWrapper::NanoDownloadWrapper () {}

NanoDownloadWrapper::~NanoDownloadWrapper () {}

void NanoDownloadWrapper::stopDownload () {     JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (t, "org/nanobit/mystory/FileDownloader", "stopAllDownloads", "()V")) {
        t.env->CallStaticVoidMethod (t.classID, t.methodID);
    }}

bool NanoDownloadWrapper::addFileToDownloadingQueue(const std::string &url, const std::string &directory,
                                                    const std::string &filename, const std::string &fileHash,
                                                    std::function<void(std::string, std::string)> downloadSuccess,
                                                    std::function<void(std::string, std::string)> downloadFailed)
{
    NanoDownloadWrapper::onSuccess = downloadSuccess;
    NanoDownloadWrapper::onFail = downloadFailed;
    
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo (
                                        t, "org/nanobit/mystory/FileDownloader", "downloadFromURL",
                                        "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring stringArg = t.env->NewStringUTF (url.c_str ());
        jstring stringArg1 = t.env->NewStringUTF (directory.c_str ());
        jstring stringArg2 = t.env->NewStringUTF (filename.c_str ());
        jstring stringArg3 = t.env->NewStringUTF (fileHash.c_str ());
        
        t.env->CallStaticVoidMethod (t.classID, t.methodID, stringArg, stringArg1, stringArg2, stringArg3);
        
        t.env->DeleteLocalRef (t.classID);
        t.env->DeleteLocalRef (stringArg);
        t.env->DeleteLocalRef (stringArg1);
        t.env->DeleteLocalRef (stringArg2);
        t.env->DeleteLocalRef (stringArg3);
    }
    return true;
}
