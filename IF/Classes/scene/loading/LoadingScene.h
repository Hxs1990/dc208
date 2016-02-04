//
//  LoadingScene.h
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//
//

#ifndef __IF__LoadingScene__
#define __IF__LoadingScene__

#include "CommonInclude.h"
#include "CCClipNode.h"

class FlyRollingText;
class FlySystemUpdateHint;


class LoadingScene :
public cocos2d::CCLayer
{
public:
	~LoadingScene();
    CREATE_FUNC(LoadingScene);
    void postLoadingLogToServer(CCObject* p);
   

protected:
	LoadingScene(void);
    // init ----
    void addLoadingBG(Point& addPt);
    void addVersionLabel();
    void addLoadingTips();
    void addPlatformLogo();
    bool init();
    
    void doResourceUpdate(CCObject* p);
    void doLogin(CCObject* p);
    void doLoginInitError(CCObject* p);
    void doLoginInitBan(CCObject* p);
    void doLoginInit(float t);
    void stepLoading(CCObject* p);
    void onConnectionLost(cocos2d::CCObject* obj);
    void onRelogin(CCObject* p);
    void forceUpdate(CCObject* p);
    void offerUpdate(CCObject* p);
    void gotoMainScene(float t);
    void onEnterMainScene(float t); // 3dtouch
    void showMainScene();
    void checkCanGotoMainScene(CCObject* p);
    
    void onServerUpdate(CCObject* p);
    
private:
    int repeatTime;
    virtual void onEnter();
    virtual void onExit();
    
    void onEnterInit();
    void clearCacheOK(CCObject *obj);
    void clearCacheCancel(CCObject *obj);
    void onNetworkAvailable(CCObject* p);
    
    void hideLoading();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool isChina();
    void showLoading();
    void loadingRest(float t);
    void loadingRestAsyn(Ref* arg);
    void unzipLocalfileAsyn(int type=0);
    void parseXML();
    void sendCmdLogin();
    void loadingAni(float t);
    void selectLogin(float t);
    void sendCmdGetServerList(CCObject* p);
    void onGetServerList(CCHttpClient* client, CCHttpResponse* response);
    void setLoadingTips();
    void addParticle();
    void onEnterFrame(CCObject* p);
    void onLoginTimeout(float t);
    void onGetServerStatus(CCHttpClient* client, CCHttpResponse* response);
    bool showServiceTerms();
    bool waitingForLogin();
    void resendGetServerList(float time);
    void onGoToHelpShift(CCObject* p);
    bool isJP();
    void show45SecondAlert(float time);
    void doWhenUidBackFromCloud(float time);
    void retry(CCObject *obj);
    void updateLoadingTip(float _time);
    int m_errorCode;
    int m_responseCode;
    std::string m_tipDialog;
    int m_tipCount;
    bool m_platformLoginFlag;
    bool m_isAdd;
    int m_steps;
    int m_getServerListRetryTime;
    CCSafeObject<CCNode> m_rollingCloud;
    CCSafeObject<CCSprite > m_loadingProgress;
    CCSafeObject<CCLabelIF> m_loadingTips;
    CCSafeObject<CCLabelIF> m_loadPerLabel;
    CCSafeObject<CCLabelIF> m_loadtips;
    CCSafeObject<CCLabelIF> m_FAQ;
    CCSafeObject<CCSprite > m_line;
    CCSafeObject<CCNode> m_FAQTouchNode;
    CCSafeObject<CCSprite > m_platformLogo;
    CCSafeObject<CCClipNode> m_clipNode;
    CCSafeObject<CCClipNode> m_barClipNode;
    CCSafeObject<CCNode> m_headParticleNode;
    CCSafeObject<FlySystemUpdateHint> m_rollHint;
    
    CCSize m_barSize;
    vector<std::string> m_loadingtips;
    
    bool isBuildCallBackDone;
    void buildingCallBack(CCObject* params);
    bool isWallCallBackDone;
    void wallCallBack(CCObject* params);
    
    void prepareImperialSceneEnterView();
    void prepareImperialSceneEnterView(float time);
};

class RollingSprite : public CCNode{
public:
    static RollingSprite *create(std::string picName, float speed);
     virtual void update(float time);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(std::string picName, float speed);
    float m_speed;
    std::string m_picName;
    CCSafeObject<CCSpriteBatchNode> m_batch;
};

#endif /* defined(__IF__LoadingScene__) */
