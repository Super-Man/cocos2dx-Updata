

#ifndef __AssetsManagerEx__
#define __AssetsManagerEx__

#include <string>

#include <mutex>

#include "cocos2d.h"
#include "extensions/ExtensionMacros.h"

NS_CC_EXT_BEGIN

class AssetsManagerDelegateProtocolEx;

/*
 *  This class is used to auto update resources, such as pictures or scripts.
 *  The updated package should be a zip file. And there should be a file named
 *  version in the server, which contains version code.
 */
class AssetsManagerEx : public Node
{
public:
    enum class ErrorCode
    {	

        // Error caused by creating a file to store downloaded data
        CREATE_FILE,
        /** Error caused by network
         -- network unavaivable
         -- timeout
         -- ...
         */
        NETWORK,
        /** There is not a new version
         */
        NO_NEW_VERSION,
        /** Error caused in uncompressing stage
         -- can not open zip file
         -- can not read file global information
         -- can not read file information
         -- can not create a directory
         -- ...
         */
        UNCOMPRESS,
		/*
			Error can not down the file
		*/
		UNDOWNED,
    };
    
// 	struct UpdateItem
// 	{
// 		std::string version;
// 		std::string zipPath;
// 		std::string zipUrl;
// 
// 		UpdateItem(std::string v, std::string p, std::string u) : version(v), zipPath(p), zipUrl(u) {}
// 	};

    /* @brief Creates a AssetsManager with new package url, version code url and storage path.
     *
     * @param packageUrl URL of new package, the package should be a zip file.
     * @param versionFileUrl URL of version file. It should contain version code of new package.
     * @param storagePath The path to store downloaded resources.
     * @js NA
     */
    AssetsManagerEx(const char* versionFileUrl = NULL, const char* storagePath = NULL);
    /**
     * @js NA
     * @lua NA
     */
    virtual ~AssetsManagerEx();
    
    typedef std::function<void(int)> ErrorCallback;
    typedef std::function<void(int)> ProgressCallback;
    typedef std::function<void(void)> SuccessCallback;

    /* @brief To access within scripting environment
     */
    static AssetsManagerEx* create(const char* versionFileUrl, const char* storagePath, ErrorCallback errorCallback, ProgressCallback progressCallback, SuccessCallback successCallback );

    /* @brief Check out if there is a new version resource.
     *        You may use this method before updating, then let user determine whether
     *        he wants to update resources.
     */
    virtual bool checkUpdate();
    
    using Node::update;
    /* @brief Download new package if there is a new version, and uncompress downloaded zip file.
     *        Ofcourse it will set search path that stores downloaded files.
     */
    virtual void update();
     
    /* @brief Gets current version code.
     */
    std::string getVersion();
    
    /* @brief Deletes recorded version code.
     */
    void deleteVersion();
    
   
    void setDelegate(AssetsManagerDelegateProtocolEx *delegate);
    
    /**
     * @js NA
     * @lua NA
     */
    AssetsManagerDelegateProtocolEx* getDelegate() const { return _delegate ;}
    
    /** @brief Sets connection time out in seconds
     */
    void setConnectionTimeout(unsigned int timeout);
    
    /** @brief Gets connection time out in secondes
     */
    unsigned int getConnectionTimeout();
    
    /* downloadAndUncompress is the entry of a new thread 
     */
    friend int assetsManagerProgressFunc(void *, double, double, double, double);

protected:
    bool downLoad();
    void checkStoragePath();
    bool uncompress();
    bool createDirectory(const char *path);
    void setSearchPath();
    void downloadAndUncompress();

private:
    /** @brief Initializes storage path.
     */
    void createStoragePath();
    
    /** @brief Destroys storage path.
     */
    void destroyStoragePath();
    
private:
    //! The path to store downloaded resources.
    std::string _storagePath;
    
    //! The version of downloaded resources.
    std::string _version;
    
    std::string _packageUrl;
    std::string _versionFileUrl;
    
    std::string _downloadedVersion;
    
	//std::deque<UpdateItem> _versionUrls;  // 版本信息队列

    void *_curl;

    unsigned int _connectionTimeout;
    
    AssetsManagerDelegateProtocolEx *_delegate; 
    
    bool _isDownloading;
    bool _shouldDeleteDelegateWhenExit;
    
};

class AssetsManagerDelegateProtocolEx
{
public:
    virtual ~AssetsManagerDelegateProtocolEx(){};
public:
    /* @brief Call back function for error
       @param errorCode Type of error
     * @js NA
     * @lua NA
     */
    virtual void onError(AssetsManagerEx::ErrorCode errorCode) {};
    /** @brief Call back function for recording downloading percent
        @param percent How much percent downloaded
        @warning    This call back function just for recording downloading percent.
              AssetsManager will do some other thing after downloading, you should
              write code in onSuccess() after downloading. 
     * @js NA
     * @lua NA
     */
    virtual void onProgress(int percent) {};
    /** @brief Call back function for success
     * @js NA
     * @lua NA
     */
    virtual void onSuccess() {};
};

// Deprecated declaration
CC_DEPRECATED_ATTRIBUTE typedef AssetsManagerEx CCAssetsManagerEx;
CC_DEPRECATED_ATTRIBUTE typedef AssetsManagerDelegateProtocolEx CCAssetsManagerDelegateProtocolEx;

NS_CC_EXT_END;

#endif /* defined(__AssetsManager__) */
