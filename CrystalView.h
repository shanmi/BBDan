#ifndef __CRYSTAL_VIEW_H__
#define __CRYSTAL_VIEW_H__

enum CrystalType
{
	kCrystal_Small,
	kCrystal_Big
};
#include "cocos2d.h"
class CrystalView : public cocos2d::CCNode
{
public:
	static CrystalView *create(int type);
	virtual bool init();
	~CrystalView(){}

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