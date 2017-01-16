#ifndef __USER_INFO_H__
#define __USER_INFO_H__

#define GAME_MUTE			"game-mute"
#define GAME_COINS_COUNT	"game-coins-count"

class UserInfo
{
public:
	static UserInfo *getInstance();
	~UserInfo(){}

public:
	void addCoins(int count);
	int getCoins();

	void setMute(bool isMute);
	bool isMute();
private:
	UserInfo(){}
};
#endif