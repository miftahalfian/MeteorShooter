#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d::ui;
using namespace cocos2d;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::Layer
{
private:
	Vector<Sprite*> meteor;
	Vector<Sprite*> peluru;

	Label* label_score;
	Label* label_nyawa;

	Sprite* meriam;

	int score = 0;
	int nyawa = 5;

	float AddX = 0;
	float AddY = 0;

	void MeteorPass(Sprite* komet);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	bool onTouchBegan(Touch* touch, Event* event);

	void AddMeteor(float dt);
	void update(float dt);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
