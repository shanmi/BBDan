#include "SquareModel.h"
#include "ContactListener.h"  
#include "Box2dFactory.h"
#include "SquareNode.h"
#include "MarbleNode.h"
#include "GameController.h"
#include "ccMacros.h"
#include "CircleNode.h"
#include "MarbleModel.h"

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
			if (!GameController::getInstance()->isCounterFull() && marble->isMoving())
			{
				GameController::getInstance()->addCounter();
			}
			if (GameController::getInstance()->getCounter() == 1)
			{
				GameController::getInstance()->setTargetPos(spriteB->getPosition());
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
				/*auto pos = GameController::getInstance()->getTargetPos();
				marble->setPosition(pos);*/
				GameController::getInstance()->addCounter();
			}
			if (square->getCollisionType() != kCollision_EliminateRow && square->getCollisionType() != kCollision_EliminateCol)
			{
				MarbleModel::theModel()->reboundMarbles();
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