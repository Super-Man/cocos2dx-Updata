#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio\CocoStudio.h"
#include <vector>

USING_NS_CC;
using namespace ui;
using namespace cocostudio;


enum BtnTag
{
	subBtnTag,
	addBtnTag,
	attackBtnTag,
	standBtnTag,
	runBtnTag,
	hurtBtnTag,
	killBtnTag,
	cheerBtnTag,
	releaseBtnTag
};


class HelloWorld : public cocos2d::Layer
{
public:
	struct animationData
	{
		std::string jsonPath;
		std::string animaName;
	};
	std::vector<animationData> pathVer;

	HelloWorld();
	
	
	~HelloWorld();



    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private: 
	LabelTTF* Scalelabel;

	int maxIndex;

	int curIndex ;
	
	float m_Scale;

	std::string animationState;


	void leftBtnCallBack(Ref* pSend);

	void rightBtnCallBack(Ref* pSend);

	void btnCallBack(Ref* pSend);

	Armature* anima;

	void updata();

};

#endif // __HELLOWORLD_SCENE_H__
