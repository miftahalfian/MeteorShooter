#ifndef _GAMEOVER_
#define _GAMEOVER_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d::ui;
using namespace cocos2d;
using namespace CocosDenshion;

class GameOver : public Layer {
private:
	void replayClick();
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOver);
};

#endif // !_GAMEOVER_

