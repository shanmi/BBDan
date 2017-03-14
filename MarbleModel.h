#ifndef __MARBLE_MODEL_H__
#define __MARBLE_MODEL_H__

#include "cocos2d.h"
#include "MarbleNode.h"
class MarbleModel
{
public:
	static MarbleModel *theModel();
	~MarbleModel(){}

public:
	MarbleNode* createMarble();
	void removeMarble(MarbleNode *node);
	void setMarbleAttr(MarbleAttr attr);
	MarbleAttr getMarbleAttr(){ return m_attr; }
	MarbleAttr getMarbleAttrByType(int type);
	std::vector<MarbleNode *> getMarbles(){ return m_marbles; }
	bool haveMarbleMoving();

	void addMarblesCount();
	void setMarblesCount(int count){ m_marblesCount = count; }
	int getMarblesCount(){ return m_marblesCount; }

	int getAttactRate(){ return m_attactRate; }
	void setAttactRate(int rate){ m_attactRate = rate; }

	int checkMarblesCount();

	bool isMarblesNerverStop();
	void reboundMarbles();
	void clearMarbles();

	void updateMarbles(cocos2d::CCRect invisibleRect, int addMarbleCount);
	void updateMarbles();
private:
	MarbleModel();

private:
	MarbleAttr m_attr;
	std::vector<MarbleNode *> m_marbles;
	int m_marblesCount;
	int m_attactRate;
	int m_tempAddCount;
};
#endif