#include "MainMenu.h"
#include "UiLayout.h"
#include "GameScene.h"
#include "GameShooterMode.h"
#include "ShopLayer.h"
#include "UserInfo.h"
#include "GameUtil.h"
#include "MarbleAttr.h"
#include "MarbleNode.h"
#include "libao\LibaoDialog.h"
#include "MyPurchase.h"

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
	CCSprite *logo = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(7));
	logo->runAction(getAnimation(2.0f, 1.0f));

	auto m_attr = NormalMarle();
	MarbleNode *marble = MarbleNode::create(m_attr);
	marble->setPosition(logo->getPosition());
	auto jump = CCJumpBy::create(1.0f, ccp(0, 0), 100, 1);
	auto action1 = CCRepeatForever::create(jump);
	marble->runAction(action1);
	m_mainLayout->addChild(marble);

	CCMenuItem *startGameOne = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(2));
	startGameOne->setTarget(this, menu_selector(MainMenu::toStartGameOne));
	auto action2 = GameUtil::getScaleAction();
	startGameOne->runAction(action2);

	CCMenuItem *startGameTwo = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(3));
	startGameTwo->setTarget(this, menu_selector(MainMenu::toStartGameTwo));
	startGameTwo->setVisible(false);

	CCMenuItem *toSetting = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	toSetting->setTarget(this, menu_selector(MainMenu::toSettingLayer));

	CCMenuItem *toShop = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	toShop->setTarget(this, menu_selector(MainMenu::toShopLayer));

	CCMenuItem *toSkin = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	toSkin->setTarget(this, menu_selector(MainMenu::toSkinLayer));

	CCMenuItem *toMoreGame = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(8));
	toMoreGame->setTarget(this, menu_selector(MainMenu::toMoreGame));

	CCMenuItem *libao1 = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(9));
	libao1->setTarget(this, menu_selector(MainMenu::toLibao1));

	CCMenuItem *libao2 = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(10));
	libao2->setTarget(this, menu_selector(MainMenu::toLibao2));
}

void MainMenu::toStartGameOne(CCObject* pSender)
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene *pScene = GameScene::scene();
	pDirector->replaceScene(pScene);
}

void MainMenu::toStartGameTwo(CCObject* pSender)
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene *pScene = GameShooterMode::scene();
	pDirector->replaceScene(pScene);
}

void MainMenu::toShopLayer(CCObject *pSender)
{
	ShopLayer *shopLayer = ShopLayer::create();
	addChild(shopLayer);
}

void MainMenu::toSettingLayer(CCObject *pSender)
{

}
void MainMenu::toSkinLayer(CCObject *pSender)
{

}

void MainMenu::toMoreGame(CCObject *pSender)
{

}

void MainMenu::toLibao1(CCObject *pSender)
{
	LibaoDialog *dialog = LibaoDialog::create(PAY_TYPE_COIN_LIBAO);
	addChild(dialog);
}

void MainMenu::toLibao2(CCObject *pSender)
{
	LibaoDialog *dialog = LibaoDialog::create(PAY_TYPE_FISH_LIBAO);
	addChild(dialog);
}

void MainMenu::toSoundSwitch(CCObject *pSender)
{
	bool isMute = UserInfo::getInstance()->isMute();
	CCMenuItem *switchOn = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	CCMenuItem *switchOff = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	if (isMute)
	{
		switchOn->setVisible(true);
		switchOff->setVisible(false);
	}
	else
	{
		switchOn->setVisible(false);
		switchOff->setVisible(true);
	}
	UserInfo::getInstance()->setMute(!isMute);
}

CCAction *MainMenu::getAnimation(float duration, float rotation)
{
	float kScaleTo = 1.1f;
	auto left = CCSpawn::create(CCScaleTo::create(duration, 1.0f, kScaleTo), CCRotateTo::create(duration, -rotation), NULL);
	auto normal1 = CCSpawn::create(CCScaleTo::create(duration, 1.0f, 1.0f), CCRotateTo::create(duration, 0), NULL);
	auto right = CCSpawn::create(CCScaleTo::create(duration, 1.0f, kScaleTo), CCRotateTo::create(duration, rotation), NULL);
	auto normal2 = CCSpawn::create(CCScaleTo::create(duration, 1.0f, 1.0f), CCRotateTo::create(duration, 0), NULL);
	auto anim = CCRepeatForever::create(CCSequence::create(left, normal1, right, normal2, NULL));
	return anim;
}