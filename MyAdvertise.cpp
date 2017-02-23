#include "MyAdvertise.h"
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

void MyAdvertise::showBannerAdvertise()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

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
		"showBannerAdvertise",
		"()V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	}
#endif
}

void MyAdvertise::showScreenAdvertise()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

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
		"showScreenAdvertise",
		"()V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	}
#endif
}

void MyAdvertise::showVideoAdvertise()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

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
		"showVideoAdvertise",
		"()V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	}
#endif
}