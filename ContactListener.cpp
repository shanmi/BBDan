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
			if (GameController::getInstance()->getCounter() == 1)
			{
				auto targetPos = GameController::getInstance()->getTargetPos();
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
			square->doCollisionAction();
			if (square->getCollisionType() == kCollision_BossEatMarble)
			{
				marble->stop();
				GameController::getInstance()->addCounter();
				if (GameController::getInstance()->getCounter() == 1)
				{
					marble->setVisible(true);
				}
				else
				{
					marble->setVisible(false);
				}
			}
			else if (square->getCollisionType() == kCollision_Rebound)
			{
				float degree = rand() % 60 + 60;
				marble->shoot(degree);
			}
			if (square->getCollisionType() != kCollision_EliminateRow && square->getCollisionType() != kCollision_EliminateCol)
			{
				MarbleModel::theModel()->reboundMarbles();
			}

			if (square->canRemoveByProps())
			{
				SoundMgr::theMgr()->playEffect(Effect_Pop);
			}
			else if (square->getCollisionType() == kCollision_AddMarble)
			{
				SoundMgr::theMgr()->playEffect(Effect_Pop1);
			}
			else if (square->getCollisionType() == kCollision_AddCoin)
			{
				SoundMgr::theMgr()->playEffect(Effect_Coin);
			}
			else if (square->getCollisionType() == kCollision_Rebound)
			{
				SoundMgr::theMgr()->playEffect(Effect_Pop2);
			}
			else if (square->getCollisionType() == kCollision_EliminateRow || square->getCollisionType() == kCollision_EliminateCol)
			{
				SoundMgr::theMgr()->playEffect(Effect_Lazer);
			}
			else
			{
				SoundMgr::theMgr()->playEffect(Effect_Pop3);
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