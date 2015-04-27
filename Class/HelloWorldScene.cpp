#include "HelloWorldScene.h"
USING_NS_CC;

HelloWorld::HelloWorld()
{
	maxIndex = 10;
	curIndex = 0;
	m_Scale = 1.0f;
	Scalelabel = NULL;
	animationState = "stand";
}

HelloWorld::~HelloWorld()
{

}


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
	animationData anipath;
	anipath.jsonPath = "CH.1.1.2/skeleton_and_texture1.ExportJson";
	anipath.animaName = "skeleton_and_texture1";
	pathVer.push_back(anipath);

	animationData anipath1;
	anipath1.jsonPath = "CH.1.1.2/skeleton_and_texture1.ExportJson";
	anipath1.animaName = "skeleton_and_texture1";
	pathVer.push_back(anipath1);


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("Common/bg.jpg");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg);

	
	Scalelabel = LabelTTF::create("", "Marker Felt", 32);
	Scalelabel->setPosition(Point(visibleSize.width / 2 ,  visibleSize.height - 80));
	Scalelabel->setHorizontalAlignment(TextHAlignment::CENTER);
	this->addChild(Scalelabel);

	Button* subBtn = Button::create("sub.png","sub.png");
	subBtn->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height - 80));
	subBtn->setTag(subBtnTag);
	subBtn->addClickEventListener(CC_CALLBACK_1(HelloWorld::btnCallBack,this));
	this->addChild(subBtn);
	
	Button* addBtn = Button::create("add.png","add.png");
	addBtn->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height - 80));
	addBtn->setTag(addBtnTag);
	addBtn->addClickEventListener(CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	this->addChild(addBtn);

	
	//¹¥»÷
	MenuItemFont* attack = MenuItemFont::create("Attack", CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	attack->setTag(attackBtnTag);
	attack->setPosition(Vec2(100, 360 + 70));

	//zhanlio
	MenuItemFont* stand = MenuItemFont::create("Stand", CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	stand->setTag(standBtnTag);
	stand->setPosition(Vec2(100, 310 + 70));

	//run
	MenuItemFont* m_run = MenuItemFont::create("Run", CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	m_run->setTag(runBtnTag);
	m_run->setPosition(Vec2(100, 260 + 70));
	//hurt
	MenuItemFont* hurt = MenuItemFont::create("Hurt", CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	hurt->setTag(hurtBtnTag);
	hurt->setPosition(Vec2(100, 210 + 70));
	//kill
	MenuItemFont* kill = MenuItemFont::create("Kill", CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	kill->setTag(killBtnTag);
	kill->setPosition(Vec2(100, 160 + 70));
	//cheer
	MenuItemFont* cheer = MenuItemFont::create("Cheer", CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	cheer->setTag(cheerBtnTag);
	cheer->setPosition(Vec2(100, 110 + 70));
	//realse
	MenuItemFont* releas = MenuItemFont::create("Release", CC_CALLBACK_1(HelloWorld::btnCallBack, this));
	releas->setTag(releaseBtnTag);
	releas->setPosition(Vec2(100, 60 + 70));



	auto menu = Menu::create(attack,stand,m_run,hurt,kill,cheer,releas,NULL);
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 1);

	Button* leftBtn = Button::create("leftoff.png", "lefton.png");
	leftBtn->setPosition(Vec2(visibleSize.width/2 - 200, 50));
	leftBtn->addClickEventListener(CC_CALLBACK_1(HelloWorld::leftBtnCallBack,this));
	this->addChild(leftBtn, 1);


	Button* rightBtn = Button::create("rightoff.png", "righton.png");
	rightBtn->setPosition(Vec2(visibleSize.width/2 + 200, 50));
	rightBtn->addClickEventListener(CC_CALLBACK_1(HelloWorld::rightBtnCallBack, this));
	this->addChild(rightBtn, 1);

	ArmatureDataManager::getInstance()->addArmatureFileInfo(pathVer[curIndex].jsonPath.c_str());

	updata();
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::leftBtnCallBack(Ref* pSend)
{
	++curIndex;
	Size winSize = Director::getInstance()->getVisibleSize();

	if (curIndex > pathVer.size()-1)
	{
		curIndex = 0;
	}
	m_Scale = 1.0f;
	updata();
}

void HelloWorld::rightBtnCallBack(Ref* pSend)
{
		--curIndex;
	if (curIndex < 0)
	{
		curIndex = pathVer.size()-1;
	}
	m_Scale = 1.0f;
	updata();
}



void HelloWorld::btnCallBack(Ref* pSend)
{
	Button* m_pBtn = (Button*)pSend;
	CCLOG("  button  tag   %d", m_pBtn->getTag());
	switch (m_pBtn->getTag())
	{
	case subBtnTag:
		{
			m_Scale -= 0.1f;
		}
		break;
	case addBtnTag:
		{
			m_Scale += 0.1f;
		}
		break;
	case attackBtnTag:
		{
			animationState = "attack";
		}
		break;
	case standBtnTag:
		{
			animationState = "stand";
		}
		break;
	case runBtnTag:
		{
			animationState = "run";
		}
		break;
	case hurtBtnTag:
		{
			animationState = "hurt";
		}
		break;
	case killBtnTag:
		{
			animationState = "skill";

		}
		break;
	case cheerBtnTag:
		{
			animationState = "cheer";
		}
		break;

	case releaseBtnTag:
		{

			animationState = "release";
		}
		break;
	default:
		break;
	}
	
	updata();
}

void HelloWorld::updata()
{
	CCLOG("animationState   : %s", animationState.c_str());
	Size winSize = Director::getInstance()->getVisibleSize();
	char scaleL[15];
	sprintf(scaleL, "%ff", m_Scale);
	Scalelabel->setString(scaleL);
	if (this->getChildByTag(10010))
	{
		this->removeChildByTag(10010);
	}
	if (curIndex == 1)
	{
		Label* number = Label::create();
		number->setString("curIndex number  is 2");
		number->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + 40));
		this->addChild(number, 1);
	}
	
	
	
	anima = Armature::create(pathVer[curIndex].animaName.c_str());
	anima->setPosition(Vec2(winSize.width / 2, winSize.height / 2-200));
	anima->getAnimation()->play(animationState.c_str());
	anima->getAnimation()->setSpeedScale(0.5f);

	anima->setTag(10010);
	anima->setScale(m_Scale);

// 	if (animationState == "run")
// 	{
// 		anima->getAnimation()->setSpeedScale(0.2f);
// 	}

	if (animationState == "skill")
	{
		ClippingNode * clipping = ClippingNode::create(anima);
		LayerColor* conten = LayerColor::create(ccc4(0, 0, 0, 200));
		conten->setSwallowsTouches(true);
		clipping->addChild(conten);
		clipping->setAlphaThreshold(0.05f);
		clipping->setInverted(true);
		this->addChild(clipping,10);
	}
	


	this->addChild(anima);

}
