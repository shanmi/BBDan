#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "CommonMacros.h"

struct INotifyView
{
	virtual void updateSquares(){}
	virtual void showGameOver(){}
};

class GameController
{
public:
	static GameController *getInstance();
	~GameController(){}

public:
	void addView(INotifyView *view);
	void removeView(INotifyView *view);
	void updateSquares(){ NOTIFY_VIEWS(updateSquares); }
	void showGameOver() { NOTIFY_VIEWS(showGameOver); }

public:
	void startOneRound();

	void addCounter();
	void resetCounter(){ m_counter = -1000000000000; }
	int getCounter(){ return m_counter; }
	void setTargetPos(cocos2d::CCPoint pos){ m_targetPos = pos; }
	cocos2d::CCPoint getTargetPos(){ return m_targetPos; }

	bool isCounterFull();
	bool isRoundOver(){ return m_bIsRoundOver; }
	void setRoundState(bool isOver) { m_bIsRoundOver = isOver; }
	bool checkGameOver();

private:
	GameController();

private:
	cocos2d::CCPoint m_targetPos;
	int m_counter;
	bool m_bIsRoundOver;
	std::vector<INotifyView*> m_views;
};

#endif