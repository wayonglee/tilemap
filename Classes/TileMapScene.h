#include"cocos2d.h"
#include"AStarNode.h"

USING_NS_CC;

class TileMapScene:public Layer
{
private:
	Vec2 bgOrigin;
	Sprite* player;
	int touchTime;
	TMXTiledMap* tilemap;
	TMXObjectGroup *objects;
	TMXLayer* meta;
	TMXLayer* ground;
	std::vector<AStarNode> vec;
	int count;
public:
	TileMapScene();
	~TileMapScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TileMapScene);
	virtual void onTouchesBegan(const std::vector<Touch*>& touches,Event* pEvent);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches,Event* pEvent);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches,Event* pEvent);
	virtual bool onTouchBegan(Touch* ptouch,Event* pEvent);
	virtual void onTouchMoved(Touch* ptouch,Event* pEvent);
	virtual void onTouchEnded(Touch* ptouch,Event* pEvent);
	void countTouchTime(float);
	Point tileCoordForPosition(Point position);
	void hitWall(float);
	void Astar(Point,Point);
	void move(float);
};