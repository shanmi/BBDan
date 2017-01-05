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
	void setMarbleAttr(MarbleAttr attr){ m_attr = attr; }
	std::vector<MarbleNode *> getMarbles(){ return m_marbles; }
	bool haveMarbleMoving();

private:
	MarbleModel();

private:
	MarbleAttr m_attr;
	std::vector<MarbleNode *> m_marbles;
};
#endif