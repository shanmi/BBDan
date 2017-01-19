#include "MainMenu.h"
#include "UiLayout.h"
#include "GameScene.h"
#include "GameShooterMode.h"
#include "UserInfo.h"
#include "GameUtil.h"
#include "MarbleAttr.h"
#include "MarbleNode.h"
#include "libao\LibaoDialog.h"
#include "MyPurchase.h"
#include "SoundMgr.h"
#include "ShopCoinLayer.h"
#include "ShopSkinLayer.h"
#include "MarbleModel.h"
#include "GameConfig.h"

USING_NS_CC;

void MainMenu::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Main, true);
	GameController::getInstance()->addView(this);
}

void MainMenu::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

bool MainMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void MainMenu::keyBackClicked()
{
	if (getChildByTag(kTag_Libao))
	{
		removeChildByTag(kTag_Libao);
		return;
	}
	if (getChildByTag(kTag_Shop))
	{
		removeChildByTag(kTag_Shop);
		return;
	}
	bool isExit = MyPurchase::sharedPurchase()->exitGame();
	if (isExit)
	{
		CCDirector::sharedDirector()->end();
	}
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

	bool isFirstIn = GameController::getInstance()->isFirshInGame();
	if (isFirstIn)
	{
		GameController::getInstance()->setFirstInGame(false);
		toLibao1(NULL);
	}

	setKeypadEnabled(true);

	return true;
}

void MainMenu::initLayout()
{
	CCSprite *logo = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(7));
	//logo->runAction(getAnimation(2.0f, 1.0f));

	auto attr = MarbleModel::theModel()->getMarbleAttr();
	m_marble = MarbleNode::create(attr);
	m_marble->setPosition(logo->getPosition());
	auto jump = CCJumpBy::create(1.0f, ccp(0, 0), 100, 1);
	auto action1 = CCRepeatForever::create(jump);
	m_marble->runAction(action1);
	m_mainLayout->addChild(m_marble);

	CCMenuItem *startGameOne = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(2));
	startGameOne->setTarget(this, menu_selector(MainMenu::toStartGameOne));
	auto action2 = GameUtil::getScaleAction();
	startGameOne->runAction(action2);

	CCMenuItem *startGameTwo = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(3));
	startGameTwo->setTarget(this, menu_selector(MainMenu::toStartGameTwo));
	startGameTwo->setVisible(false);

	CCMenuItem *toShop = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	toShop->setTarget(this, menu_selector(MainMenu::toShopLayer));
	toShop->setVisible(false);

	CCMenuItem *toSkin = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	toSkin->setTarget(this, menu_selector(MainMenu::toSkinLayer));
	//toSkin->setVisible(false);

	CCMenuItem *toMoreGame = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(8));
	toMoreGame->setTarget(this, menu_selector(MainMenu::toMoreGame));
	toMoreGame->setVisible(false);

	CCMenuItem *libao1 = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(9));
	libao1->setTarget(this, menu_selector(MainMenu::toLibao1));
	auto action3 = GameUtil::getScaleAction();
	libao1->runAction(action3);

	CCMenuItem *libao2 = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(10));
	libao2->setTarget(this, menu_selector(MainMenu::toLibao2));
	auto action4 = GameUtil::getScaleAction();
	libao2->runAction(action4);

	CCMenuItem *toMusicOff = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	toMusicOff->setTarget(this, menu_selector(MainMenu::toSoundSwitch));

	CCMenuItem *toMusicOn = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(11));
	toMusicOn->setTarget(this, menu_selector(MainMenu::toSoundSwitch));
	bool isMute = UserInfo::getInstance()->isMute();
	toMusicOff->setVisible(isMute);
	toMusicOn->setVisible(!isMute);

	CCLabelTTF *userIdLabel = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(13));
	if (userIdLabel)
	{
		std::string userId = MyPurchase::sharedPurchase()->getUserId();
		char temp[50] = { 0 };
		sprintf(temp, userIdLabel->getString(), userId.c_str());
		userIdLabel->setString(temp);
	}
	
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
	ShopCoinLayer *shopLayer = ShopCoinLayer::create();
	addChild(shopLayer, kZOrder_Shop, kTag_Shop);
}

void MainMenu::toSettingLayer(CCObject *pSender)
{

}
void MainMenu::toSkinLayer(CCObject *pSender)
{
	ShopSkinLayer *skinLayer = ShopSkinLayer::create();
	addChild(skinLayer, kZOrder_Shop, kTag_Shop);
}

void MainMenu::toMoreGame(CCObject *pSender)
{

}

void MainMenu::toLibao1(CCObject *pSender)
{
	LibaoDialog *dialog = LibaoDialog::create(PAY_TYPE_TIME_LIBAO);
	addChild(dialog, KZOrder_LibaoLayer, kTag_Libao);
}

void MainMenu::toLibao2(CCObject *pSender)
{
	LibaoDialog *dialog = LibaoDialog::create(PAY_TYPE_COIN_LIBAO);
	addChild(dialog, KZOrder_LibaoLayer, kTag_Libao);
}

void MainMenu::toSoundSwitch(CCObject *pSender)
{
	bool isMute = UserInfo::getInstance()->isMute();
	isMute = !isMute;
	CCMenuItem *toMusicOn = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(11));
	CCMenuItem *toMusicOff = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	toMusicOff->setVisible(isMute);
	toMusicOn->setVisible(!isMute);
	SoundMgr::theMgr()->setMute(isMute);
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

void MainMenu::updateMarbles()
{
	auto attr = MarbleModel::theModel()->getMarbleAttr();
	char temp[50] = { 0 };
	sprintf(temp, "marbles/ball_%d.png", attr.skin);
	m_marble->initWithFile(temp);
}