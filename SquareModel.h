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
	std::vector<SquareNode*> loadSquareList(bool autoAddScore = true);
	std::vector<SquareNode*> createSquareList(bool autoAddScore = true);
	std::vector<int> getBallListType();
	int getBallType();
	int getCurrentScore(){ return m_curScore; }
	void setCurrentScore(int score){ m_curScore = score; }

	std::vector<SquareNode*> getSquares(){ return m_squares; }
	void removeAllSquares();
	void setSquareFreezing(bool isFreezing);
	bool isFreezing(){ return m_bIsFreezing; }

	void squareMoveDown();
	void removeBelowSquares();
	void elimateSameRowSquare(SquareNode *node);
	void elimateSameColSquare(SquareNode *node);

	void clearSquares();

private:
	SquareModel();

private:
	int m_curScore;
	std::vector<SquareNode*> m_squares;
	bool m_bIsFreezing;
};
#endif