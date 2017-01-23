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
#include "PauseLayer.h"
#include "HelpLayer.h"
#include "MyPurchase.h"
#include "GameConfig.h"
#include "LibaoDialog.h"

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
, m_shootDegree(90)
, m_freezingTime(0)
, m_protectTime(0)
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
	m_mainLayout = UiLayout::create("layout/game_scene.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_mainLayout, kZOrder_Main);
	initMainLayout();

	m_characterLayout = UiLayout::create("layout/character_node.xml");
	m_characterLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_characterLayout->setPosition(ccpMult(winSize, 0.5f));
	m_characterLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_characterLayout, kZOrder_Character);
	initCharacterLayout();

	m_bottomLayout = UiLayout::create("layout/game_bottom.xml");
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

void GameShooterMode::initMainLayout()
{
	CCSprite *line_top = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(8));
	line_top->setVisible(false);
	auto worldPos = m_mainLayout->convertToWorldSpace(line_top->getPosition());
	m_topLinePos = worldPos.y;

	CCSprite *line_bottom = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(7));
	line_bottom->setPositionY(line_bottom->getPositionY() + 60);
	line_bottom->setVisible(true);
	line_bottom->setTag(kTag_Wall);
	worldPos = line_bottom->convertToWorldSpace(CCPointZero);
	m_bottomLinePos = worldPos.y;
	Box2dFactory::getInstance()->createSquare(line_bottom, true);

	CCMenuItem *pauseItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	pauseItem->setTarget(this, menu_selector(GameShooterMode::onPauseGame));
}

void GameShooterMode::initCharacterLayout()
{
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	m_character = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(11));
	/*character_body->setVisible(false);
	character_head->setVisible(false);
	m_arrow->setVisible(false);*/

	auto targetPos = GameController::getInstance()->getTargetPos();
	m_character->setRotation(m_shootDegree);
	if (targetPos.x != 0 && targetPos.y != 0)
	{
		character_body->setPosition(ccp(targetPos.x, character_body->getPositionY()));

		character_head->setPosition(ccp(targetPos.x, character_head->getPositionY()));
		//m_character->setVisible(false);
	}
	if (m_shootDegree > 60 && m_shootDegree < 90)
	{
		character_head->setRotation(60 - m_shootDegree);
	}
	else if (m_shootDegree >= 90 && m_shootDegree < 120)
	{
		character_head->setRotation(120 - m_shootDegree);
	}
	else
	{
		character_head->setRotation(0);
	}
	GameController::getInstance()->setTargetPos(m_character->getPosition());
}

void GameShooterMode::onPauseGame(CCObject *pSender)
{
	if (getChildByTag(kTag_Pause))
	{
		removeChildByTag(kTag_Pause);
		return;
	}
	if (getChildByTag(kTag_GameOver))
	{
		return;
	}
	PauseLayer *pauseLayer = PauseLayer::create();
	addChild(pauseLayer, KZOrder_PauseLayer, kTag_Pause);
}

void GameShooterMode::initBottomLayout()
{
	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(5));
	doubleAttactBtn->setTarget(this, menu_selector(GameShooterMode::onDoubleAttact));

	CCMenuItem *clearScreenBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(4));
	clearScreenBtn->setTarget(this, menu_selector(GameShooterMode::onClearScreen));

	CCMenuItem *freezingBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(3));
	freezingBtn->setTarget(this, menu_selector(GameShooterMode::onFreezing));

	CCMenuItem *helpBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(9));
	//helpBtn->setTarget(this, menu_selector(GameShooterMode::onHelpPanel));
	auto action = GameUtil::getScaleAction();
	helpBtn->runAction(action);

}

void GameShooterMode::onDoubleAttact(CCObject *pSender)
{
	int count = UserInfo::getInstance()->getPropsCount(kProp_DoubleAttact);
	bool ifCoinEnought = GameController::getInstance()->checkCoinsEnought();
	if (!ifCoinEnought && count <= 0)
	{
		// show pay point
		showLibaoDiaolg();
		return;
	}
	if (!m_bIsDoubleAttact)
	{
		if (count > 0)
		{
			UserInfo::getInstance()->addPropsCount(kProp_DoubleAttact, -1);
		}
		else
		{
			UserInfo::getInstance()->addCoins(-DOUBLE_ATTACT_COST_COIN);
		}
		updateCoins();
		m_bIsDoubleAttact = true;

		GameController::getInstance()->setDoubleAttact();
		CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(5));
		auto scaleby = CCScaleBy::create(0.5f, 1.2f);
		auto reverse = scaleby->reverse();
		auto sequence = CCSequence::create(scaleby, reverse, NULL);
		auto action = CCRepeatForever::create(sequence);
		doubleAttactBtn->runAction(action);
	}
	else
	{
		// show pay point
	}
}

void GameShooterMode::onClearScreen(CCObject *pSender)
{
	int count = UserInfo::getInstance()->getPropsCount(kProp_Clear);
	bool ifCoinEnought = GameController::getInstance()->checkCoinsEnought();
	if (!ifCoinEnought && count <= 0)
	{
		// show pay point
		showLibaoDiaolg();
	}
	if (count > 0)
	{
		UserInfo::getInstance()->addPropsCount(kProp_Clear, -1);
	}
	else
	{
		UserInfo::getInstance()->addCoins(-DOUBLE_ATTACT_COST_COIN);
	}
	updateCoins();
	GameController::getInstance()->setRoundState(false);
	SquareModel::theModel()->removeAllSquares();
	oneRoundEnd();
}

void GameShooterMode::onFreezing(CCObject *pSender)
{
	bool isFreezing = SquareModel::theModel()->isFreezing();
	int count = UserInfo::getInstance()->getPropsCount(kProp_Freezing);
	bool ifCoinEnought = GameController::getInstance()->checkCoinsEnought();
	if (!ifCoinEnought && count <= 0)
	{
		// show pay point
		showLibaoDiaolg();
	}
	if (!isFreezing)
	{
		if (count > 0)
		{
			UserInfo::getInstance()->addPropsCount(kProp_Freezing, -1);
		}
		else
		{
			UserInfo::getInstance()->addCoins(-DOUBLE_ATTACT_COST_COIN);
		}
		updateCoins();
		SquareModel::theModel()->setSquareFreezing(true);
	}
}

void GameShooterMode::onHelpPanel(CCObject *pSender)
{
	if (getChildByTag(kTag_Pause))
	{
		removeChildByTag(kTag_Pause);
		return;
	}
	HelpLayer *helpLayer = HelpLayer::create();
	addChild(helpLayer, KZOrder_PauseLayer, kTag_Pause);
}

void GameShooterMode::showLibaoDiaolg()
{
	bool isBusinessMode = MyPurchase::sharedPurchase()->isBusinessMode();
	int isYijian = GameConfig::getInstance()->m_yijian;
	if (isBusinessMode && isYijian)
	{
		MyPurchase::sharedPurchase()->payForProducts(PAY_TYPE_COIN_LIBAO);
		return;
	}
	int random = rand() % 2;
	int libaoType = PAY_TYPE_TIME_LIBAO;
	if (random == 0)
	{
		libaoType = PAY_TYPE_COIN_LIBAO;
	}
	LibaoDialog *dialog = LibaoDialog::create(libaoType);
	addChild(dialog, KZOrder_LibaoLayer);
}

void GameShooterMode::checkLibaoShow()
{
	int score = SquareModel::theModel()->getCurrentScore() - 1;
	int m_showLibaoLevel = GameConfig::getInstance()->m_showLibaoLevel;
	if (score % m_showLibaoLevel == 0)
	{
		showLibaoDiaolg();
	}
}

void GameShooterMode::initGameLayout()
{
	/*auto ballHints = BallHintModel::theModel()->createBallHints();
	for (auto iter = ballHints.begin(); iter != ballHints.end(); ++iter)
	{
	addChild(*iter, kZOrder_Layout);
	}*/
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
	auto node = getChildByTag(kTag_Streak + i);
	node->setPosition(marbles[i]->getPosition());
	}*/
}

void GameShooterMode::addMarble(float dt)
{
	auto marbles = MarbleModel::theModel()->getMarbles();
	auto actions = ActionSequence::create(this);
	MarbleAttr m_attr = FasterMarle();
	auto ball = MarbleModel::theModel()->createMarble();
	ball->setBody();
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

void GameShooterMode::initSquares()
{
	auto squares = SquareModel::theModel()->loadSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		Index index = node->getIndex();
		node->setPosition(ccp((node->getContentSize().width / 2 + SQUARE_SPACING) + index.x * (node->getContentSize().width + SQUARE_SPACING),
			m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * (7.5 - index.y)));
		addChild(node, kZOrder_Square);
	}
	bool isGameOver = GameController::getInstance()->checkGameOver();
	if (isGameOver)
	{
		showGameOver();
	}
}

void GameShooterMode::addSquares()
{
	auto squares = SquareModel::theModel()->createSquareList(false);
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		Index index = node->getIndex();
		node->setIndex(index.x, -1);
		node->setPosition(ccp(node->getContentSize().width / 2 + SQUARE_SPACING + index.x * (node->getContentSize().width + SQUARE_SPACING),
			m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * 8.5));
		addChild(node, kZOrder_Square);
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

	if (m_protectTime > 0)
	{
		m_protectTime -= dt;
	}
	if (m_protectTime < 0)
	{
		m_protectTime = 0;
		if (m_character->getChildByTag(kTag_Protect))
		{
			m_character->removeChildByTag(kTag_Protect);
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
		auto targetPos = GameController::getInstance()->getTargetPos();
		auto squares = SquareModel::theModel()->getSquares();
		for (auto iter = squares.begin(); iter != squares.end(); ++iter)
		{
			auto square = *iter;
			square->setPosition(ccp(square->getPositionX(), square->getPositionY() - (square->getContentSize().height + SQUARE_SPACING) / BALL_MOVE_SPEED));
			if (square->getPositionY() - square->getContentSize().height / 2 < m_bottomLinePos)
			{
				if (square->getSquareType() == kType_Square || square->getSquareType() == kType_Triangle)
				{
					//showGameOver();
				}
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
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto location = pTouch->getLocation();
	if (location.x - m_character->getContentSize().width / 2 < 0)
	{
		m_character->setPositionX(m_character->getContentSize().width / 2);
	}
	else if (location.x + m_character->getContentSize().width / 2 > winSize.width)
	{
		m_character->setPositionX(winSize.width - m_character->getContentSize().width / 2);
	}
	else
	{
		m_character->setPositionX(location.x);
	}
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	character_body->setPositionX(m_character->getPositionX());
	character_head->setPositionX(m_character->getPositionX());
	return true;
}

void GameShooterMode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto location = pTouch->getLocation();
	if (location.x - m_character->getContentSize().width / 2 < 0)
	{
		m_character->setPositionX(m_character->getContentSize().width / 2);
	}
	else if (location.x + m_character->getContentSize().width / 2 > winSize.width)
	{
		m_character->setPositionX(winSize.width - m_character->getContentSize().width / 2);
	}
	else
	{
		m_character->setPositionX(location.x);
	}
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	character_body->setPositionX(m_character->getPositionX());
	character_head->setPositionX(m_character->getPositionX());
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
	CCLabelAtlas *coinLabel = dynamic_cast<CCLabelAtlas*>(m_mainLayout->getChildById(5));
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

void GameShooterMode::useProtectEffect()
{
	m_protectTime = PROTECT_TIME;
	auto protect = CCSprite::create("game/protected.png");
	protect->setTag(kTag_Protect);
	m_character->addChild(protect);

}