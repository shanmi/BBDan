#include "FuhuoLibao.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"
#include "GameController.h"
#include "SquareModel.h"
#include "GameConfig.h"
#include "UserInfo.h"
#include "LibaoDialog.h"
#include "MarbleModel.h"
#include "DataHelper.h"

USING_NS_CC;

void FuhuoLibao::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Libao, true);
	GameController::getInstance()->addView(this);
	GameController::getInstance()->setGamePause(true);
}

void FuhuoLibao::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
	GameController::getInstance()->setGamePause(false);
}

bool FuhuoLibao::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

CCScene* FuhuoLibao::scene()
{
	CCScene *scene = CCScene::create();
	FuhuoLibao *layer = FuhuoLibao::create();
	scene->addChild(layer);
	return scene;
}

bool FuhuoLibao::init()
{
	if (!CCLayer::init()){
		return false;
	}
	int curLevel = SquareModel::theModel()->getCurrentScore();
	MyPurchase::sharedPurchase()->failStage(curLevel);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/fuhuo_libao.xml");
	m_mainLayout->setMenuTouchPriority(kPriority_Libao - 1);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_mainLayout);
	m_mainLayout->setScale(0.0f);
	auto action = CCScaleTo::create(0.2f, 1.0f);
	m_mainLayout->runAction(action);

	initLayout();

	return true;
}

void FuhuoLibao::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	closeItem->setTarget(this, menu_selector(FuhuoLibao::closePanel));
	bool isBusinessMode = MyPurchase::sharedPurchase()->isBusinessMode();
	int isYijian = GameConfig::getInstance()->m_yijian;
	if (isBusinessMode && isYijian)
	{
		closeItem->setTarget(this, menu_selector(FuhuoLibao::buyLibao));
	}

	CCMenuItem *buyItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	buyItem->setTarget(this, menu_selector(FuhuoLibao::buyLibao));

	CCLabelTTF *buyTip = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(7));
	if (isBusinessMode)
	{
		auto size = m_mainLayout->getContentSize();
		buyTip->setPosition(ccp(buyTip->getPositionX(), buyTip->getPositionY() - size.height * 0.76f));
		buyTip->setScale(0.5f);
		buyTip->setOpacity(120);

		int isYijian = GameConfig::getInstance()->m_yijian;
		if (isYijian)
		{
			buyTip->setVisible(false);
		}
	}
}

void FuhuoLibao::closePanel(CCObject *pSender)
{
	MarbleModel::theModel()->setAttactRate(ATTACT_RATE);
	GameController::getInstance()->backToMainMenu();
	int gameType = GameController::getInstance()->getGameType();
	switch (gameType)
	{
	case kGame_Normal:
		UserInfo::getInstance()->resetLuckyLevel();
		DataHelper::getInstance()->clearGameInfo();
		break;
	case kGame_Shoot:
		UserInfo::getInstance()->resetTargetLevel();
		GameController::getInstance()->setBossBloodCount(0);
		DataHelper::getInstance()->clearShootGameInfo();
		break;
	default:
		break;
	}
	removeFromParent();
}

void FuhuoLibao::buyLibao(CCObject *pSender)
{
#if(BBDAN_SHENBAO == 1)
	int curCoin = UserInfo::getInstance()->getCoins();
	int fuhuoCostCoin = GameConfig::getInstance()->m_fuhuoCostCoin;
	if (curCoin >= fuhuoCostCoin)
	{
		GameController::getInstance()->notifyViews();
	}
	else
	{
		LibaoDialog *dialog = LibaoDialog::create(PAY_TYPE_COIN_LIBAO);
		addChild(dialog, KZOrder_LibaoLayer, kTag_Libao);
	}
#else
	MyPurchase::sharedPurchase()->payForProducts(PAY_TYPE_FUHUO_LIBAO);
	bool isBusinessMode = MyPurchase::sharedPurchase()->isBusinessMode();
	int isYijian = GameConfig::getInstance()->m_yijian;
	if (isBusinessMode && isYijian)
	{
		removeFromParent();
	}
#endif
}

void FuhuoLibao::notifyViews()
{
	removeFromParent();
	int gameType = GameController::getInstance()->getGameType();
	switch (gameType)
	{
	case kGame_Normal:
		SquareModel::theModel()->removeBelowSquares();
		break;
	case kGame_Shoot:
		SquareModel::theModel()->removeBelowSquares(); // 先清除下面的方块
		SquareModel::theModel()->removeAllSquares();
		break;
	}

	GameController::getInstance()->notifyViews();
#if(BBDAN_SHENBAO == 1)
	int fuhuoCostCoin = GameConfig::getInstance()->m_fuhuoCostCoin;
	UserInfo::getInstance()->addCoins(-fuhuoCostCoin);
	GameController::getInstance()->notifyViews();
#endif
}