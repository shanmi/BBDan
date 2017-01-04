#include "ActionSequence.h"

USING_NS_CC;

ActionSequence::ActionSequence(CCNode *target)
:m_target(target)
, actionTime(0)
{

}

ActionSequence::~ActionSequence()
{

}

ActionSequence *ActionSequence::create(CCNode* target)
{
	ActionSequence *sequence = new ActionSequence(target);
	sequence->autorelease();
	return sequence;
}

void ActionSequence::addAction(CCFiniteTimeAction *action)
{
	auto sequence = CCSequence::create(CCDelayTime::create(actionTime), action, NULL);
	auto iter = find(m_actions.begin(), m_actions.end(), action);
	if (iter == m_actions.end())
	{
		actionTime += action->getDuration();
		m_actions.push_back(sequence);
	}
}

void ActionSequence::runActions()
{
	for (auto iter = m_actions.begin(); iter < m_actions.end(); iter++)
	{
		m_target->runAction(*iter);
	}
}