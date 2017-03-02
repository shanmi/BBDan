#include "MarbleLibao.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"

USING_NS_CC;

void MarbleLibao::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Libao, true);
}

void MarbleLibao::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool MarbleLibao::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

CCScene* MarbleLibao::scene()
{
	CCScene *scene = CCScene::create();
	MarbleLibao *layer = MarbleLibao::create();
	scene->addChild(layer);
	return scene;
}

bool MarbleLibao::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_mainLayout = UiLayout::create("layout/marble_libao.xml");
	m_mainLayout->setMenuTouchPriority(kPriority_Libao - 1);
	addChild(m_mainLayout);

	initLayout();

	return true;
}

void MarbleLibao::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	closeItem->setTarget(this, menu_selector(MarbleLibao::closePanel));

	CCMenuItem *ballItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(3));
	ballItem->setTarget(this, menu_selector(MarbleLibao::buyLibao));

}

void MarbleLibao::closePanel(CCObject *pSender)
{
	removeFromParent();
}

void MarbleLibao::buyLibao(CCObject *pSender)
{
	MyPurchase::sharedPurchase()->payForProducts(PAY_TYPE_MARBLE_LIBAO);
}