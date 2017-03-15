#include "DataHelper.h"
#include "GameConfig.h"
#include "GameController.h"
#include "MarbleModel.h"
#include "SquareModel.h"

USING_NS_CC;

DataHelper *DataHelper::getInstance()
{
	static DataHelper helper;
	return &helper;
}

void DataHelper::putInt(int src, FILE* file){
	int temp = src;
	fwrite((char *)(&temp), sizeof(temp), 1, file);
}

void DataHelper::putString(std::string str, FILE* file)
{
	fwrite(str.c_str(), str.size(), 1, file);
}

void DataHelper::getInt(int &temp, FILE* file){
	fread((char *)(&temp), sizeof(temp), 1, file);
}

void DataHelper::getString(std::string &str, FILE* file){
	fread(&str, str.size(), 1, file);
}

int DataHelper::xorEncDecInt(int ch){
	return ch ^ 0x8B7E;
}

int DataHelper::loadGameInfo(){
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + GAME_DATA;
	FILE *file = fopen(path.c_str(), "r");
	if (file) {
		int temp = -1;
		getInt(temp, file);
		if (xorEncDecInt(temp) != GAME_DATA_VERSION){
			return ERROR;
		}
		getInt(temp, file);
		int posX = xorEncDecInt(temp);
		getInt(temp, file);
		int posY = xorEncDecInt(temp);
		GameController::getInstance()->setTargetPos(ccp(posX, posY));
		getInt(temp, file);
		int marbleCount = xorEncDecInt(temp);
		MarbleModel::theModel()->setMarblesCount(marbleCount);
		getInt(temp, file);
		int attactRate = xorEncDecInt(temp);
		MarbleModel::theModel()->setAttactRate(attactRate);
		getInt(temp, file);
		int curScore = xorEncDecInt(temp);
		SquareModel::theModel()->setCurrentScore(curScore);
		getInt(temp, file);
		int size = xorEncDecInt(temp);
		for (int i = 0; i < size; i++){
			getInt(temp, file);
			int x = xorEncDecInt(temp);
			getInt(temp, file);
			int y = xorEncDecInt(temp);
			getInt(temp, file);
			int type = xorEncDecInt(temp);
			getInt(temp, file);
			int shap = xorEncDecInt(temp);
			getInt(temp, file);
			int score = xorEncDecInt(temp);

			SquareNode *node = SquareModel::theModel()->createSquareNode(type, shap);
			if (node != nullptr)
			{
				node->setBody();
				node->setIndex(x, y);
				node->setScore(score);
			}
		}
		fclose(file);
		return OK;
	}
	else{
		CCLOG("load file error.");
	}
	return ERROR;
}

int DataHelper::saveGameInfo(){
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + GAME_DATA;
	FILE *file = fopen(path.c_str(), "wb+");
	if (file) {
		putInt(xorEncDecInt(GAME_DATA_VERSION), file);
		auto targetPos = GameController::getInstance()->getTargetPos();
		putInt(xorEncDecInt(targetPos.x), file);
		putInt(xorEncDecInt(targetPos.y), file);
		auto marbles = MarbleModel::theModel()->getMarbles();
		putInt(xorEncDecInt(marbles.size()), file);
		auto attactRate = MarbleModel::theModel()->getAttactRate();
		putInt(xorEncDecInt(attactRate), file);
		int curScore = SquareModel::theModel()->getCurrentScore();
		putInt(xorEncDecInt(curScore), file);
		auto squares = SquareModel::theModel()->getSquares();
		putInt(xorEncDecInt(squares.size()), file);
		for (auto iter = squares.begin(); iter != squares.end(); iter++)
		{
			auto square = *iter;
			Index index = square->getIndex();
			int type = square->getSquareType();
			int shap = square->getTag(); //for triangle is 0~3, other is -1
			int score = square->getScore();
			putInt(xorEncDecInt(index.x), file);
			putInt(xorEncDecInt(index.y), file);
			putInt(xorEncDecInt(type), file);
			putInt(xorEncDecInt(shap), file);
			putInt(xorEncDecInt(score), file);
		}
		//putString("1234567", file);
		fclose(file);
		return OK;
	}
	else{
		CCLOG("save file error.");
	}
	return ERROR;
}

int DataHelper::clearGameInfo()
{
	MarbleModel::theModel()->setMarblesCount(0);
	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + GAME_DATA;
	return remove(path.c_str());
}

int DataHelper::loadShootGameInfo(){
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + GAME_SHOOT_DATA;
	FILE *file = fopen(path.c_str(), "r");
	if (file) {
		int temp = -1;
		getInt(temp, file);
		if (xorEncDecInt(temp) != GAME_DATA_VERSION){
			return ERROR;
		}
		getInt(temp, file);
		int posX = xorEncDecInt(temp);
		getInt(temp, file);
		int posY = xorEncDecInt(temp);
		GameController::getInstance()->setShooterPos(ccp(posX, posY));
		getInt(temp, file);
		int curScore = xorEncDecInt(temp);
		SquareModel::theModel()->setCurrentScore(curScore);
		getInt(temp, file);
		int size = xorEncDecInt(temp);
		for (int i = 0; i < size; i++){
			getInt(temp, file);
			int x = xorEncDecInt(temp);
			getInt(temp, file);
			int y = xorEncDecInt(temp);
			getInt(temp, file);
			int posx = xorEncDecInt(temp);
			getInt(temp, file);
			int posy = xorEncDecInt(temp);
			getInt(temp, file);
			int type = xorEncDecInt(temp);
			getInt(temp, file);
			int shap = xorEncDecInt(temp);
			getInt(temp, file);
			int score = xorEncDecInt(temp);
			getInt(temp, file);
			int speed = xorEncDecInt(temp);

			SquareNode *node = SquareModel::theModel()->createSquareNode(type, shap);
			if (node != nullptr)
			{
				node->setBody();
				node->setIndex(x, y);
				node->setPosition(ccp(posx, posy));
				node->setScore(score);
				node->setSpeed(speed);
			}
		}
		getInt(temp, file);
		int bossBloodCount = xorEncDecInt(temp);
		GameController::getInstance()->setBossBloodCount(bossBloodCount);
		fclose(file);
		return OK;
	}
	else{
		CCLOG("load file error.");
	}
	return ERROR;
}

int DataHelper::saveShootGameInfo(){
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + GAME_SHOOT_DATA;
	FILE *file = fopen(path.c_str(), "wb+");
	if (file) {
		putInt(xorEncDecInt(GAME_DATA_VERSION), file);
		auto shooterPos = GameController::getInstance()->getShooterPos();
		putInt(xorEncDecInt(shooterPos.x), file);
		putInt(xorEncDecInt(shooterPos.y), file);
		int curScore = SquareModel::theModel()->getCurrentScore();
		putInt(xorEncDecInt(curScore), file);
		auto squares = SquareModel::theModel()->getSquares();
		putInt(xorEncDecInt(squares.size()), file);
		for (auto iter = squares.begin(); iter != squares.end(); iter++)
		{
			auto square = *iter;
			CCPoint pos = square->getPosition();
			int type = square->getSquareType();
			int shap = square->getTag(); //for triangle is 0~3, other is -1
			int score = square->getScore();
			Index index = square->getIndex();
			int speed = square->getSpeed();
			putInt(xorEncDecInt(index.x), file);
			putInt(xorEncDecInt(index.y), file);
			putInt(xorEncDecInt(pos.x), file);
			putInt(xorEncDecInt(pos.y), file);
			putInt(xorEncDecInt(type), file);
			putInt(xorEncDecInt(shap), file);
			putInt(xorEncDecInt(score), file);
			putInt(xorEncDecInt(speed), file);
		}
		int bossBloodCount = GameController::getInstance()->getBossBloodCount();
		putInt(xorEncDecInt(bossBloodCount), file);
		fclose(file);
		return OK;
	}
	else{
		CCLOG("save file error.");
	}
	return ERROR;
}

int DataHelper::clearShootGameInfo()
{
	MarbleModel::theModel()->setMarblesCount(0);
	string path = CCFileUtils::sharedFileUtils()->getWritablePath() + GAME_SHOOT_DATA;
	return remove(path.c_str());
}
