#include "GameUtil.h"
#include "GameConfig.h"
#include "SquareModel.h"
#include "MarbleModel.h"
#include "GameController.h"
#include "UserInfo.h"

USING_NS_CC;

float GameUtil::getDegreeTwoPoints(CCPoint p1, CCPoint p2)
{
	float delta_x = (p2.x - p1.x);
	float delta_y = (p2.y - p1.y);
	int degree = 0;
	float fRadian = 0.f;
	float const_pi = 180.0f / (float)M_PI;
	if (delta_y >= 0)
	{
		if (delta_x > 0)
		{
			fRadian = atan((float)(delta_y / delta_x));
			degree = const_pi * fRadian;
		}
		else if (delta_x < 0)
		{
			fRadian = atan((float)(delta_y / delta_x)) * (-1);
			degree = const_pi * (M_PI - fRadian);
		}
		else if (delta_x == 0)
		{
			degree = 90;
		}
	}
	else if (delta_y < 0)
	{
		if (delta_x > 0)
		{
			fRadian = atan((float)(delta_y / delta_x)) * (-1);
			degree = const_pi * (2 * M_PI - fRadian);
		}
		else if (delta_x < 0)
		{
			fRadian = atan((float)(delta_y / delta_x));
			degree = const_pi * (M_PI + fRadian);
		}
		else if (delta_x == 0)
		{
			degree = 270;
		}
	}
	return degree;
}

float GameUtil::getRadian(float fDegree)
{
	return M_PI * fDegree / 180.0f;
}


float GameUtil::getDegree(float fRadian)
{
	return 180.0f * fRadian / M_PI;
}

std::string GameUtil::intToString(int value)
{
	char temp[50] = { 0 };
	sprintf(temp, "%d", value);
	return temp;
}

CCParticleExplosion *GameUtil::getExplodeEffect(){
	CCParticleExplosion *pEmitter = CCParticleExplosion::create();
	pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/explode.png"));
	pEmitter->setAutoRemoveOnFinish(true);

	ccColor4F c4Start = { 0.3, 0.4, 0.5, 0.6 };
	ccColor4F c4End = { 1, 1, 1, 1 };
	pEmitter->setStartColor(c4Start);
	pEmitter->setEndColor(c4End);
	pEmitter->setStartColorVar(c4Start);
	pEmitter->setEndColorVar(c4End);

	pEmitter->setTotalParticles(50);

	pEmitter->setSpeed(80);
	pEmitter->setSpeedVar(20);

	pEmitter->setLife(2);
	pEmitter->setLifeVar(0.3f);
	pEmitter->setScale(0.5f);

	return pEmitter;
}

CCParticleExplosion *GameUtil::getExplodeEffect(std::string image)
{
	CCParticleExplosion *pEmitter = CCParticleExplosion::create();
	pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(image.c_str()));
	pEmitter->setAutoRemoveOnFinish(true);

	ccColor4F c4Var = { 0, 0, 0, 0.0 };
	ccColor4F c4 = { 1, 1, 1, 1 };
	pEmitter->setStartColor(c4);
	pEmitter->setEndColor(c4);
	pEmitter->setStartColorVar(c4Var);
	pEmitter->setEndColorVar(c4Var);

	pEmitter->setTotalParticles(10);

	pEmitter->setRadialAccel(1);
	pEmitter->setRadialAccelVar(5);

	pEmitter->setSpeed(200);
	pEmitter->setScale(1.2f);
	pEmitter->setGravity(ccp(0, -200));

	return pEmitter;
}


CCParticleExplosion *GameUtil::getRandomExplodeEffect()
{
	int random = rand() % 8;
	std::string image = "squares/fangkuai_hong1.png";
	switch (random)
	{
	case 0:
		image = "squares/fangkuai_hong1.png";
		break;
	case 1:
		image = "squares/fangkuai_huang1.png";
		break;
	case 2:
		image = "squares/fangkuai_lan1.png";
		break;
	case 3:
		image = "squares/fangkuai_lv1.png";
		break;
	case 4:
		image = "squares/fangkuai_qing1.png";
		break;
	case 5:
		image = "squares/fangkuai_shitou1.png";
		break;
	case 6:
		image = "squares/fangkuai_tuhuang1.png";
		break;
	case 7:
		image = "squares/fangkuai_zhi1.png";
		break;
	}
	auto emitter = getExplodeEffect(image);

	return emitter;
}

cocos2d::CCParticleSystemQuad *GameUtil::getBombEffect()
{
	auto emitter = CCParticleSystemQuad::create("particle/explore.plist");
	emitter->setAutoRemoveOnFinish(true);
	std::string filename = "particle/explore.plist";
	//emitter->initWithFile(filename.c_str());
	return emitter;
}

cocos2d::CCParticleSystemQuad *GameUtil::getBombEffect2()
{
	CCParticleExplosion *pEmitter = CCParticleExplosion::create();
	pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fire.png"));
	pEmitter->setAutoRemoveOnFinish(true);

	ccColor4F c4Start = { 1.0, 0.8, 0.1, 1.0 };
	ccColor4F c4End = { 1.0, 0.8, 0.1, 1.0 };
	pEmitter->setStartColor(c4Start);
	pEmitter->setEndColor(c4End);
	/*pEmitter->setStartColorVar(c4Start);
	pEmitter->setEndColorVar(c4End);*/

	pEmitter->setTotalParticles(60);

	pEmitter->setSpeed(180);
	pEmitter->setSpeedVar(10);

	pEmitter->setLife(0.3f);
	pEmitter->setLifeVar(0.1f);
	pEmitter->setScale(0.6f);

	return pEmitter;
}


CCMotionStreak *GameUtil::getMotionStreak()
{
	auto streak = CCMotionStreak::create(0.1f, 3, 22, ccGREEN, "particle/streak.png");
	streak->setAnchorPoint(ccp(0.5f, 0.8f));
	CCActionInterval *colorAction = CCRepeatForever::create(CCSequence::create(
		CCTintTo::create(0.2f, 255, 0, 0),
		CCTintTo::create(0.2f, 0, 255, 0),
		CCTintTo::create(0.2f, 0, 0, 255),
		CCTintTo::create(0.2f, 0, 255, 255),
		CCTintTo::create(0.2f, 255, 255, 0),
		CCTintTo::create(0.2f, 255, 0, 255),
		CCTintTo::create(0.2f, 255, 255, 255),
		NULL));
	streak->runAction(colorAction);
	return streak;
}

std::string GameUtil::getBlockImage(int type, int score)
{
	char temp[50] = { 0 };
	std::string image;
	score = rand() % 500;
	if (score > 360)
	{
		image = "squares/fangkuai_shitou%d.png";
	}
	else if (score > 260)
	{
		image = "squares/fangkuai_huang%d.png";
	}
	else if (score > 200)
	{
		image = "squares/fangkuai_zhi%d.png";
	}
	else if (score > 150)
	{
		image = "squares/fangkuai_lv%d.png";
	}
	else if (score > 100)
	{
		image = "squares/fangkuai_tuhuang%d.png";
	}
	else if (score > 50)
	{
		image = "squares/fangkuai_hong%d.png";
	}
	else if (score > 10)
	{
		image = "squares/fangkuai_qing%d.png";
	}
	else
	{
		image = "squares/fangkuai_lan%d.png";
	}
	sprintf(temp, image.c_str(), type);
	return temp;
}

CCAction *GameUtil::getRepeatScaleAction()
{
	auto scaleby = CCScaleBy::create(0.6f, 1.1f);
	auto reverse = scaleby->reverse();
	auto sequence = CCSequence::create(scaleby, reverse, NULL);
	auto action = CCRepeatForever::create(sequence);
	return action;
}

CCAction *GameUtil::getOnceScaleAction()
{
	auto scaleby = CCScaleBy::create(0.5f, 1.2f);
	auto reverse = scaleby->reverse();
	auto sequence = CCSequence::create(scaleby, reverse, NULL);
	return sequence;
}

CCAction *GameUtil::getBlinkAction()
{
	auto blink = CCBlink::create(1.0f, 1);
	auto action = CCRepeatForever::create(blink);
	return action;
}

CCAction *GameUtil::getFadeInOutAction()
{
	auto fadeIn = CCFadeIn::create(1.2f);
	auto fadeOut = CCFadeOut::create(2.0f);
	auto sequence = CCSequence::create(fadeOut, fadeIn, NULL);
	auto action = CCRepeatForever::create(sequence);
	return action;
}

CCSprite *GameUtil::getAchievementEffect(int type)
{
	char temp[30] = { 0 };
	sprintf(temp, "combo/combo_%d.png", type);
	auto effect = CCSprite::create(temp);
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	effect->setPosition(ccp(-effect->getContentSize().width / 2, winSize.height *0.6f));
	auto move1 = CCMoveBy::create(0.5f, ccp(winSize.width / 2, 0));
	auto ease1 = CCEaseSineInOut::create(move1);
	auto delayMove = CCMoveBy::create(0.3f, ccp(effect->getContentSize().width / 2, 0));
	auto move2 = CCMoveBy::create(0.5f, ccp(winSize.width / 2 + effect->getContentSize().width, 0));;
	auto ease2 = CCEaseSineOut::create(move2);
	auto callback = CCCallFunc::create(effect, callfunc_selector(CCNode::removeFromParent));
	auto sequence = CCSequence::create(ease1, delayMove, ease2, callback, NULL);
	effect->runAction(sequence);
	return effect;
}

CCLabelAtlas *GameUtil::getImageNum(std::string fontPath, int num)
{
	int kLength = 11;
	std::string text = intToString(num);
	auto size = CCSprite::create(fontPath.c_str())->getContentSize();
	auto numLabel = CCLabelAtlas::create(text.c_str(), fontPath.c_str(), size.width / kLength, size.height, '0');
	numLabel->setAnchorPoint(ccp(0.5f, 0.5f));
	return numLabel;
}

CCLabelAtlas *GameUtil::getImageNum(std::string fontPath, std::string numStr)
{
	int kLength = 11;
	auto size = CCSprite::create(fontPath.c_str())->getContentSize();
	auto numLabel = CCLabelAtlas::create(numStr.c_str(), fontPath.c_str(), size.width / kLength, size.height, '0');
	numLabel->setAnchorPoint(ccp(0.5f, 0.5f));
	return numLabel;
}

//生成 [0 , length-1] 的随机序列
vector<int> GameUtil::buildRandomSequence(int length)
{
	vector<int> seq;
	for (int i = 0; i < length; ++i)
	{
		seq.push_back(i);
	}
	int index = 0;
	int tmp = 0;
	for (int i = length - 1; i > 0; i--) {
		index = (int(CCRANDOM_0_1() * 100)) % i;

		tmp = seq[i];
		seq[i] = seq[index];
		seq[index] = tmp;
	}

	return seq;
}

int GameUtil::getLuckyLevel()
{
	int curLevel = SquareModel::theModel()->getCurrentScore();
	int luckyLevelCount = GameConfig::getInstance()->m_luckyLevelCount;
	auto luckyLevel = GameConfig::getInstance()->m_luckyLevel;
	for (int i = 0; i < luckyLevelCount; i++)
	{
		int temp = luckyLevel[i];
		bool hasGetLuckyLevel = UserInfo::getInstance()->hasGetLuckyLevel(i);
		if (!hasGetLuckyLevel && temp >= curLevel)
		{
			if (temp == curLevel)
			{
				UserInfo::getInstance()->setLuckyLevel(i);
			}
			return temp;
		}
	}
	int targetLevel = 1;
	while (curLevel > targetLevel)
	{
		targetLevel += 50;
	}
	return targetLevel;
}

int GameUtil::getLastLuckyLevel()
{
	int curLevel = SquareModel::theModel()->getCurrentScore();
	int luckyLevelCount = GameConfig::getInstance()->m_luckyLevelCount;
	auto luckyLevel = GameConfig::getInstance()->m_luckyLevel;
	int lastLevel = 1;
	for (int i = 0; i < luckyLevelCount; i++)
	{
		lastLevel = luckyLevel[i];
		if (lastLevel > curLevel)
		{
			if (i >= 1)
			{
				return luckyLevel[i - 1];
			}
			else
			{
				return 1;
			}
		}
	}
	while (curLevel > lastLevel)
	{
		lastLevel += 50;
		if (curLevel > lastLevel)
		{
			lastLevel -= 50;
			break;
		}
	}
	return lastLevel;
}