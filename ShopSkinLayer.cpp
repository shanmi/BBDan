#include "ShopSkinLayer.h"
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
#include "LibaoDialog.h"
#include "MarbleModel.h"

USING_NS_CC;

enum TagPanel
{
	Tag_CoinPanel,
	Tag_BallPanel
};

void ShopSkinLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Shop, true);
	GameController::getInstance()->addView(this);
}

void ShopSkinLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

bool ShopSkinLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

CCScene* ShopSkinLayer::scene()
{
	CCScene *scene = CCScene::create();
	ShopSkinLayer *layer = ShopSkinLayer::create();
	scene->addChild(layer);
	return scene;
}

ShopSkinLayer *ShopSkinLayer::create(int marbleType /* = kMarble_Normal */)
{
	ShopSkinLayer *layer = new ShopSkinLayer();
	layer->init(marbleType);
	layer->autorelease();
	return layer;
}

bool ShopSkinLayer::init(int marbleType)
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 200));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/shop_ball_panel.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Shop - 3);
	addChild(m_mainLayout);
	m_mainLayout->setScale(0.0f);
	auto action = CCScaleTo::create(0.2f, 1.0f);
	m_mainLayout->runAction(action);

	initLayout();
	initSkinLayout();
	updateCoins();
	moveToPageIndex(marbleType);

	return true;
}

void ShopSkinLayer::initLayout()
{
	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	closeItem->setTarget(this, menu_selector(ShopSkinLayer::closePanel));

	CCMenuItem *buyItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(7));
	buyItem->setTarget(this, menu_selector(ShopSkinLayer::payMarbleItem));

	CCMenuItem * useBallItem = dynamic_cast<CCMenuItem*>((m_mainLayout->getChildById(9)));
	useBallItem->setTarget(this, menu_selector(ShopSkinLayer::useMarbleItem));

	CCMenuItem * toLeftItem = dynamic_cast<CCMenuItem*>((m_mainLayout->getChildById(2)));
	toLeftItem->setTarget(this, menu_selector(ShopSkinLayer::toLeftPanel));
	//toLeftItem->setVisible(false);

	CCMenuItem * toRightItem = dynamic_cast<CCMenuItem*>((m_mainLayout->getChildById(3)));
	toRightItem->setTarget(this, menu_selector(ShopSkinLayer::toRightPanel));

	auto action1 = GameUtil::getFadeInOutAction();
	toLeftItem->runAction(action1);
	auto action2 = GameUtil::getFadeInOutAction();
	toRightItem->runAction(action2);

	updatePage(0);
}

void ShopSkinLayer::initSkinLayout()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto size = ccp(winSize.width, winSize.height * 0.3f);
	m_pageView = PageView::create(size);
	m_pageView->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pageView->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));

	auto attr1 = NormalMarle();
	auto node1 = ShopBallNode::create(attr1);
	m_pageView->addNode(node1);

	auto attr2 = FasterMarle();
	auto node2 = ShopBallNode::create(attr2);
	m_pageView->addNode(node2);

	auto attr3 = BiggerMarle();
	auto node3 = ShopBallNode::create(attr3);
	m_pageView->addNode(node3);

	auto attr4 = DispersedMarle();
	auto node4 = ShopBallNode::create(attr4);
	m_pageView->addNode(node4);

	auto attr5 = BombMarle();
	auto node5 = ShopBallNode::create(attr5);
	m_pageView->addNode(node5);

	m_pageView->setTouchPriority(kPriority_Shop - 2);
	m_pageView->setTag(Tag_BallPanel);
	m_pageView->setCallbackListener(std::bind(&ShopSkinLayer::updatePage, this, std::placeholders::_1));
	addChild(m_pageView);

	m_pageView->setScale(0.0f);
	auto action = CCScaleTo::create(0.2f, 1.0f);
	m_pageView->runAction(action);
}

void ShopSkinLayer::closePanel(CCObject *pSender)
{
	removeFromParent();
}

void ShopSkinLayer::toLeftPanel(CCObject *pSender)
{
	int pageIndex = m_pageView->moveToLeft();
	updatePage(pageIndex);
}

void ShopSkinLayer::toRightPanel(CCObject *pSender)
{
	int pageIndex = m_pageView->moveToRight();
	updatePage(pageIndex);
}

void ShopSkinLayer::updatePage(int pageIndex)
{
	m_pageIndex = pageIndex;
	CCMenuItem * toLeftItem = dynamic_cast<CCMenuItem*>((m_mainLayout->getChildById(2)));
	CCMenuItem * toRightItem = dynamic_cast<CCMenuItem*>((m_mainLayout->getChildById(3)));
	CCMenuItem * buyItem = dynamic_cast<CCMenuItem*>((m_mainLayout->getChildById(7)));
	CCMenuItem * useBallItem = dynamic_cast<CCMenuItem*>((m_mainLayout->getChildById(9)));
	CCLabelAtlas *ballPrice = dynamic_cast<CCLabelAtlas *>((m_mainLayout->getChildById(8)));
	ballPrice->setZOrder(buyItem->getZOrder() + 1);

	if (pageIndex == 0)
	{
		toLeftItem->setVisible(false);
		toRightItem->setVisible(true);
	}
	else if (pageIndex == m_pageView->getCount() - 1)
	{
		toLeftItem->setVisible(true);
		toRightItem->setVisible(false);
	}
	else
	{
		toLeftItem->setVisible(true);
		toRightItem->setVisible(true);
	}

	if (pageIndex == 0)
	{
		buyItem->setVisible(false);
		ballPrice->setVisible(false);
	}
	else
	{
		buyItem->setVisible(true);
		ballPrice->setVisible(true);
		int price = GameConfig::getInstance()->m_nMarble[pageIndex - 1][2];
		std::string priceStr = GameUtil::intToString(price);
		ballPrice->setString(priceStr.c_str());
	}

	bool isUnlock = UserInfo::getInstance()->isUnlock(m_pageIndex);
	useBallItem->setVisible(isUnlock);
	buyItem->setVisible(!isUnlock);
	ballPrice->setVisible(!isUnlock);
}

void ShopSkinLayer::moveToPageIndex(int pageIndex)
{
	m_pageView->moveToPage(pageIndex);
	updatePage(pageIndex);
}

void ShopSkinLayer::payMarbleItem(CCObject *pSender)
{
	int diamondCount = UserInfo::getInstance()->getCoins();
	int price = GameConfig::getInstance()->m_nMarble[m_pageIndex - 1][2];
	if (diamondCount > price)
	{
		UserInfo::getInstance()->addCoins(-price);
		UserInfo::getInstance()->unlockMarble(m_pageIndex);
		GameController::getInstance()->updateCoins();
	}
	else
	{
		LibaoDialog *dialog = LibaoDialog::create(PAY_TYPE_COIN_LIBAO);
		addChild(dialog);
	}
}

void ShopSkinLayer::useMarbleItem(CCObject *pSender)
{
	MarbleAttr attr;
	switch (m_pageIndex)
	{
	case 0:
		attr = NormalMarle();
		break;
	case 1:
		attr = FasterMarle();
		break;
	case 2:
		attr = BiggerMarle();
		break;
	case 3:
		attr = DispersedMarle();
		break;
	case 4:
		attr = BombMarle();
		break;
	}
	removeFromParent();
	MarbleModel::theModel()->setMarbleAttr(attr);
	GameController::getInstance()->updateMarbles();
	GameController::getInstance()->updateMarbleType(m_pageIndex);
}

void ShopSkinLayer::updateCoins()
{
	CCLabelAtlas *diamond_label = dynamic_cast<CCLabelAtlas *>(m_mainLayout->getChildById(6));
	int diamondCount = UserInfo::getInstance()->getCoins();
	std::string countStr = GameUtil::intToString(diamondCount);
	diamond_label->setString(countStr.c_str());

	updatePage(m_pageIndex);
}