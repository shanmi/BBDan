#include "CrystalView.h"
#include "CCFunctionAction.h"
#include "ActionSequence.h"

USING_NS_CC;

CrystalView::CrystalView(int type)
:m_type(type)
{

}

CrystalView *CrystalView::create(int type)
{
	CrystalView *crystal = new CrystalView(type);
	crystal->init();
	crystal->autorelease();
	return crystal;
}

bool CrystalView::init()
{
	if (!CCNode::init())
	{
		return false;
	}
	switch (m_type)
	{
	case kCrystal_Small:
		m_image = CCSprite::create("game/crystal1.png");
		m_bloodCount = 2;
		break;
	case kCrystal_Big:
		m_bloodCount = 3;
		m_image = CCSprite::create("game/crystal2.png");
		break;
	}
	addChild(m_image);
	auto size = m_image->getContentSize();
	setContentSize(size);


	return true;
}

void CrystalView::addBloodCount(int count)
{
	m_bloodCount += count;
}

void CrystalView::runDieEffect()
{
	stopAllActions();
	auto actions = ActionSequence::create(this);
	auto blink = CCBlink::create(1.0f, 10);
	actions->addAction(blink);
	CCFunctionAction *callback = CCFunctionAction::create([=](){
		removeFromParent();
	});
	actions->addAction(callback);
	actions->runActions();
}