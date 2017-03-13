#ifndef __USER_INFO_H__
#define __USER_INFO_H__

#define GAME_MUTE			"game-mute"
#define GAME_COINS_COUNT	"game-coins-count"
#define GAME_BEST_SCORE		"game-best-score"
#define GAME_UNLOCK_MARBLE  "game-unlock-marble"
#define GAME_FISH_GUIDE		"game-fish-guide"
#define GAME_PROP_COUNT     "game-prop-count"
#define GAME_MARBLE_TYPE    "game-marble-type"
#define GAME_LUCKY_LEVEL    "game-lucky-level"
#define GAME_TARGET_LEVEL   "game-target-level"

class UserInfo
{
public:
	static UserInfo *getInstance();
	~UserInfo(){}

public:
	void setMute(bool isMute);
	bool isMute();

	void addCoins(int count);
	int getCoins();

	void setBestScore(int score);
	int getBestScore();

	void setCurMarbleType(int type);
	int getCurMarbleType();

	void unlockMarble(int type);
	void unlockAllMarble();
	bool isUnlock(int type);

	void setFishGuide(bool finish);
	bool isFishGuideFinish();

	void addPropsCount(int type, int count);
	int getPropsCount(int type);

	bool hasGetLuckyLevel(int level);
	void setLuckyLevel(int level);
	void resetLuckyLevel();

	void setTargetLevel();
	int getTargetLevel();
	void resetTargetLevel();

private:
	UserInfo(){}
};
#endif