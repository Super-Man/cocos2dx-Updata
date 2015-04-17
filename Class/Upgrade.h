//  Upgrade.h
//
// 更新类
//
//  Created by ChenJinJun on 14-12-20.
//


#ifndef __UPGRADE_HEADER__
#define __UPGRADE_HEADER__


#include <cocos2d.h>
#include <extensions/cocos-ext.h>
#include <CustomAssetsManager.hpp>
#include <ui/CocosGUI.h>
#include <vector>


USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

typedef std::function<void(int)> UpgradeCallback;

class Upgrade : 
	public cocos2d::Layer, 
	public CustomAssetsManagerDelegateProtocol
{
public:

	struct VersionInf
	{
		int id;
		std::string version;
		std::string zipUrl;
	};

	std::vector<VersionInf> VersionVer;

	std::vector<std::string> TipsVer;
	Upgrade();
	
	virtual ~Upgrade();

	bool init();
	
	/**************************
		创建更新类实例
		参数说明: 
		pUpgradeCallback  更新结果回掉函数
	***************************/
	static Upgrade* create(UpgradeCallback pUpgradeCallback);
	

	void upgrade();	//检查版本更新

	virtual void onError(CustomAssetsManager::ErrorCode errorCode);		//错误信息
	virtual void onProgress(int percent);	//更新下载进度
	virtual void onSuccess();		//下载成功

	//CREATE_FUNC(Upgrade);
private:
	void initDownloadDir();		//创建下载目录

	bool getVersionInfo();		//获取版本信息
	//CC_SYNTHESIZE(std::vector,)
	
	static size_t downLoadPackaged(void *ptr, size_t size, size_t nmemb, void *userdata); //存到vector里

	void loadTipsInfo();

	void actionCallBacn();
private:
	std::string _pathToSave;
	cocos2d::Label *_showDownloadInfo;
	CustomAssetsManager *assetManager;
	cocos2d::Label *tipsLabel;
	int nowVersionId;
	int tipsCur;
	std::string localVersionStr;

	CC_SYNTHESIZE(UpgradeCallback, m_pUpgradeCallback, UpgradeCallback)
};


#endif
