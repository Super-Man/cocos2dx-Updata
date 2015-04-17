#ifndef __BRUCE_PROTOC_ERRORCODECOMMON_HPP__
#define __BRUCE_PROTOC_ERRORCODECOMMON_HPP__

#include <string>
#include <vector>
using namespace std;

namespace BruCe
{
	namespace protocol
	{

		enum ErrorCode
		{
			SUCCESS = 0,
			A2C_KEY_ERROR = 10,
			RESULT_FAILD_CREATE_FILE,
			RESULT_FAILD_NETWORK,
			RESULT_FAILD_UNCOMPRESS,
			RESULT_FAILD_UNDOWNED
		};

//Protocol Definitions:
	}
}

#endif
