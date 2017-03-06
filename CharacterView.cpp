#include "CharacterView.h"
#include "UiLayout.h"
#include "CommonMacros.h"
#include "GameController.h"
#include "BallHintModel.h"

USING_NS_CC;

CharacterView::CharacterView()
{

}

bool CharacterView::init()
{
	if (!CCNode::init())
	{
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_characterLayout = UiLayout::create("layout/character_node.xml");
	m_characterLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_characterLayout->setPosition(ccpMult(winSize, 0.5f));
	m_characterLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_characterLayout, kZOrder_Character);
	initCharacterLayout();

	return true;
}

void CharacterView::initCharacterLayout()
{
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	m_arrow = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(11));

	auto targetPos = GameController::getInstance()->getTargetPos();
	if (targetPos.x != 0 && targetPos.y != 0)
	{
		character_body->setPosition(ccp(targetPos.x, character_body->getPositionY()));

		character_head->setPosition(ccp(targetPos.x, character_head->getPositionY()));
		m_arrow->setVisible(false);
	}
}

cocos2d::CCNode *CharacterView::getBodyById(int id)
{
	return m_characterLayout->getChildById(id);
}

void CharacterView::checkPosition()
{
	//check character's position
	auto targetPos = GameController::getInstance()->getTargetPos();
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	auto moveTo = CCMoveTo::create(0.5f, ccp(targetPos.x, character_body->getPositionY()));
	character_body->runAction(moveTo);

	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	auto moveTo2 = CCMoveBy::create(0.5f, ccp(targetPos.x - character_body->getPositionX(), 0));
	character_head->runAction(moveTo2);

}

void CharacterView::resetCharacterPos()
{
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	character_head->setRotation(0);
}

void CharacterView::touchCallback(float degree)
{
	m_arrow->setRotation(180 - degree);
	if (degree < 6 || degree > 174)
	{
		m_arrow->setVisible(false);
		BallHintModel::theModel()->setHintVisible(false);
	}
	else
	{
		m_arrow->setVisible(true);
		BallHintModel::theModel()->setHintVisible(true);
	}

	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	if (degree < 90)
	{
		character_head->setPositionX(character_body->getPositionX() - 20);
		character_body->setFlipX(true);
		character_head->setFlipX(true);

		m_arrow->setAnchorPoint(ccp(0.73f, 0.35f));
		m_arrow->setPosition(ccp(character_body->getPositionX() + 10, character_body->getPositionY() + 6));
		m_arrow->setFlipY(true);
	}
	else
	{
		character_head->setPositionX(character_body->getPositionX());
		character_body->setFlipX(false);
		character_head->setFlipX(false);

		m_arrow->setAnchorPoint(ccp(0.73f, 0.65f));
		m_arrow->setPosition(ccp(character_body->getPositionX() - 12, character_body->getPositionY() + 6));
		m_arrow->setFlipY(false);
	}

	if (degree > 60 && degree < 90)
	{
		character_head->setRotation(60 - degree);
	}
	else if (degree >= 90 && degree < 120)
	{
		character_head->setRotation(120 - degree);
	}
	else
	{
		character_head->setRotation(0);
	}
}


void CharacterView::characterMove(float offsetX)
{
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	character_body->setPositionX(character_body->getPositionX() + offsetX);
	character_head->setPositionX(character_head->getPositionX() + offsetX);
	m_arrow->setPositionX(m_arrow->getPositionX() + offsetX);

}