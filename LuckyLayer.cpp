#include "LuckyLayer.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "ActionSequence.h"
#include "LuckyUtil.h"
#include "CCFunctionAction.h"
#include "UserInfo.h"
#include "LuckyView.h"
#include "GameUtil.h"
#include "MyAdvertise.h"

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
	LuckyUtil::getInstance()->addView(this);
}

void LuckyLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	LuckyUtil::getInstance()->removeView(this);
}

bool LuckyLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

LuckyLayer::LuckyLayer(int type)
:m_type(type)
{

}

LuckyLayer *LuckyLayer::create(int type /* = kLucky_Free */)
{
	LuckyLayer *dialog = new LuckyLayer(type);
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

void LuckyLayer::initLayout(bool refresh /* = false */)
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	closeItem->setTarget(this, menu_selector(LuckyLayer::closePanel));

	CCMenuItem *freeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	freeItem->setTarget(this, menu_selector(LuckyLayer::startDraw));
	CCMenuItem *againItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	againItem->setTarget(this, menu_selector(LuckyLayer::showVideoDialog));

	CCSprite *label1 = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(8));
	CCSprite *label2 = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(9));
	auto fadeAction = GameUtil::getFadeInOutAction();
	if (m_type == kLucky_Video)
	{
		label1->setVisible(true);
		label1->runAction(fadeAction);
		againItem->setVisible(true);
	}
	else
	{
		label1->setVisible(false);
		againItem->setVisible(false);
	}

	if (m_type == kLucky_Close)
	{
		label2->setVisible(true);
		label2->runAction(fadeAction);
	}
	else
	{
		label2->setVisible(false);
	}
	if (m_type == kLucky_Free)
	{
		freeItem->setVisible(true);
	}
	else
	{
		freeItem->setVisible(false);
	}

	CCSprite *panelBg = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(1));
	panelBg->setVisible(false);

	CCSprite *arrow = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(2));
	arrow->setZOrder(panelBg->getZOrder() + 2);

	if (!refresh)
	{
		m_itemLayout = UiLayout::create("layout/lucky_item.xml");
		m_itemLayout->setMenuTouchPriority(kPriority_LuckyLayer - 1);
		m_itemLayout->setZOrder(arrow->getZOrder() - 1);
		m_itemLayout->setAnchorPoint(ccp(0.5f, 0.5f));
		m_itemLayout->setPosition(panelBg->getPosition());
		m_mainLayout->addChild(m_itemLayout, panelBg->getZOrder() + 1);
		m_itemLayout->setScale(0.0f);
		auto action1 = CCScaleTo::create(0.2f, 1.0f);
		m_itemLayout->runAction(action1);
	}

	initItemLayout();

}

void LuckyLayer::initItemLayout()
{
	CCSprite *dispersed = dynamic_cast<CCSprite*>(m_itemLayout->getChildById(10));
	CCSprite *faster = dynamic_cast<CCSprite*>(m_itemLayout->getChildById(12));
	CCSprite *bigger = dynamic_cast<CCSprite*>(m_itemLayout->getChildById(7));
	CCSprite *bomb = dynamic_cast<CCSprite*>(m_itemLayout->getChildById(9));

	bool dispersedUnLock = UserInfo::getInstance()->isUnlock(kMarble_Dispersed);
	bool fasterUnLock = UserInfo::getInstance()->isUnlock(kMarble_Faster);
	bool biggerUnLock = UserInfo::getInstance()->isUnlock(kMarble_Bigger);
	bool bombUnLock = UserInfo::getInstance()->isUnlock(kMarble_Bomb);

	std::string diamondImg = "lucky/youxijiemian_lunpan_10zhuanshi.png";
	if (dispersedUnLock)
	{
		dispersed->initWithFile(diamondImg.c_str());
	}
	if (fasterUnLock)
	{
		faster->initWithFile(diamondImg.c_str());
	}
	if (biggerUnLock)
	{
		bigger->initWithFile(diamondImg.c_str());
	}
	if (bombUnLock)
	{
		bomb->initWithFile(diamondImg.c_str());
	}

}

void LuckyLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
	GameController::getInstance()->updateProgress();
}

void LuckyLayer::startDraw(CCObject *pSender)
{
	CCMenuItem *freeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	CCMenuItem *againItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	freeItem->setEnabled(false);
	againItem->setEnabled(false);

	ActionSequence *actions = ActionSequence::create(m_itemLayout);
	m_itemLayout->setRotation(0);
	auto rotate1 = CCRotateBy::create(1.0f, 360 * 3);
	actions->addAction(rotate1);
	int kCount = 4;
	for (int i = 0; i < kCount; i++)
	{
		auto rotate2 = CCRotateBy::create(0.6f + 0.3f * i, 360 * 1);
		actions->addAction(rotate2);
	}

	int type = kLucky_MaxCount;
	float degree = LuckyUtil::getInstance()->getRotateDegree(type);
	float time = (degree / 360) * kCount * 0.5f;
	auto rotate3 = CCRotateBy::create(time, degree);
	actions->addAction(rotate3);

	auto delayTime = CCDelayTime::create(0.6f);
	actions->addAction(delayTime);

	CCFunctionAction *callback = CCFunctionAction::create([=]()
	{
		m_type++;
		getLuckyItem(type);
		freeItem->setEnabled(true);
		againItem->setEnabled(true);
	});
	actions->addAction(callback);
	actions->runActions();
}

void LuckyLayer::showVideoDialog(CCObject *pSender)
{
#if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	MyAdvertise::getInstance()->showVideoAdvertise();
#else
	onVideoCallback();
#endif
}

void LuckyLayer::updateView()
{
	initLayout(true);
}

void LuckyLayer::onVideoCallback()
{
	startDraw(NULL);
}

void LuckyLayer::getLuckyItem(int type)
{
	LuckyView *luckydialog = LuckyView::create(type);
	addChild(luckydialog);
}