#include "GameShooterMode.h"
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
#include "UserInfo.h"
#include "MainMenu.h"

USING_NS_CC;

void GameShooterMode::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
	GameController::getInstance()->addView(this);
}

void GameShooterMode::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

void GameShooterMode::draw()
{
	CCLayer::draw();
	Box2dFactory::getInstance()->debugDraw();
}

void GameShooterMode::keyBackClicked()
{
	GameController::getInstance()->backToMainMenu();
}

GameShooterMode::GameShooterMode()
: m_moveCounter(0)
, m_freezingTime(0)
{

}

GameShooterMode::~GameShooterMode()
{

}

GameShooterMode *GameShooterMode::create()
{
	GameShooterMode *layer = new GameShooterMode;
	layer->init();
	layer->autorelease();
	return layer;
}

CCScene *GameShooterMode::scene()
{
	CCScene *scene = CCScene::create();
	GameShooterMode *layer = GameShooterMode::create();
	scene->addChild(layer);
	return scene;
}

bool GameShooterMode::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	setKeypadEnabled(true);
	Box2dFactory::getInstance()->initPhysics(true);

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

void GameShooterMode::initTopLayout()
{
	CCSprite *line_top = dynamic_cast<CCSprite*>(m_topLayout->getChildById(2));
	auto worldPos = m_topLayout->convertToWorldSpace(line_top->getPosition());
	m_topLinePos = worldPos.y;
}

void GameShooterMode::initBottomLayout()
{
	CCSprite *line_bottom = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(2));
	line_bottom->setTag(kTag_Wall);
	auto worldPos = line_bottom->convertToWorldSpace(CCPointZero);
	m_bottomLinePos = worldPos.y;
	Box2dFactory::getInstance()->createSquare(line_bottom, true);

	updateCoins();

	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(6));
	doubleAttactBtn->setTarget(this, menu_selector(GameShooterMode::onDoubleAttact));

	m_character = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(3));
	m_character->setPosition(ccp(100, m_bottomLinePos + 15));

	CCMenuItem *clearScreenBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(8));
	clearScreenBtn->setTarget(this, menu_selector(GameShooterMode::onClearScreen));

	CCMenuItem *freezingBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(9));
	freezingBtn->setTarget(this, menu_selector(GameShooterMode::onFreezing));

}

void GameShooterMode::onDoubleAttact(CCObject *pSender)
{
	bool ifCoinEnought = GameController::getInstance()->checkCoinsEnought();
	if (ifCoinEnought)
	{
		UserInfo::getInstance()->addCoins(-DOUBLE_ATTACT_COST_COIN);
		updateCoins();

		CCMenuItem *item = (CCMenuItem*)(pSender);
		item->setVisible(false);
		GameController::getInstance()->setDoubleAttact();
	}
	else
	{
		// show pay point
	}
}

void GameShooterMode::onClearScreen(CCObject *pSender)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	if (isRoundOver)
	{
		GameController::getInstance()->setRoundState(false);
		SquareModel::theModel()->removeAllSquares();
		oneRoundEnd();
	}
}

void GameShooterMode::onFreezing(CCObject *pSender)
{
	bool isFreezing = SquareModel::theModel()->isFreezing();
	if (!isFreezing)
	{
		SquareModel::theModel()->setSquareFreezing(true);
	}
}

void GameShooterMode::initGameLayout()
{

}

void GameShooterMode::initPhysicBorder()
{
	m_world = Box2dFactory::getInstance()->getWorld();
	Box2dFactory::getInstance()->initBorders(m_topLinePos, m_bottomLinePos);
}

void GameShooterMode::initMarbles()
{
	schedule(schedule_selector(GameShooterMode::updateStreak));

	addMarble(0);
}

void GameShooterMode::updateStreak(float dt)
{
	/*auto marbles = MarbleModel::theModel()->getMarbles();
	for (int i = 0; i < marbles.size(); i++)
	{
	auto node = getChildByTag(100 + i);
	node->setPosition(marbles[i]->convertToWorldSpace(CCPointZero));
	}*/
}

void GameShooterMode::initSquares()
{
	for (int i = 0; i < 3; i++)
	{
		auto squares = SquareModel::theModel()->createSquareList(false);
		for (int j = 0; j < squares.size(); j++)
		{
			auto node = squares[j];
			node->setPosition(ccp((node->getContentSize().width / 2 + SQUARE_SPACING) + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), 
				m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * (i + 6.5)));
			addChild(node);
		}
	}
}

void GameShooterMode::addMarble(float dt)
{
	auto marbles = MarbleModel::theModel()->getMarbles();
	auto actions = ActionSequence::create(this);
	MarbleAttr m_attr = FasterMarle();
	auto ball = MarbleModel::theModel()->createMarble();
	ball->setPosition(ccp(m_character->getPositionX(), m_bottomLinePos + ball->getContentSize().height / 2 + 4));
	addChild(ball);
	ball->setMovingState(true);
	ball->shooterShoot();
	ball->setVisible(true);
	auto attr = MarbleModel::theModel()->getMarbleAttr();
	auto action1 = CCDelayTime::create(0.2f / attr.speed);
	auto action2 = CCFunctionAction::create([=]()
	{
		addMarble(0);
	});
	actions->addAction(action1);
	actions->addAction(action2);
	actions->runActions();

	auto streak = GameUtil::getMotionStreak();
	streak->setTag(100 + marbles.size());
	addChild(streak);
}

void GameShooterMode::addSquares()
{
	auto squares = SquareModel::theModel()->createSquareList(false);
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		node->setPosition(ccp(node->getContentSize().width / 2 + SQUARE_SPACING + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), 
			m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 8.5));
		addChild(node);
	}
}

void GameShooterMode::update(float dt)
{

	int32 velocityIterations = 8;
	int32 positionIterations = 1;

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
			MarbleModel::theModel()->removeMarble(marble);
		}
	}

	bool isFreezing = SquareModel::theModel()->isFreezing();
	if (isFreezing)
	{
		m_freezingTime += dt;
		if (m_freezingTime > FREEZING_TIME)
		{
			m_freezingTime = 0;
			SquareModel::theModel()->setSquareFreezing(false);
		}
	}
	else
	{
		auto squares = SquareModel::theModel()->getSquares();
		for (auto iter = squares.begin(); iter != squares.end(); ++iter)
		{
			auto square = *iter;
			square->setPosition(ccp(square->getPositionX(), square->getPositionY() - (square->getContentSize().height + SQUARE_SPACING) / BALL_MOVE_SPEED));
			if (square->getPositionY() < m_bottomLinePos)
			{
				SquareModel::theModel()->removeSquareNode(square);
			}
		}
		m_moveCounter++;
		if (m_moveCounter > BALL_MOVE_SPEED)
		{
			m_moveCounter = 0;
			addSquares();
		}
	}
	
	//check squares by not check tool
	GameController::getInstance()->checkSquares(true);

}

bool GameShooterMode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	auto location = pTouch->getLocation();
	m_character->setPositionX(location.x - m_character->getContentSize().width/2);
	return true;
}

void GameShooterMode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	auto location = pTouch->getLocation();
	m_character->setPositionX(location.x - m_character->getContentSize().width / 2);
}

void GameShooterMode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

}

void GameShooterMode::oneRoundEnd()
{
	//check tools when one round end and delete "0 score" tool
	GameController::getInstance()->checkSquares(true);

	//reset doubleAttact buttom
	GameController::getInstance()->resetAttactRate();
	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(6));

}

void GameShooterMode::updateMarbles()
{
	// just show adding marble action
}

void GameShooterMode::updateCoins()
{
	int coinCount = UserInfo::getInstance()->getCoins();
	std::string countStr = GameUtil::intToString(coinCount);
	CCLabelTTF *coinLabel = dynamic_cast<CCLabelTTF*>(m_bottomLayout->getChildById(5));
	coinLabel->setString(countStr.c_str());
}

void GameShooterMode::addSquareNode(SquareNode *node)
{
	addChild(node);
}

void GameShooterMode::showGameOver()
{
	CCLog("is game over");
	SquareModel::theModel()->removeBelowSquares();
	//GameController::getInstance()->setRoundState(false);
}