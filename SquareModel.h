#ifndef _SQUARE_MODEL_H_
#define _SQUARE_MODEL_H_

#include "SquareNode.h"

class SquareModel
{
public:
	static SquareModel *theModel();
	~SquareModel(){}

public:
	SquareNode *createSquareNode(int type);
	void removeSquareNode(SquareNode *node);
	std::vector<SquareNode*> createSquareList();
	std::vector<int> getBallListIndex();
	std::vector<int> getBallListType();

	std::vector<SquareNode*> getSquares(){ return m_squares; }

	void squareMoveDown();

private:
	SquareModel(){ m_curScore = 1; }

private:
	int m_curScore;
	std::vector<SquareNode*> m_squares;
};
#endif