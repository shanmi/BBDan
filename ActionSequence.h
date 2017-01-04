#ifndef __ACTION_SEQUENCE_H__
#define __ACTION_SEQUENCE_H__

#include "cocos2d.h"

class ActionSequence
	:public cocos2d::CCObject
{
public:
	static ActionSequence *create(cocos2d::CCNode* target);
	void addAction(cocos2d::CCFiniteTimeAction *action);
	void runActions();

private:
	ActionSequence(cocos2d::CCNode *target);
	~ActionSequence();

private:
	cocos2d::CCNode *m_target;
	std::vector<cocos2d::CCFiniteTimeAction*> m_actions;
	float actionTime;
};

#endif