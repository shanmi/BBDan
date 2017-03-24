#include "BossView.h"
#include "ActionSequence.h"
#include "CCFunctionAction.h"
#include "Box2dFactory.h"
#include "CommonMacros.h"
#include "GameController.h"
#include "UserInfo.h"

USING_NS_CC;
USING_NS_CC_EXT;

BossView::BossView(int type)
: m_type(type)
, m_body(NULL)
, m_posIndex(0)
{

}

BossView *BossView::create(int type)
{
	BossView *view = new BossView(type);
	view->init();
	view->autorelease();
	return view;
}

bool BossView::init()
{
	if (!CCNode::init())
	{
		return false;
	}

	setTag(kTag_Boss);
	int targetLevel = GameController::getInstance()->getTargetLevel();
	int bloodCount = GameController::getInstance()->getBossBloodCount();
	k_bloodRecord = targetLevel;
	m_bloodCount = bloodCount;

	m_animPath = "animation/boss/diren001_bossyouling.ExportJson";
	CCPoint anchor = ccp(0.5f, 0.5f);
	switch (m_type)
	{
	case kBoss_Ghost:
		m_animPath = "animation/boss/diren001_bossyouling.ExportJson";
		anchor = ccp(0.5f, 0.7f);
		break;
	case kBoss_Spider:
		m_animPath = "animation/boss/diren002_bosszhizhu.ExportJson";
		anchor = ccp(0.5f, 0.5f);
		break;
	case kBoss_Moth:
		m_animPath = "animation/boss/diren003_bossfeier.ExportJson";
		anchor = ccp(0.5f, 0.5f);
		break;

	}
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(m_animPath.c_str());
	int pos1 = m_animPath.rfind("/");
	int pos2 = m_animPath.rfind(".");
	std::string armatureName = m_animPath.substr(pos1 + 1, pos2 - pos1 - 1);
	m_armature = CCArmature::create(armatureName.c_str());
	m_armature->getAnimation()->playByIndex(0);
	m_armature->setAnchorPoint(anchor);
	addChild(m_armature);

	setContentSize(ccp(m_armature->getContentSize().width, m_armature->getContentSize().height * 0.7f));
	setBody();

	CCSprite *progress_bg = CCSprite::create("animation/boss/boss_bai_bg.png");
	progress_bg->setPosition(ccp(m_armature->getPositionX(), m_armature->getPositionY() + m_armature->getContentSize().height / 2));
	addChild(progress_bg);

	m_bloodProgress = CCProgressTimer::create(CCSprite::create("animation/boss/boss_bar_progress.png"));
	m_bloodProgress->setType(kCCProgressTimerTypeBar);
	m_bloodProgress->setMidpoint(ccp(0, 1));
	m_bloodProgress->setBarChangeRate(ccp(1, 0));
	float curProgress = m_bloodCount * 100.0f / k_bloodRecord;
	m_bloodProgress->setPercentage(curProgress);
	m_bloodProgress->setPosition(ccp(progress_bg->getContentSize().width / 2, progress_bg->getContentSize().height / 2));
	progress_bg->addChild(m_bloodProgress);

	return true;
}

void BossView::doSleepAction()
{
	m_armature->getAnimation()->play("daiji");
}

void BossView::startMoveAction()
{
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	m_armature->getAnimation()->play("yidong");
	bool isGamePause = GameController::getInstance()->isGamePause();

	auto actions = ActionSequence::create(this);
	if (!isGamePause)
	{
		int kMovePosCount = 3;
		m_posIndex = (m_posIndex + 1) % kMovePosCount;
		CCPoint targetPos = getPosition();
		switch (m_posIndex)
		{
		case 0:
			targetPos = ccp(winSize.width / 2, targetPos.y + m_armature->getContentSize().height);
			break;
		case 1:
			targetPos = ccp(m_armature->getContentSize().width*0.6f, targetPos.y - m_armature->getContentSize().height);
			break;
		case 2:
			targetPos = ccp(winSize.width - m_armature->getContentSize().width*0.6f, targetPos.y);
			break;
		}
		auto moveTo = CCMoveTo::create(1.0f, targetPos);
		actions->addAction(moveTo);
		CCFunctionAction *doSleep = CCFunctionAction::create([=](){
			doSleepAction();
			if (getPositionX() < winSize.width / 2)
			{
				m_armature->setScaleX(-1);
			}
			else
			{
				m_armature->setScaleX(1);
			}
		});
		actions->addAction(doSleep);
		auto delay1 = CCDelayTime::create(0.5f);
		actions->addAction(delay1);
		CCFunctionAction *doAttact = CCFunctionAction::create([=](){
			doAttactAction();
		});
		actions->addAction(doAttact);
		auto delay2 = CCDelayTime::create(0.5f);
		actions->addAction(delay2);
		CCFunctionAction *doEffect = CCFunctionAction::create([=](){
			bossAttactEffect();
		});
		actions->addAction(doEffect);
		auto delay3 = CCDelayTime::create(1.5f);
		actions->addAction(delay3);
	}

	CCFunctionAction *doMove = CCFunctionAction::create([=](){
		startMoveAction();
	});
	actions->addAction(doMove);
	actions->runActions();

}

void BossView::doAttactAction()
{
	m_armature->getAnimation()->play("attack");
}

void BossView::setBody()
{
	m_body = Box2dFactory::getInstance()->createSquare(this);
}

void BossView::setPosition(const cocos2d::CCPoint &position)
{
	CCNode::setPosition(position);
	if (m_body)
	{
		b2Vec2 post;
		post = b2Vec2((float)((getPosition().x) / PTM_RATIO), (float)(((getPosition().y)) / PTM_RATIO));
		float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
		m_body->SetTransform(post, angle);
	}
}

void BossView::bossAttactEffect()
{
	GameController::getInstance()->bossAttactEffect(m_type);
}

void BossView::addBlood(int count)
{ 
	m_bloodCount += count; 
	if (m_bloodCount == 0)
	{
		runDieEffect();
	}
	float progress = m_bloodCount * 100.0f / k_bloodRecord;
	m_bloodProgress->runAction(CCProgressFromTo::create(0.2f, m_bloodProgress->getPercentage(), progress));
	GameController::getInstance()->setBossBloodCount(m_bloodCount);
}

void BossView::runDieEffect()
{
	stopAllActions();
	auto actions = ActionSequence::create(this);
	auto blink = CCBlink::create(1.0f, 10);
	actions->addAction(blink);
	CCFunctionAction *callback = CCFunctionAction::create([=](){
		CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(m_animPath.c_str());
		Box2dFactory::getInstance()->removeBody(m_body);
		removeFromParent();
	});
	actions->addAction(callback);
	actions->runActions();
}