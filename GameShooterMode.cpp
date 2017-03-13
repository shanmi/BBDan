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
#include "ShopSkinLayer.h"
#include "CharacterView.h"
#include "DataHelper.h"
#include "FuhuoLibao.h"

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
, m_shotgunsTime(0)
, m_addMarbleTime(0)
, m_bIsDoubleAttact(false)
, m_bIsGameOver(false)
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
	GameController::getInstance()->setGameType(kGame_Shoot);

	setKeypadEnabled(true);
	Box2dFactory::getInstance()->initPhysics(true);
	DataHelper::getInstance()->loadShootGameInfo();

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_mainLayout = UiLayout::create("layout/game_scene2.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	m_mainLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_mainLayout, kZOrder_Main);
	initMainLayout();

	m_characterView = CharacterView::create();
	addChild(m_characterView, kZOrder_Character);
	m_arrow = dynamic_cast<CCSprite *>(m_characterView->getBodyById(11));
	m_characterView->initShooterPos();

	m_topLayout = UiLayout::create("layout/game_top2.xml");
	m_topLayout->setPosition(ccp(0, winSize.height - m_topLayout->getContentSize().height));
	m_topLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_topLayout, kZOrder_Layout);
	initTopLayout();

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
	line_bottom->setTag(kTag_Wall);
	worldPos = line_bottom->convertToWorldSpace(CCPointZero);
	m_bottomLinePos = worldPos.y;
	Box2dFactory::getInstance()->createSquare(line_bottom, true);

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
	DataHelper::getInstance()->saveShootGameInfo();
	PauseLayer *pauseLayer = PauseLayer::create();
	addChild(pauseLayer, KZOrder_PauseLayer, kTag_Pause);
}

void GameShooterMode::initTopLayout()
{
	CCMenuItem *pauseItem = dynamic_cast<CCMenuItem*>(m_topLayout->getChildById(6));
	pauseItem->setTarget(this, menu_selector(GameShooterMode::onPauseGame));

	CCMenuItem *helpBtn = dynamic_cast<CCMenuItem*>(m_topLayout->getChildById(9));
	helpBtn->setTarget(this, menu_selector(GameShooterMode::onHelpPanel));

	CCSprite *progress_bg = dynamic_cast<CCSprite*>(m_topLayout->getChildById(14));
	CCSprite *logo = dynamic_cast<CCSprite*>(m_topLayout->getChildById(15));
	CCSprite *target = dynamic_cast<CCSprite*>(m_topLayout->getChildById(16));
	CCSprite *arrow = dynamic_cast<CCSprite*>(m_topLayout->getChildById(18));
	CCLabelAtlas *targetLabel = dynamic_cast<CCLabelAtlas*>(m_topLayout->getChildById(17));

	UiLayout *layout = UiLayout::create("layout/game_top.xml");
	CCSprite *start = dynamic_cast<CCSprite*>(layout->getChildById(15));
	float startPos = start->getPositionX();

	arrow->setZOrder(arrow->getZOrder() + 1);
	auto moveBy = CCMoveBy::create(1.1f, ccp(0, 10));
	auto sequence = CCSequence::create(moveBy, moveBy->reverse(), NULL);
	auto repeat = CCRepeatForever::create(sequence);
	arrow->runAction(repeat);

	int curLevel = SquareModel::theModel()->getCurrentScore();
	int targetLevel = UserInfo::getInstance()->getTargetLevel();
	int lastLevel = 0;// targetLevel <= 10 ? 0 : targetLevel / 2;
	float rate = (target->getPositionX() - startPos) / (targetLevel - lastLevel);
	float gotoPos = startPos + rate * (curLevel - lastLevel);
	logo->setPositionX(gotoPos);
	arrow->setPositionX(gotoPos);

	float progressPos = 100 * gotoPos / layout->getContentSize().width;
	m_progressTimer = CCProgressTimer::create(CCSprite::create("game/youxijiemian_jindutiao_jindu.png"));
	m_progressTimer->setType(kCCProgressTimerTypeBar);
	m_progressTimer->setMidpoint(ccp(0, 1));
	m_progressTimer->setBarChangeRate(ccp(1, 0));
	m_progressTimer->setPercentage(progressPos);
	m_progressTimer->setPosition(ccp(progress_bg->getContentSize().width / 2, progress_bg->getContentSize().height / 2));
	progress_bg->addChild(m_progressTimer);

	auto topPanel = CCSprite::create("game/youxijiemian_jindutiao_biankuang.png");
	topPanel->setPosition(ccp(progress_bg->getContentSize().width / 2, progress_bg->getContentSize().height / 2));
	progress_bg->addChild(topPanel);

}

void GameShooterMode::initBottomLayout()
{
	CCMenuItem *doubleAttactBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(5));
	doubleAttactBtn->setTarget(this, menu_selector(GameShooterMode::onDoubleAttact));

	CCMenuItem *clearScreenBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(4));
	clearScreenBtn->setTarget(this, menu_selector(GameShooterMode::onClearScreen));

	CCMenuItem *freezingBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(3));
	freezingBtn->setTarget(this, menu_selector(GameShooterMode::onFreezing));

	for (int i = kMarble_Faster; i <= kMarble_Bomb; i++)
	{
		CCMenuItem *ballBtn = dynamic_cast<CCMenuItem*>(m_bottomLayout->getChildById(i + 9));
		ballBtn->setTag(i);
		ballBtn->setTarget(this, menu_selector(GameShooterMode::onMarbleChange));
		bool isUnlock = UserInfo::getInstance()->isUnlock(i);
		if (!isUnlock)
		{
			ballBtn->setColor(ccc3(60, 60, 60));
		}
		else
		{
			ballBtn->setColor(ccc3(255, 255, 255));
		}
	}

}

void GameShooterMode::onDoubleAttact(CCObject *pSender)
{
	int count = UserInfo::getInstance()->getPropsCount(kProp_DoubleAttact);
	int coinCount = UserInfo::getInstance()->getCoins();
	int doubleAttactCost = GameConfig::getInstance()->m_doubleAttactCost;
	if (coinCount < doubleAttactCost  && count <= 0)
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
	int coinCount = UserInfo::getInstance()->getCoins();
	int hammerCost = GameConfig::getInstance()->m_hammerCost;
	if (coinCount < hammerCost && count <= 0)
	{
		// show pay point
		showLibaoDiaolg();
		return;
	}
	if (count > 0)
	{
		UserInfo::getInstance()->addPropsCount(kProp_Clear, -1);
	}
	else
	{
		UserInfo::getInstance()->addCoins(-hammerCost);
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
	int coinCount = UserInfo::getInstance()->getCoins();
	int freezingCost = GameConfig::getInstance()->m_freezingCost;
	if (coinCount < freezingCost  && count <= 0)
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
	int score = SquareModel::theModel()->getCurrentScore();
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
	//addMarble(0);
}

void GameShooterMode::addMarble(float dt)
{
	auto attr = MarbleModel::theModel()->getMarbleAttr();
	m_addMarbleTime += dt;
	if (m_addMarbleTime < BALL_SPEED / attr.speed)
	{
		return;
	}
	m_addMarbleTime = 0;
	int shootCount = 1;
	if (m_shotgunsTime > 0)
	{
		shootCount = 3;
	}
	if (attr.skin == kMarble_Dispersed)
	{
		shootCount += 2;
	}
	for (int i = 0; i < shootCount; i++)
	{
		auto ball = MarbleModel::theModel()->createMarble();
		ball->setBody();
		ball->setPosition(ccp(m_arrow->getPositionX(), m_bottomLinePos + ball->getContentSize().height / 2 + 4));
		addChild(ball);
		ball->setMovingState(true);

		if (shootCount == 1)
		{
			ball->shooterShoot(90);
		}
		else
		{
			ball->shooterShoot(75 + 15 * i);
		}
		ball->setVisible(true);
	}
	auto actions = ActionSequence::create(this);
	auto action1 = CCDelayTime::create(0.1f / attr.speed);
	//actions->addAction(action1);
	auto callback = CCFunctionAction::create([=]()
	{
		addMarble(0);
	});
	//actions->addAction(callback);
	actions->runActions();
}

void GameShooterMode::initSquares()
{
	auto squares = SquareModel::theModel()->loadSquareList();
	for (int i = 0; i < squares.size(); i++)
	{
		auto node = squares[i];
		if (node->getPositionY() <= 0)
		{
			Index index = node->getIndex();
			node->setPosition(ccp((node->getContentSize().width / 2 + SQUARE_SPACING) + index.x * (node->getContentSize().width + SQUARE_SPACING),
				m_bottomLinePos + (node->getContentSize().height + SQUARE_SPACING) * (7.5 - index.y)));
		}
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
	auto allsquares = SquareModel::theModel()->getSquares();
	for (auto iter = allsquares.begin(); iter != allsquares.end(); ++iter)
	{
		auto square = *iter;
		Index index = square->getIndex();
		square->setIndex(index.x, index.y + 1);
	}

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
	DataHelper::getInstance()->saveShootGameInfo();
	updateProgress();
}

void GameShooterMode::update(float dt)
{
	if (m_bIsGameOver)
	{
		return;
	}
	int32 velocityIterations = 8;
	int32 positionIterations = 1;

	m_world->Step(dt, velocityIterations, positionIterations);
	m_world->ClearForces();

	addMarble(dt);

	auto marbles = MarbleModel::theModel()->getMarbles();
	for (auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto &marble = (*iter);
		auto body = marble->getBody();
		MarbleNode *ball = (MarbleNode*)(body->GetUserData());
		float distance = body->GetLinearVelocity().x*body->GetLinearVelocity().x + body->GetLinearVelocity().y*body->GetLinearVelocity().y;
		if (distance > 0 && distance < 400)
		{
			float angle = GameUtil::getDegreeTwoPoints(ccp(0, 0), ccp(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
			marble->shooterShoot(angle);
		}
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

	if (m_shotgunsTime > 0)
	{
		m_shotgunsTime -= dt;
	}

	if (m_protectTime > 0)
	{
		m_protectTime -= dt;
	}
	if (m_protectTime < 0)
	{
		m_protectTime = 0;
		if (m_arrow->getChildByTag(kTag_Protect))
		{
			m_arrow->removeChildByTag(kTag_Protect);
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
			if (square->getPositionY() - square->getContentSize().height / 2 < m_bottomLinePos)
			{
				if (square->canRemoveByProps())
				{
					defenseCrash(square);
				}
				else
				{
					square->setScore(0);
				}
			}
		}
		m_moveCounter++;
		if (m_moveCounter > BALL_MOVE_SPEED)
		{
			m_moveCounter = 0;

			int curLevel = SquareModel::theModel()->getCurrentScore();
			int targetLevel = UserInfo::getInstance()->getTargetLevel();
			if (curLevel >= targetLevel)
			{
				auto remainSquares = SquareModel::theModel()->getRemainSqaure();
				if (remainSquares <= 0)
				{
					UserInfo::getInstance()->setTargetLevel();
					SquareModel::theModel()->setCurrentScore(0);
				}
				return;
			}
			addSquares();
		}
	}

	//check squares by not check tool
	GameController::getInstance()->checkSquares(true);
}

bool GameShooterMode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_bIsGameOver)
	{
		return false;
	}
	auto location = pTouch->getLocation();
	m_characterView->checkShooterPos(location);
	return true;
}

void GameShooterMode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_bIsGameOver)
	{
		return;
	}
	auto location = pTouch->getLocation();
	m_characterView->checkShooterPos(location);
	GameController::getInstance()->setShooterPos(location);
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
	CCLabelAtlas *coinLabel = dynamic_cast<CCLabelAtlas*>(m_topLayout->getChildById(5));
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
	if (m_arrow->getChildByTag(kTag_Protect))
	{
		m_arrow->removeChildByTag(kTag_Protect);
	}
	auto protect = CCSprite::create("game/protected.png");
	protect->setTag(kTag_Protect);
	m_arrow->addChild(protect);
}

void GameShooterMode::useShotGunsEffect()
{
	m_shotgunsTime = SHOTGUNS_TIME;
}

void GameShooterMode::getBloodEffect()
{
	CCLog("add blood effect");
}

void GameShooterMode::onMarbleChange(cocos2d::CCObject *pSender)
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

void GameShooterMode::notifyViews()
{
	updateCoins();
	DataHelper::getInstance()->saveShootGameInfo();
	m_bIsGameOver = false;
}

void GameShooterMode::defenseCrash(SquareNode *node)
{
	Index index = node->getIndex();
	CCSprite *defense = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(10+index.x));
	if (defense->isVisible())
	{
		SquareModel::theModel()->removeSameColSquare(node);
		defense->setVisible(false);
	}
	else
	{
		if (!m_bIsGameOver)
		{
			FuhuoLibao *fuhuoLibao = FuhuoLibao::create();
			addChild(fuhuoLibao, KZOrder_GameOver, kTag_GameOver);
			m_bIsGameOver = true;
		}
	}
}

void GameShooterMode::updateProgress()
{
	UiLayout *layout = UiLayout::create("layout/game_top2.xml");
	CCSprite *start = dynamic_cast<CCSprite*>(layout->getChildById(15));
	float startPos = start->getPositionX();

	CCSprite *progress_bg = dynamic_cast<CCSprite*>(m_topLayout->getChildById(14));
	CCSprite *logo = dynamic_cast<CCSprite*>(m_topLayout->getChildById(15));
	CCSprite *target = dynamic_cast<CCSprite*>(m_topLayout->getChildById(16));
	CCLabelAtlas *targetLabel = dynamic_cast<CCLabelAtlas*>(m_topLayout->getChildById(17));
	CCSprite *arrow = dynamic_cast<CCSprite*>(m_topLayout->getChildById(18));

	int curLevel = SquareModel::theModel()->getCurrentScore();
	int targetLevel = UserInfo::getInstance()->getTargetLevel();
	int lastLevel = 0;// targetLevel <= 10 ? 0 : targetLevel / 2;
	float gotoPos;
	if (targetLevel == lastLevel)
	{
		gotoPos = target->getPositionX();
		/*LuckyLayer *luckyLayer = LuckyLayer::create();
		addChild(luckyLayer, KZOrder_LuckyLayer);*/
	}
	else
	{
		float rate = (target->getPositionX() - startPos) / (targetLevel - lastLevel);
		gotoPos = startPos + rate * (curLevel - lastLevel);
	}

	auto moveTo = CCMoveTo::create(1, ccp(gotoPos, logo->getPositionY()));
	logo->runAction(moveTo);
	moveTo = CCMoveTo::create(1, ccp(gotoPos, arrow->getPositionY()));
	arrow->runAction(moveTo);

	float percentage = 100 * gotoPos / layout->getContentSize().width;
	float curPercentage = m_progressTimer->getPercentage();
	m_progressTimer->runAction(CCProgressFromTo::create(1, curPercentage, percentage));

	targetLabel->setString(GameUtil::intToString(targetLevel).c_str());
}