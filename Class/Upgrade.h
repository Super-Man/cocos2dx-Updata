//  Gobal.h
//
//  Created by Sharezer on 14-07-26.
//
//

#ifndef _UPGRADE_H_
#define _UPGRADE_H_
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "AssetsManagerEx.h"
#include "ui/CocosGUI.h"
#include <vector>
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class Upgrade : public cocos2d::CCLayer, public AssetsManagerDelegateProtocolEx
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

	virtual bool init();

	void upgrade(cocos2d::Ref* pSender);	//检查版本更新

	virtual void onError(AssetsManagerEx::ErrorCode errorCode);		//错误信息
	virtual void onProgress(int percent);	//更新下载进度
	virtual void onSuccess();		//下载成功
	CREATE_FUNC(Upgrade);
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
	AssetsManagerEx *assetManager;
	cocos2d::Label *tipsLabel;
	int nowVersionId;
	int tipsCur;
	std::string localVersionStr;
};


#endif
