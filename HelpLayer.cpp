#include "HelpLayer.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"
#include "GameController.h"
#include "GameConfig.h"

USING_NS_CC;

void HelpLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Pause, true);
	GameController::getInstance()->addView(this);
	GameController::getInstance()->setGamePause(true);
}

void HelpLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
	GameController::getInstance()->setGamePause(false);
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

	auto colorLayer = CCLayerColor::create(ccc4(60, 60, 60, 250));
	addChild(colorLayer);

	int gameType = GameController::getInstance()->getGameType();
	switch (gameType)
	{
	case kGame_Normal:
		initLayout();
		break;
	case kGame_Shoot:
		initLayout2();
		break;
	}
	return true;
}

void HelpLayer::initLayout()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_mainLayout = UiLayout::create("layout/help_panel.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Pause - 1);
	addChild(m_mainLayout);

	CCMenuItem *backItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(44));
	backItem->setTarget(this, menu_selector(HelpLayer::closePanel));

	char temp[200] = { 0 };
	CCLabelTTF *doubleAttactLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(4));
	int cost = GameConfig::getInstance()->m_doubleAttactCost;
	auto str = doubleAttactLabel->getString();
	sprintf(temp, str, cost);
	doubleAttactLabel->setString(temp);

	CCLabelTTF *freezingLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(7));
	cost = GameConfig::getInstance()->m_freezingCost;
	str = freezingLabel->getString();
	sprintf(temp, str, cost);
	freezingLabel->setString(temp);

	CCLabelTTF *hammerLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(10));
	cost = GameConfig::getInstance()->m_hammerCost;
	str = hammerLabel->getString();
	sprintf(temp, str, cost);
	hammerLabel->setString(temp);

	CCLabelTTF *showFireLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(29));
	int level = GameConfig::getInstance()->m_showFireLevel;
	str = showFireLabel->getString();
	sprintf(temp, str, level);
	showFireLabel->setString(temp);

	CCLabelTTF *addFireLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(30));
	level = GameConfig::getInstance()->m_addFireLevel;
	str = addFireLabel->getString();
	sprintf(temp, str, level);
	addFireLabel->setString(temp);

	CCLabelTTF *addIronLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(34));
	level = GameConfig::getInstance()->m_showIronLevel;
	str = addIronLabel->getString();
	sprintf(temp, str, level);
	addIronLabel->setString(temp);
}

void HelpLayer::initLayout2()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_mainLayout = UiLayout::create("layout/help_panel2.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Pause - 1);
	addChild(m_mainLayout);

	CCMenuItem *backItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(44));
	backItem->setTarget(this, menu_selector(HelpLayer::closePanel));

	char temp[200] = { 0 };
	CCLabelTTF *showDoubleAttactTimeLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(3));
	sprintf(temp, showDoubleAttactTimeLabel->getString(), DOUBLE_ATTACT_TIME);
	showDoubleAttactTimeLabel->setString(temp);

	CCLabelTTF *doubleAttactLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(4));
	int cost = GameConfig::getInstance()->m_doubleAttactCost;
	auto str = doubleAttactLabel->getString();
	sprintf(temp, str, cost);
	doubleAttactLabel->setString(temp);


	CCLabelTTF *showFreezingTimeLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(6));
	sprintf(temp, showFreezingTimeLabel->getString(),FREEZING_TIME);
	showFreezingTimeLabel->setString(temp);

	CCLabelTTF *freezingLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(7));
	cost = GameConfig::getInstance()->m_freezingCost;
	str = freezingLabel->getString();
	sprintf(temp, str, cost);
	freezingLabel->setString(temp);


	CCLabelTTF *hammerLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(10));
	cost = GameConfig::getInstance()->m_hammerCost;
	str = hammerLabel->getString();
	sprintf(temp, str, cost);
	hammerLabel->setString(temp);

	CCLabelTTF *addIronLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(34));
	int level = GameConfig::getInstance()->m_showIronLevel;
	str = addIronLabel->getString();
	sprintf(temp, str, level);
	addIronLabel->setString(temp);
}

void HelpLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
}

