#ifndef __MARBLEATTR_H__
#define __MARBLEATTR_H__

#include "CommonMacros.h"
struct MarbleAttr
{
	int skin;
	float speed;
	int damage;
	MarbleAttr(){}
	MarbleAttr(int skin, float speed, int damage)
	{
		this->skin = skin;
		this->speed = speed;
		this->damage = damage;
	}
};

struct NormalMarle : public MarbleAttr
{
	NormalMarle() : MarbleAttr(kMarble_Normal, 1.0f, 1){}
};

struct FasterMarle : public MarbleAttr
{
	FasterMarle() : MarbleAttr(kMarble_Faster, 2.0f, 1){}
};

struct BiggerMarle : public MarbleAttr
{
	BiggerMarle() : MarbleAttr(kMarble_Biger, 1.0f, 2){}
};

struct DispersedMarle : public MarbleAttr
{
	DispersedMarle() : MarbleAttr(kMarble_Dispersed, 1.0f, 1){}
};

struct AcrossdMarle : public MarbleAttr
{
	AcrossdMarle() : MarbleAttr(kMarble_Across, 1.0f, 1){}
};

struct BombMarle : public MarbleAttr
{
	BombMarle() : MarbleAttr(kMarble_Bomb, 1.0f, 1){}
};

#endif