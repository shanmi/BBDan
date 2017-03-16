#include "CrystalView.h"
#include "CCFunctionAction.h"
#include "ActionSequence.h"

USING_NS_CC;

void CrystalView::onEnter()
{
	CCNode::onEnter();
	GameController::getInstance()->addView(this);
}

void CrystalView::onExit()
{
	CCNode::onExit();
	GameController::getInstance()->removeView(this);
}

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
	case kCrystal_1:
		m_image = CCSprite::create("game/crystal1.png");
		break;
	case kCrystal_2:
		m_image = CCSprite::create("game/crystal2.png");
		break;
	case kCrystal_3:
		m_image = CCSprite::create("game/crystal1.png");
		break;
	}
	m_bloodCount = GameController::getInstance()->getCrystalBlood(m_type);
	addChild(m_image);
	auto size = m_image->getContentSize();
	setContentSize(size);
	addBloodCount(0);

	return true;
}

void CrystalView::addBloodCount(int count)
{
	m_bloodCount += count;
	GameController::getInstance()->setCrystalBlood(m_type, m_bloodCount);
	if (m_bloodCount <= 0)
	{
		setVisible(false);
	}
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

void CrystalView::rebuildCrystal()
{
	m_bloodCount = GameController::getInstance()->getCrystalBlood(m_type);
	setVisible(true);
}

void CrystalView::addCrystalEffect()
{
	m_bloodCount++;
	setVisible(true);
}