#include "MyAdvertise.h"
#include "LuckyUtil.h"
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif 
USING_NS_CC;

MyAdvertise *MyAdvertise::getInstance()
{
	static MyAdvertise *instance;
	return instance;
}

void MyAdvertise::callStringMethod(std::string method)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/AdvertiseApi";

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave) {
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		method.c_str(),
		"()V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	}
#endif
}

void MyAdvertise::showBannerAdvertise()
{
	callStringMethod("showBannerAdvertise");
}

void MyAdvertise::closeBannerAdvertise()
{
	callStringMethod("closeBannerAdvertise");
}

void MyAdvertise::showScreenAdvertise()
{
	callStringMethod("showScreenAdvertise");
}

void MyAdvertise::closeScreenAdvertise()
{
	callStringMethod("closeScreenAdvertise");
}

void MyAdvertise::showVideoAdvertise()
{
	callStringMethod("showVideoAdvertise");
}

void MyAdvertise::closeVideoAdvertise()
{
	callStringMethod("closeVideoAdvertise");
}


#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
#ifdef __cplusplus  
extern "C"
{
#endif  

	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_AdvertiseApi_nativeAdvertise(JNIEnv* env, jobject thiz, jint result)
	{
		LuckyUtil::getInstance()->onVideoCallback();
	}
#ifdef __cplusplus  
}
#endif 
#endif