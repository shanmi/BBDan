#include "ContactListener.h"  
#include "Box2dFactory.h"
#include "SquareNode.h"
#include "MarbleNode.h"
#include "GameController.h"
#include "ccMacros.h"

USING_NS_CC;

void ContactListener::BeginContact(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	auto spriteA = (CCNode*)bodyA->GetUserData();
	auto spriteB = (MarbleNode*)bodyB->GetUserData();

	if (spriteA != nullptr && spriteB != nullptr)
	{
		if (spriteA->getTag() == 100)
		{
			if (!GameController::getInstance()->isCounterFull() && spriteB->isMoving())
			{
				GameController::getInstance()->addCounter();
			}
			if (GameController::getInstance()->getCounter() == 1)
			{
				GameController::getInstance()->setTargetPos(spriteB->getPosition());
			}
			spriteB->stop();
		}
		else
		{
			auto square = (SquareNode*)bodyA->GetUserData();
			square->addScore(-1);
		}
	}
}


void ContactListener::EndContact(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	auto spriteA = (CCSprite*)bodyA->GetUserData();
	auto spriteB = (CCSprite*)bodyB->GetUserData();

	if (spriteA != nullptr && spriteB != nullptr)
	{
		/*spriteA->setColor(Color3B::WHITE);
		spriteB->setColor(Color3B::WHITE);*/
	}
}


void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}


void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}