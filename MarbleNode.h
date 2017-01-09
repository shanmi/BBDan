#ifndef __MARBLE_NODE_H__
#define __MARBLE_NODE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

struct MarbleAttr
{
	int skin;
	int damage;
};

class MarbleNode : public cocos2d::CCNode
{
public:
	static MarbleNode *create(MarbleAttr attr);
	~MarbleNode();

	virtual bool init();
	virtual void setPosition(const cocos2d::CCPoint &position);
	
public:
	void setBody(b2Body *body){ m_body = body; }
	b2Body *getBody(){ return m_body; }

	void shoot(float degree);
	void stop();

	void setMovingState(bool isMoving){ m_bIsMoving = isMoving; }
	bool isMoving(){ return m_bIsMoving; }

	void moveToTargetPos();

	bool isTrueStop(){ return m_bTrueStop; }

	void addReboundTimes(int times = 1){ m_reboundTimes += times; }
	int getReboundTimes(){ return m_reboundTimes; }

private:
	MarbleNode(MarbleAttr attr);

private:
	MarbleAttr m_attr;
	b2Body *m_body;
	bool m_bIsMoving;
	bool m_bTrueStop;
	int m_reboundTimes;
};
#endif