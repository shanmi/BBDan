#include "LoginLayer.h"
#include "VisibleRect.h"
#include "SoundMgr.h"
#include "MyPurchase.h"
#include "UiLayout.h"
#include "GameConfig.h"
#include "LoginUtils.h"
#include "ActionSequence.h"
#include "CCFunctionAction.h"
#include "UserInfo.h"

bool LoginLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;
}

void LoginLayer::onEnter(){
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Login, true);
}

void LoginLayer::onExit(){
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool LoginLayer::init(){
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/login_panel.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setScale(0.9f);
	addChild(m_mainLayout);

	/*CCMenuItem * closeBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(2)));
	closeBtn->setTarget(this, menu_selector(LoginLayer::closePannel));*/

	CCMenuItem * closeItem = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(3)));
	closeItem->setTarget(this, menu_selector(LoginLayer::closePannel));

	int days = LoginUtils::getInstance()->getTargetDays();
	for (int i = 0; i < 7; i++){
		if (i < days)
		{
			std::string img = "login/login_get.png";
			if (i == 6)
			{
				img = "login/login_get2.png";
			}
			CCSprite *pAlready = CCSprite::create(img.c_str());
			CCSprite * panel = dynamic_cast<CCSprite *>((m_mainLayout->getChildById(4 + i)));
			pAlready->setPosition(ccp(panel->getPositionX(), panel->getPositionY()+10));
			m_mainLayout->addChild(pAlready, panel->getZOrder() + 1);
			if (i < days - 1)
			{
				panel->setColor(ccc3(60, 60, 60));
			}
			if (LoginUtils::getInstance()->checkLoginReward() && i == (days - 1)){
				pAlready->setVisible(false);
				pAlready->setScale(3.0f);
				auto actions = ActionSequence::create(pAlready);
				auto delay = CCDelayTime::create(0.5f);
				auto scaleTo = CCScaleTo::create(0.6f, 1.0f);
				auto callback = CCFunctionAction::create([=]()
				{
					auto progressTimer = CCProgressTimer::create(CCSprite::create(img.c_str()));
					progressTimer->setType(kCCProgressTimerTypeBar);
					progressTimer->setMidpoint(ccp(0, 1));
					progressTimer->setBarChangeRate(ccp(1, 0));
					progressTimer->runAction(CCProgressFromTo::create(0.2f, 0, 100));
					progressTimer->setPosition(pAlready->getPosition());
					m_mainLayout->addChild(progressTimer);

					pAlready->setVisible(false);
					panel->setColor(ccc3(60, 60, 60));
					getLoginReward();
					LoginUtils::getInstance()->setTargetDays();
				});
				actions->addAction(delay);
				actions->addAction(callback);
				actions->addAction(scaleTo);
				actions->runActions();
			}
		}
	}

	return true;
}

void LoginLayer::getLoginReward()
{
	int days = LoginUtils::getInstance()->getTargetDays();
	switch (days)
	{
	case 1:
		UserInfo::getInstance()->addPropsCount(kProp_Clear, 2);
		break;
	case 2:
		UserInfo::getInstance()->addCoins(15);
		break;
	case 3:
		UserInfo::getInstance()->addPropsCount(kProp_DoubleAttact, 3);
		break;
	case 4:
		UserInfo::getInstance()->addPropsCount(kProp_Freezing, 5);
		break;
	case 5:
		UserInfo::getInstance()->addCoins(30);
		break;
	case 6:
		UserInfo::getInstance()->addCoins(50);
		break;
	case 7:
		UserInfo::getInstance()->addPropsCount(kProp_Clear, 2);
		UserInfo::getInstance()->addPropsCount(kProp_DoubleAttact, 2);
		UserInfo::getInstance()->addPropsCount(kProp_Freezing, 2);
		UserInfo::getInstance()->unlockMarble(kMarble_Dispersed);
		break;
	}
}

void LoginLayer::closePannel(CCObject *pSender)
{
	removeFromParent();
}

