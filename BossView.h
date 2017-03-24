#ifndef __BOSS_VIEW_H__
#define __BOSS_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class BossView : public cocos2d::CCNode
{
public:
	~BossView(){}
	static BossView *create(int type);
	virtual bool init();
	virtual void setPosition(const cocos2d::CCPoint &position);
public:
	void setBody();
	void doSleepAction();
	void startMoveAction();
	void doAttactAction();

	void bossAttactEffect();
	void addBlood(int count);
	void runDieEffect();
private:
	BossView(int type);

private:
	cocos2d::extension::CCArmature *m_armature;
	cocos2d::CCProgressTimer *m_bloodProgress;
	b2Body *m_body;
	std::string m_animPath;
	int m_type;
	int k_bloodRecord;
	int m_bloodCount;
	int m_posIndex;
};
#endif