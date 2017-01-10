#ifndef __COMMONMACROS_H__
#define __COMMONMACROS_H__


enum TouchPriority
{
	kPanelTouchPriority = -10,
};

enum SquareType
{
	kType_Empty = 20,
	kType_Square = 35,
	kType_Triangle = 19,
	kType_AddMarble = 12,
	kType_AddCoin = 5,
	kType_Rebound = 4,
	kType_EliminateRow = 2,
	kType_EliminateCol = 3,
};

enum CollisionType
{
	kCollision_Square,
	kCollision_Triangle,
	kCollision_AddMarble,
	kCollision_AddCoin,
	kCollision_Rebound,
	kCollision_EliminateRow,
	kCollision_EliminateCol,
};

enum MarbleType
{
	kMarble_Normal,
	kMarble_Faster,
	kMarble_Biger,
	kMarble_Dispersed,
	kMarble_Across,
	kMarble_Bomb,
};
enum ViewTag
{
	kTag_Wall = 100,
	kTag_Marble = 200,
};

#define NOTIFY_VIEWS(_FUNC_ ,...)						\
for (size_t index = 0; index < m_views.size(); ++index) \
{														\
	m_views[index]->_FUNC_(__VA_ARGS__);				\
}

#endif