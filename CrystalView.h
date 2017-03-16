#ifndef __CRYSTAL_VIEW_H__
#define __CRYSTAL_VIEW_H__

#include "cocos2d.h"
#include "GameController.h"

enum CrystalType
{
	kCrystal_1,
	kCrystal_2,
	kCrystal_3,
};

class CrystalView :
	public cocos2d::CCNode
	, INotifyView
{
public:
	static CrystalView *create(int type);
	virtual bool init();
	~CrystalView(){}

	virtual void onEnter();
	virtual void onExit();
	virtual void addCrystalEffect();
	virtual void rebuildCrystal();
public:
	int getBloodCount(){ return m_bloodCount; }
	void addBloodCount(int count);

	void runDieEffect();

private:
	CrystalView(int type);

public:
	cocos2d::CCSprite *m_image;
	int m_type;
	int m_bloodCount;
};
#endif