#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

class GameController
{
public:
	static GameController *getInstance();
	~GameController(){}

public:
	void startOneRound();
	void endOneRound();

	bool isRoundOver() { return m_bIsRoundOver; }

private:
	GameController();

private:
	bool m_bIsRoundOver;
};

#endif