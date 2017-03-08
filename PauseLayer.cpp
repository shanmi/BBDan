#include "PauseLayer.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"
#include "GameController.h"

USING_NS_CC;

void PauseLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Pause, true);
	GameController::getInstance()->addView(this);
	CCDirector::sharedDirector()->pause();
}

void PauseLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
	CCDirector::sharedDirector()->resume();
}

bool PauseLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

bool PauseLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/pause_panel.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Pause - 1);
	addChild(m_mainLayout);

	initLayout();
	return true;
}

void PauseLayer::initLayout()
{
	CCMenuItem *backItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	backItem->setTarget(this, menu_selector(PauseLayer::closePanel));

	CCMenuItem *homeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	homeItem->setTarget(this, menu_selector(PauseLayer::backToHome));

}

void PauseLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
}

void PauseLayer::backToHome(CCObject *pSender)
{
	removeFromParent();
	GameController::getInstance()->backToMainMenu();
}

void PauseLayer::notifyViews()
{
	removeFromParent();
}