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

typedef enum {
	PAY_TYPE_DIAMOND1 = 1, //钻石1
	PAY_TYPE_DIAMOND2, //钻石2
	PAY_TYPE_DIAMOND3, //钻石3
	PAY_TYPE_DIAMOND4, //钻石4

	PAY_TYPE_TIME_LIBAO = 10,
	PAY_TYPE_COIN_LIBAO,
	PAY_TYPE_FUHUO_LIBAO,
	PAY_TYPE_FISH_LIBAO,
	PAY_TYPE_MARBLE_LIBAO
} MyPayProducts;

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
	void payForProducts(MyPayProducts product);
	void showToast(const char * hint);
	bool exitGame();
	void exitGameMM();
	bool musicEnable();
	void moreGame();
	MyPurchaseResult* s_pRetListener;
	int m_nReturnResult;
	void delayPayUpdate(float dt);

	void startStage(const char * level);
	void successStage(const char * level);
	void failStage(const char * level);
	int isBusinessMode();
	std::string getUserId();
private:
	MyPurchase();
	virtual ~MyPurchase();
	static MyPurchase* s_pPurchase;
};

#endif