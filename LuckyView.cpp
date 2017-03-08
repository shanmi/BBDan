#include "LuckyView.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"
#include "GameController.h"
#include "GameConfig.h"
#include "UserInfo.h"
#include "LuckyUtil.h"

USING_NS_CC;

void LuckyView::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Reward, true);
}

void LuckyView::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool LuckyView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

LuckyView::LuckyView(int type)
:m_type(type)
{

}

LuckyView *LuckyView::create(int type)
{
	LuckyView *dialog = new LuckyView(type);
	dialog->init();
	dialog->autorelease();
	return dialog;
}

bool LuckyView::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 200));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/lucky_effect.xml");
	m_mainLayout->setMenuTouchPriority(kPriority_Reward - 1);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_mainLayout);
	m_mainLayout->setScale(0.0f);
	auto action = CCScaleTo::create(0.2f, 1.0f);
	m_mainLayout->runAction(action);

	initLayout();

	return true;
}

void LuckyView::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	closeItem->setTarget(this, menu_selector(LuckyView::closePanel));

	initRewardItem();
}

void LuckyView::initRewardItem()
{
	bool dispersedUnLock = UserInfo::getInstance()->isUnlock(kMarble_Dispersed);
	bool fasterUnLock = UserInfo::getInstance()->isUnlock(kMarble_Faster);
	bool biggerUnLock = UserInfo::getInstance()->isUnlock(kMarble_Bigger);
	bool bombUnLock = UserInfo::getInstance()->isUnlock(kMarble_Bomb);

	CCSprite * rewardItem = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(3));
	std::string imgstr;
	switch (m_type)
	{
	case kLucky_Faster:
		if (fasterUnLock)
		{
			imgstr = "lucky/youxijiemian_lunpan_10zhuanshi.png";
			UserInfo::getInstance()->addCoins(10);
		}
		else
		{
			imgstr = "lucky/youxijiemian_lunpan_suduqiu.png";
			UserInfo::getInstance()->unlockMarble(kMarble_Faster);
		}
		break;
	case kLucky_Hammer:
		imgstr = "lucky/youxijiemian_lunpan_cuizi2.png";
		UserInfo::getInstance()->addPropsCount(kProp_Clear, 2);
		break;
	case kLucky_Bigger:
		if (biggerUnLock)
		{
			imgstr = "lucky/youxijiemian_lunpan_10zhuanshi.png";
			UserInfo::getInstance()->addCoins(10);
		}
		else
		{
			imgstr = "lucky/youxijiemian_lunpan_zongjiqiu.png";
			UserInfo::getInstance()->unlockMarble(kMarble_Bigger);
		}
		break;
	case kLucky_Freezing:
		imgstr = "lucky/youxijiemian_lunpan_jinzi2.png";
		UserInfo::getInstance()->addPropsCount(kProp_Freezing, 2);
		break;
	case kLucky_Bomb:
		if (bombUnLock)
		{
			imgstr = "lucky/youxijiemian_lunpan_10zhuanshi.png";
			UserInfo::getInstance()->addCoins(10);
		}
		else
		{
			imgstr = "lucky/youxijiemian_lunpan_zadanqiu.png";
			UserInfo::getInstance()->unlockMarble(kMarble_Bomb);
		}
		break;
	case kLucky_Dispersed:
		if (dispersedUnLock)
		{
			imgstr = "lucky/youxijiemian_lunpan_10zhuanshi.png";
			UserInfo::getInstance()->addCoins(10);
		}
		else
		{
			imgstr = "lucky/youxijiemian_lunpan_sanseqiu.png";
			UserInfo::getInstance()->unlockMarble(kMarble_Dispersed);
		}
		break;
	case kLucky_DoubleAttact:
		imgstr = "lucky/youxijiemian_lunpan_shanghai2.png";
		UserInfo::getInstance()->addPropsCount(kProp_DoubleAttact, 2);
		break;
	default:
		break;
	}
	rewardItem->initWithFile(imgstr.c_str());
	GameController::getInstance()->notifyViews();
	LuckyUtil::getInstance()->updateView();
}

void LuckyView::closePanel(CCObject *pSender)
{
	removeFromParent();
}

void LuckyView::show(CCObject *pSender)
{

	if (m_callback)
		m_callback();
	removeFromParent();
}

void LuckyView::notifyViews()
{
	
}