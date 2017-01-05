#ifndef __COMMONMACROS_H__
#define __COMMONMACROS_H__


enum TouchPriority
{
	kPanelTouchPriority = -10,
};

enum SquareType
{
	kType_Empty = 20,
	kType_Square = 30,
	kType_Triangle = 14,
	kType_AddMarble = 10,
	kType_AddCoin = 6,
	kType_Rebound = 5,
	kType_EliminateRow = 7,
	kType_EliminateCol = 8,
};

#define NOTIFY_VIEWS(_FUNC_ ,...)						\
for (size_t index = 0; index < m_views.size(); ++index) \
{														\
	m_views[index]->_FUNC_(__VA_ARGS__);				\
}

#endif