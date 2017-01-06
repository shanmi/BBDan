#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#define GAME_COINS_COUNT "game-coins-count"

class GameData
{
public:
	static GameData *getInstance();
	~GameData(){}

public:
	void addCoins(int count);
	int getCoins();

private:
	GameData(){}
};
#endif