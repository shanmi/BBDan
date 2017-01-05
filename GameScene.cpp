#include "GameScene.h"
#include "UiLayout.h"
#include "GameUtil.h"
#include "Config.h"
#include "SquareModel.h"
#include "BallHintModel.h"
#include "Box2dFactory.h"
#include "MarbleNode.h"
#include "MarbleModel.h"
#include "ActionSequence.h"
#include "CCFunctionAction.h"

USING_NS_CC;

void GameScene::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
	GameController::getInstance()->addView(this);
}

void GameScene::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

void GameScene::draw()
{
	CCLayer::draw();
	Box2dFactory::getInstance()->debugDraw();
}

GameScene::GameScene()
: m_shootDegree(0)
{

}

GameScene::~GameScene()
{

}

GameScene *GameScene::create()
{
	GameScene *layer = new GameScene;
	layer->init();
	layer->autorelease();
	return layer;
}

CCScene *GameScene::scene()
{
	CCScene *scene = CCScene::create();
	GameScene *layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_topLayout = UiLayout::create("layout/common_top.xml");
	m_topLayout->setPosition(ccp(0, winSize.height - m_topLayout->getContentSize().height));
	addChild(m_topLayout);
	initTopLayout();

	m_bottomLayout = UiLayout::create("layout/common_bottom.xml");
	m_bottomLayout->setPosition(ccp(0, 0));
	addChild(m_bottomLayout);
	initBottomLayout();
	initGameLayout();

	initPhysicBorder();

	initMarbles();
	initSquares();
	

	scheduleUpdate();
	return true;
}

void GameScene::initTopLayout()
{
	CCSprite *line_top = dynamic_cast<CCSprite*>(m_topLayout->getChildById(2));
	auto worldPos = m_topLayout->convertToWorldSpace(line_top->getPosition());
	m_topLinePos = worldPos.y;
}

void GameScene::initBottomLayout()
{
	CCSprite *line_bottom = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(2));
	line_bottom->setTag(100);
	auto worldPos = line_bottom->convertToWorldSpace(CCPointZero);
	m_bottomLinePos = worldPos.y;

	Box2dFactory::getInstance()->createSquare(line_bottom);
}

void GameScene::initGameLayout()
{
	auto ballHints = BallHintModel::theModel()->createBallHints();
	for (auto iter = ballHints.begin(); iter != ballHints.end(); ++iter)
	{
		addChild(*iter);
	}

	m_touchPoint = CCSprite::create("start_touch_point.png");
	addChild(m_touchPoint);
	m_touchPoint->setVisible(false);

	m_arrow = CCSprite::create("arrow.png");
	m_arrow->setAnchorPoint(ccp(1.0f, 0.5f));
	m_arrow->setPosition(ccp(100, m_bottomLinePos + 15));
	addChild(m_arrow);
	m_arrow->setVisible(false);
	GameController::getInstance()->setTargetPos(m_arrow->getPosition());
}

void GameScene::initPhysicBorder()
{
	m_world = Box2dFactory::getInstance()->getWorld();
	Box2dFactory::getInstance()->initBorders(m_topLinePos, m_bottomLinePos);
}

void GameScene::initMarbles()
{
	for (int i = 0; i < 1; i++)
	{
		auto marble = MarbleModel::theModel()->createMarble();
		marble->setPosition(m_arrow->getPosition());
		addChild(marble);
	}
}

void GameScene::initSquares()
{
	auto squares = SquareModel::theModel()->createSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		node->setPosition(ccp((node->getContentSize().width / 2 + SQUARE_SPACING) + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 7.5));
		addChild(node);
	}
}

void GameScene::addSquares()
{
	auto squares = SquareModel::theModel()->createSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		node->setPosition(ccp(node->getContentSize().width / 2 + SQUARE_SPACING + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 8.5));
		addChild(node);
	}
}

void GameScene::update(float dt)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	if (isRoundOver)
	{
		auto targetPos = GameController::getInstance()->getTargetPos();
		m_arrow->setPosition(targetPos);
		return;
	}

	int32 velocityIterations = 10;
	int32 positionIterations = 10;

	m_world->Step(dt, velocityIterations, positionIterations);
	m_world->ClearForces();

	auto marbles = MarbleModel::theModel()->getMarbles();
	for (auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto &marble = (*iter);
		auto body = marble->getBody();
		MarbleNode *ball = (MarbleNode*)(body->GetUserData());
		if (ball->isMoving())
		{
			b2Vec2 ballPosition = body->GetPosition();
			ball->setPosition(ccp(ballPosition.x * PTM_RATIO, ballPosition.y * PTM_RATIO));
		}
		else
		{
			marble->moveToTargetPos();
		}
		
	}
	
	auto squares = SquareModel::theModel()->getSquares();
	for (auto iter = squares.begin(); iter!=squares.end(); ++iter)
	{
		auto square = *iter;
		if (square->getScore() <= 0)
		{
			SquareModel::theModel()->removeSquareNode(square);
			return;
		}
	}
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	if (!isRoundOver)
	{
		return false;
	}
	auto location = pTouch->getLocation();
	m_touchPoint->setPosition(location);
	m_touchPoint->setVisible(true);

	return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	auto curPos = pTouch->getLocation();
	auto prePos = m_touchPoint->getPosition();

	m_shootDegree = GameUtil::getDegreeTwoPoints(curPos, prePos);
	m_arrow->setRotation(180 - m_shootDegree);
	if (m_shootDegree < 0)
	{
		m_arrow->setVisible(false);
		BallHintModel::theModel()->setHintVisible(false);
	}
	else
	{
		m_arrow->setVisible(true);
		BallHintModel::theModel()->setHintVisible(true);
	}

	BallHintModel::theModel()->updatePosition(curPos, prePos, m_arrow->getPosition(), m_arrow->getContentSize().width);
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_arrow->isVisible())
	{
		GameController::getInstance()->startOneRound();
	}
	m_touchPoint->setVisible(false);
	m_arrow->setVisible(false);
	BallHintModel::theModel()->setHintVisible(false);

	GameController::getInstance()->setRoundState(false);
	auto actions = ActionSequence::create(this);
	auto marbles = MarbleModel::theModel()->getMarbles();
	for (auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto &ball = (*iter);
		ball->setMovingState(true);
		auto action1 = CCFunctionAction::create([=]()
		{
			ball->shoot(m_shootDegree);
			ball->setVisible(true);
		});
		auto action2 = CCDelayTime::create(0.1f);
		actions->addAction(action1);
		actions->addAction(action2);
	}
	actions->runActions();
}

void GameScene::updateSquares()
{
	addSquares();
	SquareModel::theModel()->squareMoveDown();
}

void GameScene::showGameOver()
{
	CCLog("is game over");
	SquareModel::theModel()->removeBelowSquares();
}