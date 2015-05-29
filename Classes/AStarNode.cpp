#include "AStarNode.h"


AStarNode::AStarNode(void)
{
}


AStarNode::~AStarNode(void)
{
}

void AStarNode::setPos(int xx, int yy)
{
	x = xx;
	y = yy;
}

void AStarNode::setF(float ff)
{
	f = ff;
}

void AStarNode::setG(float gg)
{
	g = gg;
}

void AStarNode::setH(float hh)
{
	h = hh;
}

bool AStarNode::operator < (const AStarNode& n)const
{
	if(x<n.x)
		return true;
	else if(x == n.x)
		return y<n.y;
	//else if(left.x == right.x&&left.y==right.y&&left.px<right.px)
	//	return true;
	//else if(left.x == right.x&&left.y==right.y&&left.px==right.px&&left.py<right.py)
	//	return true;
	else return false;
}

bool AStarNode::operator == (const AStarNode& n)const
{
	return (x==n.x&&y==n.y);
}

void AStarNode::setPPos(int xx, int yy)
{
	px = xx;
	py = yy;
}