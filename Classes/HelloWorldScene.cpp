#include "HelloWorldScene.h"
#include "AppDelegate.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
	auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
	AddX = (visibleSize.width - AppDelegate::designResolutionSize.width) / 2;
	AddY = (visibleSize.height - AppDelegate::designResolutionSize.height) / 2;

	Sprite* background = Sprite::create("bg.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2)); //atur posisi background di tengah layar
	this->addChild(background);

	//buat Sprite untuk meriam
	meriam = Sprite::create("meriam.png");
	meriam->setPosition(Vec2(384 + AddX, 220 + AddY));
	this->addChild(meriam, 1);
	//1 = z order. Secara default z order = 0. z order bwt mengurutkan tampilan dlm layer. 0 = paling bawah, 1 = diatas 0, dst.

	//membuat Sprite gambar hati
	Sprite* live = Sprite::create("hati.png");
	live->setPosition(Vec2(60, visibleSize.height - 60));
	this->addChild(live, 1);

	//membuat label nyawa
	label_nyawa = Label::createWithTTF("x5", "fonts/arial.ttf", 48);
	label_nyawa->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT); //anchor point = titik tumpuan. anchor_middle_left, berarti titik tumpuan di kiri tengah
	label_nyawa->setPosition(Vec2(105, visibleSize.height - 60));
	label_nyawa->setTextColor(Color4B::BLACK);
	label_nyawa->enableOutline(Color4B::WHITE, 3);
	this->addChild(label_nyawa, 1);

	label_score = Label::createWithTTF("Score: 0", "fonts/arial.ttf", 48);
	label_score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	label_score->setPosition(Vec2(visibleSize.width - 30, visibleSize.height - 60));
	label_score->setTextColor(Color4B::BLACK);
	label_score->enableOutline(Color4B::WHITE, 3);
	this->addChild(label_score, 1);

	gameover = Sprite::create("gameover.png");
	gameover->setPosition(Vec2(384 + AddX, 741 + AddY));
	gameover->setVisible(false);
	this->addChild(gameover);

	replay = Button::create("b_replay.png", "b_replay1.png", "b_replay.png");
	replay->setPosition(Vec2(384 + AddX, 574 + AddY));
	replay->setVisible(false);
	replay->setEnabled(false);
	replay->addClickEventListener(CC_CALLBACK_0(HelloWorld::replayClick, this));
	this->addChild(replay);

	//listener untuk keyboard atau keypad
	auto listener_keyboard = EventListenerKeyboard::create();
	listener_keyboard->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_keyboard, this);

	//listener untuk touch
	auto listener_touch = EventListenerTouchOneByOne::create();
	// ter-trigger ketika kamu baru menyentuh layar
	listener_touch->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_touch, this);

	this->schedule(schedule_selector(HelloWorld::AddMeteor), 1);
	this->scheduleUpdate();
    
    return true;
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		//ketika nekan tombol back (android), maka akan keluar. Tombol Escape di windows diidentikkan dengan back di android.
		Director::getInstance()->end();
	}
}

bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
	if (nyawa > 0) {
		Vec2 touchLocation = touch->getLocation();

		//jarak antara posisi touch dengan posisi meriam
		Vec2 offset = touchLocation - meriam->getPosition();

		//atan = kebalikan dari tan, atan2f digunakan untuk mencari sudut dari panjang y dan x (tan = y/x).
		//harus di-convert ke degrees (derajat) dulu, soalnya hasilnya berbentuk radian
		float rotation = CC_RADIANS_TO_DEGREES(atan2f(offset.y, offset.x));

		//karena meriamnya otomatis menghadap ke atas, jadi nilai 90 dikurangi variabel rotation
		meriam->setRotation(90 - rotation);

		//buat projectile (peluru)
		auto projectile = Sprite::create("peluru.png");
		projectile->setPosition(meriam->getPosition());
		this->addChild(projectile);

		//normalisasi vector offset
		offset.normalize();

		//hitung jarak jauh lontaran projectile
		Vec2 jarak_lontaran = offset * 2000;

		//posisi perkiraan projectile ketika setelah di tembak
		Vec2 target = jarak_lontaran + projectile->getPosition();

		auto actionMove = MoveTo::create(2.0f, target); //kecepatan peluru 2 detik
		auto actionRemove = RemoveSelf::create(); //hancurkan kalo sudah sampai di target
		projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));
		peluru.pushBack(projectile); //masukkan ke dalam Vector peluru
	}

	return true;
}

void HelloWorld::AddMeteor(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//acak posisi meteor
	float posX = random(200.0f, visibleSize.width - 200.0f);

	//buat sprite komet
	Sprite* komet = Sprite::create("meteor1.png");
	komet->setPosition(Vec2(posX, visibleSize.height + komet->getContentSize().height / 2));
	this->addChild(komet);

	//buat animasi komet
	Vector<SpriteFrame*> animFrames(15);
	char str[100] = { 0 };
	for (int i = 1; i <= 3; i++)
	{
		sprintf(str, "meteor%d.png", i);
		auto frame = SpriteFrame::create(str, Rect(0, 0, komet->getContentSize().width, komet->getContentSize().height));
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	auto animate = Animate::create(animation);
	komet->runAction(RepeatForever::create(animate));

	//acak kecepatan komet dari 1 sampai 3 detik
	float speed = random(1.0f, 3.0f);
	//190 adalah posisi perkiraan ketika komet jatuh ke tanah.
	//CallFunc digunakan untuk memanggil fungsi pada saat runAction
	komet->runAction(Sequence::create(MoveTo::create(speed, Vec2(posX, 190 + AddY)), CallFunc::create(CC_CALLBACK_0(HelloWorld::MeteorPass, this, komet)), nullptr));
	meteor.pushBack(komet);
}

void HelloWorld::update(float dt)
{
	//mengecek tubrukan antara peluru dan meteor
	for (int i = 0; i < (int)peluru.size(); i++) {
		bool peluru_kena = false;

		for (int j = 0; j < (int)meteor.size(); j++) {
			//cek apakah BoundingBox milik meteor berpotongan dengan boundingbox milik peluru
			if (meteor.at(j)->getBoundingBox().intersectsRect(peluru.at(i)->getBoundingBox())) {
				//peluru kena
				peluru_kena = true;

				//buat sprite animasi meledak
				auto sprite = Sprite::create("meledak1.png");
				sprite->setPosition(meteor.at(j)->getPosition());
				Vector<SpriteFrame*> animFrames(15);
				for (int k = 1; k <= 4; k++)
				{
					auto frame = SpriteFrame::create(String::createWithFormat("meledak%d.png", k)->getCString(), Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height));
					animFrames.pushBack(frame);
				}

				auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
				auto animate = Animate::create(animation);
				sprite->runAction(Sequence::create(animate, RemoveSelf::create(), nullptr));
				this->addChild(sprite);

				//hapus meteor dan peluru yang bertubrukan
				meteor.at(j)->removeFromParent();
				meteor.erase(meteor.begin() + j);
				peluru.at(i)->removeFromParent();
				peluru.erase(peluru.begin() + i);

				//mainkan sound effect ledakan
				SimpleAudioEngine::getInstance()->playEffect("sound/explosion.mp3");

				//tambahkan score dan tampilkan di label
				score += 10;
				label_score->setString(String::createWithFormat("Score: %d", score)->getCString());

				break;
			}
		}

		//kalau peluru udah kena maka looping break
		if (peluru_kena) break;
	}
}

void HelloWorld::MeteorPass(Sprite * komet)
{
	//buat efek ledakan
	EfekLedakan(komet->getPosition());

	//hapus meteor
	komet->removeFromParent();
	meteor.eraseObject(komet);

	if (nyawa > 0) {
		//nyawa di kurangi, dan tampilkan
		nyawa--;
		label_nyawa->setString(String::createWithFormat("x%d", nyawa)->getCString());
	}
	else {
		//kalau nyawa sama dengan 0, maka tampilkan gameover

		//hentikan penambahan meteor
		this->unschedule(schedule_selector(HelloWorld::AddMeteor));
		//hentikan fungsi update
		this->unscheduleUpdate();

		//munculkan sprite gameover dan replay
		gameover->setVisible(true);
		replay->setVisible(true);
		replay->setEnabled(true);
	}
}

void HelloWorld::EfekLedakan(Vec2 position)
{
	//buat sprite animasi meledak
	auto sprite = Sprite::create("meledak1.png");
	sprite->setPosition(position);
	Vector<SpriteFrame*> animFrames(15);
	for (int k = 1; k <= 4; k++)
	{
		auto frame = SpriteFrame::create(String::createWithFormat("meledak%d.png", k)->getCString(), Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height));
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	auto animate = Animate::create(animation);
	sprite->runAction(Sequence::create(animate, RemoveSelf::create(), nullptr));
	this->addChild(sprite);

	//mainkan sound effect ledakan
	SimpleAudioEngine::getInstance()->playEffect("sound/explosion.mp3");

	//efek goncangan layar
	float interval = 0.f;
	float duration = 0.5f;
	float speed = 6.0f;
	float magnitude = 2.0f;

	static float elapsed = 0.f;

	this->schedule([=](float dt) {
		float randomStart = random(-1000.0f, 1000.0f);
		elapsed += dt;

		float percentComplete = elapsed / duration;

		// We want to reduce the shake from full power to 0 starting half way through
		float damper = 1.0f - clampf(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

		// Calculate the noise parameter starting randomly and going as fast as speed allows
		float alpha = randomStart + speed * percentComplete;

		// map noise to [-1, 1]
		float x = noise(alpha, 0.0f) * 2.0f - 1.0f;
		float y = noise(0.0f, alpha) * 2.0f - 1.0f;

		x *= magnitude * damper;
		y *= magnitude * damper;
		this->setPosition(x, y);

		if (elapsed >= duration)
		{
			elapsed = 0;
			this->unschedule("Shake");
			this->setPosition(Vec2::ZERO);
		}

	}, interval, CC_REPEAT_FOREVER, 0.f, "Shake");
}

float HelloWorld::noise(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void HelloWorld::replayClick()
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}
