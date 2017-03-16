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

	int gameType = GameController::getInstance()->getGameType();
	switch (gameType)
	{
	case kGame_Normal:
		initCharacterLayout();
		break;
	case kGame_Shoot:
		initCharacterLayout2();
		break;
	}

	return true;
}

void CharacterView::initCharacterLayout()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_characterLayout = UiLayout::create("layout/character_node.xml");
	m_characterLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_characterLayout->setPosition(ccpMult(winSize, 0.5f));
	m_characterLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_characterLayout, kZOrder_Character);

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

void CharacterView::initCharacterLayout2()
{
	m_characterLayout = UiLayout::create("layout/character_node2.xml");
	m_characterLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_characterLayout->setMenuTouchPriority(kPriority_Game - 1);
	addChild(m_characterLayout, kZOrder_Character);
	setContentSize(m_characterLayout->getContentSize());

	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(4));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(3));
	m_arrow = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(2));

	auto targetPos = GameController::getInstance()->getShooterPos();
	if (targetPos.x != 0 && targetPos.y != 0)
	{
		/*character_body->setPosition(ccp(targetPos.x, character_body->getPositionY()));

		character_head->setPosition(ccp(targetPos.x, character_head->getPositionY()));
		m_arrow->setVisible(false);*/
		setPosition(targetPos);
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
	character_head->setRotation(0);
	auto moveTo2 = CCMoveBy::create(0.5f, ccp(targetPos.x - character_body->getPositionX(), 0));
	character_head->runAction(moveTo2);
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

void CharacterView::checkShooterPos(cocos2d::CCPoint position)
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	if (position.x - m_arrow->getContentSize().width / 2 < 0)
	{
		m_arrow->setPositionX(m_arrow->getContentSize().width / 2);
	}
	else if (position.x + m_arrow->getContentSize().width / 2 > winSize.width)
	{
		m_arrow->setPositionX(winSize.width - m_arrow->getContentSize().width / 2);
	}
	else
	{
		m_arrow->setPositionX(position.x);
	}
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));
	character_body->setPositionX(m_arrow->getPositionX());
	character_head->setPositionX(m_arrow->getPositionX());
}

void CharacterView::initShooterPos()
{
	CCSprite *character_body = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(9));
	CCSprite *character_head = dynamic_cast<CCSprite*>(m_characterLayout->getChildById(10));

	auto shooterPos = GameController::getInstance()->getShooterPos();
	if (shooterPos.x != 0 && shooterPos.y != 0)
	{
		character_body->setPosition(ccp(shooterPos.x, character_body->getPositionY()));

		character_head->setPosition(ccp(shooterPos.x, character_head->getPositionY()));

		m_arrow->setPosition(ccp(shooterPos.x, m_arrow->getPositionY()));
		m_arrow->setVisible(false);
	}
}