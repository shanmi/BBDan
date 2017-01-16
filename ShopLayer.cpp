#include "ShopLayer.h"
#include "CommonMacros.h"
#include "PageView.h"
#include "ShopNode.h"
#include "UiLayout.h"
#include "ListSlideView.h"
#include "MarbleAttr.h"
#include "GameConfig.h"
#include "UserInfo.h"

USING_NS_CC;

enum TagPanel
{
	Tag_CoinPanel,
	Tag_BallPanel
};

void ShopLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Shop, true);
	GameController::getInstance()->addView(this);
}

void ShopLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

bool ShopLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

CCScene* ShopLayer::scene()
{
	CCScene *scene = CCScene::create();
	ShopLayer *layer = ShopLayer::create();
	scene->addChild(layer);
	return scene;
}

bool ShopLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_mainLayout = UiLayout::create("layout/shop_panel.xml");
	addChild(m_mainLayout);

	initLayout();
}

void ShopLayer::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	closeItem->setTarget(this, menu_selector(ShopLayer::closePanel));

	CCMenuItem *coinItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(2));
	coinItem->setTarget(this, menu_selector(ShopLayer::toCoinPanel));

	CCMenuItem *ballItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(3));
	ballItem->setTarget(this, menu_selector(ShopLayer::toBallPanel));

	updateCoins();
}

void ShopLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
}

void ShopLayer::toCoinPanel(CCObject *pSender)
{
	if (getChildByTag(Tag_CoinPanel))
	{
		return;
	}
	if (getChildByTag(Tag_BallPanel))
	{
		removeChildByTag(Tag_BallPanel);
	}
	ListSlideView *shopList = ListSlideView::create(ccp(m_mainLayout->getContentSize().width*0.9f, m_mainLayout->getContentSize().height*0.6f));
	addChild(shopList);
	shopList->setTag(Tag_CoinPanel);
	shopList->setAnchorPoint(ccp(0.5f, 1));
	shopList->setPosition(ccp(m_mainLayout->getContentSize().width / 2, m_mainLayout->getContentSize().height * 0.65f));
	shopList->setSpacing(0, 0);
	shopList->setColumnSize(1);
	for (int i = 0; i < GameConfig::getInstance()->m_nDiamondCount; ++i)
	{
		ShopConfig config;
		config.id = GameConfig::getInstance()->m_nDiamond[i][0];
		config.price = GameConfig::getInstance()->m_nDiamond[i][1];
		config.diamond = GameConfig::getInstance()->m_nDiamond[i][2];
		config.present = GameConfig::getInstance()->m_nDiamond[i][3];
		config.iconId = GameConfig::getInstance()->m_nDiamond[i][4];
		auto node = ShopCoinNode::create(config);
		node->setCallbackListener(std::bind(&ShopLayer::payCallback, this));
		char temp[20];
		sprintf(temp, "%d", i);
		shopList->addNode(node);
	}
	shopList->setTouchPriority(kPriority_Shop - 2);
}

void ShopLayer::payCallback()
{
	CCLOG("ShopNode.....................................");
}

void ShopLayer::toBallPanel(CCObject *pSender)
{
	if (getChildByTag(Tag_BallPanel))
	{
		return;
	}
	if (getChildByTag(Tag_CoinPanel))
	{
		removeChildByTag(Tag_CoinPanel);
	}
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto size = ccp(winSize.width, winSize.height / 4);
	PageView *pageView = PageView::create(size);
	pageView->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	for (int i = 0; i < 3; i++)
	{
		NormalMarle attr = NormalMarle();
		auto node = ShopBallNode::create(attr);
		pageView->addNode(node);
	}
	pageView->setTouchPriority(kPriority_Shop);
	pageView->setTag(Tag_BallPanel);
	addChild(pageView);
}

void ShopLayer::updateCoins()
{
	UiLayout *layout = UiLayout::create("layout/shop_panel.xml");
	CCLabelTTF *temp_label = dynamic_cast<CCLabelTTF *>(layout->getChildById(5));
	CCLabelTTF *diamond_label = dynamic_cast<CCLabelTTF *>(m_mainLayout->getChildById(5));
	char temp[30] = { 0 };
	int coinCount = UserInfo::getInstance()->getCoins();
	sprintf(temp, temp_label->getString(), coinCount);
	diamond_label->setString(temp);
}