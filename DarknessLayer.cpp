#include "DarknessLayer.h"

USING_NS_CC;

void DarknessLayer::onEnter()
{
	CCLayer::onEnter();
}

void DarknessLayer::onExit()
{
	CCLayer::onExit();
}

bool DarknessLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	mGradientLayer = CCLayerGradient::create();
	mGradientLayer->initWithColor(ccc4(0, 0, 0, 180), ccc4(0, 0, 0, 255), ccp(0, 1));
	addChild(mGradientLayer);

	return true;
}