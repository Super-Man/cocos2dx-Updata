#include "AppDelegate.h"
#include <Upgrade.h>
#include <vector>
#include <ErrorCodeCommon.hpp>
#include "HelloWorldScene.h"
#include <GetVersion.hpp>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// to do ...
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// to do ...
#else
	#include <Windows.h>
#endif

USING_NS_CC;

typedef void(*GameCreate)();

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Polo");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	std::string _pathToSave = CCFileUtils::getInstance()->getWritablePath();
	_pathToSave += "download";

	vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
	vector<string>::iterator iter = searchPaths.begin();
	searchPaths.insert(iter, _pathToSave);
	FileUtils::getInstance()->setSearchPaths(searchPaths);

	std::string s = UserDefault::getInstance()->getStringForKey("localVersion");
	GetVersionInfo* m_pInfo = new GetVersionInfo(s);

	if (m_pInfo->getVersionVer().size() != 0)
	{
		auto sece = Scene::create();

		auto layer = Upgrade::create(std::bind(&AppDelegate::up, this, std::placeholders::_1), m_pInfo->getVersionVer());

		sece->addChild(layer);
		director->runWithScene(sece);
		
	}
	else
	{
		auto scene = HelloWorld::createScene();
		director->runWithScene(scene);

	}
	

//	std::vector<std::string> searchPaths = FileUtils::getInstance()->getSearchPaths();
//	CCLog("fuck  run  this ");


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::up(int m_pErrorCode)
{

	auto scene = HelloWorld::createScene();

	Director::getInstance()->replaceScene(scene);
// 	if (m_pErrorCode == BruCe::protocol::ErrorCode::SUCCESS)
// 	{
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
// 		// to do ...
// #elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 		// to do ...
// 		CCLog("%s:%d", __FILE__, __LINE__);
// #else
// 		std::string szLibPath = FileUtils::getInstance()->fullPathForFilename("libCore_d.dll");
// 		HINSTANCE xHINSTANCE = LoadLibraryA(szLibPath.c_str());
// 
// 		GameCreate xGameCreate = (GameCreate)GetProcAddress(xHINSTANCE, "GameCreate");
// 
// 		if (nullptr == xGameCreate)
// 		{
// 			FreeLibrary(xHINSTANCE);
// 		}
// 
// 		xGameCreate();
// #endif

// 
// 	}

}
