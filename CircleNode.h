#ifndef __CIRCLE_NODE_H__
#define __CIRCLE_NODE_H__

#include "SquareNode.h"
#include "CommonMacros.h"

class CircleAddMarbleNode : public SquareNode
{
public:
	CircleAddMarbleNode() : SquareNode(){ m_squareType = kType_AddMarble; }
	static CircleAddMarbleNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleEliRowNode : public SquareNode
{
public:
	CircleEliRowNode() : SquareNode(){ m_squareType = kType_EliminateRow; }
	static CircleEliRowNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleEliColNode : public SquareNode
{
public:
	CircleEliColNode() : SquareNode(){ m_squareType = kType_EliminateCol; }
	static CircleEliColNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleEliCrossNode : public SquareNode
{
public:
	CircleEliCrossNode() : SquareNode(){ m_squareType = kType_EliminateCross; }
	static CircleEliCrossNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleReboundNode : public SquareNode
{
public:
	CircleReboundNode() : SquareNode(){ m_squareType = kType_Rebound; }
	static CircleReboundNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleAddCoinNode : public SquareNode
{
public:
	CircleAddCoinNode() : SquareNode(){ m_squareType = kType_AddCoin; }
	static CircleAddCoinNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleProtectNode : public SquareNode
{
public:
	CircleProtectNode() : SquareNode(){ m_squareType = kType_Protect; }
	static CircleProtectNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleShotgunsNode : public SquareNode
{
public:
	CircleShotgunsNode() : SquareNode(){ m_squareType = kType_Shotguns; }
	static CircleShotgunsNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleBloodNode : public SquareNode
{
public:
	CircleBloodNode() : SquareNode(){ m_squareType = kType_Blood; }
	static CircleBloodNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

class CircleRocketNode : public SquareNode
{
public:
	CircleRocketNode() : SquareNode(){ m_squareType = kType_Rocket; }
	static CircleRocketNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void runRemoveAction();
	virtual void doCollisionAction();
};

#endif