#ifndef _SQUARE_NODE_H_
#define _SQUARE_NODE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "CommonMacros.h"

struct CommonAction
{
	virtual void doCollisionAction(){}
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

	virtual void doScaleAction();
	virtual void doCollisionAction();
public:
	void setIndex(int index){ m_index = index; }
	int getIndex() { return m_index; }
	
	int getCollisionType(){ return m_collisionType; }

	void addScore(int score);
	int getScore() { return m_score; }

	void setBody(b2Body *body){ m_body = body; }
	b2Body *getBody(){ return m_body; }

	void moveDown();

	bool shouldRemoveDirectly();

protected:
	b2Body *m_body;
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
	virtual void doCollisionAction();
};

#endif