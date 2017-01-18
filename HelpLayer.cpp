#include "HelpLayer.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"
#include "GameController.h"

USING_NS_CC;

void HelpLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Pause, true);
	GameController::getInstance()->addView(this);
}

void HelpLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

bool HelpLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

bool HelpLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/help_panel.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Pause - 1);
	addChild(m_mainLayout);

	initLayout();
	return true;
}

void HelpLayer::initLayout()
{
	CCMenuItem *backItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(2));
	backItem->setTarget(this, menu_selector(HelpLayer::closePanel));

}

void HelpLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
}

