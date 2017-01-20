#ifndef _BALL_HINT_MODEL_H_
#define _BALL_HINT_MODEL_H_

#include "cocos2d.h"
class BallHintModel
{
public:
	static BallHintModel *theModel();
	~BallHintModel(){}

public:
	std::vector<cocos2d::CCSprite*> createBallHints();
	std::vector<cocos2d::CCSprite*> getBallHints(){ return m_ballHints; }
	void setHintVisible(bool visible);
	void updatePosition(cocos2d::CCPoint pos1, cocos2d::CCPoint pos2, cocos2d::CCPoint startPos, float arrowWidth);
	cocos2d::CCPoint getShootPosition(){ return m_shootPosition; }

	void clearHints();
private:
	BallHintModel(){}

private:
	std::vector<cocos2d::CCSprite*> m_ballHints;
	cocos2d::CCPoint m_shootPosition;
};
#endif