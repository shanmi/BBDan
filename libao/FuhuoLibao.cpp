#include "FuhuoLibao.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"
#include "GameController.h"
#include "SquareModel.h"

USING_NS_CC;

enum TagPanel
{
	Tag_CoinPanel,
	Tag_BallPanel
};

void FuhuoLibao::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Libao, true);
	GameController::getInstance()->addView(this);
}

void FuhuoLibao::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
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

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/fuhuo_libao.xml");
	m_mainLayout->setMenuTouchPriority(kPriority_Libao - 1);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_mainLayout);

	initLayout();

	return true;
}

void FuhuoLibao::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	closeItem->setTarget(this, menu_selector(FuhuoLibao::closePanel));

	CCMenuItem *buyItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	buyItem->setTarget(this, menu_selector(FuhuoLibao::buyLibao));

	CCLabelTTF *buyTip = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(7));
	bool isBusinessMode = MyPurchase::sharedPurchase()->isBusinessMode();
	if (isBusinessMode)
	{
		auto size = m_mainLayout->getContentSize();
		buyTip->setPosition(ccp(size.width * 0.5f, -size.height));
		buyTip->setScale(0.5f);
		buyTip->setOpacity(120);
	}
}

void FuhuoLibao::closePanel(CCObject *pSender)
{
	removeFromParent();
	GameController::getInstance()->backToMainMenu();
}

void FuhuoLibao::buyLibao(CCObject *pSender)
{
	MyPurchase::sharedPurchase()->payForProducts(PAY_TYPE_FUHUO_LIBAO);
}

void FuhuoLibao::updateCoins()
{
	removeFromParent();
	SquareModel::theModel()->removeBelowSquares();
}