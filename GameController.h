#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "CommonMacros.h"
#include "Config.h"

class SquareNode;
class MarbleNode;
struct INotifyView
{
	virtual void oneRoundEnd(){}
	virtual void updateMarbles(){}
	virtual void notifyViews(){}
	virtual void showGameOver(){}
	virtual void addSquareNode(SquareNode *node){}
	virtual void useProtectEffect(){}
	virtual void useShotGunsEffect(){}
	virtual void getBloodEffect(){}
	virtual void showAddCoinEffect(SquareNode *node){}
	virtual void showAddMarbleEffect(SquareNode *node){}
	virtual void updateMarbleType(int type){}
	virtual void updateProgress(){}
	virtual void addBoss(){}
	virtual void bossAttactEffect(int type){}
};

class GameController
{
public:
	static GameController *getInstance();
	~GameController(){}

public:
	void addView(INotifyView *view);
	void removeView(INotifyView *view);
	void oneRoundEnd(){ NOTIFY_VIEWS(oneRoundEnd); }
	void showGameOver() { NOTIFY_VIEWS(showGameOver); }
	void updateMarbles() { NOTIFY_VIEWS(updateMarbles); }
	void notifyViews() { NOTIFY_VIEWS(notifyViews); }
	void addSquareNode(SquareNode *node){ NOTIFY_VIEWS(addSquareNode, node); }
	void useProtectEffect() { NOTIFY_VIEWS(useProtectEffect); }
	void useShotGunsEffect() { NOTIFY_VIEWS(useShotGunsEffect); }
	void getBloodEffect(){ addBloodCount(1); NOTIFY_VIEWS(getBloodEffect); }
	void showAddCoinEffect(SquareNode *node){ NOTIFY_VIEWS(showAddCoinEffect, node); }
	void showAddMarbleEffect(SquareNode *node){ NOTIFY_VIEWS(showAddMarbleEffect, node); }
	void updateMarbleType(int type){ NOTIFY_VIEWS(updateMarbleType, type); }
	void updateProgress(){ NOTIFY_VIEWS(updateProgress); }

	void addBoss(){ NOTIFY_VIEWS(addBoss); }
	void bossAttactEffect(int type){ NOTIFY_VIEWS(bossAttactEffect, type); }

public:
	void startOneRound();

	void addCounter(){ ++m_counter; }
	void resetCounter(){ m_counter = 0; }
	int getCounter(){ return m_counter; }
	void setTargetPos(cocos2d::CCPoint pos){ m_targetPos = pos; }
	cocos2d::CCPoint getTargetPos(){ return m_targetPos; }

	void setShooterPos(cocos2d::CCPoint pos){ m_shooterPos = pos; }
	cocos2d::CCPoint getShooterPos(){ return m_shooterPos; }

	bool isCounterFull();
	bool isRoundOver(){ return m_bIsRoundOver; }
	void setRoundState(bool isOver) { m_bIsRoundOver = isOver; }
	void checkSquares(bool isRoundEnd = false);
	void updateMarblePos();
	bool isGameOver();
	bool checkGameOver();

	void setDoubleAttact(){ m_attactRate = ATTACT_RATE * 2; }
	void resetAttactRate(){ m_attactRate = ATTACT_RATE; }
	int getAttactRate();

	void createPropByMarble(MarbleNode *marble);

	void backToMainMenu();

	bool isFirshInGame(){ return m_bISFirstIn; }
	void setFirstInGame(bool firtIn){ m_bISFirstIn = firtIn; }

	int getBloodCount(){ return m_bloodCount; }
	void addBloodCount(int count)
	{ 
		m_bloodCount += count; 
		if (m_bloodCount >= PLAYER_BLOOD_COUNT)
		{
			m_bloodCount = PLAYER_BLOOD_COUNT;
		}
	}

	int getGameType(){ return m_gameType; }
	void setGameType(int type){ m_gameType = type; }

	bool isGamePause(){ return m_bIsGamePause; }
	void setGamePause(bool isPause) { m_bIsGamePause = isPause; }

	int getBossBloodCount(){ return m_bossBloodCount; }
	void setBossBloodCount(int count){ m_bossBloodCount = count; }

private:
	GameController();

private:
	cocos2d::CCPoint m_targetPos;
	cocos2d::CCPoint m_shooterPos;
	std::vector<INotifyView*> m_views;
	int m_counter;
	int m_attactRate;
	bool m_bIsRoundOver;
	bool m_bISFirstIn;
	int m_bloodCount;
	int m_gameType;
	bool m_bIsGamePause;
	int m_bossBloodCount;
};

#endif