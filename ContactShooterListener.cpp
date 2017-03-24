#include "SquareModel.h"
#include "ContactShooterListener.h"  
#include "Box2dFactory.h"
#include "SquareNode.h"
#include "MarbleNode.h"
#include "GameController.h"
#include "ccMacros.h"
#include "CircleNode.h"
#include "MarbleModel.h"
#include "BossView.h"

USING_NS_CC;

void ContactShooterListener::BeginContact(b2Contact* contact)
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

		if (node->getTag() == kTag_Wall || node->getTag() == kTag_Wall_Top)
		{
			marble->setVisible(false);
			marble->stop();
		}
		else if (node->getTag() == kTag_Boss)
		{
			marble->setVisible(false);
			marble->stop();
			auto boss = dynamic_cast<BossView*>(node);
			boss->addBlood(-1);
		}
		else
		{
			auto square = dynamic_cast<SquareNode*>(node);
			square->doCollisionAction();
			if (square->getSquareType() == kType_Rebound)
			{
				float degree = rand() % 60 + 60;
				marble->shoot(degree);
			}
			marble->stop();
			marble->setVisible(false);
		}
	}
}


void ContactShooterListener::EndContact(b2Contact* contact)
{

}


void ContactShooterListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{

}


void ContactShooterListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}