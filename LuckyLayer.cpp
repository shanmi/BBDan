#include "LuckyLayer.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "GameController.h"
#include "ActionSequence.h"
#include "LuckyUtil.h"
#include "CCFunctionAction.h"
#include "UserInfo.h"

USING_NS_CC;

enum TagPanel
{
	Tag_CoinPanel,
	Tag_BallPanel,
};

void LuckyLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_LuckyLayer, true);
	GameController::getInstance()->addView(this);
}

void LuckyLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

bool LuckyLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

LuckyLayer::LuckyLayer(bool isFree)
:m_bIsFree(isFree)
{

}

LuckyLayer *LuckyLayer::create(bool isFree /* = false */)
{
	LuckyLayer *dialog = new LuckyLayer(isFree);
	dialog->init();
	dialog->autorelease();
	return dialog;
}

bool LuckyLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/lucky_panel.xml");

	m_mainLayout->setMenuTouchPriority(kPriority_LuckyLayer - 1);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_mainLayout);
	m_mainLayout->setScale(0.0f);
	auto action = CCScaleTo::create(0.2f, 1.0f);
	m_mainLayout->runAction(action);

	initLayout();

	return true;
}

void LuckyLayer::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	closeItem->setTarget(this, menu_selector(LuckyLayer::closePanel));

	CCMenuItem *freeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	freeItem->setTarget(this, menu_selector(LuckyLayer::startDraw));
	freeItem->setVisible(m_bIsFree);

	CCMenuItem *againItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	againItem->setTarget(this, menu_selector(LuckyLayer::showVideoDialog));
	againItem->setVisible(!m_bIsFree);

	CCSprite *arrow = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(2));
	m_itemLayout = UiLayout::create("layout/lucky_item.xml");
	m_itemLayout->setMenuTouchPriority(kPriority_LuckyLayer - 1);
	m_itemLayout->setZOrder(arrow->getZOrder() - 1);
	m_itemLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_itemLayout->setPosition(arrow->getPosition());
	m_mainLayout->addChild(m_itemLayout);
	m_itemLayout->setScale(0.0f);
	auto action1 = CCScaleTo::create(0.2f, 1.0f);
	m_itemLayout->runAction(action1);

}

void LuckyLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
	GameController::getInstance()->updateProgress();
}

void LuckyLayer::startDraw(CCObject *pSender)
{
	ActionSequence *actions = ActionSequence::create(m_itemLayout);
	m_itemLayout->setRotation(0);
	auto rotate1 = CCRotateBy::create(1.0f, 360 * 3);
	actions->addAction(rotate1);
	int kCount = 4;
	for (int i = 0; i < kCount; i++)
	{
		auto rotate2 = CCRotateBy::create(0.6f + 0.2f * i, 360 * 1);
		actions->addAction(rotate2);
	}

	int type = kLucky_Max;
	float degree = LuckyUtil::getInstance()->getRotateDegree(type);
	auto rotate3 = CCRotateBy::create(kCount * 0.2f, degree);
	actions->addAction(rotate3);

	CCFunctionAction *callback = CCFunctionAction::create([=]()
	{
		switch (type)
		{
		case kLucky_Faster:
			UserInfo::getInstance()->unlockMarble(kMarble_Faster);
			break;
		case kLucky_Hammer:
			UserInfo::getInstance()->addPropsCount(kProp_Clear, 2);
			break;
		case kLucky_Bigger:
			UserInfo::getInstance()->unlockMarble(kMarble_Bigger);
			break;
		case kLucky_Freezing:
			UserInfo::getInstance()->addPropsCount(kProp_Freezing, 2);
			break;
		case kLucky_Bomb:
			UserInfo::getInstance()->unlockMarble(kMarble_Bomb);
			break;
		case kLucky_Dispersed:
			UserInfo::getInstance()->unlockMarble(kMarble_Dispersed);
			break;
		case kLucky_DoubleAttact:
			UserInfo::getInstance()->addPropsCount(kProp_DoubleAttact, 2);
			break;
		default:
			break;
		}
	});
	actions->runActions();
}

void LuckyLayer::showVideoDialog(CCObject *pSender)
{

}

void LuckyLayer::updateCoins()
{
	removeFromParent();
}