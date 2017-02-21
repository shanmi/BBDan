#ifndef __SOUND_MGR_H__
#define __SOUND_MGR_H__

#define Background_Music	"sounds/music.ogg"
#define Effect_Pop			"sounds/pop.ogg"	
#define Effect_Pop1			"sounds/pop1.ogg"
#define Effect_Pop2			"sounds/pop2.ogg"
#define Effect_Pop3			"sounds/pop3.ogg"
#define Effect_Lazer		"sounds/lazer.ogg"
#define Effect_Coin			"sounds/coin.ogg"
#define Effect_GameOver		"sounds/gameover.ogg"

class SoundMgr
{
public:
	static SoundMgr *theMgr();
	void init();

	void playBackground();
	void playEffect(const char *effect);
	void setMute(bool isMute);
	bool isMute();

private:
	SoundMgr() :m_bIsMute(true){}
	~SoundMgr(){}

private:
	bool m_bIsMute;
};
#endif