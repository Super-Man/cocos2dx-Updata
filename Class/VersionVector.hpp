#ifndef __VERSION_VECTOR_HEADER__
#define __VERSION_VECTOR_HEADER__

#include <vector>
#include <string>

struct VersionInfo
{
	int id;
	std::string version;
	std::string zipUrl;
};

typedef std::vector<VersionInfo> VersionVer;

#endif
