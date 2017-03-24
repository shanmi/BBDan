#include "CrystalView.h"
#include "CCFunctionAction.h"
#include "ActionSequence.h"
#include "UiLayout.h"
#include "GameUtil.h"

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
	m_mainLayout = UiLayout::create("layout/crystal_node.xml");
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(m_mainLayout);

	m_image = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(1));
	m_count = dynamic_cast<CCLabelAtlas*>(m_mainLayout->getChildById(2));

	m_bloodCount = GameController::getInstance()->getCrystalBlood(m_type);
	m_count->setString(GameUtil::intToString(m_bloodCount).c_str());

	char temp[100] = { 0 };
	int index = 1;
	if (m_bloodCount <= 0)
	{
		index = 3;
	}
	else if (m_bloodCount == 1)
	{
		index = 2;
	}
	sprintf(temp, "game/sejimosi_kuangshi%d_%d.png", m_type+1, index);
	m_image->initWithFile(temp);

	auto size = m_image->getContentSize();
	setContentSize(size);
	addBloodCount(0);

	return true;
}

void CrystalView::addBloodCount(int count)
{
	m_bloodCount += count;
	m_count->setString(GameUtil::intToString(m_bloodCount).c_str());
	if (m_bloodCount <= 0)
	{
		m_bloodCount = 0;
		m_count->setVisible(false);
	}
	else
	{
		m_count->setVisible(true);
	}

	GameController::getInstance()->setCrystalBlood(m_type, m_bloodCount);
	char temp[100] = { 0 };
	int index = 1;
	if (m_bloodCount <= 0)
	{
		index = 3;
	}
	else if (m_bloodCount == 1)
	{
		index = 2;
	}
	sprintf(temp, "game/sejimosi_kuangshi%d_%d.png", m_type + 1, index);
	m_image->initWithFile(temp);
	if (count > 0)
	{
		auto effect = GameUtil::getCryStalEffect();
		addChild(effect);
	}
	else if (count < 0)
	{
		auto effect = GameUtil::getExplodeEffect("particle/crystal_particle.png");
		effect->setPosition(getPosition());
		getParent()->addChild(effect);
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
}

void CrystalView::addCrystalEffect()
{
	addBloodCount(1);
}