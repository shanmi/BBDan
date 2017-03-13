#include "SquareNode.h"
#include "GameUtil.h"
#include "Box2dFactory.h"
#include "GameController.h"
#include "ActionSequence.h"
#include "CCFunctionAction.h"
#include "Config.h"
#include "MarbleModel.h"
#include "SquareModel.h"
#include "SoundMgr.h"
#include "GameConfig.h"
#include "DataHelper.h"

USING_NS_CC;

SquareNode::SquareNode()
:m_scoreLabel(NULL)
, m_body(NULL)
, m_squareType(kType_Square)
{

}

SquareNode *SquareNode::create()
{
	SquareNode *node = new SquareNode();
	node->autorelease();
	node->init();
	return node;
}

bool SquareNode::init()
{
	m_score = SquareModel::theModel()->getCurrentScore();
	if (m_score % 10 == 0)
	{
		int random = rand() % 100;
		if (random < 50)
		{
			m_score = m_score * 2;
		}
	}
	m_image = GameUtil::getBlockImage(kBlock_Square, m_score);
	m_imageSprite = CCSprite::create(m_image.c_str());
	addChild(m_imageSprite);
	auto fanIn = CCFadeIn::create(0.6f);
	m_imageSprite->runAction(fanIn);

	auto size = m_imageSprite->getContentSize();
	setContentSize(ccp(64, 64));

	m_scoreLabel = GameUtil::getImageNum(FONT_WHITE, m_score);
	addChild(m_scoreLabel);
	setPerfectScale();

	return true;
}

void SquareNode::setPosition(const cocos2d::CCPoint &position)
{
	CCNode::setPosition(position);
	if (m_body)
	{
		b2Vec2 post;
		post = b2Vec2((float)(getPosition().x / PTM_RATIO), (float)((getPosition().y) / PTM_RATIO));
		float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
		m_body->SetTransform(post, angle);
	}
}

void SquareNode::addScore(int score)
{
	m_score += score;
	std::string scoreStr = GameUtil::intToString(m_score);
	if (m_scoreLabel)
	{
		m_scoreLabel->setString(scoreStr.c_str());
		m_image = GameUtil::getBlockImage(kBlock_Square, m_score);
		m_imageSprite->initWithFile(m_image.c_str());
		setPerfectScale();
	}
}

void SquareNode::setPerfectScale()
{
	if (m_scoreLabel)
	{
		float scale = 1.0f;
		int tempScore = m_score / 100;
		while (tempScore > 0)
		{
			scale -= 0.2f;
			tempScore = tempScore / 10;
		}
		m_scoreLabel->setScale(scale);
	}
}

bool SquareNode::isSameColor(SquareNode *node)
{
	if (!node || node->getSquareType() != kType_Square)
	{
		return false;
	}
	auto image = node->getImage();
	if (m_image == image)
	{
		return true;
	}
	return false;
}

void SquareNode::moveDown(bool isLastOne /* = false */)
{
	auto actions = ActionSequence::create(this);
	auto delay = CCFadeIn::create(0.6f);
	auto move = CCMoveBy::create(1.0f, ccp(0, -getContentSize().height - SQUARE_SPACING));
	auto callback1 = CCFunctionAction::create([=]()
	{
		GameController::getInstance()->setRoundState(true);
		GameController::getInstance()->checkGameOver();
		DataHelper::getInstance()->saveGameInfo();
	});
	actions->addAction(delay);
	actions->addAction(move);
	if (isLastOne)
	{
		actions->addAction(callback1);
	}
	if (m_squareType == kType_Square || m_squareType == kType_Triangle)
	{
		auto callback2 = CCFunctionAction::create([=]()
		{
			int curScore = m_score;
			Index newIndex = Index(m_index.x + 1, m_index.y);
			auto neighbour = SquareModel::theModel()->getSquareByIndex(newIndex);
			int curLevel = SquareModel::theModel()->getCurrentScore();
			int addFireLevel = GameConfig::getInstance()->m_addFireLevel;
			int splitSquareLevel = GameConfig::getInstance()->m_splitSquareLevel;
			if (!neighbour && newIndex.x < BALL_COL_SIZE && curScore / 2 > 0 && (curLevel >= splitSquareLevel || curLevel >= addFireLevel))
			{
				SquareNode *square = NULL;
				if (m_squareType == kType_Square && curLevel >= splitSquareLevel)
				{
					square = SquareModel::theModel()->createSquareNode(kType_Square);
				}
				else if (m_squareType == kType_Triangle && curLevel >= addFireLevel)
				{
					square = SquareModel::theModel()->createSquareNode(kType_BossEatMarble);
				}
				if (square)
				{
					addScore(-(curScore - m_score / 2));
					auto shake = CCShaky3D::create(0.4f, ccp(2, 2), 2, 10);
					runAction(shake);

					square->setBody();
					square->setIndex(newIndex);
					square->setScore(curScore - m_score);
					square->setPosition(getPosition());
					getParent()->addChild(square);

					square->setScale(0);
					auto scaleTo = CCScaleTo::create(0.5f, 1.0f);
					auto moveTo = CCMoveTo::create(0.5f, ccp(getPositionX() + getContentSize().width + SQUARE_SPACING, getPositionY()));
					auto action = CCSpawn::create(scaleTo, moveTo, NULL);
					square->runAction(action);
				}
			}
		});
		actions->addAction(callback2);
		/*auto callback3 = CCFunctionAction::create([=]()
		{
		int curScore = m_score;
		Index newIndex = Index(m_index.x, m_index.y - 1);
		auto neighbour = SquareModel::theModel()->getSquareByIndex(newIndex);
		int curLevel = SquareModel::theModel()->getCurrentScore();
		int addIronLevel = GameConfig::getInstance()->m_addIronLevel;
		bool isSame = isSameColor(neighbour);
		if (neighbour && isSame && curLevel >= addIronLevel)
		{
		addScore(neighbour->getScore());
		auto actions = ActionSequence::create(neighbour);
		auto scaleTo = CCScaleTo::create(0.5f, 0.0f);
		auto moveTo = CCMoveTo::create(0.5f, getPosition());
		auto action = CCSpawn::create(scaleTo, moveTo, NULL);
		auto callback = CCFunctionAction::create([=]()
		{
		SquareModel::theModel()->removeSquareNode(neighbour);
		});
		actions->addAction(action);
		actions->addAction(callback);
		actions->runActions();
		}
		});
		actions->addAction(callback3);*/
	}
	actions->runActions();
}

bool SquareNode::shouldRemoveDirectly()
{
	MarbleAttr attr;
	switch (m_squareType)
	{
	case kType_Square:
	case kType_Triangle:
	case kType_AddMarble:
	case kType_AddCoin:
		return true;
		break;
	case kType_Iron:
		attr = MarbleModel::theModel()->getMarbleAttr();
		if (attr.skin == kMarble_Dispersed)
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool SquareNode::canRemoveByProps()
{
	switch (m_squareType)
	{
	case kType_Square:
	case kType_Triangle:
	case kType_BossEatMarble:
	case kType_Iron:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void SquareNode::runRemoveAction()
{
	auto explore = GameUtil::getRandomExplodeEffect();
	explore->setPosition(getPosition());
	getParent()->addChild(explore);
	removeBody();
	removeFromParent();
}

void SquareNode::doScaleAction()
{
	setScale(1.0f);
	auto scaleTo = CCScaleTo::create(0.1f, 1.2f);
	auto scaleBack = CCScaleTo::create(0.1f, 1.0f);
	auto actions = CCSequence::create(scaleTo, scaleBack, NULL);
	runAction(actions);
}

void SquareNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createSquare(this);
}

void SquareNode::removeBody()
{
	if (m_body)
	{
		//Box2dFactory::getInstance()->removeBody(m_body);
	}
}

void SquareNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Pop);
	showBombAction();

	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);
	int skin = MarbleModel::theModel()->getMarbleAttr().skin;
	if (skin == kMarble_Bomb)
	{
		SquareModel::theModel()->elimateAroundSquare(this);
	}
}

void SquareNode::showBombAction()
{
	int skin = MarbleModel::theModel()->getMarbleAttr().skin;
	if (skin == kMarble_Bomb)
	{
		if (getChildrenCount() < 8)
		{
			auto explore = GameUtil::getBombEffect2();
			explore->setPosition(ccp(-explore->getContentSize().width / 2, -explore->getContentSize().height / 2));
			addChild(explore, kZOrder_Square + 1);
			/*explore->setPosition(getPosition());
			getParent()->addChild(explore, kZOrder_Square + 1);*/

			/*auto explore = GameUtil::getRandomExplodeEffect();
			explore->setPosition(ccp(-explore->getContentSize().width / 2, -explore->getContentSize().height / 2));
			addChild(explore, kZOrder_Square + 1);*/
		}
	}
}

void SquareNode::setFreezing(bool isFreezing)
{
	if (isFreezing)
	{
		auto freezing = CCSprite::create("game/freezing.png");
		addChild(freezing, 100, 100);
	}
	else
	{
		removeChildByTag(100);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
TriangleNode *TriangleNode::create(int shap /* = -1 */)
{
	TriangleNode *node = new TriangleNode();
	node->autorelease();
	node->init(shap);
	return node;
}

bool TriangleNode::init(int shap /* = -1 */)
{
	m_score = SquareModel::theModel()->getCurrentScore();
	if (m_score % 10 == 0)
	{
		int random = rand() % 100;
		if (random < 50)
		{
			m_score = m_score * 2;
		}
	}

	if (shap < 0)
	{
		shap = rand() % 4;
	}
	char temp[50] = { 0 };
	sprintf(temp, "half_%d.png", shap);
	m_image = GameUtil::getBlockImage(kBlock_Triangle, m_score);
	m_imageSprite = CCSprite::create(m_image.c_str());
	addChild(m_imageSprite);
	auto fanIn = CCFadeIn::create(0.6f);
	m_imageSprite->runAction(fanIn);

	auto size = m_imageSprite->getContentSize();
	setContentSize(ccp(64, 64));

	m_scoreLabel = GameUtil::getImageNum(FONT_WHITE, m_score);
	switch (shap)
	{
	case 0:
		m_scoreLabel->setPosition(ccp(-size.width*0.15f, -size.height*0.2f));
		m_imageSprite->setRotation(-90);
		break;
	case 1:
		m_scoreLabel->setPosition(ccp(-size.width*0.15f, size.height*0.15f));
		m_imageSprite->setRotation(0);
		break;
	case 2:
		m_scoreLabel->setPosition(ccp(size.width*0.15f, size.height*0.15f));
		m_imageSprite->setRotation(90);
		break;
	case 3:
		m_scoreLabel->setPosition(ccp(size.width*0.15f, -size.height*0.2f));
		m_imageSprite->setRotation(180);
		break;
	}
	addChild(m_scoreLabel);
	this->setTag(shap);
	setPerfectScale();

	return true;
}

void TriangleNode::setPerfectScale()
{
	if (m_scoreLabel)
	{
		float scale = 1.0f;
		int tempScore = m_score / 10;
		while (tempScore > 0)
		{
			scale -= 0.2f;
			tempScore = tempScore / 10;
		}
		m_scoreLabel->setScale(scale);
	}
}

void TriangleNode::setPosition(const cocos2d::CCPoint &position)
{
	CCNode::setPosition(position);
	if (m_body)
	{
		b2Vec2 post;
		post = b2Vec2((float)((getPosition().x - getContentSize().width / 2) / PTM_RATIO), (float)(((getPosition().y - getContentSize().height / 2)) / PTM_RATIO));
		float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
		m_body->SetTransform(post, angle);
	}
}

void TriangleNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createTriangle(this);
}

void TriangleNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Pop);
	showBombAction();

	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);
	int skin = MarbleModel::theModel()->getMarbleAttr().skin;
	if (skin == kMarble_Bomb)
	{
		SquareModel::theModel()->elimateAroundSquare(this);
	}
}

void TriangleNode::addScore(int score)
{
	m_score += score;
	std::string scoreStr = GameUtil::intToString(m_score);
	if (m_scoreLabel)
	{
		m_scoreLabel->setString(scoreStr.c_str());
		m_image = GameUtil::getBlockImage(kBlock_Triangle, m_score);
		m_imageSprite->initWithFile(m_image.c_str());
		setPerfectScale();
	}
}


/////////////////////////////////////////////////////////////////////////////////////
BossEatMarbleNode *BossEatMarbleNode::create()
{
	BossEatMarbleNode *node = new BossEatMarbleNode();
	node->autorelease();
	node->init();
	return node;
}

bool BossEatMarbleNode::init()
{
	m_score = 1;// SquareModel::theModel()->getCurrentScore();

	/*m_imageSprite = CCSprite::create("boss_0.png");
	addChild(m_imageSprite);
	auto fanIn = CCFadeIn::create(0.6f);
	m_imageSprite->runAction(fanIn);*/

	CCParticleSun* sun = CCParticleSun::create();
	sun->setStartSize(60);
	sun->setEmissionRate(100);
	sun->setAnchorPoint(ccp(0.5f, 0.5f));
	sun->setPosition(0, 0);
	sun->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fire.png"));
	addChild(sun);

	m_imageSprite = CCSprite::create("squares/fangkuai_hengxiao.png");
	auto size = m_imageSprite->getContentSize();
	setContentSize(ccp(64, 64));

	return true;
}

void BossEatMarbleNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createCircle(this, m_imageSprite->getContentSize(), true);
}

void BossEatMarbleNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Pop);
	showBombAction();

	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);
}

void BossEatMarbleNode::runRemoveAction()
{
	removeBody();
	removeFromParent();
}


/////////////////////////////////////////////////////////////////////////////////////
IronNode *IronNode::create()
{
	IronNode *node = new IronNode();
	node->autorelease();
	node->init();
	return node;
}

bool IronNode::init()
{
	m_score = 1;// SquareModel::theModel()->getCurrentScore();

	m_imageSprite = CCSprite::create("squares/fangkuai_tie.png");
	addChild(m_imageSprite);
	auto fanIn = CCFadeIn::create(0.6f);
	m_imageSprite->runAction(fanIn);

	auto size = m_imageSprite->getContentSize();
	setContentSize(size);

	return true;
}

void IronNode::setBody()
{
	m_body = Box2dFactory::getInstance()->createSquare(this);
}

void IronNode::doCollisionAction()
{
	SoundMgr::theMgr()->playEffect(Effect_Pop);
	showBombAction();

	int attactRate = GameController::getInstance()->getAttactRate();
	addScore(-attactRate);
}

void IronNode::runRemoveAction()
{
	auto explore = GameUtil::getExplodeEffect("squares/fangkuai_tie.png");
	explore->setPosition(getPosition());
	getParent()->addChild(explore, kZOrder_Square + 1);
	removeBody();
	removeFromParent();
}
