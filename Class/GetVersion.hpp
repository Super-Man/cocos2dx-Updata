#ifndef __GET_VERSION_HEADER__
#define __GET_VERSION_HEADER__


#include "VersionVector.hpp"
#include "curl/curl.h"
#include <curl/easy.h>
#include <tinyxml2/tinyxml2.h>
#include "cocos2d.h"
USING_NS_CC;
class GetVersionInfo
{
public:
	GetVersionInfo(std::string localStr);

	~GetVersionInfo();

	bool getInfo();

	static size_t downLoadPackaged(void *ptr, size_t size, size_t nmemb, void *userdata); //¥ÊµΩvector¿Ô

	VersionVer getVersionVer();

	VersionVer m_VersionVer;
private:
	int nowVersionId;
	std::string localVersionStr;

};

#endif
