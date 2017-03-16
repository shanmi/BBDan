#ifndef __DATA_HELPER_H__
#define __DATA_HELPER_H__

#include "cocos2d.h"
class DataHelper
{
public:
	static DataHelper *getInstance();

	void putInt(int src, FILE* file);
	void getInt(int &temp, FILE* file);
	void putFloat(float src, FILE* file);
	void getFloat(float &temp, FILE* file);
	void putString(std::string str, FILE* file);
	void getString(std::string &str, FILE* file);
	int xorEncDecInt(int ch);
	int saveGameInfo();
	int loadGameInfo();
	int clearGameInfo();

	int saveShootGameInfo();
	int loadShootGameInfo();
	int clearShootGameInfo();
};
#endif