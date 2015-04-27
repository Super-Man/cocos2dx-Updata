//  Upgrade.cpp
//
// 更新类
//
//  Created by ChenJinJun on 14-12-20.
//



#include "Upgrade.h"
#include "curl/curl.h"
#include <curl/easy.h>
#include <tinyxml2/tinyxml2.h>
#include <ErrorCodeCommon.hpp>


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
tipsCur(0)
{
	
}

Upgrade::~Upgrade()
{
	
}

Upgrade* Upgrade::create(UpgradeCallback pUpgradeCallback, VersionVer versionVe)
{
	if (nullptr == pUpgradeCallback)
	{
		return nullptr;
	}

	Upgrade* _new = new Upgrade();
	if (_new && versionVe.size() != 0 )
	{
		_new->setUpgradeCallback(pUpgradeCallback);
		_new->m_VersionVer = versionVe;
	}
	if (_new->init())
	{
		_new->autorelease();
		return _new;
	}
	
	
	CC_SAFE_DELETE(_new);
	return nullptr;


}


bool Upgrade::init()
{
//	CCLog("%s:%d", __FILE__, __LINE__);

		if (!Layer::init())
		{
			return false;
		}


	Size winSize = Director::getInstance()->getWinSize();

	//获取本地版本
	CCLOG("xml----location----%s", UserDefault::getInstance()->getXMLFilePath().c_str());
	std::string s =	UserDefault::getInstance()->getXMLFilePath().c_str();
//	CCLog("%s:%d", s.c_str(), __LINE__);
	


	//创建下载目录文件夹
	initDownloadDir();

	//加载tips 提示信息
	loadTipsInfo();


	LoadingBar *pLoadingBar = LoadingBar::create("load.png");
	pLoadingBar->setTag(1001);
	pLoadingBar->setPosition(Vec2(winSize.width / 2, 100));

	this->addChild(pLoadingBar);


	_showDownloadInfo = Label::create();
	_showDownloadInfo->setString("");
	_showDownloadInfo->setSystemFontSize(20);

	this->addChild(_showDownloadInfo);
	_showDownloadInfo->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 20));

	//提示label
	tipsLabel = Label::create();
	tipsLabel->setString(TipsVer[0]);
	tipsLabel->setSystemFontSize(20);
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


	upgrade();

	return true;
}

void Upgrade::onError(CustomAssetsManager::ErrorCode errorCode)
{
	if (errorCode == CustomAssetsManager::ErrorCode::UNCOMPRESS)
	{
		_showDownloadInfo->setString("no new version");

		m_pUpgradeCallback(BruCe::protocol::ErrorCode::RESULT_FAILD_UNCOMPRESS);
	}
	else if (errorCode == CustomAssetsManager::ErrorCode::NETWORK)
	{
		_showDownloadInfo->setString("network error");
		m_pUpgradeCallback(BruCe::protocol::ErrorCode::RESULT_FAILD_NETWORK);
	}
	else if (errorCode == CustomAssetsManager::ErrorCode::CREATE_FILE)
	{
		_showDownloadInfo->setString("create file error");
	}
	else if (errorCode == CustomAssetsManager::ErrorCode::UNDOWNED)
	{
		m_pUpgradeCallback(BruCe::protocol::ErrorCode::RESULT_FAILD_UNDOWNED);
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
	if (m_VersionVer.size())
	{
		UserDefault::getInstance()->setStringForKey("localVersion", m_VersionVer.begin()->version.c_str());
		m_VersionVer.erase(m_VersionVer.begin());
	}
	
	if (m_VersionVer.size() != 0)
	{
		assetManager = new CustomAssetsManager(m_VersionVer.begin()->zipUrl.c_str(), _pathToSave.c_str());
		assetManager->setDelegate(this);
		assetManager->setConnectionTimeout(3);
		assetManager->update();
	}
	else
	{
		m_pUpgradeCallback(BruCe::protocol::ErrorCode::SUCCESS);
		
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


	vector<string> searchPaths = FileUtils::getInstance()->getSearchPaths();
	vector<string>::iterator iter = searchPaths.begin();
	searchPaths.insert(iter, _pathToSave);
	FileUtils::getInstance()->setSearchPaths(searchPaths);
}


void Upgrade::upgrade()
{
	
	
	_showDownloadInfo->setString("");
	
	assetManager = new CustomAssetsManager(m_VersionVer.begin()->zipUrl.c_str(), _pathToSave.c_str());
	assetManager->setDelegate(this);
	assetManager->setConnectionTimeout(3);
	assetManager->update();
		
}

void Upgrade::loadTipsInfo()
{

	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();

	if (doc == nullptr)
	{
		return;
	}

	std::string sFilePath = FileUtils::getInstance()->fullPathForFilename("tips.xml");

	auto data=  FileUtils::getInstance()->getDataFromFile(sFilePath.c_str());

	doc->Parse((char*)data.getBytes(),data.getSize());

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


