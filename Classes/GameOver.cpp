#include "GameOver.h"
#include "HelloWorldScene.h"
#include "AppDelegate.h"

cocos2d::Scene * GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameOver::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	float AddX = (visibleSize.width - AppDelegate::designResolutionSize.width) / 2;
	float AddY = (visibleSize.height - AppDelegate::designResolutionSize.height) / 2;

	Sprite* background = Sprite::create("bg.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2)); //atur posisi background di tengah layar
	this->addChild(background);

	Sprite* gameover = Sprite::create("gameover.png");
	gameover->setPosition(Vec2(384 + AddX, 741 + AddY));
	this->addChild(gameover);

	Button* replay = Button::create("b_replay.png", "b_replay1.png", "b_replay.png");
	replay->setPosition(Vec2(384 + AddX, 574 + AddY));
	replay->addClickEventListener(CC_CALLBACK_0(GameOver::replayClick, this));
	this->addChild(replay);

	return true;
}

void GameOver::replayClick()
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}