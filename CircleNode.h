#ifndef __CIRCLE_NODE_H__
#define __CIRCLE_NODE_H__

#include "SquareNode.h"
#include "CommonMacros.h"

class CircleAddMarbleNode : public SquareNode
{
public:
	CircleAddMarbleNode() : SquareNode(){ m_collisionType = kCollision_AddMarble; }
	static CircleAddMarbleNode *create();
	virtual bool init();
	virtual void doCollisionAction();
};

class CircleEliRowNode : public SquareNode
{
public:
	CircleEliRowNode() : SquareNode(){ m_collisionType = kCollision_EliminateRow; }
	static CircleEliRowNode *create();
	virtual bool init();
	virtual void doCollisionAction();
};

class CircleEliColNode : public SquareNode
{
public:
	CircleEliColNode() : SquareNode(){ m_collisionType = kCollision_EliminateCol; }
	static CircleEliColNode *create();
	virtual bool init();
	virtual void doCollisionAction();
};


class CircleReboundNode : public SquareNode
{
public:
	CircleReboundNode() : SquareNode(){ m_collisionType = kCollision_Rebound; }
	static CircleReboundNode *create();
	virtual bool init();
	virtual void doCollisionAction();
};

class CircleAddCoinNode : public SquareNode
{
public:
	CircleAddCoinNode() : SquareNode(){ m_collisionType = kCollision_AddCoin; }
	static CircleAddCoinNode *create();
	virtual bool init();
	virtual void doCollisionAction();
};

#endif