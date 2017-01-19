#ifndef __MY_PURCHASE_H__
#define __MY_PURCHASE_H__

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#endif 
#include "CommonMacros.h"

USING_NS_CC;

enum PayResult{
	PAY_RESULT_SUCCESS,
	PAY_RESULT_FAILED,
	PAY_RESULT_CANCEL,
	PAY_RESULT_TIMEOUT
};

enum ToastIndex
{
	Toast_
};

enum MyPayProducts{
	PAY_TYPE_DIAMOND1 = 1, //钻石1
	PAY_TYPE_DIAMOND2, //钻石2
	PAY_TYPE_DIAMOND3, //钻石3
	PAY_TYPE_DIAMOND4, //钻石4

	PAY_TYPE_TIME_LIBAO = 10,
	PAY_TYPE_COIN_LIBAO,
	PAY_TYPE_FUHUO_LIBAO,
	PAY_TYPE_FISH_LIBAO,
	PAY_TYPE_MARBLE_LIBAO
};

class MyPurchaseResult
{
public:
	void onPayResult(int ret, const char* msg);
	void setCallback(int payType){ m_nCurrentPayType = payType; }
	MyPurchaseResult();
private:
	int m_nCurrentPayType;
};

class MyPurchase
{
public:
	static MyPurchase* sharedPurchase();
	static void purgePurchase();
	void loadIAPPlugin();
	void setPayResult(int result);
	void payForProducts(int product);
	void showToast(int index);
	bool exitGame();
	void exitGameMM();
	bool musicEnable();
	void moreGame();
	MyPurchaseResult* s_pRetListener;
	void delayPayUpdate(float dt);

	void startStage(int level);
	void successStage(int level);
	void failStage(int level);
	int isBusinessMode();
	std::string getUserId();
private:
	MyPurchase();
	virtual ~MyPurchase();
	static MyPurchase* s_pPurchase;
};

#endif