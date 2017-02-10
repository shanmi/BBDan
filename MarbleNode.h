#ifndef __MARBLE_NODE_H__
#define __MARBLE_NODE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "MarbleAttr.h"

class MarbleNode : public cocos2d::CCNode
{
public:
	static MarbleNode *create(MarbleAttr attr);
	~MarbleNode();

	virtual bool init();
	virtual void setPosition(const cocos2d::CCPoint &position);
	
public:
	void setBody();
	b2Body *getBody(){ return m_body; }

	void shoot(float degree);
	void shooterShoot(float degree);
	void stop();

	void setMovingState(bool isMoving){ m_bIsMoving = isMoving; }
	bool isMoving(){ return m_bIsMoving; }

	void moveToTargetPos();

	bool isTrueStop(){ return m_bTrueStop; }

	void addReboundTimes(int times = 1){ m_reboundTimes += times; }
	int getReboundTimes(){ return m_reboundTimes; }

	void initWithFile(const char* imagePath);
	void updateStreak(float delta);
	void addMotionStreak();

private:
	MarbleNode(MarbleAttr attr);

private:
	cocos2d::CCSprite *m_playerBall;
	cocos2d::CCMotionStreak *m_streak;
	MarbleAttr m_attr;
	b2Body *m_body;
	bool m_bIsMoving;
	bool m_bTrueStop;
	int m_reboundTimes;
};
#endif