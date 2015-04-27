# cocos2dx-Updata
修改AssetsMangerEx，然后实现更新，C++项目中，用来更新资源包。

其中version.xml 是存放在一个URL地址上的一个版本信息列表。

此例子没有涉及到 加密与解密

add
  GetVersion.cpp  是从URL上去读取 版本信息。 然后存在一个vector中。
  VersionVector.hpp 是定义 存取版本信息的vector。 
  
  ErrorCodeCommon.hpp 是定义的 更新错误指令
  
  Upgrade  相当于一个loading界面，只不过有现在的时候 才用。 不下载的时候 用另外的一个loading界面
  

