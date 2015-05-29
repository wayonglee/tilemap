#include"cocos2d.h"

USING_NS_CC;

class AStarNode
{
public:
	int x;
	int y;
	float f;
	float g;
	float h;
	int px;
	int py;

	AStarNode(void);
	~AStarNode(void);
	void setPos(int,int);
	void setF(float);
	void setG(float);
	void setH(float);
	//CREATE_FUNC(AStarNode);
	bool operator < (const AStarNode& n) const;
	bool operator == (const AStarNode& n) const;
	void setPPos(int,int);
};