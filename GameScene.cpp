#include "LibaoDialog.h"
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
#include "GuideLayer.h"
#include "FuhuoLibao.h"
#include "PauseLayer.h"
#include "MyPurchase.h"
#include "HelpLayer.h"
#include "GameConfig.h"
#include "ClippingLayer.h"
#include "DarknessLayer.h"
#include "ShopSkinLayer.h"

USING_NS_CC;

void GameScene::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Game, true);
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
	onPauseGame(NULL);
}

GameScene::GameScene()
: m_shootDegree(0)
, m_addMarbleCount(0)
, m_bIsDoubleAttact(false)
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
	setAccelerometerEnabled(true);
	Box2dFactory::getInstance()->initPhysics(false);
	GameUtil::loadGameInfo();

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

	m_topLayout = UiLayout::create("layout/game_top.xml");
	m_topLayout->setPosition(ccp(0, winSize.height - m_topLayout->getContentSize().height));
	m_topLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_topLayout, kZOrder_Layout);
	initTopLayout();

	m_bottomLayout = UiLayout::create("layout/game_bottom.xml");
	m_bottomLayout->setPosition(ccp(0, 0));
	m_bottomLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_bottomLayout, kZOrder_Layout);
	initBottomLayout();
	initGameLayout();

	initPhysicBorder();

	initMarbles();
	int type = MarbleModel::theModel()->getMarbleAttr().skin;
	updateMarbleType(type);

	initSquares();
	updateMarbleCount();

	//checkFishGuide();

	updateCoins();
	updateScore();

	scheduleUpdate();
	return true;
}

void GameScene::initMainLayout()
{
	CCSprite *line_top = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(8));
	line_top->setVisible(false);
	auto worldPos = m_mainLayout->convertToWorldSpace(line_top->getPosition());
	m_topLinePos = worldPos.y;

	CCSprite *line_bottom = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(7));
	line_bottom->setVisible(false);
	line_bottom->setTag(kTag_Wall);
	worldPos = line_bottom->convertToWorldSpace(CCPointZero);
	m_bottomLinePos = worldPos.y;
	Box2dFactory::getInstance()->createSquare(line_bottom, true);
}

void GameScene::onPauseGame(CCObject *pSender)
{
	if (getChildByTag(kTag_Pause))
	{
		removeChildByTag(kTag_Pause);
		return;
	}
	if (getChildByTag(kTag_Libao))
	{
		removeChildByTag(kTag_Libao);
		return;
	}
	if (getChildByTag(kTag_GameOver))
	{
		return;
	}
	PauseLayer *pauseLayer = PauseLayer::create();
	addChild(pauseLayer, KZOrder_PauseLayer, kTag_Pause);
}

void GameScene::initCharacterLayout()
{
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	m_arrow = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(11));
	/*character_body->setVisible(false);
	character_head->setVisible(false);
	m_arrow->setVisible(false);*/

	auto targetPos = GameController::getInstance()->getTargetPos();
	if (targetPos.x != 0 && targetPos.y != 0)
	{
		character_body->setPosition(ccp(targetPos.x, character_body->getPositionY()));

		character_head->setPosition(ccp(targetPos.x, character_head->getPositionY()));
		m_arrow->setVisible(false);
	}
}

void GameScene::initTopLayout()
{
	CCMenuItem *pauseItem = dynamic_cast<CCMenuItem*>(m_topLayout->getChildById(6));
	pauseItem->setTarget(this, menu_selector(GameScene::onPauseGame));

	CCMenuItem *helpBtn = dynamic_cast<CCMenuItem*>(m_topLayout->getChildById(9));
	helpBtn->setTarget(this, menu_selector(GameScene::onHelpPanel));
	auto action = GameUtil::getRepeatScaleAction();
	helpBtn->runAction(action);
}

void GameScene::initBottomLayout()
{
	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(5));
	doubleAttactBtn->setTarget(this, menu_selector(GameScene::onDoubleAttact));

	CCMenuItem *clearScreenBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(4));
	clearScreenBtn->setTarget(this, menu_selector(GameScene::onClearScreen));

	CCMenuItem *freezingBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(3));
	freezingBtn->setTarget(this, menu_selector(GameScene::onFreezing));

	for (int i = kMarble_Faster; i <= kMarble_Bomb; i++)
	{
		CCMenuItem *ballBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(i + 9));
		ballBtn->setTag(i);
		ballBtn->setTarget(this, menu_selector(GameScene::onMarbleChange));
		bool isUnlock = UserInfo::getInstance()->isUnlock(i);
		if (!isUnlock)
		{
			ballBtn->setColor(ccc3(60, 60, 60));
		}
	}
	
}

void GameScene::onDoubleAttact(CCObject *pSender)
{
	int count = UserInfo::getInstance()->getPropsCount(kProp_DoubleAttact);
	int coinCount = UserInfo::getInstance()->getCoins();
	int doubleAttactCost = GameConfig::getInstance()->m_doubleAttactCost;
	if (coinCount >= doubleAttactCost && count <= 0)
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
			UserInfo::getInstance()->addCoins(-doubleAttactCost);
		}
		updateCoins();
		m_bIsDoubleAttact = true;

		GameController::getInstance()->setDoubleAttact();
		updateMarbleCount();
		CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(5));
		auto scaleby = CCScaleBy::create(0.5f, 1.2f);
		auto reverse = scaleby->reverse();
		auto sequence = CCSequence::create(scaleby, reverse, NULL);
		auto action = CCRepeatForever::create(sequence);
		doubleAttactBtn->runAction(action);
	}
	else
	{

	}
}

void GameScene::onClearScreen(CCObject *pSender)
{
	int count = UserInfo::getInstance()->getPropsCount(kProp_Clear);
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	int coinCount = UserInfo::getInstance()->getCoins();
	int hammerCost = GameConfig::getInstance()->m_hammerCost;
	if (coinCount >= hammerCost && count <= 0)
	{
		// show pay point
		showLibaoDiaolg();
		return;
	}
	if (isRoundOver)
	{
		ClippingLayer *clippingLayer = ClippingLayer::create();
		addChild(clippingLayer, KZOrder_LibaoLayer, kTag_Libao);
		/*if (count > 0)
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
		oneRoundEnd();*/
	}
}

void GameScene::onFreezing(CCObject *pSender)
{
	bool isFreezing = SquareModel::theModel()->isFreezing();
	int count = UserInfo::getInstance()->getPropsCount(kProp_Freezing);
	int coinCount = UserInfo::getInstance()->getCoins();
	int freezingCost = GameConfig::getInstance()->m_freezingCost;
	if (coinCount >= freezingCost && count <= 0)
	{
		// show pay point
		showLibaoDiaolg();
		return;
	}
	if (!isFreezing)
	{
		if (count > 0)
		{
			UserInfo::getInstance()->addPropsCount(kProp_Freezing, -1);
		}
		else
		{
			UserInfo::getInstance()->addCoins(-freezingCost);
		}
		updateCoins();
		SquareModel::theModel()->setSquareFreezing(true);
	}
}

void GameScene::onHelpPanel(CCObject *pSender)
{
	if (getChildByTag(kTag_Pause))
	{
		removeChildByTag(kTag_Pause);
		return;
	}
	HelpLayer *helpLayer = HelpLayer::create();
	addChild(helpLayer, KZOrder_PauseLayer, kTag_Pause);
}

void GameScene::showLibaoDiaolg()
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
	addChild(dialog, KZOrder_LibaoLayer, kTag_Libao);
}

void GameScene::onMarbleChange(cocos2d::CCObject *pSender)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	bool isGameOver = GameController::getInstance()->isGameOver();

	if (!isRoundOver || isGameOver)
	{
		return;
	}
	int tag = ((CCMenuItem*)pSender)->getTag();

	bool isUnlock = UserInfo::getInstance()->isUnlock(tag);
	if (isUnlock)
	{
		updateMarbleType(tag);
	}
	else
	{
		ShopSkinLayer *skinLayer = ShopSkinLayer::create(tag);
		addChild(skinLayer, kZOrder_Shop, kTag_Shop);
	}
}

void GameScene::checkLibaoShow()
{
	int score = SquareModel::theModel()->getCurrentScore() - 1;
	int m_showLibaoLevel = GameConfig::getInstance()->m_showLibaoLevel;
	if (score % m_showLibaoLevel == 0)
	{
		showLibaoDiaolg();
	}
}

void GameScene::initGameLayout()
{
	auto ballHints = BallHintModel::theModel()->createBallHints();
	for (auto iter = ballHints.begin(); iter != ballHints.end(); ++iter)
	{
		addChild(*iter, kZOrder_Layout);
	}

	m_touchCircle = CCSprite::create("game/start_touch_bg.png");
	addChild(m_touchCircle, kZOrder_Layout);
	m_touchCircle->setVisible(false);

	m_touchPoint = CCSprite::create("game/start_touch_point.png");
	addChild(m_touchPoint, kZOrder_Layout);
	m_touchPoint->setVisible(false);

	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	char temp[50] = { 0 };
	int attactRate = MarbleModel::theModel()->getAttactRate();
	sprintf(temp, "%d:%d", attactRate, START_BALL_SIZE);
	m_marbleCount = GameUtil::getImageNum(FONT_WHITE, temp);
	m_marbleCount->setPosition(ccp(character_head->getContentSize().width / 2, character_head->getContentSize().height + m_marbleCount->getContentSize().height));
	character_head->addChild(m_marbleCount);

	/*DarknessLayer *layer = DarknessLayer::create();
	addChild(layer, kZOrder_Darkness);*/
}

void GameScene::initPhysicBorder()
{
	m_world = Box2dFactory::getInstance()->getWorld();
	Box2dFactory::getInstance()->initBorders(m_topLinePos, m_bottomLinePos);
}

void GameScene::initMarbles()
{
	m_addMarbleCount = MarbleModel::theModel()->getMarblesCount();
	if (m_addMarbleCount < START_BALL_SIZE)
	{
		m_addMarbleCount = START_BALL_SIZE;
	}
	auto targetPos = GameController::getInstance()->getTargetPos();
	for (int i = 0; i < m_addMarbleCount; i++)
	{
		auto marble = MarbleModel::theModel()->createMarble();
		marble->setBody();
		if (targetPos.x == 0 && targetPos.y == 0)
		{
			marble->setPosition(ccp(m_arrow->getPositionX(), m_bottomLinePos + marble->getContentSize().height / 2 + 10));
		}
		else
		{
			marble->setPosition(targetPos);
		}
		addChild(marble, kZOrder_Marble);
		if (i == 0)
		{
			marble->setVisible(true);
		}
		else
		{
			marble->setVisible(false);
		}
		GameController::getInstance()->setTargetPos(marble->getPosition());
	}
}

void GameScene::initSquares()
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
	bool isGameOver = GameController::getInstance()->isGameOver();
	if (isGameOver)
	{
		showGameOver();
	}
}

void GameScene::addSquares()
{
	auto squares = SquareModel::theModel()->createSquareList();
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

void GameScene::update(float dt)
{
	bool isRoundOver = GameController::getInstance()->isRoundOver();
	if (isRoundOver)
	{
		//check character's position
		/*auto targetPos = GameController::getInstance()->getTargetPos();
		CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
		character_body->setPosition(ccp(targetPos.x, character_body->getPositionY()));

		CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
		character_head->setPosition(ccp(targetPos.x, character_head->getPositionY()));*/
		return;
	}

	//updateMarbleCount();

	int32 velocityIterations = 10;
	int32 positionIterations = 10;

	m_world->Step(dt, velocityIterations, positionIterations);
	m_world->ClearForces();

	auto marbles = MarbleModel::theModel()->getMarbles();
	int marbleCount = MarbleModel::theModel()->getMarblesCount();
	for (auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto marble = (*iter);
		if (m_arrow->boundingBox().containsPoint(marble->getPosition()))
		{
			marble->setVisible(false);
		}
		else
		{
			marble->setVisible(true);
		}
		auto body = marble->getBody();
		MarbleNode *ball = (MarbleNode*)(body->GetUserData());
		if (ball->isMoving())
		{
			b2Vec2 ballPosition = body->GetPosition();
			ball->setPosition(ccp(ballPosition.x * PTM_RATIO, ballPosition.y * PTM_RATIO));
			if (ballPosition.y * PTM_RATIO < 0)
			{
				marble->setMovingState(false);
				GameController::getInstance()->addCounter();
			}
		}
		else
		{
			if (!marble->isTrueStop())
			{
				//m_addMarbleCount++;
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
	m_touchPos = pTouch->getLocation();
	onTouchCallback();
	return true;
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
#if(NEW_SHOOT_MODE == 1)
	
#else
	if (!m_touchPoint->isVisible())
	{
		return;
	}
#endif
	m_touchPos = pTouch->getLocation();
	onTouchCallback();
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_arrow->isVisible())
	{
		m_touchPoint->setVisible(false);
		m_touchCircle->setVisible(false);
		return;
	}

	GameController::getInstance()->startOneRound();
	m_touchPoint->setVisible(false);
	m_touchCircle->setVisible(false);
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	character_head->setRotation(0);
	BallHintModel::theModel()->setHintVisible(false);

	GameController::getInstance()->setRoundState(false);
	auto actions = ActionSequence::create(this);
	auto marbles = MarbleModel::theModel()->getMarbles();
	m_addMarbleCount = marbles.size();

	auto shootPos = BallHintModel::theModel()->getShootPosition();
	for (auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto &ball = (*iter);
		//ball->setPosition(shootPos);
		ball->setMovingState(true);
		auto action1 = CCFunctionAction::create([=]()
		{
			m_addMarbleCount--;
			if (m_addMarbleCount == 0)
			{
				auto fadeOut = CCFadeOut::create(0.6f);
				m_marbleCount->runAction(fadeOut);
			}
			ball->shoot(m_shootDegree);
			ball->setVisible(true);
		});
		auto action2 = CCDelayTime::create(0.1f);
		actions->addAction(action1);
		actions->addAction(action2);
	}
	auto action3 = CCFunctionAction::create([=]()
	{
		m_arrow->setVisible(false);
	});
	actions->addAction(action3);
	actions->runActions();

	//check guideLayer
	if (getChildByTag(kTag_GuideLayer))
	{
		removeChildByTag(kTag_GuideLayer);
		UserInfo::getInstance()->setFishGuide(true);
	}
}

void GameScene::onTouchCallback()
{
	m_touchCircle->setPosition(m_touchPos);
	m_touchPoint->setPosition(m_touchPos);
#if(NEW_SHOOT_MODE == 1)
	auto prePos = m_touchCircle->getPosition();
	auto targetPos = GameController::getInstance()->getTargetPos();

	m_shootDegree = GameUtil::getDegreeTwoPoints(targetPos, m_touchPos /*prePos*/);
	float radian = GameUtil::getRadian(m_shootDegree);
	float newX = prePos.x - m_touchCircle->getContentSize().width * 0.5f * cos(radian);
	float newY = prePos.y - m_touchCircle->getContentSize().width * 0.5f * sin(radian);
	m_touchPoint->setPosition(ccp(newX, newY));

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

	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	if (m_shootDegree < 90)
	{
		character_head->setPositionX(character_body->getPositionX() - 20);
		character_body->setFlipX(true);
		character_head->setFlipX(true);

		m_arrow->setAnchorPoint(ccp(0.73f, 0.35f));
		m_arrow->setPosition(ccp(character_body->getPositionX() + 10, character_body->getPositionY() + 6));
		m_arrow->setFlipY(true);
	}
	else
	{
		character_head->setPositionX(character_body->getPositionX());
		character_body->setFlipX(false);
		character_head->setFlipX(false);

		m_arrow->setAnchorPoint(ccp(0.73f, 0.65f));
		m_arrow->setPosition(ccp(character_body->getPositionX() - 12, character_body->getPositionY() + 6));
		m_arrow->setFlipY(false);
	}

#if(NEW_SHOOT_MODE == 1)
	BallHintModel::theModel()->updatePosition(targetPos, m_touchPos, targetPos, m_arrow->getContentSize().width);
#else
	BallHintModel::theModel()->updatePosition(m_touchPos, prePos, targetPos, m_arrow->getContentSize().width);
#endif

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
#else
	m_touchCircle->setVisible(true);
	m_touchPoint->setVisible(true);
#endif
}

void GameScene::didAccelerate(CCAcceleration* pAccelerationValue)
{
	int gravityLevel = GameConfig::getInstance()->m_gravityLevel;
	int curLevel = SquareModel::theModel()->getCurrentScore();
	if (curLevel < gravityLevel)
	{
		return;
	}

	bool isRoundOver = GameController::getInstance()->isRoundOver();
	bool isGameOver = GameController::getInstance()->isGameOver();
	
	if (!isRoundOver || isGameOver)
	{
		return;
	}
	characterMove(pAccelerationValue->x);
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
		SquareModel::theModel()->exchangeSquarePosition();
		addSquares();
		SquareModel::theModel()->squareMoveDown();
	}

	//reset doubleAttact buttom
	GameController::getInstance()->resetAttactRate();
	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(5));
	doubleAttactBtn->setScale(0.7f);
	doubleAttactBtn->stopAllActions();
	m_bIsDoubleAttact = false;

	//check marbles
	int addCount = MarbleModel::theModel()->checkMarblesCount();
	auto marbles = MarbleModel::theModel()->getMarbles();
	auto targetPos = GameController::getInstance()->getTargetPos();
	for (int i = 0; i < addCount; i++)
	{
		auto marble = MarbleModel::theModel()->createMarble();
		marble->setBody();
		marble->setPosition(targetPos);
		addChild(marble, kZOrder_Marble);
	}
	GameController::getInstance()->updateMarblePos();
	m_addMarbleCount = marbles.size() + addCount;
	updateMarbleCount();
	auto fadeIn = CCFadeIn::create(0.6f);
	m_marbleCount->runAction(fadeIn);

	//check tools when one round end and delete "0 score" tool
	GameController::getInstance()->checkSquares(true);

	//check character's position
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	auto moveTo = CCMoveTo::create(0.5f, ccp(targetPos.x, character_body->getPositionY()));
	character_body->runAction(moveTo);

	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	auto moveTo2 = CCMoveBy::create(0.5f, ccp(targetPos.x - character_body->getPositionX(), 0));
	character_head->runAction(moveTo2);

	updateScore();

	checkLibaoShow();
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
	CCLabelAtlas *coinLabel = dynamic_cast<CCLabelAtlas*>(m_topLayout->getChildById(5));
	coinLabel->setString(countStr.c_str());
	auto action = GameUtil::getOnceScaleAction();
	coinLabel->runAction(action);
	updatePropsCount();
}

void GameScene::updateScore()
{
	int score = SquareModel::theModel()->getCurrentScore() - 1;
	std::string countStr = GameUtil::intToString(score);
	CCLabelAtlas *scoreLabel = dynamic_cast<CCLabelAtlas*>(m_topLayout->getChildById(3));
	scoreLabel->setString(countStr.c_str());
	auto action1 = GameUtil::getOnceScaleAction();
	scoreLabel->runAction(action1);

	bool showEffect = false;
	int bestScore = UserInfo::getInstance()->getBestScore();
	if (score > bestScore)
	{
		bestScore = score;
		UserInfo::getInstance()->setBestScore(score);
		showEffect = true;
	}
	std::string bestStr = GameUtil::intToString(bestScore);
	CCLabelAtlas *bestScoreLabel = dynamic_cast<CCLabelAtlas*>(m_topLayout->getChildById(13));
	bestScoreLabel->setString(bestStr.c_str());
	if (showEffect)
	{
		auto action2 = GameUtil::getOnceScaleAction();
		bestScoreLabel->runAction(action2);
	}
}

void GameScene::updatePropsCount()
{
	for (int i = 0; i < 3; i++)
	{
		int propCount = UserInfo::getInstance()->getPropsCount(i);
		std::string countStr = GameUtil::intToString(propCount);
		CCLabelAtlas *propLabel = dynamic_cast<CCLabelAtlas*>(m_bottomLayout->getChildById(6 + i));
		propLabel->setString(countStr.c_str());
		auto action = GameUtil::getOnceScaleAction();
		propLabel->runAction(action);
	}
}

void GameScene::updateMarbleCount()
{
	char temp[50] = { 0 };
	if (m_addMarbleCount > MarbleModel::theModel()->getMarblesCount())
	{
		//m_addMarbleCount = MarbleModel::theModel()->getMarblesCount();
	}
	int attactRate = GameController::getInstance()->getAttactRate();
	sprintf(temp, "%d:%d", attactRate, m_addMarbleCount);
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
	FuhuoLibao *fuhuoLibao = FuhuoLibao::create();
	addChild(fuhuoLibao, KZOrder_GameOver, kTag_GameOver);
	//GameController::getInstance()->setRoundState(false);
}

void GameScene::showAddCoinEffect(SquareNode *node)
{
	if (node)
	{
		auto coinEffect = CCSprite::create(node->getImage().c_str());
		coinEffect->setPosition(node->getPosition());
		addChild(coinEffect, kZOrder_Effect);
		CCLabelAtlas *coinLabel = dynamic_cast<CCLabelAtlas*>(m_topLayout->getChildById(5));
		auto pos = m_topLayout->convertToWorldSpace(coinLabel->getPosition());
		auto jumpTo = CCJumpTo::create(0.6f, pos, -200, 1);
		auto callback = CCCallFunc::create(coinEffect, callfunc_selector(CCNode::removeFromParent));
		auto sequence = CCSequence::create(jumpTo, callback, NULL);
		coinEffect->runAction(sequence);
	}
}

void GameScene::showAddMarbleEffect(SquareNode *node)
{
	if (node)
	{
		auto coinEffect = CCSprite::create(node->getImage().c_str());
		coinEffect->setPosition(node->getPosition());
		addChild(coinEffect, kZOrder_Effect);
		auto pos = m_characterLayout->convertToWorldSpace(m_arrow->getPosition());
		auto jumpTo = CCJumpTo::create(0.6f, pos, 300, 1);
		auto callback = CCCallFunc::create(coinEffect, callfunc_selector(CCNode::removeFromParent));
		auto sequence = CCSequence::create(jumpTo, callback, NULL);
		coinEffect->runAction(sequence);
	}
}

void GameScene::updateMarbleType(int type)
{
	auto attr = MarbleModel::theModel()->getMarbleAttrByType(type);
	MarbleModel::theModel()->setMarbleAttr(attr);
	MarbleModel::theModel()->clearMarbles();
	initMarbles();
	for (int i = kMarble_Faster; i <= kMarble_Bomb; i++)
	{
		CCMenuItem *ballBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(i + 9));
		ballBtn->stopAllActions();
		ballBtn->setScale(1.0f);
		if (type == i)
		{
			ballBtn->setColor(ccc3(255, 255, 255));
			auto action = GameUtil::getRepeatScaleAction();
			ballBtn->runAction(action);
		}
	}
	updateMarbleCount();
}

void GameScene::checkFishGuide()
{
	bool isGuideFinish = UserInfo::getInstance()->isFishGuideFinish();
	if (!isGuideFinish)
	{
		GuideLayer *guideLayer = GuideLayer::create();
		addChild(guideLayer, KZOrder_GuideLayer, kTag_GuideLayer);
	}
}

void GameScene::characterMove(float offsetX)
{
	if (fabs(offsetX) < 0.2f)
	{
		return;
	}
	offsetX = offsetX * 6;
	if (offsetX < 0)
	{
		offsetX = -offsetX * offsetX;
	}
	else
	{
		offsetX = offsetX * offsetX;
	}
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	if (offsetX < 0 && m_arrow->getPositionX() - m_arrow->getContentSize().width / 2 < 0)
	{
		return;
	}
	else if (offsetX > 0 && m_arrow->getPositionX() + m_arrow->getContentSize().width / 2 > winSize.width)
	{
		return;
	}

	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	m_arrow = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(11));
	character_body->setPositionX(character_body->getPositionX() + offsetX);
	character_head->setPositionX(character_head->getPositionX() + offsetX);
	m_arrow->setPositionX(m_arrow->getPositionX() + offsetX);

	auto marbles = MarbleModel::theModel()->getMarbles();
	for(auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto marble = *iter;
		marble->setPositionX(marble->getPositionX() + offsetX);
	}
	auto targetPos = GameController::getInstance()->getTargetPos();
	targetPos.x = targetPos.x + offsetX;
	GameController::getInstance()->setTargetPos(targetPos);
	onTouchCallback();
}

