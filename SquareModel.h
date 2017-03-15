#ifndef _SQUARE_MODEL_H_
#define _SQUARE_MODEL_H_

#include "SquareNode.h"

class SquareModel
{
public:
	static SquareModel *theModel();
	~SquareModel(){}

public:
	SquareNode *createSquareNode(int type, int shap = -1);
	void removeSquareNode(SquareNode *node);
	std::vector<SquareNode*> loadSquareList();
	std::vector<SquareNode*> createSquareList();
	std::vector<int> getBallListType1();
	std::vector<int> getBallListType2();
	int getBallType(int probsArr[10][3], int size);
	int getCurrentScore(){ return m_curScore; }
	void setCurrentScore(int score){ m_curScore = score; }

	std::vector<SquareNode*> getSquares(){ return m_squares; }
	SquareNode *getSquareByIndex(Index index);
	void removeAllSquares();
	void setSquareFreezing(bool isFreezing);
	bool isFreezing(){ return m_bIsFreezing; }

	void squareMoveDown();
	void removeBelowSquares();
	void elimateSameRowSquare(SquareNode *node);
	void elimateSameColSquare(SquareNode *node);
	void elimateAroundSquare(SquareNode *node);

	void removeAroundSquare(SquareNode *node);
	void removeSameRowSquare(SquareNode *node);
	void removeSameColSquare(SquareNode *node);
	void exchangeSquarePosition();

	void clearSquares();
	std::vector<SquareNode*> getRemainSqaure();

	SquareNode *addDoubleScore();
	SquareNode *addDoubleSpeed();
	SquareNode *createRandomSquare();

private:
	SquareModel();

private:
	int m_curScore;
	std::vector<SquareNode*> m_squares;
	bool m_bIsFreezing;
};
#endif