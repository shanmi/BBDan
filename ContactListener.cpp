#include "SquareModel.h"
#include "ContactListener.h"  
#include "Box2dFactory.h"
#include "SquareNode.h"
#include "MarbleNode.h"
#include "GameController.h"
#include "ccMacros.h"
#include "CircleNode.h"
#include "MarbleModel.h"
#include "SoundMgr.h"

USING_NS_CC;

void ContactListener::BeginContact(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	auto spriteA = (CCNode*)bodyA->GetUserData();
	auto spriteB = (CCNode*)bodyB->GetUserData();

	if (spriteA != nullptr && spriteB != nullptr)
	{
		MarbleNode *marble;
		CCNode *node;
		if (spriteA->getTag() == kTag_Marble)
		{
			marble = dynamic_cast<MarbleNode*>(spriteA);
			node = dynamic_cast<CCNode*>(spriteB);
		}
		else
		{
			marble = dynamic_cast<MarbleNode*>(spriteB);
			node = dynamic_cast<CCNode*>(spriteA);
		}

		if (node->getTag() == kTag_Wall)
		{
			if (marble->isMoving())
			{
				GameController::getInstance()->addCounter();
			}
			auto targetPos = GameController::getInstance()->getTargetPos();
			auto winSize = CCDirector::sharedDirector()->getWinSize();
			if (GameController::getInstance()->getCounter() == 1 || targetPos.x < 0 || targetPos.x > winSize.width || targetPos.y < 0)
			{
				GameController::getInstance()->setTargetPos(ccp(spriteB->getPositionX(), targetPos.y));
				marble->setVisible(true);
			}
			else
			{
				marble->setVisible(false);
			}
			marble->stop();
			MarbleModel::theModel()->reboundMarbles();
		}
		else
		{
			auto square = dynamic_cast<SquareNode*>(node);
			auto targetPos = GameController::getInstance()->getTargetPos();
			if (square->getPositionY() - square->getContentSize().height < targetPos.y && square->getSquareType() != kType_Rebound)
			{
				return;
			}
			square->doCollisionAction();
			if (square->getSquareType() == kType_BossEatMarble)
			{
				auto attr = MarbleModel::theModel()->getMarbleAttr();
				if (!attr.skin == kMarble_Faster)
				{
					marble->stop();
					GameController::getInstance()->addCounter();
				}
			}
			else if (square->getSquareType() == kType_Rebound)
			{
				float degree = rand() % 60 + 60;
				marble->shoot(degree);
			}
			if (square->getSquareType() != kType_EliminateRow && square->getSquareType() != kType_EliminateCol
				&& square->getSquareType() != kType_EliminateCross && square->getSquareType() != kType_Iron)
			{
				MarbleModel::theModel()->reboundMarbles();
			}
			//if (square->getSquareType() == kType_Iron) //铁块反弹一定角度
			//{
			//	CCLog("square=================%d", square->getScore());
			//}
			//当弹珠在垂直方向不断打击方块的情况
			auto body = marble->getBody();
			auto velocity = body->GetLinearVelocity();
			if (fabs(velocity.x) < 0.1 && velocity.y < -1 &&
				marble->getPositionX() > square->getPositionX() - square->getContentSize().width / 2 - marble->getContentSize().width &&
				marble->getPositionX() < square->getPositionX() + square->getContentSize().width / 2 + marble->getContentSize().width)
			{
				square->addScore(-square->getScore() / 2); //普通方块加快消除
				if (square->getSquareType() == kType_Iron) //铁块反弹一定角度
				{
					marble->shoot(45);
				}
			}
			//当弹珠在水平方向不断打击方块的情况
			if (fabs(velocity.y) < 0.1 && fabs(velocity.x) > 1 &&
				marble->getPositionY() > square->getPositionY() - square->getContentSize().height / 2 - marble->getContentSize().height &&
				marble->getPositionY() < square->getPositionY() + square->getContentSize().height / 2 + marble->getContentSize().height)
			{
				square->addScore(-square->getScore() / 2); //普通方块加快消除
				if (square->getSquareType() == kType_Iron) //铁块反弹一定角度
				{
					marble->shoot(45);
				}
			}
		}
	}
	else
	{
		MarbleNode *marble;
		if (spriteA != nullptr)
		{
			marble = dynamic_cast<MarbleNode*>(spriteA);
		}
		else
		{
			marble = dynamic_cast<MarbleNode*>(spriteB);
		}
		marble->addReboundTimes();

		bool isMarbleNeverStop = MarbleModel::theModel()->isMarblesNerverStop();
		if (isMarbleNeverStop)
		{
			GameController::getInstance()->createPropByMarble(marble);
		}
	}
}


void ContactListener::EndContact(b2Contact* contact)
{

}


void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}


void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}