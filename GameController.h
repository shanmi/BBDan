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
	virtual void addCrystalEffect(){}
	virtual void useShotGunsEffect(){}
	virtual void showAddCoinEffect(SquareNode *node){}
	virtual void showAddMarbleEffect(SquareNode *node){}
	virtual void updateMarbleType(int type){}
	virtual void updateProgress(){}
	virtual void addBoss(){}
	virtual void bossAttactEffect(int type){}
	virtual void rebuildCrystal(){}
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
	void addCrystalEffect() { NOTIFY_VIEWS(addCrystalEffect); }
	void useShotGunsEffect() { NOTIFY_VIEWS(useShotGunsEffect); }
	void showAddCoinEffect(SquareNode *node){ NOTIFY_VIEWS(showAddCoinEffect, node); }
	void showAddMarbleEffect(SquareNode *node){ NOTIFY_VIEWS(showAddMarbleEffect, node); }
	void updateMarbleType(int type){ NOTIFY_VIEWS(updateMarbleType, type); }
	void updateProgress(){ NOTIFY_VIEWS(updateProgress); }

	void addBoss(){ NOTIFY_VIEWS(addBoss); }
	void bossAttactEffect(int type){ NOTIFY_VIEWS(bossAttactEffect, type); }
	void rebuildCrystal(){ NOTIFY_VIEWS(rebuildCrystal) }

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

	int getGameType(){ return m_gameType; }
	void setGameType(int type){ m_gameType = type; }

	bool isGamePause(){ return m_bIsGamePause; }
	void setGamePause(bool isPause) { m_bIsGamePause = isPause; }

	int getBossType(){ return m_bossType; }
	void setBossType(int type){ m_bossType = type; }

	int getBossBloodCount(){ return m_bossBloodCount; }
	void setBossBloodCount(int count){ m_bossBloodCount = count; }

	std::vector<int> getCrystalBloods(){ return m_crystalBloods; }
	int getCrystalBlood(int index);
	void setCrystalBlood(int index, int bloodCount);
	void resetCrystalBloods(){ m_crystalBloods = { 2, 3, 2 }; rebuildCrystal(); }

	bool isCanMove(){ return m_bCanMove; }
	void setCanMove(bool canMove) { m_bCanMove = canMove; }

	void setTargetLevel(int level);
	int getTargetLevel();
	void resetTargetLevel();

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
	int m_gameType;
	bool m_bIsGamePause;
	int m_bossType;
	int m_bossBloodCount;
	std::vector<int> m_crystalBloods;
	bool m_bCanMove;
	int m_targetLevel;
};

#endif