#include "MarbleModel.h"
#include "MarbleNode.h"
#include "Config.h"
#include "CommonMacros.h"
#include "Box2dFactory.h"
#include "UserInfo.h"
#include "GameUtil.h"
#include "GameController.h"

MarbleModel::MarbleModel()
{
	int type = UserInfo::getInstance()->getCurMarbleType();
	m_attr = getMarbleAttrByType(type);

	m_marblesCount = 0;
	m_attactRate = 1;
	m_tempAddCount = 0;
}
MarbleModel *MarbleModel::theModel()
{
	static MarbleModel model;
	return &model;
}

MarbleNode* MarbleModel::createMarble()
{
	//m_attr = FasterMarle();
	MarbleNode *marble = MarbleNode::create(m_attr);
	marble->setVisible(false);
	m_marbles.push_back(marble);
	m_marblesCount = m_marbles.size();
	return marble;
}

void MarbleModel::removeMarble(MarbleNode *node)
{
	auto iter = find(m_marbles.begin(), m_marbles.end(), node);
	if (iter != m_marbles.end())
	{
		auto marble = *iter;
		m_marbles.erase(iter);
		Box2dFactory::getInstance()->removeBody(marble->getBody());
		marble->removeFromParent();
	}
}

void MarbleModel::setMarbleAttr(MarbleAttr attr)
{
	m_attr = attr;
	UserInfo::getInstance()->setCurMarbleType(m_attr.skin);
}

MarbleAttr MarbleModel::getMarbleAttrByType(int type)
{
	MarbleAttr attr;
	switch (type)
	{
	case kMarble_Normal:
		attr = NormalMarle();
		break;
	case kMarble_Faster:
		attr = FasterMarle();
		break;
	case kMarble_Bigger:
		attr = BiggerMarle();
		break;
	case kMarble_Dispersed:
		attr = DispersedMarle();
		break;
	case kMarble_Bomb:
		attr = BombMarle();
		break;
	case kMarble_Across:
		attr = AcrossdMarle();
		break;
	default:
		attr = NormalMarle();
		break;
	}
	return attr;
}

bool MarbleModel::haveMarbleMoving()
{
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		if (!(*iter)->isTrueStop())
		{
			return true;
		}
	}
	return false;
}

void MarbleModel::addMarblesCount()
{
	m_tempAddCount++;
}

int MarbleModel::checkMarblesCount()
{
	int newAddCount = 0;
	if (m_tempAddCount >= m_attactRate)
	{
		newAddCount = m_tempAddCount / m_attactRate;
		m_marblesCount += newAddCount;
		m_tempAddCount = m_tempAddCount % m_attactRate;
	}
	if (m_marblesCount >= 100)
	{
		m_marblesCount = m_marblesCount / 2;
		m_attactRate++;
	}

	if (m_marblesCount == (m_marbles.size() + newAddCount) / 2)
	{
		for (int i = 0; i<m_marblesCount - newAddCount; i++)
		{
			removeMarble(m_marbles[i]);
		}
	}
	int curCount = m_marbles.size();
	if (m_marblesCount > curCount)
	{
		return m_marblesCount - curCount;
	}
	return 0;
}

bool MarbleModel::isMarblesNerverStop()
{
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		auto marble = *iter;
		auto body = marble->getBody();
		auto velocity = body->GetLinearVelocity();
		if ((*iter)->getReboundTimes() >= REBOUND_TIMES)
		{
			if (fabs(velocity.x) > 1 && fabs(velocity.y) < 1)
			{
				return true;
			}
		}
	}
	return false;
}

void MarbleModel::reboundMarbles()
{
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		auto &marble = *iter;
		marble->addReboundTimes(-marble->getReboundTimes());
	}
}

void MarbleModel::clearMarbles()
{
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		auto marble = *iter;
		if (marble->getParent())
		{
			marble->runRemoveAction();
		}
	}
	m_marbles.clear();
}

void MarbleModel::updateMarbles()
{
	auto marbles = getMarbles();
	for (auto iter = marbles.begin(); iter != marbles.end(); ++iter)
	{
		auto &marble = (*iter);
		auto body = marble->getBody();
		MarbleNode *ball = (MarbleNode*)(body->GetUserData());
		float distance = body->GetLinearVelocity().x*body->GetLinearVelocity().x + body->GetLinearVelocity().y*body->GetLinearVelocity().y;
		if (distance > 0 && distance < 400)
		{
			float angle = GameUtil::getDegreeTwoPoints(ccp(0, 0), ccp(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
			marble->shooterShoot(angle);
		}
		if (ball->isMoving())
		{
			b2Vec2 ballPosition = body->GetPosition();
			ball->setPosition(ccp(ballPosition.x * PTM_RATIO, ballPosition.y * PTM_RATIO));
		}
		else
		{
			MarbleModel::theModel()->removeMarble(marble);
		}
	}
}

void MarbleModel::updateMarbles(cocos2d::CCRect invisibleRect, int addMarbleCount)
{
	auto m_marbles = getMarbles();
	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		if (addMarbleCount == 50 && addMarbleCount < m_marbles.size())
		{
			return;
		}

		auto marble = (*iter);
		if (!marble)
		{
			m_marbles.erase(iter);
			continue;
		}
	}

	for (auto iter = m_marbles.begin(); iter != m_marbles.end(); ++iter)
	{
		auto marble = (*iter);
		if (invisibleRect.containsPoint(marble->getPosition()))
		{
			marble->setVisible(false);
		}
		else
		{
			marble->setVisible(true);
		}
		auto body = marble->getBody();
		MarbleNode *ball = (MarbleNode*)(body->GetUserData());
		float distance = body->GetLinearVelocity().x*body->GetLinearVelocity().x + body->GetLinearVelocity().y*body->GetLinearVelocity().y;
		if (distance > 0 && distance < 400)
		{
			float angle = GameUtil::getDegreeTwoPoints(ccp(0, 0), ccp(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
			marble->shoot(angle);
		}
		if (ball->isMoving())
		{
			b2Vec2 ballPosition = body->GetPosition();
			ball->setPosition(ccp(ballPosition.x * PTM_RATIO, ballPosition.y * PTM_RATIO));
			if (ballPosition.y * PTM_RATIO < 0)
			{
				marble->setMovingState(false);
				GameController::getInstance()->addCounter();
			}
		}
		else
		{
			if (!marble->isTrueStop())
			{
				marble->moveToTargetPos();
			}
		}
	}
}