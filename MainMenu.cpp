#include "MainMenu.h"
#include "UiLayout.h"
#include "GameScene.h"
#include "GameShooterMode.h"
#include "ShopLayer.h"

USING_NS_CC;

void MainMenu::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Main, true);
}

void MainMenu::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool MainMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void MainMenu::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}

CCScene* MainMenu::scene()
{
	CCScene *scene = CCScene::create();
	MainMenu *layer = MainMenu::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenu::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_mainLayout = UiLayout::create("layout/main_menu.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Main - 1);
	addChild(m_mainLayout);

	initLayout();

	return true;
}

void MainMenu::initLayout()
{
	CCMenuItem *startGameOne = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(2));
	startGameOne->setTarget(this, menu_selector(MainMenu::onStartGameOne));

	CCMenuItem *startGameTwo = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(3));
	startGameTwo->setTarget(this, menu_selector(MainMenu::onStartGameTwo));

	CCMenuItem *toBallShop = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	toBallShop->setTarget(this, menu_selector(MainMenu::onBallShop));
}



void MainMenu::onStartGameOne(CCObject* pSender)
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene *pScene = GameScene::scene();
	pDirector->replaceScene(pScene);
}

void MainMenu::onStartGameTwo(CCObject* pSender)
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene *pScene = GameShooterMode::scene();
	pDirector->replaceScene(pScene);
}

void MainMenu::onBallShop(CCObject *pSender)
{
	ShopLayer *shopLayer = ShopLayer::create();
	addChild(shopLayer);
}