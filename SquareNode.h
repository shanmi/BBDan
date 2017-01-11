#ifndef _SQUARE_NODE_H_
#define _SQUARE_NODE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "CommonMacros.h"

struct CommonAction
{
	virtual void setBody(){}
	virtual void doCollisionAction(){}
	virtual void runRemoveAction(){}
};

class SquareNode : public cocos2d::CCNode
	, public CommonAction
{
public:
	static SquareNode *create();
	SquareNode();
	~SquareNode(){}

	virtual bool init();
	virtual void setPosition(const cocos2d::CCPoint &position);

	virtual void setBody();
	virtual void doCollisionAction();
	virtual void runRemoveAction();
public:
	void setIndex(int index){ m_index = index; }
	int getIndex() { return m_index; }

	void addScore(int score);
	int getScore() { return m_score; }

	int getCollisionType(){ return m_collisionType; }
	b2Body *getBody(){ return m_body; }

	void setFreezing(bool isFreezing);

	void moveDown(bool isLastOne = false);
	void doScaleAction();
	bool shouldRemoveDirectly();
	void showBombAction();

protected:
	b2Body *m_body;
	cocos2d::CCSprite *m_image;
	cocos2d::CCLabelTTF *m_scoreLabel;
	int m_collisionType;
	int m_score;
	int m_index;

};

class TriangleNode : public SquareNode
{
public:
	TriangleNode() : SquareNode(){ m_collisionType = kCollision_Triangle; }
	static TriangleNode *create();
	virtual bool init();
	virtual void setPosition(const cocos2d::CCPoint &position);
	virtual void setBody();
	virtual void doCollisionAction();
};

class BossEatMarbleNode : public SquareNode
{
public:
	BossEatMarbleNode() : SquareNode(){ m_collisionType = kCollision_BossEatMarble; }
	static BossEatMarbleNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void doCollisionAction();
};

#endif