#include "ShopCoinLayer.h"
#include "CommonMacros.h"
#include "PageView.h"
#include "ShopNode.h"
#include "UiLayout.h"
#include "ListSlideView.h"
#include "MarbleAttr.h"
#include "GameConfig.h"
#include "UserInfo.h"
#include "GameUtil.h"
#include "MyPurchase.h"

USING_NS_CC;

enum TagPanel
{
	Tag_CoinPanel,
	Tag_BallPanel
};

void ShopCoinLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Shop, true);
	GameController::getInstance()->addView(this);
}

void ShopCoinLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

bool ShopCoinLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

CCScene* ShopCoinLayer::scene()
{
	CCScene *scene = CCScene::create();
	ShopCoinLayer *layer = ShopCoinLayer::create();
	scene->addChild(layer);
	return scene;
}

bool ShopCoinLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/shop_coin_panel.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Shop - 1);
	addChild(m_mainLayout);

	initLayout();
	//initCoinsLayout();
	updateCoins();

	return true;
}

void ShopCoinLayer::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	closeItem->setTarget(this, menu_selector(ShopCoinLayer::closePanel));

	for (int i = 0; i < 4; i++)
	{
		CCMenuItem *ballItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(7 + i));
		ballItem->setTag(PAY_TYPE_DIAMOND1 + i);
		ballItem->setTarget(this, menu_selector(ShopCoinLayer::buyCoinItem));
	}
	

	updateCoins();
}

void ShopCoinLayer::initCoinsLayout()
{
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
		node->setCallbackListener(std::bind(&ShopCoinLayer::payCallback, this));
		char temp[20];
		sprintf(temp, "%d", i);
		shopList->addNode(node);
	}
	shopList->setTouchPriority(kPriority_Shop - 2);
}

void ShopCoinLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
}

void ShopCoinLayer::buyCoinItem(CCObject *pSender)
{
	int tag = ((CCNode*)pSender)->getTag();
	MyPurchase::sharedPurchase()->payForProducts(tag);
}

void ShopCoinLayer::payCallback()
{
	CCLOG("ShopNode.....................................");
}

void ShopCoinLayer::updateCoins()
{
	CCLabelAtlas *diamond_label = dynamic_cast<CCLabelAtlas *>(m_mainLayout->getChildById(6));
	int diamondCount = UserInfo::getInstance()->getCoins();
	std::string countStr = GameUtil::intToString(diamondCount);
	diamond_label->setString(countStr.c_str());
}