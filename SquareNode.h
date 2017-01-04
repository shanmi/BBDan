#ifndef _SQUARE_NODE_H_
#define _SQUARE_NODE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class SquareNode : public cocos2d::CCNode
{
public:
	static SquareNode *create(int score);
	SquareNode(int score);
	~SquareNode(){}

	virtual bool init();
public:
	void setIndex(int index){ m_index = index; }
	int getIndex() { return m_index; }
	
	void addScore(int score);
	int getScore() { return m_score; }

	void setBody(b2Body *body){ m_body = body; }
	b2Body *getBody(){ return m_body; }

	void moveDown();

protected:
	b2Body *m_body;
	cocos2d::CCLabelTTF *m_scoreLabel;
	int m_score;
	int m_index;

};

class TriangleNode : public SquareNode
{
public:
	TriangleNode(int score) : SquareNode(score){}
	static TriangleNode *create(int score);
	virtual bool init();
};

#endif