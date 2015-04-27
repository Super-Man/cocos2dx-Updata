#include "GetVersion.hpp"
#include <iostream>
GetVersionInfo::GetVersionInfo(std::string localStr):localVersionStr(""),nowVersionId(0)
{
		localVersionStr = localStr;
		getInfo();
}

GetVersionInfo::~GetVersionInfo()
{

}

bool GetVersionInfo::getInfo()
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
		return false;
	}
	return true;
}

size_t GetVersionInfo::downLoadPackaged(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	GetVersionInfo* pUpgrade = (GetVersionInfo*)userdata;

	char *pData = (char*)ptr;

	tinyxml2::XMLDocument doc;

	doc.Parse(pData);

	tinyxml2::XMLElement* pElement = doc.RootElement()->FirstChildElement();

	while (pElement)
	{
		VersionInfo mVer;
		mVer.id = pElement->IntAttribute("Id");
		mVer.version = pElement->Attribute("version");
		if (mVer.version == pUpgrade->localVersionStr)
		{
			pUpgrade->nowVersionId = mVer.id;
		}
		mVer.zipUrl = pElement->Attribute("URL");

		pUpgrade->m_VersionVer.push_back(mVer);
		pElement = pElement->NextSiblingElement();
	}
	VersionVer::iterator ite = pUpgrade->m_VersionVer.begin();
	for (; ite != pUpgrade->m_VersionVer.end();)
	{
		if (ite->id <= pUpgrade->nowVersionId)
		{
			ite = pUpgrade->m_VersionVer.erase(ite);
		}
		else
		{
			++ite;
		}
	}
	return nmemb;
}

VersionVer GetVersionInfo::getVersionVer()
{
	return m_VersionVer;
}
