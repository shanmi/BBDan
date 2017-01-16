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
#include "UserInfo.h"
#include "MainMenu.h"
#include "SoundMgr.h"

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

void GameScene::keyBackClicked()
{
	GameController::getInstance()->backToMainMenu();
}

GameScene::GameScene()
: m_shootDegree(0)
, m_addMarbleCount(0)
, m_bIsShoot(false)
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

	setKeypadEnabled(true);
	Box2dFactory::getInstance()->initPhysics(false);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_topLayout = UiLayout::create("layout/common_top.xml");
	m_topLayout->setPosition(ccp(0, winSize.height - m_topLayout->getContentSize().height));
	addChild(m_topLayout, kZOrder_Layout);
	initTopLayout();

	m_bottomLayout = UiLayout::create("layout/common_bottom.xml");
	m_bottomLayout->setPosition(ccp(0, 0));
	addChild(m_bottomLayout, kZOrder_Layout);
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
	line_bottom->setTag(kTag_Wall);
	auto worldPos = line_bottom->convertToWorldSpace(CCPointZero);
	m_bottomLinePos = worldPos.y;
	Box2dFactory::getInstance()->createSquare(line_bottom, true);

	updateCoins();

	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(6));
	doubleAttactBtn->setTarget(this, menu_selector(GameScene::onDoubleAttact));

	CCSprite *character_body = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(3));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(12));
	m_arrow = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(11));
	//m_arrow->setVisible(false);
	GameController::getInstance()->setTargetPos(m_arrow->getPosition());

	CCMenuItem *clearScreenBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(8));
	clearScreenBtn->setTarget(this, menu_selector(GameScene::onClearScreen));

	CCMenuItem *freezingBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(9));
	freezingBtn->setTarget(this, menu_selector(GameScene::onFreezing));

}

void GameScene::onDoubleAttact(CCObject *pSender)
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

void GameScene::onClearScreen(CCObject *pSender)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	if (isRoundOver)
	{
		GameController::getInstance()->setRoundState(false);
		SquareModel::theModel()->removeAllSquares();
		oneRoundEnd();
	}
}

void GameScene::onFreezing(CCObject *pSender)
{
	bool isFreezing = SquareModel::theModel()->isFreezing();
	if (!isFreezing)
	{
		SquareModel::theModel()->setSquareFreezing(true);
	}
}

void GameScene::initGameLayout()
{
	auto ballHints = BallHintModel::theModel()->createBallHints();
	for (auto iter = ballHints.begin(); iter != ballHints.end(); ++iter)
	{
		addChild(*iter, kZOrder_Layout);
	}

	m_touchPoint = CCSprite::create("start_touch_point.png");
	addChild(m_touchPoint, kZOrder_Layout);
	m_touchPoint->setVisible(false);

	CCSprite *character_head = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(12));
	char temp[50] = { 0 };
	sprintf(temp, "x%d", 10);
	m_marbleCount = CCLabelTTF::create(temp, LABEL_FONT, 23);
	m_marbleCount->setPosition(ccp(character_head->getContentSize().width / 2, character_head->getContentSize().height + m_marbleCount->getContentSize().height));
	character_head->addChild(m_marbleCount);
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
		marble->setPosition(ccp(m_arrow->getPositionX(), m_bottomLinePos + marble->getContentSize().height / 2 + 4));
		addChild(marble, kZOrder_Marble);
		if (i == 0)
		{
			marble->setVisible(true);
		}
		else
		{
			marble->setVisible(false);
		}
		auto streak = GameUtil::getMotionStreak();
		streak->setTag(100 + i);
		addChild(streak, kZOrder_Marble);
	}
	schedule(schedule_selector(GameScene::updateStreak));
}

void GameScene::updateStreak(float dt)
{
	auto marbles = MarbleModel::theModel()->getMarbles();
	for (int i = 0; i < marbles.size(); i++)
	{
		auto node = getChildByTag(100 + i);
		node->setPosition(marbles[i]->convertToWorldSpace(CCPointZero));
	}
}

void GameScene::initSquares()
{
	auto squares = SquareModel::theModel()->createSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		node->setPosition(ccp((node->getContentSize().width / 2 + SQUARE_SPACING) + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), 
			m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 7.5));
		addChild(node, kZOrder_Square);
	}
}

void GameScene::addSquares()
{
	auto squares = SquareModel::theModel()->createSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		node->setPosition(ccp(node->getContentSize().width / 2 + SQUARE_SPACING + node->getIndex() * (node->getContentSize().width + SQUARE_SPACING), 
			m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 8.5));
		addChild(node, kZOrder_Square);
	}
}

void GameScene::update(float dt)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	if (isRoundOver)
	{
		/*auto targetPos = GameController::getInstance()->getTargetPos();
		m_arrow->setPosition(targetPos);*/
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

	BallHintModel::theModel()->updatePosition(curPos, prePos, m_arrow->getPosition(), m_arrow->getContentSize().width - 10);


	CCSprite *character_body = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(3));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(12));
	if (m_shootDegree < 90)
	{
		character_head->setPositionX(character_body->getPositionX() - 20);
		character_body->setFlipX(true);
		character_head->setFlipX(true);

		m_arrow->setPositionX(character_body->getPositionX() + 20);
		m_arrow->setFlipY(true);
	}
	else
	{
		character_head->setPositionX(character_body->getPositionX());
		character_body->setFlipX(false);
		character_head->setFlipX(false);

		m_arrow->setPositionX(character_body->getPositionX() - 15);
		m_arrow->setFlipY(false);
	}

	if (m_shootDegree > 60 && m_shootDegree < 90)
	{
		character_head->setRotation(60 - m_shootDegree);
	}
	else if (m_shootDegree > 90 && m_shootDegree < 120)
	{
		character_head->setRotation(120 - m_shootDegree);
	}
	else
	{
		character_head->setRotation(0);
	}
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_arrow->isVisible())
	{
		m_touchPoint->setVisible(false);
		return;
	}
	
	GameController::getInstance()->startOneRound();
	m_touchPoint->setVisible(false);
	m_arrow->setVisible(false);
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(12)); 
	character_head->setRotation(0);
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

void GameScene::oneRoundEnd()
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
		addChild(marble, kZOrder_Marble);
		auto streak = GameUtil::getMotionStreak();
		streak->setTag(100 + marbles.size() + i);
		addChild(streak, kZOrder_Marble);
	}
	updateMarbleCount();

	//check tools when one round end and delete "0 score" tool
	GameController::getInstance()->checkSquares(true);

	//reset doubleAttact buttom
	GameController::getInstance()->resetAttactRate();
	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(6));

	//check character's position
	auto targetPos = GameController::getInstance()->getTargetPos();
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(3));
	auto moveTo = CCMoveTo::create(0.5f, ccp(targetPos.x, character_body->getPositionY()));
	character_body->runAction(moveTo);

	CCSprite *character_head = dynamic_cast<CCSprite*>(m_bottomLayout->getChildById(12));
	auto moveTo2 = CCMoveBy::create(0.5f, ccp(targetPos.x - character_body->getPositionX(), 0));
	character_head->runAction(moveTo2);
}

void GameScene::updateMarbles()
{
	// just show adding marble action
	m_addMarbleCount++;
}

void GameScene::updateCoins()
{
	int coinCount = UserInfo::getInstance()->getCoins();
	std::string countStr = GameUtil::intToString(coinCount);
	CCLabelTTF *coinLabel = dynamic_cast<CCLabelTTF*>(m_bottomLayout->getChildById(5));
	coinLabel->setString(countStr.c_str());
}

void GameScene::updateMarbleCount()
{
	char temp[50] = { 0 };
	if (m_addMarbleCount > MarbleModel::theModel()->getMarblesCount())
	{
		m_addMarbleCount = MarbleModel::theModel()->getMarblesCount();
	}
	sprintf(temp, "x%d", m_addMarbleCount);
	m_marbleCount->setString(temp);
}

void GameScene::addSquareNode(SquareNode *node)
{
	addChild(node, kZOrder_Square);
}

void GameScene::showGameOver()
{
	CCLog("is game over");
	SoundMgr::theMgr()->playEffect(Effect_GameOver);
	SquareModel::theModel()->removeBelowSquares();
	//GameController::getInstance()->setRoundState(false);
}