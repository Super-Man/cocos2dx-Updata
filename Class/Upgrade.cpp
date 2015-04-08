#include "Upgrade.h"
#include "HelloWorldScene.h"


#include "curl/curl.h"
#include <curl/easy.h>
#include <tinyxml2/tinyxml2.h>



#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
#define DOWNLOAD_FIEL		"download"	//下载后保存的文件夹名

Upgrade::Upgrade():
_pathToSave(""),
_showDownloadInfo(NULL),
assetManager(NULL),
nowVersionId(0),
tipsCur(0),
localVersionStr("")
{

}

Upgrade::~Upgrade()
{
	
}

bool Upgrade::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();

	//获取本地版本
	CCLOG("xml----location----%s", UserDefault::getInstance()->getXMLFilePath().c_str());
	localVersionStr = UserDefault::getInstance()->getStringForKey("localVersion");
	
	if (localVersionStr == "")
	{
		CCLOG("  localVersion  information is null ");
	}

	//获取URL上的版本队列
	if (!getVersionInfo())
	{
		CCLOG("no new version =====");
		return false;
	}

	//创建下载目录文件夹
	initDownloadDir(); 

	//加载tips 提示信息
	loadTipsInfo();	
	

	LoadingBar *pLoadingBar = LoadingBar::create("load.png");
	pLoadingBar->setTag(1001);
	pLoadingBar->setPosition(Vec2(winSize.width / 2, 100));

	this->addChild(pLoadingBar);


	_showDownloadInfo = Label::createWithSystemFont("", "Arial", 20);
	this->addChild(_showDownloadInfo);
	_showDownloadInfo->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 20));

	//提示label
	tipsLabel = Label::create(TipsVer[0], "Arail", 20);
	tipsLabel->setTag(1002);
	tipsLabel->setPosition(Vec2(winSize.width / 2, winSize.height / 2+20));
	this->addChild(tipsLabel);

	

	//淡出淡出
	auto *fadeout = FadeOut::create(1.5f);
	auto *fadein = FadeIn::create(1.5f);
	auto *repeatForever = RepeatForever::create(Sequence::create(fadeout,
																CallFunc::create(CC_CALLBACK_0(Upgrade::actionCallBacn, this)),
																fadein,
																DelayTime::create(2.0f),
																nullptr));
	tipsLabel->runAction(repeatForever);


	auto itemLabel2 = MenuItemLabel::create(
		Label::createWithSystemFont("Upgrade", "Arail", 20), CC_CALLBACK_1(Upgrade::upgrade, this));

	auto menu = Menu::create(itemLabel2, NULL);
	this->addChild(menu);

	itemLabel2->setPosition(Vec2(winSize.width / 2, winSize.height / 2 ));

	menu->setPosition(Vec2::ZERO);

	return true;
}

void Upgrade::onError(AssetsManagerEx::ErrorCode errorCode)
{
	if (errorCode == AssetsManagerEx::ErrorCode::NO_NEW_VERSION)
	{
		_showDownloadInfo->setString("no new version");
	}
	else if (errorCode == AssetsManagerEx::ErrorCode::NETWORK)
	{
		_showDownloadInfo->setString("network error");
	}
	else if (errorCode == AssetsManagerEx::ErrorCode::CREATE_FILE)
	{
		_showDownloadInfo->setString("create file error");
	}
}

void Upgrade::onProgress(int percent)
{
	if (percent < 0)
		return;
	
	LoadingBar* loadingBar = static_cast<LoadingBar*>(this->getChildByTag(1001));
	loadingBar->setPercent(percent);

	char progress[20];
	snprintf(progress, 20, "download %d%%", percent);
	_showDownloadInfo->setString(progress);
}

void Upgrade::onSuccess()
{
	CCLOG("------------Upgrade --------   download success");
	_showDownloadInfo->setString("download success");
	CC_SAFE_DELETE(assetManager);
	
	UserDefault::getInstance()->setStringForKey("localVersion", VersionVer.begin()->version.c_str());
	VersionVer.erase(VersionVer.begin());
	if (VersionVer.size()!=0)
	{
		assetManager = new AssetsManagerEx(VersionVer.begin()->zipUrl.c_str(), _pathToSave.c_str());
		assetManager->setDelegate(this);
		assetManager->setConnectionTimeout(3);
		assetManager->update();
	}
	else
	{
		std::vector<std::string>searchPath;
		searchPath.push_back(_pathToSave);
		FileUtils::getInstance()->setSearchPaths(searchPath);

		auto scene = HelloWorld::scene();
		Director::getInstance()->replaceScene(scene);
	}
	
}


void Upgrade::initDownloadDir()
{
	CCLOG("initDownloadDir");
	_pathToSave = CCFileUtils::getInstance()->getWritablePath();
	_pathToSave += DOWNLOAD_FIEL;
CCLOG("Path: %s", _pathToSave.c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;
	pDir = opendir(_pathToSave.c_str());
	if (!pDir)
	{
		mkdir(_pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(_pathToSave.c_str(), 0);
	}
#endif
	CCLOG("initDownloadDir end");
}

// void Upgrade::reset(Ref* pSender)
// {
// 	_showDownloadInfo->setString("");
// 	// Remove downloaded files
// #if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
// 	string command = "rm -r ";
// 	// Path may include space.
// 	command += "\"" + _pathToSave + "\"";
// 	system(command.c_str());
// #else
// 	std::string command = "rd /s /q ";
// 	// Path may include space.
// 	command += "\"" + _pathToSave + "\"";
// 	system(command.c_str());
// #endif
// 	initDownloadDir();
// }

void Upgrade::upgrade(Ref* pSender)
{
	
	
	_showDownloadInfo->setString("");

// 	vector<VersionInf>::iterator ite = VersionVer.begin();
// 	for (; ite != VersionVer.end(); ++ite)
// 	{	
	if (VersionVer.size()<=0)
	{
		CCLOG("no new version ");
	}
	else
	{
		assetManager = new AssetsManagerEx(VersionVer.begin()->zipUrl.c_str(), _pathToSave.c_str());
		assetManager->setDelegate(this);
		assetManager->setConnectionTimeout(3);
		assetManager->update();
	}
		
	//}

}

bool Upgrade::getVersionInfo()
{
	std::string txURL = "http://180.168.116.166:8083/cfg/chenjinjun/Version.xml";

	CURLcode res;
	void* _curl;
	_curl = curl_easy_init();
	curl_easy_setopt(_curl, CURLOPT_URL, txURL.c_str());
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackaged);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, 5L);
	res = curl_easy_perform(_curl);
	curl_easy_cleanup(_curl);
	if (res != 0)
	{
		CCLOG("=========%d", res);
		CCLOG("the curl is error====");
		return false;
	}
	return true;
}

size_t Upgrade::downLoadPackaged(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	Upgrade* pUpgrade = (Upgrade*)userdata;

	char *pData = (char*)ptr;

	tinyxml2::XMLDocument doc;

	doc.Parse(pData);

	tinyxml2::XMLElement* pElement = doc.RootElement()->FirstChildElement();

	while (pElement)
	{
		VersionInf mVer;
		mVer.id = pElement->IntAttribute("Id");

		mVer.version = pElement->Attribute("version");
		if (mVer.version == pUpgrade->localVersionStr)
		{
			pUpgrade->nowVersionId = mVer.id;
		}
		mVer.zipUrl = pElement->Attribute("URL");

		pUpgrade->VersionVer.push_back(mVer);
		pElement = pElement->NextSiblingElement();
	}
	vector<VersionInf>::iterator ite = pUpgrade->VersionVer.begin();
	for (; ite != pUpgrade->VersionVer.end();)
	{		
		if (ite->id <= pUpgrade->nowVersionId)
		{
			ite = pUpgrade->VersionVer.erase(ite);
		}
		else
		{
			++ite;
		}
	}
	return nmemb;
}

void Upgrade::loadTipsInfo()
{
	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();

	if (doc == nullptr)
	{
		return;
	}
	doc->LoadFile("tips.xml");
	tinyxml2::XMLElement* pElement = doc->RootElement()->FirstChildElement();
	while (pElement)
	{
		string tps = pElement->Attribute("tips");
		TipsVer.push_back(tps);

		pElement = pElement->NextSiblingElement();
	}

}

void Upgrade::actionCallBacn()
{
	++tipsCur;
	if (tipsCur >= TipsVer.size())
	{
		tipsCur = 0;
	}
	tipsLabel = (Label*)this->getChildByTag(1002);
	tipsLabel->setString(TipsVer[tipsCur]);

}
