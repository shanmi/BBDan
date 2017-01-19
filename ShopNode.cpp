#include "ShopNode.h"
#include "UiLayout.h"
#include "CommonMacros.h"
#include "MyPurchase.h"
#include "GameConfig.h"
#include "GameUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;

ShopBallNode::ShopBallNode(const MarbleAttr attr)
:m_attr(attr)
{

}

ShopBallNode *ShopBallNode::create(const MarbleAttr attr)
{
	ShopBallNode *node = new ShopBallNode(attr);
	node->init();
	node->autorelease();
	return node;
}

bool ShopBallNode::init()
{
	m_mainLayout = UiLayout::create("layout/shop_ball_node.xml");
	m_mainLayout->setMenuTouchPriority(kPriority_Shop - 1);
	addChild(m_mainLayout);

	setContentSize(m_mainLayout->getContentSize());

	initLayout();
	return true;
}


void ShopBallNode::initLayout()
{
	/*CCMenuItem *leftItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(3));
	leftItem->setTarget(this, menu_selector(ShopSkinLayer::toLeftPanel));

	CCMenuItem *rightItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(4));
	rightItem->setTarget(this, menu_selector(ShopSkinLayer::toRightPanel));*/

	CCSprite * ballDesc = dynamic_cast<CCSprite *>((m_mainLayout->getChildById(1)));
	char temp[50] = { 0 };
	sprintf(temp, "shop/ball_%d.png", m_attr.skin);
	ballDesc->initWithFile(temp);
}

void ShopBallNode::buyItem(CCObject *pSender)
{
	CCLog("buyItem.....................................");
}


///////////////////////////////////////////////////////////////////////////////////////////////////
ShopCoinNode *ShopCoinNode::create(const ShopConfig &config)
{
	auto node = new ShopCoinNode(config);
	node->init();
	node->autorelease();
	return node;
}

bool ShopCoinNode::init()
{
	m_mainLayout = UiLayout::create("layout/shop_coin_node.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	auto size = m_mainLayout->getContentSize();
	m_mainLayout->setPosition(ccpMult(size, 0.5f));
	addChild(m_mainLayout);
	setContentSize(size);

	initMainLayout();

	return true;
}

void ShopCoinNode::initMainLayout()
{
	CCMenuItem * buyBtn = dynamic_cast<CCMenuItem *>((m_mainLayout->getChildById(2)));
	buyBtn->setTarget(this, menu_selector(ShopCoinNode::buyItem));

	CCLabelTTF *diamond_label = dynamic_cast<CCLabelTTF *>(m_mainLayout->getChildById(4));
	char temp[30] = { 0 };
	sprintf(temp, diamond_label->getString(), m_config.diamond);
	diamond_label->setString(temp);

	/*if (m_config.iconId > 5)
	{
		m_config.iconId = 5;
	}
	else if (m_config.iconId < 1)
	{
		m_config.iconId = 1;
	}*/
	/*sprintf(temp, "shop/icon_%d.png", m_config.iconId);
	CCSprite *iconSpr = CCSprite::create(temp);
	iconSpr->setScale(0.6f);
	iconSpr->setAnchorPoint(ccp(0.3f, 0.3f));
	iconSpr->setPosition(icon->getPosition());
	m_mainLayout->addChild(iconSpr);
	iconSpr->setZOrder(diamond->getZOrder() - 1);
	CCSprite *bg = dynamic_cast<CCSprite *>(m_mainLayout->getChildById(1));
	bg->setZOrder(iconSpr->getZOrder() - 1);*/

}

void ShopCoinNode::buyItem(CCObject *pSender)
{
	CCLOG("itemId==========================%d", m_config.id);
	MyPurchase::sharedPurchase()->payForProducts(m_config.id);
}

void ShopCoinNode::payCallBack(CCNode* nouse, int payType, int payResult, int payIntNoUse){
	if (payResult != PAY_RESULT_SUCCESS){
		return;
	}
	if (m_callback)
	{
		m_callback();
	}
}