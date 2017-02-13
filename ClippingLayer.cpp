#include "ClippingLayer.h"
#include "SquareModel.h"
#include "UserInfo.h"
#include "Config.h"
#include "GameController.h"
#include "GameUtil.h"

USING_NS_CC;

void ClippingLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Clipping, true);
}

void ClippingLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool ClippingLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 160));

	auto m_container = CCNode::create();
	CCClippingNode *m_clippingNode = CCClippingNode::create();
	m_clippingNode->setInverted(true);
	m_clippingNode->setAlphaThreshold(1.0f);
	m_clippingNode->setStencil(m_container);
	m_clippingNode->addChild(colorLayer);
	addChild(m_clippingNode);


	auto squares = SquareModel::theModel()->getSquares();
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto square = *iter;
		if (square->canRemoveByProps())
		{
			auto node = CCSprite::create("squares/fangkuai_hong1.png");
			node->setPosition(square->getPosition());
			m_container->addChild(node);
		}
	}

	return true;
}

bool ClippingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	auto location = pTouch->getLocation();
	auto squares = SquareModel::theModel()->getSquares();
	for (auto iter = squares.begin(); iter != squares.end(); ++iter)
	{
		auto square = *iter;
		CCRect rect(square->getPositionX() - square->getContentSize().width / 2, square->getPositionY() - square->getContentSize().height / 2, 
			square->getContentSize().width, square->getContentSize().height);
		if (square->canRemoveByProps() && rect.containsPoint(location))
		{
			SquareModel::theModel()->removeSameRowSquare(square);
			int count = UserInfo::getInstance()->getPropsCount(kProp_Clear);
			if (count > 0)
			{
				UserInfo::getInstance()->addPropsCount(kProp_Clear, -1);
			}
			else
			{
				UserInfo::getInstance()->addCoins(-DOUBLE_ATTACT_COST_COIN);
			}
			GameController::getInstance()->updateCoins();
			removeFromParent();
			GameUtil::saveGameInfo();
			break;
		}
	}
	removeFromParent();
	return true;
}