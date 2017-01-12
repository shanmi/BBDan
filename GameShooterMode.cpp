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
#include "GameData.h"

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

GameShooterMode::GameShooterMode()
: m_shootDegree(0)
, m_addMarbleCount(0)
, m_bIsShoot(false)
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

	CCSprite *character = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(3));
	character->setPosition(ccp(100, m_bottomLinePos + 15));

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
		GameData::getInstance()->addCoins(-DOUBLE_ATTACT_COST_COIN);
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
		SquareModel::theModel()->clearSquares();
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
	auto ballHints = BallHintModel::theModel()->createBallHints();
	for (auto iter = ballHints.begin(); iter != ballHints.end(); ++iter)
	{
		addChild(*iter);
	}

	m_touchPoint = CCSprite::create("start_touch_point.png");
	addChild(m_touchPoint);
	m_touchPoint->setVisible(false);

	CCSprite *character = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(3));
	m_arrow = CCSprite::create("arrow.png");
	m_arrow->setAnchorPoint(ccp(1.0f, 0.5f));
	m_arrow->setPosition(character->getPosition());
	addChild(m_arrow);
	m_arrow->setVisible(false);

	char temp[50] = { 0 };
	sprintf(temp, "X%d", 10);
	m_marbleCount = CCLabelTTF::create(temp, LABEL_FONT, 23);
	m_marbleCount->setPosition(ccp(character->getContentSize().width / 2, character->getContentSize().height + m_marbleCount->getContentSize().height));
	character->addChild(m_marbleCount);
}

void GameShooterMode::initPhysicBorder()
{
	m_world = Box2dFactory::getInstance()->getWorld();
	Box2dFactory::getInstance()->initBorders(m_topLinePos, m_bottomLinePos);
}

void GameShooterMode::initMarbles()
{
	for (int i = 0; i < 10; i++)
	{
		auto marble = MarbleModel::theModel()->createMarble();
		marble->setPosition(ccp(m_arrow->getPositionX(), m_bottomLinePos + marble->getContentSize().height / 2 + 4));
		addChild(marble);
		if (i == 0)
		{
			marble->setVisible(true);
			GameController::getInstance()->setTargetPos(marble->getPosition());
		}
		else
		{
			marble->setVisible(false);
		}
		auto streak = GameUtil::getMotionStreak();
		streak->setTag(100 + i);
		addChild(streak);
	}
	schedule(schedule_selector(GameShooterMode::updateStreak));
}

void GameShooterMode::updateStreak(float dt)
{
	auto marbles = MarbleModel::theModel()->getMarbles();
	for (int i = 0; i < marbles.size(); i++)
	{
		auto node = getChildByTag(100 + i);
		node->setPosition(marbles[i]->convertToWorldSpace(CCPointZero));
	}
}

void GameShooterMode::initSquares()
{
	auto squares = SquareModel::theModel()->createSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		node->setPosition(ccp((node->getContentSize().width / 2 + SQUARE_SPACING) + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 7.5));
		addChild(node);
	}
}

void GameShooterMode::addSquares()
{
	auto squares = SquareModel::theModel()->createSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		node->setPosition(ccp(node->getContentSize().width / 2 + SQUARE_SPACING + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 8.5));
		addChild(node);
	}
}

void GameShooterMode::update(float dt)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	if (isRoundOver)
	{
		auto targetPos = GameController::getInstance()->getTargetPos();
		m_arrow->setPosition(targetPos);
		return;
	}

	updateMarbleCount();

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
			if (!marble->isTrueStop())
			{
				m_addMarbleCount++;
				marble->moveToTargetPos();
			}
		}
	}
	
	//check squares by not check tool
	GameController::getInstance()->checkSquares();

}

bool GameShooterMode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
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

void GameShooterMode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	auto curPos = pTouch->getLocation();
	auto prePos = m_touchPoint->getPosition();

	m_shootDegree = GameUtil::getDegreeTwoPoints(curPos, prePos);
	m_arrow->setRotation(180 - m_shootDegree);
	if (m_shootDegree < 6 || m_shootDegree > 174)
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

void GameShooterMode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_arrow->isVisible())
	{
		m_touchPoint->setVisible(false);
		return;
	}
	
	GameController::getInstance()->startOneRound();
	m_touchPoint->setVisible(false);
	m_arrow->setVisible(false);
	BallHintModel::theModel()->setHintVisible(false);

	GameController::getInstance()->setRoundState(false);
	auto actions = ActionSequence::create(this);
	auto marbles = MarbleModel::theModel()->getMarbles();
	m_addMarbleCount = marbles.size();
	for (auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto &ball = (*iter);
		ball->setMovingState(true);
		auto action1 = CCFunctionAction::create([=]()
		{
			m_addMarbleCount--;
			ball->shoot(m_shootDegree);
			ball->setVisible(true);
		});
		auto action2 = CCDelayTime::create(0.1f);
		actions->addAction(action1);
		actions->addAction(action2);
	}
	actions->runActions();
}

void GameShooterMode::oneRoundEnd()
{
	bool isFreezing = SquareModel::theModel()->isFreezing();
	if (isFreezing)
	{
		SquareModel::theModel()->setSquareFreezing(false);
		GameController::getInstance()->setRoundState(true);
	}
	else
	{
		addSquares();
		SquareModel::theModel()->squareMoveDown();
	}

	//check marbles
	auto marbles = MarbleModel::theModel()->getMarbles();
	int addCount = MarbleModel::theModel()->checkMarblesCount();
	auto pos = GameController::getInstance()->getTargetPos();
	for (int i = 0; i < addCount; i++)
	{
		auto marble = MarbleModel::theModel()->createMarble();
		marble->setPosition(pos);
		addChild(marble);
		auto streak = GameUtil::getMotionStreak();
		streak->setTag(100 + marbles.size() + i);
		addChild(streak);
	}
	updateMarbleCount();

	//check tools when one round end and delete "0 score" tool
	GameController::getInstance()->checkSquares(true);

	//reset doubleAttact buttom
	GameController::getInstance()->resetAttactRate();
	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(6));

	//check character's position
	auto targetPos = GameController::getInstance()->getTargetPos();
	CCSprite *character = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(3));
	auto moveTo = CCMoveTo::create(0.5f, targetPos);
	character->runAction(moveTo);
}

void GameShooterMode::updateMarbles()
{
	// just show adding marble action
	m_addMarbleCount++;
}

void GameShooterMode::updateCoins()
{
	int coinCount = GameData::getInstance()->getCoins();
	std::string countStr = GameUtil::intToString(coinCount);
	CCLabelTTF *coinLabel = dynamic_cast<CCLabelTTF*>(m_bottomLayout->getChildById(5));
	coinLabel->setString(countStr.c_str());
}

void GameShooterMode::updateMarbleCount()
{
	char temp[50] = { 0 };
	if (m_addMarbleCount > MarbleModel::theModel()->getMarblesCount())
	{
		m_addMarbleCount = MarbleModel::theModel()->getMarblesCount();
	}
	sprintf(temp, "X%d", m_addMarbleCount);
	m_marbleCount->setString(temp);
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