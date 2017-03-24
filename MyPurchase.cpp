#include "MyPurchase.h"
#include "cocos2d.h"
#include "UserInfo.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include "CCWinRTUtils.h"
#endif
#include "GameConfig.h"
#include "GameController.h"
using namespace cocos2d;


MyPurchase::MyPurchase()
:s_pRetListener(NULL)
{

}

MyPurchase::~MyPurchase(){
	if (s_pRetListener){
		delete s_pRetListener;
		s_pRetListener = NULL;
	}
}

MyPurchase* MyPurchase::sharedPurchase(){
	static MyPurchase instance;
	return &instance;
}

void MyPurchase::loadIAPPlugin(){
	if (s_pRetListener == NULL){
		s_pRetListener = new MyPurchaseResult();
	}
}

void MyPurchase::payForProducts(int product){
	CCLOG("payForProducts product = %d", product);
	s_pRetListener->setCallback(product);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	s_pRetListener->onPayResult(PAY_RESULT_SUCCESS, (const char*)"");
#else
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
		"Pay",
		"(I)V");
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID, product);
	}
#endif
}

void MyPurchase::showToast(int index){
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
		"showToast",
		"(I)V");  
	if (isHave) {
		minfo.env->CallVoidMethod(jobj, minfo.methodID, index);
	}

#endif
}

bool MyPurchase::exitGame(){
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
		"isExitGame",
		"()Z");
	jboolean isExit = true;
	if (isHave) {
		isExit = minfo.env->CallBooleanMethod(jobj, minfo.methodID);
	}
	return isExit;
#else
	return true;
#endif
}

bool MyPurchase::musicEnable(){
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
		"isMusicEnable",
		"()Z");
	CCLOG("musicEnable isMusicEnable isHave = %d", isHave);
	jboolean isEnable = true;
	if (isHave) {
		isEnable = minfo.env->CallBooleanMethod(jobj, minfo.methodID);
		CCLOG("musicEnable minfo isEnable = %d", isEnable);
	}
	return isEnable;
#else
	return true;
#endif
}

void MyPurchase::moreGame(){
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
		"moreGame",
		"()V");  
	if (isHave) { 
		minfo.env->CallVoidMethod(jobj, minfo.methodID); 
	}
#endif
}

void MyPurchase::setPayResult(int result)
{	
	s_pRetListener->onPayResult(result, NULL);
}

#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
#ifdef __cplusplus  
extern "C"
{
#endif  

	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_PayAndroidApi_nativePayResultToCPP(JNIEnv* env, jobject thiz, jint result)
	{
		MyPurchase::sharedPurchase()->setPayResult(result);//s_pRetListener->onPayResult(result,"");
	}
	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_PayAndroidApi_nativePayResultcencal(JNIEnv* env, jobject thiz, jint result)
	{
		MyPurchase::sharedPurchase()->setPayResult(result);//s_pRetListener->onPayResult(result,"");
	}
	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_PayAndroidApi_nativeExitToCPP(JNIEnv* env, jobject thiz, jint result)
	{
		if (result == 1)
		{
			CCDirector::sharedDirector()->end();
		}
	}
#ifdef __cplusplus  
}
#endif 
#endif

void MyPurchaseResult::onPayResult(int ret, const char* msg){
	CCLOG("onPayResult ret  = %d ,m_nCurrentPayType = %d", ret, m_nCurrentPayType);
	if (ret == PAY_RESULT_SUCCESS){
		int orderCoins = 0;
		int buyCount = 0;
		int presentCount = 0;
		switch (m_nCurrentPayType){
		case PAY_TYPE_DIAMOND1:
		case PAY_TYPE_DIAMOND2:
		case PAY_TYPE_DIAMOND3:
		case PAY_TYPE_DIAMOND4:
			//assert(m_nCurrentPayType == GameData::getInstance()->m_nDiamond[m_nCurrentPayType-1][0]);
			buyCount = GameConfig::getInstance()->m_nDiamond[m_nCurrentPayType-1][2];
			presentCount = GameConfig::getInstance()->m_nDiamond[m_nCurrentPayType - 1][3];
			break;
		case PAY_TYPE_TIME_LIBAO:
			UserInfo::getInstance()->addPropsCount(kProp_Clear, 5);
			UserInfo::getInstance()->addPropsCount(kProp_DoubleAttact, 5);
			UserInfo::getInstance()->addPropsCount(kProp_Freezing, 5);
			break;
		case PAY_TYPE_MARBLE_LIBAO:
			UserInfo::getInstance()->unlockAllMarble();
			break;
		case PAY_TYPE_COIN_LIBAO:
			buyCount = 200;
			break;
		case PAY_TYPE_FUHUO_LIBAO:

			break;

		}
		orderCoins = buyCount + presentCount;
		CCLOG("Buy success, orderCoins=======================%d", orderCoins);
		UserInfo::getInstance()->addCoins(orderCoins);
		GameController::getInstance()->notifyViews();
	}

}

MyPurchaseResult::MyPurchaseResult()
: m_nCurrentPayType(-1)
{

}

void MyPurchase::startStage(int level)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		"startStage",
		"(I)V");
	if (isHave)
	{
		minfo.env->CallVoidMethod(jobj, minfo.methodID, level);
	}

#endif
}

void MyPurchase::successStage(int level){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		"successStage",
		"(I)V");
	if (isHave)
	{
		minfo.env->CallVoidMethod(jobj, minfo.methodID, level);
	}

#endif
}

void MyPurchase::failStage(int level){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		"failStage",
		"(I)V");
	if (isHave)
	{
		minfo.env->CallVoidMethod(jobj, minfo.methodID, level);
	}

#endif
}

int MyPurchase::isBusinessMode(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
	jint carrietType;
	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		"isBusinessMode",
		"()I");
	if (isHave)
	{
		carrietType = minfo.env->CallIntMethod(jobj, minfo.methodID);
	}
	return carrietType;
#endif
	return 1;
}

std::string MyPurchase::getUserId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	const char* funstr = "org/cocos2dx/lib/PayAndroidApi";

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		funstr,
		"rtnActivity",
		"()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
	isHave = JniHelper::getMethodInfo(minfo,
		funstr,
		"getUserId",
		"()Ljava/lang/String;");
	if (isHave)
	{
		jstring jstr = (jstring)(minfo.env->CallObjectMethod(jobj, minfo.methodID));
		return JniHelper::jstring2string(jstr);
	}
	else
	{
		return "61462952";
	}
#else
	return "61462952";
#endif
}