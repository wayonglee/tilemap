#include"cocos2d.h"

USING_NS_CC;

class AStarNode
{
public:
	int x;
	int y;
	int f;
	int g;
	int h;
	int px;
	int py;

	AStarNode(void);
	~AStarNode(void);
	void setPos(int,int);
	void setF(int);
	void setG(int);
	void setH(int);
	//CREATE_FUNC(AStarNode);
	bool operator < (const AStarNode& n) const;
	bool operator == (const AStarNode& n) const;
	void setPPos(int,int);
};