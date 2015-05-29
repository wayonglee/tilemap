#include "TileMapScene.h"
#include<set>
#include<queue>


TileMapScene::TileMapScene()
{
}


TileMapScene::~TileMapScene()
{
}


Scene* TileMapScene::createScene()
{
	auto scene = Scene::create();

	auto layer = TileMapScene::create();

	scene->addChild(layer);

	return scene;
}

bool TileMapScene::init()
{
	if(!Layer::init())
	{
		return false;
	}

	tilemap = TMXTiledMap::create("res/desert.tmx");
	tilemap->setScale(1);
	bgOrigin = Vec2(0,0);
	this->addChild(tilemap,-1,1);//添加地图



	objects = tilemap->getObjectGroup("Objects");
	auto spawnPoint = objects->getObject("SpawnPoint");
	auto x = spawnPoint["x"].asFloat();
	auto y = spawnPoint["y"].asFloat();
	player = Sprite::create("res/Player.png");
	player->setAnchorPoint(Vec2(0,0));
	player->setPosition(x, y);
	addChild(player,1,2);

	meta = tilemap->getLayer("Meta");
	meta->setVisible(false);

	ground = tilemap->getLayer("Ground");

	//this->schedule(schedule_selector(TileMapScene::hitWall));

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(TileMapScene::onTouchesBegan,this);
	listener->onTouchesMoved = CC_CALLBACK_2(TileMapScene::onTouchesMoved,this);
	listener->onTouchesEnded = CC_CALLBACK_2(TileMapScene::onTouchesEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);//增加多点触摸监听器

	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->onTouchBegan = CC_CALLBACK_2(TileMapScene::onTouchBegan,this);
	listener1->onTouchMoved = CC_CALLBACK_2(TileMapScene::onTouchMoved,this);
	listener1->onTouchEnded = CC_CALLBACK_2(TileMapScene::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1,this);//增加单点触摸监听器

	return true;
}

void TileMapScene::onTouchesBegan(const std::vector<Touch*>& touches,Event* pEvent)
{

}
void TileMapScene::onTouchesMoved(const std::vector<Touch*>& touches,Event* pEvent)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if(touches.size()==1)
	{
		auto point = touches[0]->getLocation();
		auto prePoint = touches[0]->getPreviousLocation();
		auto offSetX = point.x-prePoint.x;
		auto offSetY = point.y-prePoint.y;
		auto map = (TMXTiledMap*)this->getChildByTag(1);
		auto player = (Sprite*)this->getChildByTag(2);
		map->setPosition(Vec2(map->getPosition().x+offSetX,map->getPosition().y+offSetY));
		player->setPosition(Vec2(player->getPosition().x+offSetX,player->getPosition().y+offSetY));
		bgOrigin = Vec2(bgOrigin.x+offSetX,bgOrigin.y+offSetY);

		if(bgOrigin.x>0)//防止出现黑边
		{
			map->setPosition(Vec2(map->getPosition().x-bgOrigin.x,map->getPosition().y));
			player->setPosition(Vec2(player->getPosition().x-bgOrigin.x,player->getPosition().y));
			bgOrigin.x = 0;
		}
		if(bgOrigin.y>0)
		{
			map->setPosition(Vec2(map->getPosition().x,map->getPosition().y-bgOrigin.y));
			player->setPosition(Vec2(player->getPosition().x,player->getPosition().y-bgOrigin.y));
			bgOrigin.y = 0;
		}
		if(bgOrigin.x < visibleSize.width - map->getBoundingBox().size.width)
		{
			map->setPosition(Vec2(map->getPosition().x+visibleSize.width - map->getBoundingBox().size.width - bgOrigin.x,map->getPosition().y));
			player->setPosition(Vec2(player->getPosition().x+visibleSize.width - map->getBoundingBox().size.width - bgOrigin.x,player->getPosition().y));
			bgOrigin.x = visibleSize.width - map->getBoundingBox().size.width;
		}
		if(bgOrigin.y < visibleSize.height - map->getBoundingBox().size.height)
		{
			map->setPosition(Vec2(map->getPosition().x,map->getPosition().y+visibleSize.height - map->getBoundingBox().size.height - bgOrigin.y));
			player->setPosition(Vec2(player->getPosition().x,player->getPosition().y+visibleSize.height - map->getBoundingBox().size.height - bgOrigin.y));
			bgOrigin.y = visibleSize.height - map->getBoundingBox().size.height;
		}
	}
	if(touches.size()>1&&1==0)//缩放
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		auto map = (TMXTiledMap*)this->getChildByTag(1);
		auto p1 = touches[0]->getLocation();
		auto p2 = touches[1]->getLocation();
		auto pp1 = touches[0]->getPreviousLocation();
		auto pp2 = touches[1]->getPreviousLocation();
		auto curDistance = p1.distance(p2);
		auto preDistance = pp1.distance(pp2);

		auto midX = (p1.x+p2.x)/2;
		auto midY = (p1.y+p2.y)/2;
		auto realMidX = midX - bgOrigin.x;
		auto realMidY = midY - bgOrigin.y;
		map->setAnchorPoint(Vec2(realMidX/map->getBoundingBox().size.width,realMidY/map->getBoundingBox().size.height));
		map->setPosition(midX,midY);
		auto scale = map->getScale()*curDistance/preDistance;
		map->setScale(scale);
		bgOrigin = Vec2(midX-map->getBoundingBox().size.width*map->getAnchorPoint().x,midY-map->getBoundingBox().size.height*map->getAnchorPoint().y);
	}
}
void TileMapScene::onTouchesEnded(const std::vector<Touch*>& touches,Event* pEvent)
{

}

bool TileMapScene::onTouchBegan(Touch* ptouch,Event* pEvent)
{
	touchTime = 0;
	this->schedule(schedule_selector(TileMapScene::countTouchTime));
	return true;
}
void TileMapScene::onTouchMoved(Touch* ptouch,Event* pEvent)
{
}
void TileMapScene::onTouchEnded(Touch* ptouch,Event* pEvent)
{
	auto movePoint = ptouch->getLocation();
	auto playerPoint = player->getPosition()+Vec2(16,16);
	auto moveDistance = movePoint.distance(playerPoint);
	//auto offSetX = movePoint.x - playerPoint.x;
	//auto offSetY = movePoint.y - playerPoint.y;

	auto realMovePoint = movePoint-bgOrigin;
	auto realPlayerPoint = playerPoint-bgOrigin;
	//if(realMovePoint.x<=1280&&realMovePoint.x>=0&&realMovePoint.y<=1280&&realMovePoint.y>=0);
	{
		Point tileCoord = tileCoordForPosition(realMovePoint);
		int tileGid = meta->getTileGIDAt(tileCoord);
		if(tileGid)
		{
			auto properties = tilemap->getPropertiesForGID(tileGid).asValueMap();
			if(!properties.empty())
			auto collision = properties["Collidable"].asString();
		}

		if(touchTime<10&&!tileGid)
		{
			int x = movePoint.x;
			int y = movePoint.y;
			auto playerTileCoord = tileCoordForPosition(realPlayerPoint);
			auto dist = playerTileCoord.distance(tileCoord);
			if(playerTileCoord==tileCoord)
				return;
			this->unscheduleAllSelectors();
			vec.clear();
			Astar(playerTileCoord,tileCoord);
			int c = count;
			this->schedule(schedule_selector(TileMapScene::move),0.2,c,0);
		}
	}
}

void TileMapScene::countTouchTime(float)
{
	touchTime++;
}

Point TileMapScene::tileCoordForPosition(Point position)
{
    int x = position.x / tilemap->getTileSize().width;
    int y = ((tilemap->getMapSize().height * tilemap->getTileSize().height) - position.y) / tilemap->getTileSize().height;
    return Point(x, y);
}

void TileMapScene::hitWall(float)
{
	auto playerPoint = player->getPosition();
	auto realPlayerPoint = playerPoint-bgOrigin;
	int tileGid = meta->getTileGIDAt(tileCoordForPosition(realPlayerPoint));
	if(tileGid)
	{
		player->stopAllActions();
	}
}

AStarNode top(std::set<AStarNode>& open)
{
	std::set<AStarNode>::iterator temp;
	AStarNode t;
	int minF = 10000;
	for(temp=open.begin();temp!=open.end();temp++)
	{
		if(temp->f<minF)
		{
			minF=temp->f;
			t=*temp;
		}
	}
	return t;
}

void TileMapScene::Astar(Point S,Point E)
{
	std::set<AStarNode> open;
	std::set<AStarNode> close;
	AStarNode start;
	AStarNode end;
	AStarNode t;
	AStarNode cur;
	int sx = S.x;
	int sy = S.y;
	int ex = E.x;
	int ey = E.y;
	start.setPos(sx,sy);
	start.setG(0);
	start.setH(abs(sx-ex)+abs(sy-ey));
	start.setF(start.g+start.h);
	start.setPPos(sx,sy);
	end.setPos(ex,ey);
	open.insert(start);
	int a[] = {-1, 0, 1, 0, -1,-1, 1, 1};
	int b[] = { 0, 1, 0,-1, -1, 1,-1, 1};
	while(!open.empty())
	{
		cur = top(open);
		close.insert(cur);
		open.erase(cur);
		if(cur == end)
		{
			break;
		}
		CCLOG ("OPENSIZE:%d\n", open.size());
		CCLOG ("CLOSESIZE:%d\n", close.size());
		for(int i = 0; i < 8; ++i)
		{
			if(cur.x + a[i] >= 0 && cur.x +a[i]<=39 && cur.y + b[i] >= 0 && cur.y +b[i]<=39)
			{
				t.setPos(cur.x + a[i], cur.y + b[i]);
				if(i<=3)t.setG(cur.g+1);
				else t.setG(cur.g+1.4);
				t.setH(abs(end.x-t.x)+abs(end.y-t.y));
				t.setF(t.g+t.h);
				t.setPPos(cur.x,cur.y);
				auto oft = open.find(t);
				int block = meta->getTileGIDAt(Vec2(t.x,t.y));
				if(close.find(t)!=close.end()||block)
					continue;
				if(oft==open.end())
				{
					open.insert(t);
				}
				else if(oft->f>t.f)
				{
					open.erase(*oft);
					open.insert(t);
				}
			}
		}
	}
	auto test = close;
	//cur = end;
	while(!(cur==start))
	{
		vec.push_back(cur);
		cur.setPos(cur.px,cur.py);
		cur = *close.find(cur);
	}
	count = vec.size()-1;
}

void TileMapScene::move(float)
{
	player->stopAllActions();
	auto moveTo = MoveTo::create(0.2,Vec2(vec[count].x*32+bgOrigin.x,(39-vec[count].y)*32+bgOrigin.y));
	player->runAction(moveTo);
	count--;
	vec.pop_back();
}