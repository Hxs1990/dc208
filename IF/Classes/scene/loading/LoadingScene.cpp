//
//  LoadingScene.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//
//

#include "LoadingScene.h"
#include "../../Ext/CCDevice.h"
#include "SceneController.h"
#include "SoundController.h"
#include "LoginCommand.h"
#include "PopupViewController.h"
#include "KoreaPermitPopUPView.h"
#include "AppLibHelper.h"
#include "NetController.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "InitCommand.h"
#include "HelpshiftCocos2dx.h"
#include "SuggestionView.h"
#include <spine/Json.h>
#include "IFLoadingSceneArmyNode.h"
#include "SaoGuangEffectSprite.h"
#include "FunBuildController.h"
#include "IFSkeletonDataManager.h"
#include "md5.h"
#include "CCThreadManager.h"
#include "../../view/popup/RechargeACTVCell.h"

#include "PermitionForServicePopupView.hpp"
#include "UIComponent.h"
#include "GuideController.h"
#include "FBUtilies.h"
#include "FlyRollingText.h"
#include "FlySystemUpdateHint.h"
#include "ServerUpdateView.h"
#include "GmController.h"

#include "DynamicResourceController.h"


#define MSG_MAIN_THREAD_XML 453
#define LOADING_STEP        80
#define LOADING_LOGO_TAG    666
#define LOADING_1 667
#define LOADING_2 668
#define LOADING_3 669
#define LOADING_4 670

#define LOADING_GLOW 671


LoadingScene::~LoadingScene(){
}

LoadingScene::LoadingScene(): m_steps(LOADING_STEP),m_platformLoginFlag(false){}

bool LoadingScene::init()
{
//    this->setScale(2.0);
//    this->setAnchorPoint(ccp(0,0));
    bool bRet = false;
    addParticle();
    if (CCLayer::init()) {
        if (CCCommonUtils::isIosAndroidPad() && !CCCommonUtils::getIsHDViewPort())
        {
            CCCommonUtils::setIsHDViewPort(true);
        }
        m_getServerListRetryTime = 0;
        CCLabelIF::initFont();
        GameController::getInstance();
        //loadingLog统计
        GameController::getInstance()->setLoadingLog("LoadingScene", "init");
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        m_isAdd = false;
        auto loadingBarBG = CCLoadSprite::createSprite("loading_bar_BG.png");
        loadingBarBG->setTag(LOADING_1);
        auto loadingBar = CCLoadSprite::createSprite("loading_bar.png");
        loadingBar->setTag(LOADING_2);
        m_loadingProgress = CCLoadSprite::createSprite("loading_progress.png");
        
        string _uuid = "";
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(CCCommonUtils::isUserNewUid()){
            _uuid = cocos2d::extension::CCDevice::getCurrentAndroidUDID();
        }else{
            _uuid = cocos2d::extension::CCDevice::getDeviceUid();
        }
#else
        _uuid = cocos2d::extension::CCDevice::getDeviceUid();
#endif
        
        GlobalData::shared()->version = cocos2d::extension::CCDevice::getVersionName();
        string _version = GlobalData::shared()->version.c_str();
//        _version = _version.substr(0,_version.length()-3);
#if COCOS2D_DEBUG > 0
        CCLabelIF* label = CCLabelIF::create(CCString::createWithFormat("Inner-V %s : hudson Code %s : deviceID %s",_version.c_str(),cocos2d::extension::CCDevice::getVersionCode().c_str(),_uuid.c_str())->getCString());
#else
        CCLabelIF* label = CCLabelIF::create(CCString::createWithFormat("V %s (%s)",_version.c_str(),cocos2d::extension::CCDevice::getVersionCode().c_str())->getCString());
#endif
        
		label->setScale(0.5f);
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setScale(0.8f);
        }
        label->setFontSize(40);
        label->setColor({255,255,255});
        label->setAnchorPoint(CCPoint(1,1));
		label->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width-10,CCDirector::sharedDirector()->getWinSize().height));
        
        auto addFireParticle = [](CCNode *parent, std::string particleName, const CCPoint &pt){
            auto particle = ParticleController::createParticle(particleName);
            auto newBatch = ParticleController::createParticleBatch(particle);
            if (CCCommonUtils::isIosAndroidPad())
            {
                particle->setScale(2.4);
            }
            newBatch->addChild(particle);
            particle->setPosition(pt);
            parent->addChild(newBatch);
        };

        auto addPt = ccp(0, 0);
        if(SceneController::getInstance()->showBG){
            SceneController::getInstance()->showBG->setPosition(ccp(size.width/2, size.height/2));
            addChild(SceneController::getInstance()->showBG);
            addPt = ccp(0, size.height / 2);
            
            auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 100), size.width, size.height);
            colorLayer->setAnchorPoint(ccp(0, 0));
            addChild(colorLayer);
        }else{
            if(isJP()){

            CCLoadSprite::doLoadResource(LOADING_PATH, 1, true);
            auto spine1 = IFLoadingSceneArmyNode::create("Loading/Loading_1.atlas", "Spine/Loading/loading3.json", "loading3");
            this->addChild(spine1);
            if (CCCommonUtils::isIosAndroidPad())
            {
                spine1->setScale(2.25);
                spine1->setAnchorPoint(ccp(0, 0));
                spine1->setPosition(ccp(70, -200));
            }
           
            
            auto fireParticle1 = CCNode::create();
            this->addChild(fireParticle1);
            
            auto spine2 = IFLoadingSceneArmyNode::create("Loading/Loading_1.atlas", "Spine/Loading/loading2.json", "loading2");
            this->addChild(spine2);

            if (CCCommonUtils::isIosAndroidPad())
            {
                spine2->setScale(2.25);
                spine2->setAnchorPoint(ccp(0, 0));
                spine2->setPosition(ccp(100, -250));
            }
            
            auto fireParticle2 = CCNode::create();
            this->addChild(fireParticle2);

            auto spine3 = IFLoadingSceneArmyNode::create("Loading/Loading_1.atlas", "Spine/Loading/sj.json", "animation");
            this->addChild(spine3);

            if (CCCommonUtils::isIosAndroidPad())
            {
                spine3->setScale(2.25);
                spine3->setAnchorPoint(ccp(0, 0));
                spine3->setPosition(ccp(100, -250));
            }
            
            auto fireParticle3 = CCNode::create();
            this->addChild(fireParticle3);
            
            addFireParticle(fireParticle1, "LoadingFireNew_1", ccp(size.width / 2, 570));
            addFireParticle(fireParticle2, "LoadingFireNew_2", ccp(size.width / 2, 280));
            addFireParticle(fireParticle3, "LoadingFireNew_3", ccp(size.width / 2, 0));
            if (CCCommonUtils::isIosAndroidPad())
            {
                fireParticle1->setScale(2.f);
                fireParticle2->setScale(2.f);
                fireParticle3->setScale(2.f);
            }
            addFireParticle(fireParticle3, "LoadingFireNew_4", ccp(size.width / 2, 570));
            if (CCCommonUtils::isIosAndroidPad())
            {
                fireParticle3->setScale(2.f);
            }
            addFireParticle(fireParticle3, "LoadingFireNew_5", ccp(size.width * 3, 570));
            if (CCCommonUtils::isIosAndroidPad())
            {
                fireParticle3->setScale(2.f);
            }
            addFireParticle(fireParticle3, "LoadingFireNew_6", ccp(size.width / 2, 0));
            if (CCCommonUtils::isIosAndroidPad())
            {
                fireParticle3->setScale(2.f);
            }
            addFireParticle(fireParticle3, "LoadingFireNew_7", ccp(size.width / 2, 0));
            if (CCCommonUtils::isIosAndroidPad())
            {
                fireParticle3->setScale(2.f);
            }
            addFireParticle(fireParticle3, "LoadingFireNew_8", ccp(size.width / 2, 0));
            if (CCCommonUtils::isIosAndroidPad())
            {
                fireParticle3->setScale(2.f);
            }
            addFireParticle(fireParticle3, "LoadingFireNew_9", ccp(size.width / 2, 0));
            if (CCCommonUtils::isIosAndroidPad())
            {
                fireParticle3->setScale(2.f);
            }
            }else{
                auto loadingBG = CCLoadSprite::createSprite("loading.png");
                loadingBG->setAnchorPoint(ccp(0, 1));
                loadingBG->setScale(640.0 / loadingBG->getContentSize().width);
                loadingBG->setPositionY(size.height);
                if(CCCommonUtils::isIosAndroidPad())
                {
                    loadingBG->setScale(1536/653.0);
                }
                
                this->addChild(loadingBG);

                m_rollingCloud = RollingSprite::create("cloud_ani.png", 30.0);
                this->addChild(m_rollingCloud);
                m_rollingCloud->setPosition(loadingBG->getPosition());
                
                auto fireParticle = CCNode::create();
                this->addChild(fireParticle);
                addFireParticle(fireParticle, "LoadingMan_cloud", ccp(size.width / 2, size.height - 130));
                
                auto kingSpine = IFLoadingSceneArmyNode::create("Loading/Loading_3.atlas", "Spine/Loading/loading.json", "loading", 0.5);
                this->addChild(kingSpine);
                kingSpine->setPositionY(1050);
                kingSpine->setPositionX(-55);
                if(CCCommonUtils::isIosAndroidPad())
                {
                    kingSpine->setScale(2.18);
                    kingSpine->setPosition(ccp(10,2050));
                }
            }
        }
		addChild(label);
        
        m_loadPerLabel = CCLabelIF::create("0%");
        m_loadPerLabel->setFntFile("Arial_Bold_Border.fnt");
        m_loadPerLabel->setColor({255,255,255});
        m_loadPerLabel->setFontSize(24);
        m_loadPerLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
        m_loadPerLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        m_loadPerLabel->setAnchorPoint(ccp(0.5, 0.5));
        m_loadPerLabel->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,30)+ addPt);
//        addChild(m_loadPerLabel);
        m_loadPerLabel->setTag(LOADING_4);
        
        float _tipX = CCDirector::sharedDirector()->getWinSize().width/2-40;
        m_loadtips = CCLabelIF::create(_lang("E100074").c_str());
        m_loadtips->setFntFile("Arial_Bold_Border.fnt");
        m_loadtips->setColor({236, 186, 135});
        m_loadtips->setFontSize(20);
        m_loadtips->setHorizontalAlignment(kCCTextAlignmentCenter);
        m_loadtips->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        m_loadtips->setAnchorPoint(ccp(0.5, 0.5));
        m_loadtips->setPosition(ccp(_tipX,30) + addPt);
        m_loadtips->setVisible(false);
        addChild(m_loadtips);
        
        int __height = 30;
        std::string language = LocalController::shared()->getLanguageFileName();
        if (!CCLabelIF::canBeSupportedWithBmpFont(language))
        {
            __height = -20;
        }

        
        m_FAQ = CCLabelIF::create(_lang("E100075").c_str());
        m_FAQ->setFntFile("Arial_Bold_Border.fnt");
        m_FAQ->setColor(ccGREEN);
        m_FAQ->setFontSize(20);
        m_FAQ->setHorizontalAlignment(kCCTextAlignmentCenter);
        m_FAQ->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        m_FAQ->setAnchorPoint(ccp(0.5, 0.5));
        m_FAQ->setPosition(ccp(_tipX + m_loadtips->getContentSize().width/2 - __height,30) + addPt);
        m_FAQ->setVisible(false);
        addChild(m_FAQ);
        
        m_line = CCLoadSprite::createSprite("FAQ_line.png");
        m_line->setAnchorPoint(ccp(0.5, 0));
        m_line->setScaleX(m_FAQ->getContentSize().width/m_line->getContentSize().width-5);
        m_line->setPosition(ccp(_tipX + m_loadtips->getContentSize().width/2 - __height,33-m_FAQ->getContentSize().height/2) + addPt);
        m_line->setVisible(false);
        addChild(m_line);
        
        m_FAQTouchNode = CCNode::create();
        m_FAQTouchNode->setContentSize(Size(m_FAQ->getContentSize().width +10, m_FAQ->getContentSize().height+10));
        m_FAQTouchNode->setAnchorPoint(ccp(0.5, 0.5));
        m_FAQTouchNode->setPosition(ccp(_tipX + m_loadtips->getContentSize().width/2,30) + addPt);
        m_FAQTouchNode->setScale(2);
        addChild(m_FAQTouchNode);
        
        //        CCSize lbsz = loadingBar->getContentSize();
        CCSize lbBGsz = loadingBar->getContentSize();

        loadingBar->setPosition(ccp(size.width/2,lbBGsz.height*3) + addPt);
        addChild(loadingBar);
        
        CCSize lpsz = m_loadingProgress->getContentSize();
        m_barSize = lpsz+CCSizeMake(0, 30);
//        if (CCCommonUtils::isIOSPad() || CCCommonUtils::isAdriodPad())
//            m_barSize = lpsz*2+CCSizeMake(0, 30);
        
        m_loadingProgress->setAnchorPoint(ccp(0,0));
        m_loadingProgress->setPosition(ccp(0, lpsz.height/2));
        m_barClipNode = CCClipNode::create(m_barSize.width, m_barSize.height);
        m_barClipNode->addChild(m_loadingProgress);
        m_barClipNode->setPosition(ccp((size.width-lpsz.width)/2,lbBGsz.height*3-lpsz.height) + addPt);
        m_loadingProgress->setContentSize(CCSizeMake(m_barSize.width*0.1, m_barSize.height));
        addChild(m_barClipNode);
        
        loadingBarBG->setPosition(ccp(size.width/2,lbBGsz.height*3) + addPt);
        addChild(loadingBarBG);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            loadingBarBG->setPosition(ccp(size.width/2,lbBGsz.height*3*2.4) + addPt);
            loadingBarBG->setScale(2.f);
            
            m_barClipNode->setPosition(ccp((size.width-lpsz.width*2)/2.0,(lbBGsz.height*3-lpsz.height)*2.4) + addPt);
            m_barClipNode->setScale(2.f);
            
            loadingBar->setPosition(ccp(size.width/2,lbBGsz.height*3*2.2) + addPt);
            loadingBar->setScale(2.f);
        }
        
        //加流动的光
        m_clipNode = CCClipNode::create(m_barClipNode->getContentSize().width, m_barClipNode->getContentSize().height);
        m_clipNode->setAnchorPoint(ccp(0, 0));
        m_clipNode->setPosition(ccp((size.width-lpsz.width)/2,lbBGsz.height*3-30) + addPt);
        
        auto tmpSpr = CCLoadSprite::createSprite("Loading_bar_glow.png");
        tmpSpr->setTag(LOADING_GLOW);
        tmpSpr->setAnchorPoint(ccp(0, 0));
        tmpSpr->setPosition(ccp(-tmpSpr->getContentSize().width, 8) + addPt);
        m_clipNode->addChild(tmpSpr);
        CCActionInterval * move1 = CCMoveTo::create(1.5, ccp(lpsz.width,8));
        CCActionInterval * move2 = CCMoveTo::create(0, ccp(-tmpSpr->getContentSize().width,8));
        tmpSpr->runAction(CCRepeatForever::create(CCSequence::create(move1,move2,NULL)));
        
        addChild(m_clipNode);
       
        if (CCCommonUtils::isIosAndroidPad())
        {
//            tmpSpr->setPosition(ccp(-tmpSpr->getContentSize().width, 8) + addPt);
            m_clipNode->setPosition(ccp((size.width-lpsz.width*2)/2,(lbBGsz.height*3-30)*2.4) + addPt);
            m_clipNode->setScale(2.f);
        }
        
        //加进度条头上的光效
        m_headParticleNode = CCNode::create();
        m_headParticleNode->setPosition(ccp((size.width-lpsz.width)/2,lbBGsz.height*3) + addPt);
        for (int i=1; i<=3; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Loading_%d",i)->getCString());
            m_headParticleNode->addChild(particle);
        }
        addChild(m_headParticleNode);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_headParticleNode->setPosition(ccp((size.width-lpsz.width)/2,lbBGsz.height*3*2.35) + addPt);
            m_headParticleNode->setScale(2.f);
        }
        
        
        
        
        if(isJP()){
            auto logFireNode1 = CCNode::create();
            this->addChild(logFireNode1);
            addFireParticle(logFireNode1, "LoadingLOGO_1", ccp(225, size.height - 170));
            addFireParticle(logFireNode1, "LoadingLOGO_1", ccp(430, size.height - 230));

            //logo
            auto logo = CCLoadSprite::createSprite("logo.png");
            logo->setAnchorPoint(ccp(0.5, 0.5));
        
            logo->setPosition(ccp(size.width/2, size.height - 200));// -lpsz.height/2
            logo->setTag(LOADING_LOGO_TAG);
            addChild(logo);
        
            if (CCCommonUtils::isIosAndroidPad())
            {
                logo->setScale(1.5f);
                logo->setPosition(ccp(size.width/2, size.height - 300));// -lpsz.height/2
            }
        
            auto logFireNode2 = CCNode::create();
            this->addChild(logFireNode2);
            addFireParticle(logFireNode2, "LoadingLOGO_2", ccp(225, size.height - 180));
            addFireParticle(logFireNode2, "LoadingLOGO_2", ccp(430, size.height - 240));
        }else{
            //logo
            auto logo = CCLoadSprite::createSprite("logo.png");
            logo->setAnchorPoint(ccp(0.5, 0));
            
            logo->setPosition(ccp(size.width/2, 150));//
            logo->setTag(LOADING_LOGO_TAG);
            addChild(logo);
            
            auto logFireNode1 = CCNode::create();
            this->addChild(logFireNode1);
//            addFireParticle(logFireNode1, "LoadingMan_fire", ccp(320, 600));
            if (CCCommonUtils::isIosAndroidPad())
            {
                logFireNode1->setScale(2.4);
                logo->setScale(1.7f);
                logo->setPositionY(600);
            }

            auto logFireNode2 = CCNode::create();
            this->addChild(logFireNode2);
//            addFireParticle(logFireNode2, "LoadingMan_cold", ccp(200, logo->getPositionY() + logo->getContentSize().height / 4));
            if (CCCommonUtils::isIosAndroidPad())
            {
                logFireNode2->setScale(2.4);
            }
        }
//
        auto tipsBg = CCLoadSprite::createSprite("loading_tips_bg.png");
        tipsBg->setTag(LOADING_3);
        tipsBg->setAnchorPoint(ccp(0.5, 0.5));
        tipsBg->setScale(1.2f);
        tipsBg->setPosition(ccp(size.width/2,m_barClipNode->getPositionY()+ 78));// -lpsz.height/2
        
        addChild(tipsBg);

        
        auto tipsSize = CCSizeMake(tipsBg->getContentSize().width, tipsBg->getContentSize().height);
        
        m_loadingTips = CCLabelIF::create("loading...");
        m_loadingTips->setColor({152,137,103});
        m_loadingTips->setFontSize(24);
        m_loadingTips->setDimensions(CCSizeMake(tipsSize.width*0.85, tipsSize.height*1.8));
        m_loadingTips->setHorizontalAlignment(kCCTextAlignmentCenter);
        m_loadingTips->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        m_loadingTips->setAnchorPoint(ccp(0.5, 0.5));
        m_loadingTips->setPosition(ccp(tipsBg->getPosition().x, tipsBg->getPosition().y));
        addChild(m_loadingTips);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            tipsBg->setScale(2.4);
            tipsBg->setPosition(ccp(size.width/2,(m_barClipNode->getPositionY()+ 78)*2.f));
            
            m_loadingTips->setFontSize(48);
            m_loadingTips->setDimensions(CCSizeMake(tipsSize.width*0.85*2.f, tipsSize.height*1.8));
            m_loadingTips->setPosition(ccp(tipsBg->getPosition().x, tipsBg->getPosition().y*1.f));
        }
        
        if(CCFileUtils::sharedFileUtils()->isFileExist("platform_logo.png")) {
            // 渠道logo
            m_platformLogo = CCSprite::create("platform_logo.png");
            if(m_platformLogo) {
                CCSize size = CCDirector::sharedDirector()->getWinSize();
                m_platformLogo->setPosition(ccp(size.width/2, size.height/2));
//                m_platformLogo->setScaleX(size.width / 640);
//                m_platformLogo->setScaleY(size.height / 852);
                addChild(m_platformLogo);
            }
        }
        setCleanFunction([](){
            CCLoadSprite::releaseResourceBySceneId(SCENE_ID_LOADING);
        });
//        CCCommonUtils::recordStepByHttp("4");
        bRet = true;
    }
    this->scheduleOnce(schedule_selector(LoadingScene::show45SecondAlert), 180);
    CCLOG("test init end");
    return bRet;
}

bool LoadingScene::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_FAQ->isVisible() && isTouchInside(m_FAQTouchNode, pTouch)) {
        
        onGoToHelpShift(NULL);
        //PopupViewController::getInstance()->addPopupInView(SuggestionView::create());
        return true;
    }
    
    return false;
}
void LoadingScene::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void LoadingScene::onGoToHelpShift(CCObject* p)
{
    ValueMap meta;
    meta["loadingLog"] = GameController::getInstance()->getLoadingLogString();
    HelpshiftCocos2dx::addProperties(meta);
    
    ValueVector tags;
    tags.push_back(Value("loading"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    tags.push_back(Value("ios"));
#else
    if(GlobalData::shared()->analyticID != "market_global")
        tags.push_back(Value(GlobalData::shared()->analyticID));
#endif
    meta[HS_TAGS_KEY] = tags;
    
    ValueMap config;
    config[HS_META_DATA_KEY] = meta;
    GlobalData::shared()->isBind = true;
    HelpshiftCocos2dx::showConversation(config);
    postLoadingLogToServer(NULL);
}

void LoadingScene::hideLoading( )
{
    CCLog("LoadingScene hideLoading" );
    
    auto loadingPtr = getChildByTag(LOADING_1);
    if( loadingPtr ){
        loadingPtr->setVisible(false);
    }
    
    loadingPtr = getChildByTag(LOADING_2);
    if( loadingPtr ){
        loadingPtr->setVisible(false);
    }
    
    if ( m_clipNode ){
        loadingPtr = m_clipNode->getChildByTag(LOADING_GLOW);
        if( loadingPtr ) loadingPtr->setVisible(false);
    }
    
    if(m_loadingProgress){
        m_loadingProgress->setVisible(false);
    }
    
    if( m_headParticleNode ){
        m_headParticleNode->setVisible(false);
    }
    
    
}

void LoadingScene::postLoadingLogToServer(CCObject* p){
    std::string uid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID, "");
    string deviceId = cocos2d::extension::CCDevice::getDeviceUid();
    std::string country = GlobalData::shared()->fromCountry;
    std::string platform = GlobalData::shared()->analyticID;
    std::string version = cocos2d::extension::CCDevice::getVersionName();
    std::string step = GameController::getInstance()->getCokLoadingLogString(); //getLoadingLogString();
    if(p){
        CCString* _str = dynamic_cast<CCString*>(p);
        step  += "|"+ string(_str->getCString()) + "|";
    }
//    std::vector<std::string> vector;
//    cocos2d::extension::CCDevice::getLocalIP(vector);
    std::string ip = ""; // 手机IP
//    if (vector.size() > 0) {
//        ip = vector[0];
//    }
    std::string serverIp = NetController::shared()->getRealIp();
    std::string serverZone = NetController::shared()->getZone();
    int serverPort = NetController::shared()->getRealPort();
    std::string connection = CCString::createWithFormat("%s:%d:%s", serverIp.c_str(), serverPort, serverZone.c_str())->getCString();
    
    CCHttpRequest* request = new CCHttpRequest();
    std::string param = CCString::createWithFormat("uid=%s&deviceId=%s&country=%s&ip=%s&version=%s&platform=%s&step=%s&connection=%s",
                                                   uid.c_str(),
                                                   deviceId.c_str(),
                                                   country.c_str(),
                                                   ip.c_str(),
                                                   version.c_str(),
                                                   platform.c_str(),
                                                   step.c_str(),
                                                   connection.c_str())->getCString();
//    CCString *url = CCString::createWithFormat("http://10.1.5.196:9000/client/loading");
    CCString *url = CCString::createWithFormat("http://public.cok.elexapp.com/client/loading");
    CCLog("loadingLog:%s", url->getCString());
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    const char* postData = param.c_str();
    request->setRequestData(postData, strlen(postData));

    CCHttpClient::getInstance()->send(request);
    request->release();
}

void LoadingScene::addParticle(){
    ParticleController::initParticle();
//    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
//    auto newBatch = ParticleController::createParticleBatch();
//    auto particle1 = ParticleController::createParticle("LoadFire_1");
//    particle1->setPosition(ccp(winsize.width / 2, 0));
//    newBatch->addChild(particle1);
//    auto particle2 = ParticleController::createParticle("LoadFire_2");
//    particle2->setPosition(ccp(winsize.width / 2, 0));
//    newBatch->addChild(particle2);
//    this->addChild(newBatch);
//    auto newBatch1 = ParticleController::createParticleBatch();
//    auto particle3 = ParticleController::createParticle("LoadFire_3");
//    particle3->setPosition(ccp(winsize.width / 2, 0));
//    newBatch1->addChild(particle3);
//    this->addChild(newBatch1);
}
static std::thread* loadWorldMap = NULL;

void LoadingScene::onEnter(){
    CCLayer::onEnter();
    
    //setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    //setTouchEnabled(true);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doLogin), MSG_GameStart, NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInitError), MSG_LoginInitError, NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInitBan), MSG_LoginInitBan, NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doResourceUpdate), MSG_LoginSucess, NULL);//
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInit), MSG_LoginInit, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::stepLoading), MSG_INIT_FINISH, NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::onRelogin), MSG_RELOGIN, NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::onEnterFrame), MSG_SHOW_LOADINGS_FQA, NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::checkCanGotoMainScene), PERMITION_CHECK_CONFIRM,NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::onNetworkAvailable), "NetworkAvailable", NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::postLoadingLogToServer), "postLoadingLogToServer", NULL);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LoadingScene::onServerUpdate), MSG_SERVER_UPDATE, NULL);//
  
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LoadingScene::onEnterFrame), this, 30.0,kCCRepeatForever, 0.0f, false);
 //   CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    //music
//    if( NULL == loadWorldMap )
//    {
//        if( TMXTiledMap::getCacheMapInfo("WorldMap.tmx") == NULL ||
//            TMXTiledMap::getCacheMapInfo("WorldMap1.tmx") == NULL )
//        {
//            //loadWorldMap = new std::thread([](){
//                TMXMapInfo* mapInfo = new TMXMapInfo();
//                if(mapInfo->initWithTMXFile("WorldMap.tmx"))
//                {
//                    TMXTiledMap::setCacheMapInfo("WorldMap.tmx", mapInfo);
//                }
//                else {
//                    mapInfo->release();
//                }
//                TMXMapInfo* mapInfo1 = new TMXMapInfo();
//                if(mapInfo1->initWithTMXFile("WorldMap1.tmx")) {
//                    TMXTiledMap::setCacheMapInfo("WorldMap1.tmx", mapInfo1);
//                }
//                else {
//                    mapInfo1->release();
//                }
//            //});
//        }
//
//    }

//    if (GlobalData::shared()->clearCache_switch)  // 3Dtouch  清理缓存
//    {
        std::string _shortItemType = GlobalData::shared()->shortItemType;
        if (!_shortItemType.compare(shortItemType_clearCache) )
        {
            //            CCCommonUtils::clearGameCache();
            auto _dialog = YesNoDialog::show(_lang("139505").c_str()
                                             , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::clearCacheOK), NULL)
                                             , 0
                                             , true
                                             , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::clearCacheCancel), NULL)
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("101274").c_str());
            _dialog->setNoButtonTitle(_lang("108532").c_str());
            
            return;
            
        }
//    }
    
//    初始化的数据移到下面的方法
    onEnterInit();
}

void LoadingScene::onEnterInit()
{
    GlobalData::shared()->analyticID = cocos2d::extension::CCDevice::getPublishChannel();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GlobalData::shared()->fromCountry = AppLibHelper::getLocalCountry();
#endif
    if (GlobalData::shared()->needLogin()) {
        if(GlobalData::shared()->analyticID != "cn_mi"
           && GlobalData::shared()->analyticID != "cn_mihy"
           ){
            GlobalData::shared()->platformUID = "";
            //重登时候清除掉悬浮窗等
            cocos2d::extension::CCDevice::doPlatformLoading();
        }
    }
    
    if(isChina() && !m_isAdd){
        CCSprite *logo = dynamic_cast<CCSprite*>(this->getChildByTag(LOADING_LOGO_TAG));
        if(logo){
            logo->setDisplayFrame(CCLoadSprite::loadResource("logo_only.png"));
            logo->setPositionY(logo->getPositionY() + 40);
        }
        auto layer = CCLayerColor::create(ccc4(0, 0, 0, 200), CCDirector::sharedDirector()->getWinSize().width, 70);
        this->addChild(layer);
        layer->setAnchorPoint(ccp(0.5, 0));
        
        auto label = CCLabelIF::create(_lang("4100009").c_str());
        this->addChild(label);
        label->setDimensions(CCSize(CCDirector::sharedDirector()->getWinSize().width - 16, 0));
        label->setColor(ccc3(141, 94, 75));
        label->setFontSize(21);
        label->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 35));
        label->setAlignment(kCCTextAlignmentCenter);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        
        auto addY = [](CCNode *node){
            if(node){
                int add = 30;
                if (CCCommonUtils::isIosAndroidPad())
                {
                    add *= 2.f;
                }
                node->setPositionY(node->getPositionY() + add);
            }
        };
        addY(m_loadPerLabel);
        addY(m_loadtips);
        addY(m_FAQ);
        addY(m_line);
        addY(m_FAQTouchNode);
        addY(m_barClipNode);
        addY(m_clipNode);
        addY(m_headParticleNode);
        addY(m_loadingTips);
        addY(this->getChildByTag(LOADING_1));
        addY(this->getChildByTag(LOADING_2));
        addY(this->getChildByTag(LOADING_3));
        addY(this->getChildByTag(LOADING_4));
    }
    
    m_tipCount = 1;
    m_tipDialog = "";
    setLoadingTips();
    LoadingScene::showLoading();
    
    //为月卡做记录
    //    int loadCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("day_load_count");
    //    loadCount+=1;
    //    CCUserDefault::sharedUserDefault()->setIntegerForKey("day_load_count", loadCount);
    //    CCUserDefault::sharedUserDefault()->flush();
    schedule(schedule_selector(LoadingScene::loadingRest), 0.2f);
    schedule(schedule_selector(LoadingScene::updateLoadingTip), 7.0f);
    CCLog("LoadingScene onEnterInit" );
}

void LoadingScene::clearCacheOK(CCObject *obj)
{
    GlobalData::shared()->shortItemType = "";
    CCCommonUtils::clearGameCache();
//    onEnterInit();
    GameController::getInstance()->quitGame();
}

void LoadingScene::clearCacheCancel(CCObject *obj)
{
    GlobalData::shared()->shortItemType = "";
    onEnterInit();
}

void LoadingScene::onExit(){

    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "NetworkAvailable");//
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
    this->unschedule(schedule_selector(LoadingScene::resendGetServerList));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_LoginInitError);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_LoginSucess);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GameStart);//
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_LoginInit);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INIT_FINISH);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RELOGIN);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_LOADINGS_FQA);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PERMITION_CHECK_CONFIRM);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SERVER_UPDATE);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_LoginInitBan);//
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "postLoadingLogToServer");//
    
    setTouchEnabled(false);
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(LoadingScene::onEnterFrame), this);
    IFSkeletonDataManager::shared()->purge();
    CCLayer::onExit();
    
    
    if( loadWorldMap != NULL ) {
        loadWorldMap->join();
        CC_SAFE_DELETE(loadWorldMap);
    }
}

void LoadingScene::onServerUpdate(CCObject* p)
{
    CCLog("LoadingScene onServerUpdate" );
    
    if( m_rollHint == nullptr )
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        m_rollHint = FlySystemUpdateHint::create(0,true,_lang("113864"),FLY_HINT_SERVER_STOP,"");
        if( m_rollHint )
        {
            addChild(m_rollHint);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_rollHint->setPosition(ccp(size.width*0.5, size.height-420));
            } else {
                m_rollHint->setPosition(ccp(size.width*0.5, size.height-210));
            }
        }
    }
    
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupView( ServerUpdateView::create() );
    
    hideLoading();
}

void LoadingScene::onEnterFrame(CCObject* p)
{
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(LoadingScene::onEnterFrame), this);

    GameController::getInstance()->setLoadingLog("LoadingScene", "onEnterFrame");
    
    m_loadtips->setVisible(true);
    m_FAQ->setVisible(true);
    m_line->setVisible(true);
    
    FBUtilies::addEventToFaceBook("LoadingScene_onEnterFrame");
    
    PopupViewController::getInstance()->removeAllPopupView();
    auto popupview = PopupViewController::getInstance()->getCurrentPopupView();
    auto yesnoView = dynamic_cast<YesNoDialog*>(popupview);
    if(!yesnoView)
    {
        YesNoDialog::showVariableTitle(_lang("E100085").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL),_lang("E100075").c_str(),false);
    }
}

void LoadingScene::setLoadingTips(){
    // get tips content
    //int playLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey(ACCOUNT_USER_LEVEL,0);
    int level = CCUserDefault::sharedUserDefault()->getIntegerForKey(FUN_BUILD_MAIN_LEVEL);
    if(level<=0){
        level = 1;
    }
    auto tipsDict = LocalController::shared()->DBXMLManager()->getGroupByKey("loadingTips");
    if(!tipsDict){
        return;
    }
    m_loadingtips.clear();
    CCDictElement *pObj;
    string loadingTip = "";
    CCDICT_FOREACH(tipsDict, pObj) {
        auto infoDict = _dict(pObj->getObject());
        auto levelRange = string(infoDict->valueForKey("level_range")->getCString());
        auto id = string(infoDict->valueForKey("id")->getCString());
        auto description = string(infoDict->valueForKey("description")->getCString());
        
        vector<string> levels;
        CCCommonUtils::splitString(levelRange, ";", levels);
        if( levels.size()<2 )
            continue;
        
        if (level < atoi(levels[0].c_str()) || level > atoi(levels[1].c_str())) {
            continue;
        }
        CCCommonUtils::splitString(description, ";", m_loadingtips);
    }
    if( m_loadingtips.size()<=0 )
        return;
    
    srand(time(NULL));
    if(m_loadingtips.size()>0){
        int index = rand()%m_loadingtips.size();
        loadingTip = _lang(m_loadingtips[index].c_str());
        CCLOG("index=%d",index);
        m_tipDialog = m_loadingtips[index];
        m_loadingTips->setString(loadingTip.c_str());
    }
}

void LoadingScene::updateLoadingTip(float _time){
    m_tipCount += 1;
    if(m_loadingtips.size()>1 && m_loadingTips){
        int index = 0;
        string loadingTip = "";
        for (int i=0; i<50; i++) {
            index = rand()%m_loadingtips.size();
            string dialog = m_loadingtips[index];
            int pos = m_tipDialog.find(dialog);
            if (pos==-1) {
                loadingTip = _lang(m_loadingtips[index].c_str());
                m_tipDialog.append(",");
                m_tipDialog.append(dialog);
                break;
            }
        }
        if(loadingTip!=""){
            m_loadingTips->setString(loadingTip.c_str());
        }
    }
    if (m_tipCount==m_loadingtips.size()) {
        m_tipDialog = "";
        m_tipCount= 0;
    }
}

void LoadingScene::doLoginInit(float t)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginInit");
    if (!GlobalData::shared()->isInitFlag && !GlobalData::shared()->isInDataParsing)
    {
        //loadingLog统计
//        if(GlobalData::shared()->analyticID == "cn1" && NetController::shared()->getConnectType() == "lx"){
//            NetController::shared()->setCloseLXConnection(true);
//            NetController::shared()->disconnect();
//        }
        CCLog( "zym doLoginInit");
        GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginInit-false");
        InitCommand* cmd=new InitCommand();
        cmd->sendAndRelease();
    }
    else
    {
        unschedule(schedule_selector(LoadingScene::doLoginInit));
    }
    //CCLOG("zymcreateserver");
    //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification( MSG_SERVER_UPDATE );
}

void LoadingScene::doLoginInitError(CCObject* p){
    unschedule(schedule_selector(LoadingScene::doLoginInit));
//    YesNoDialog::show(_lang("114141").c_str(), NULL);
    
    PopupViewController::getInstance()->removeAllPopupView();
    YesNoDialog::showVariableTitle(_lang("E100211").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL),_lang("E100075").c_str(),false);
}

void LoadingScene::doLoginInitBan(CCObject* p)
{
    unschedule(schedule_selector(LoadingScene::doLoginInit));
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
}

void LoadingScene::show45SecondAlert(float time){
    
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
    
    PopupViewController::getInstance()->removeAllPopupView();
    
    FBUtilies::addEventToFaceBook("LoadingScene_show45SecondAlert");
    
    auto _dialog = YesNoDialog::show(_lang("E100093").c_str()
                                     , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::retry), NULL)
                                     , 0
                                     , true
                                     , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL)
                                     );
    _dialog->showCancelButton();
    _dialog->setYesButtonTitle(_lang("105248").c_str());
    _dialog->setNoButtonTitle(_lang("E100075").c_str());
}

void LoadingScene::retry(CCObject *obj){
   
    selectLogin(0.0); //add by hujiuixng
}

void LoadingScene::doLogin(CCObject* p)
{
    //loadingLog统计
    
 
    //GameController::getInstance()->setLoadingLog("LoadingScene", "doLogin");
    
    
    sendCmdLogin();
}

void LoadingScene::sendCmdLogin()
{
    CCLog("in sendCmdLogin");
    schedule(schedule_selector(LoadingScene::onLoginTimeout), 0.5);
    // password retry count reset
    GlobalData::shared()->isLoginFlag = true;
    
    if( NetController::shared()->getIslogining() )
    {
        CCLog("zym doLogin skip");
        GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginskip");
        //zym  测试一个
        if( GlobalData::shared()->analyticID == "cn1" )
            return;
    }
    else
        GameController::getInstance()->setLoadingLog("LoadingScene", "doLogin");
    
    NetController::shared()->setIslogining(true);
    
    NetController::shared()->clearRequestQueue();
    LoginCommand* cmd=new LoginCommand();
    cmd->setDefaultUserName();
    cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onConnectionLost), NULL));
    cmd->sendAndRelease();
}

void LoadingScene::onLoginTimeout(float t)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onLoginTimeout");
    unschedule(schedule_selector(LoadingScene::onLoginTimeout));
    CCHttpRequest* request = new CCHttpRequest();
    CCString* url;
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    string _platformUID = GlobalData::shared()->platformUID;
    string _Country = GlobalData::shared()->fromCountry;
    string _gameUid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
    string _lang = LocalController::shared()->getLanguageFileName();
    string _serverId = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID, "");
    string _serverIp = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP, "");
#if COCOS2D_DEBUG == 1
    url = CCString::createWithFormat("http://10.1.6.72:8081/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s",_uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
#else
#if COCOS2D_DEBUG == 2
    url = CCString::createWithFormat("http://p1.cok.elexapp.com/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s",_uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
#else
    url = CCString::createWithFormat("http://p1.cok.elexapp.com/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s",_uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
#endif
#endif
    
    CCLOG("get_server_status list URL: %s",url->getCString());
    
    
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(LoadingScene::onGetServerStatus));
    request->setTag("get_server_status");
    CCHttpClient::getInstance()->setTimeoutForConnect(15);
    CCHttpClient::getInstance()->send(request);
    request->release();
    
}

void LoadingScene::onGetServerStatus(CCHttpClient* client, CCHttpResponse* response)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerStatus");
    if (!response)
    {
        return;
    }

    m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
        string _tmp = response->getErrorBuffer();
        GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerStatus2-" + _tmp);
//        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), NULL),_lang("105248").c_str(),true);
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    Json *c = Json_create(serverInfo.c_str());
    if(!c){
        return;
    }
    int _code =Json_getInt(c,"cokerrcode",0);//新版用这个cokerrcode为0 和1  代替code 防止http 200也进来了
    if (_code != 0) {
        
        std::string _message= Json_getString(c, "message", "");
        if (_message == "")
        {
            _message = _lang("E100069");
          
        }
        int time = Json_getInt(c, "timelist", 0);
        auto popupview = PopupViewController::getInstance()->getCurrentPopupView();
        auto yesnoView = dynamic_cast<ServerUpdateView*>(popupview);
        if(!yesnoView)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification( MSG_SERVER_UPDATE );
             //YesNoDialog::showVariableTitle(_message.c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true,time);
            
        }
    }
    Json_dispose(c);
}

bool LoadingScene::showServiceTerms(){
    bool showServiceTerms = false;
    if (GlobalData::shared()->analyticID == "nstore"
        || GlobalData::shared()->analyticID == "tstore"){
        showServiceTerms = CCUserDefault::sharedUserDefault()->getStringForKey("service_terms_check","") == "";
    }
    return showServiceTerms;
}

bool LoadingScene::waitingForLogin(){
    if (GlobalData::shared()->needLogin() && GlobalData::shared()->platformUID == "") {
        return true;
    }
    return false;
}

//根据不同情况选择登录方式
void LoadingScene::selectLogin(float t){
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "selectLogin");
    //根据平台判断是否需要先登录平台账号
    bool loadingWait = waitingForLogin();
    //服务条款
    bool serviceTerms = showServiceTerms();
    if (loadingWait || serviceTerms){
        CCLOG("select login wait");
        if (!m_platformLoginFlag) {
            CCLOG("select login strat schedule");
            m_platformLoginFlag = true;
            schedule(schedule_selector(LoadingScene::selectLogin), 0.5f);
            if(serviceTerms){
                vector<std::string> serviceTermsStr = AppLibHelper::getServiceTerms(1);
                vector<std::string> personalTermsStr = AppLibHelper::getServiceTerms(2);
                PopupViewController::getInstance()->addPopupView(KoreaPermitPopUPView::create("", "", serviceTermsStr, "", personalTermsStr));
            }
        }
        if(loadingWait){
            //解决百度初始化异步问题，由java控制是否调用登陆
            cocos2d::extension::CCDevice::doPlatformLogin();
        }
        return;
    }
    else if (m_platformLoginFlag)
    {
        m_platformLoginFlag = false;
        CCLOG("select login end schedule");
        unschedule(schedule_selector(LoadingScene::selectLogin));
    }
    //1.判断本地缓存
    string gameUID = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
    string _uuid = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_UUID,"");
    string _buildCode = CCUserDefault::sharedUserDefault()->getStringForKey(BUILDCODE,"");
   
#if DEBUG
    //zyt
//    GmController::getInstance()->gmUid = "138939645000001";
//    
//    GmController::getInstance()->isGmLogin = 1;
#endif
 
    if(gameUID == "" && _buildCode=="" && _uuid == "")
    {
        //2.根据URL请求，选择登录方式
        sendCmdGetServerList(NULL);
    }
    else
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GameStart);
    }
}

void LoadingScene::onRelogin(CCObject* p)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onRelogin");
    selectLogin(0.0);
}

void LoadingScene::sendCmdGetServerList(CCObject* p){
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "sendCmdGetServerList");

    CCHttpRequest* request = new CCHttpRequest();
    CCString* url;
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    string _platformUID = GlobalData::shared()->platformUID;
    string _platformToken = GlobalData::shared()->platformToken;
    string _Country = GlobalData::shared()->fromCountry;
    string param = "";
    
    auto currentTime = time(NULL);
    std::string timeStr = CC_ITOA(double(currentTime));

    std::string key = _uuid + timeStr + "@z4LugUwrDN0MqOJtzmHPK7Ss-k4taAUOZh1M3Yu9";
    MD5 md5(key);
    std::string md5Str = md5.toString();
    
    bool isChinaPhone = isChina();
    
    if (_platformUID != "") {
        string _PublishRegion = cocos2d::extension::CCDevice::getPublishChannel();
        param = CCString::createWithFormat("uuid=%s&loginFlag=%d&pfId=%s&pf=%s&token=%s&country=%s&t=%s&sig=%s",_uuid.c_str(),1,_platformUID.c_str(),_PublishRegion.c_str(),_platformToken.c_str(),_Country.c_str(),timeStr.c_str(),md5Str.c_str())->getCString();
#if COCOS2D_DEBUG == 1
        url = CCString::createWithFormat("http://%s:8081/gameservice/getserverlist.php?%s",SERVERLIST_IP,param.c_str());
#else
#if COCOS2D_DEBUG == 2
        url = CCString::createWithFormat("http://%s:8081/phpserver/gameservice/getserverlist.php?%s",SERVERLIST_IP,param.c_str());
#else
        if(isChinaPhone && m_getServerListRetryTime % 2 == 0){
            url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s","p3cok.elexapp.com",param.c_str());
        }else{
            url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s","p1cok.elexapp.com",param.c_str());
        }
#endif
#endif
    }
    else
    {
        param = CCString::createWithFormat("uuid=%s&loginFlag=%d&country=%s&t=%s&sig=%s",_uuid.c_str(),1,_Country.c_str(),timeStr.c_str(),md5Str.c_str())->getCString();
#if COCOS2D_DEBUG == 1
        url = CCString::createWithFormat("http://%s:8081/gameservice/getserverlist.php?%s",SERVERLIST_IP,param.c_str());
#else
#if COCOS2D_DEBUG == 2
        url = CCString::createWithFormat("http://%s:8081/phpserver/gameservice/getserverlist.php?%s",SERVERLIST_IP,param.c_str());
#else
        if(isChinaPhone && m_getServerListRetryTime % 2 == 0){
            url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s","p3cok.elexapp.com",param.c_str());
        }else{
            url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s","p1cok.elexapp.com",param.c_str());
        }
#endif
#endif
    }
    CCLOG("COK server list URL: %s",url->getCString());
    //url = CCString::createWithFormat("http://%s/gameservice/getserverlist.php?%s","p1cok.elexapp.com",param.c_str());
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(LoadingScene::onGetServerList));
    request->setTag("get_server_list");
    CCHttpClient::getInstance()->setTimeoutForConnect(30);
    CCHttpClient::getInstance()->send(request);
    request->release();
    m_getServerListRetryTime++;
    int maxTime = 3;
    if(m_getServerListRetryTime <= maxTime){
        this->scheduleOnce(schedule_selector(LoadingScene::resendGetServerList), 10);
    }
}

void LoadingScene::resendGetServerList(float time){
    sendCmdGetServerList(NULL);
}

void LoadingScene::onGetServerList(CCHttpClient* client, CCHttpResponse* response)
{
    this->unschedule(schedule_selector(LoadingScene::resendGetServerList));
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerList");
    
    if (!response)
    {
        PopupViewController::getInstance()->removeAllPopupView();
        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), NULL),_lang("105248").c_str(),true);
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
        if (strcmp("get_server_list",response->getHttpRequest()->getTag())!=0) {
            // return;
        }
    }
    m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
//        LocalController::shared()->init();
        
        // lzy ping玩家设备和服务器连接状态并将数据上传服务器
        string _uid = GlobalData::shared()->playerInfo.uid;
        CCCommonUtils::sendHelpshiftLog(NetController::shared()->getIp().c_str(), _uid.c_str(), "getIP-onGetServerList-failed");
        CCCommonUtils::sendHelpshiftLog(NetController::shared()->getRealIp().c_str(), _uid.c_str(), "getRealIP-onGetServerList-failed");
        
        PopupViewController::getInstance()->removeAllPopupView();
        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), NULL),_lang("105248").c_str(),true);
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    if(serverInfo == "")
    {
        CCLOG("response data is null!");
        PopupViewController::getInstance()->removeAllPopupView();
        YesNoDialog::showVariableTitle(_lang("E100092").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onGoToHelpShift), NULL),_lang("E100075").c_str(),false);
        return;
    }
    Json *c = Json_create(serverInfo.c_str());
    if(!c){
        return;
    }
    Json *serverList=Json_getItem(c,"serverList");
    
    if(!serverList){
        return;
    }
    int country=Json_getInt(c,"country",0);

    //get serverInfo
    Json *c2 = Json_getItemAt(serverList, 0);
    if(!c2){
        return;
    }
    string id = Json_getString(c2,"id","-1");
    string port = Json_getString(c2, "port","-1");
    string zone = Json_getString(c2, "zone","-1");
    string ip = Json_getString(c2, "ip","-1");
    string gameUid = Json_getString(c2, "gameUid", "");
    string _uuid = Json_getString(c2, "uuid", "");
    string _buildCode = Json_getString(c2, "buildCode", "");
    Json_dispose(c);
    
    GlobalData::shared()->loginServerInfo.recommandCountry = country;
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, _uuid);
    CCUserDefault::sharedUserDefault()->setStringForKey(SERVER_ID, id);
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, gameUid);
    CCUserDefault::sharedUserDefault()->setStringForKey(BUILDCODE, _buildCode);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
    
//#if COCOS2D_DEBUG != 0
//    测试用切换服务器
//    ip = "10.1.6.72";
//    zone = "COK1";
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "1386973893000001");
    
 //   ip = "169.45.155.123";
 //    zone = "COK";
 //   CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
 //    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
 //    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
 //   CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "401685474000000");//107
  //   ip = "s1.cok.elexapp.com";
 //    zone = "COK1";
 //    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
 //    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone.c_str());
  //  CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
  //   CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "2004014375000001");
//#endif
    
 
#if DEBUG
    //zyt
//                ip = "s804.cok.elexapp.com";
//                zone= "COK804";
//                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//                // CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,9933);
//                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "87401825000804");
    //习惯守候
    //    //
 //           ip = "s1171.cok.elexapp.com";
 //         zone= "COK1171";
 //       CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
 //       CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
 //       CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
 //      // CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,9933);
 //       CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "112312973001087");
//
//    //huchao
//        ip = "s532.cok.elexapp.com";
//        zone= "COK532";
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "5296a63618f73b1663a0d8c4314c5df1804ae9891437645242843");
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
////        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,9933);
//        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "909504798000489");
//
    
//       //wuni
//        ip = "s532.cok.elexapp.com";
//        zone= "COK532";
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "58a978fd14285b476fcd3f972b0ea140152133661437444127113");
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,9933);
//        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "1218517119000125");
    
//    //gavinZhang
//    ip = "s532.cok.elexapp.com";
//    zone= "COK532";
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//     CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "48481227000546");
    
//        //Gavin Blake
//        ip = "s1111.cok.elexapp.com";
//        zone= "COK1111";
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "1456785592000870");
    
//    ip = "s173.cok.elexapp.com";
//    zone= "COK173";
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "9c7dbd8ee8e57b64a13807fe10a57e27fbd9cc1f1427936700152");
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "29294154000235");
//         //跑起
//        ip = "s532.cok.elexapp.com";
//        zone = "COK532";
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "819031593000343");
//////
////        //飞起
//        ip = "s532.cok.elexapp.com";
//        zone = "COK532";
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//          CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,9933);
//        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "17547430000555");
    
//            ip = "s564.cok.elexapp.com";
//            zone = "COK564";
//            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//              CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//            CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,9933);
//            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "60926353000256");
    
//            ip = "s601.cok.elexapp.com";
//            zone = "COK601";
//            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//            CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,9933);
//            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "955406846000497");
//    
    
       //真空的灵魂
//    ip = "10.1.6.72";
//    zone = "COK1";
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//     CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "1C2BD6B8-291A-44E5-927E-C0E956B08440_1450338018");
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "1385806382000001");
//    CCUserDefault::sharedUserDefault()->flush();
    
//    ip = "10.1.6.72";
//    zone = "COK1";
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//     CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "1383239301000001");
////
    
//          //习惯
//        ip = "10.1.6.73";
//        zone = "COK11";
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "138701905000001");
//        CCUserDefault::sharedUserDefault()->flush();
 
    
//    ip = "10.1.6.72";
//    zone = "COK1";
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//     CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "1384819604000001");
//    CCUserDefault::sharedUserDefault()->flush();
    
//    138939645000001
//    1387736473000001
//        ip = "10.1.6.72";
//        zone = "COK1";
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
//         CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, "");
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
//        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(port.c_str()));
//        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, "138939645000001");
//        CCUserDefault::sharedUserDefault()->flush();
    
#endif
    
  
    CCUserDefault::sharedUserDefault()->flush();
    
    NetController::shared()->setIp(ip);
    
    NetController::shared()->setZone(zone);

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GameStart);
}

bool LoadingScene::isJP(){
    std::string country = GlobalData::shared()->fromCountry;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    country = AppLibHelper::getLocalCountry();
#endif
    if(country == "JA" || country == "ja"){
        return true;
    }
    return false;
}

void LoadingScene::loadingRest(float t)
{
    

    
//    CCThreadManager::getInstance()->runInNewThread(this, callfuncO_selector(LoadingScene::loadingRestAsyn),nullptr);
    unschedule(schedule_selector(LoadingScene::loadingRest));
    repeatTime = 0;

    if(CCCommonUtils::isUserNewUid()){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string _andriodOpenUid = cocos2d::extension::CCDevice::getCurrentAndroidUDID();
        CCString *android = CCString::createWithFormat("deviceId: %s", _andriodOpenUid.c_str());
        CCLog(android->getCString());
        if(_andriodOpenUid == ""){
            this->schedule(CC_SCHEDULE_SELECTOR(LoadingScene::doWhenUidBackFromCloud), 0.1, kCCRepeatForever, 0.0);
        }else{
            selectLogin(0.0);
        }
#else
        selectLogin(0.0);
#endif
    }else{
        selectLogin(0.0);
    }
//
//    
//    selectLogin(0.0);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    unzipLocalfileAsyn(0);
#endif

    loadingRestAsyn(nullptr);

}

void LoadingScene::doWhenUidBackFromCloud(float time){
    bool stopFlag = false;
    repeatTime++;
    int maxTime = 40;
    if(repeatTime > maxTime){
        stopFlag = true;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string _andriodOpenUid = cocos2d::extension::CCDevice::getCurrentAndroidUDID();
    CCString *android = CCString::createWithFormat("deviceId: %s", _andriodOpenUid.c_str());
    CCLog(android->getCString());
    if(_andriodOpenUid != ""){
        stopFlag = true;
    }
#endif
    
    if(stopFlag){
        this->unschedule(CC_SCHEDULE_SELECTOR(LoadingScene::doWhenUidBackFromCloud));
        selectLogin(0.0);
    }
}

void LoadingScene::unzipLocalfileAsyn(int type)
{
    DynamicResourceController::getInstance()->uncompressLocalZip(type);
}

void LoadingScene::loadingRestAsyn(Ref* arg)
{
    if( NULL == loadWorldMap )
    {
        CCLOG("loadingRestAsyn begin");
        if( TMXTiledMap::getCacheMapInfo("WorldMap.tmx") == NULL ||
           TMXTiledMap::getCacheMapInfo("WorldMap1.tmx") == NULL )
        {
            //loadWorldMap = new std::thread([](){
            TMXMapInfo* mapInfo = new TMXMapInfo();
            if(mapInfo->initWithTMXFile("WorldMap.tmx"))
            {
                TMXTiledMap::setCacheMapInfo("WorldMap.tmx", mapInfo);
            }
            else {
                mapInfo->release();
            }
            TMXMapInfo* mapInfo1 = new TMXMapInfo();
            if(mapInfo1->initWithTMXFile("WorldMap1.tmx")) {
                TMXTiledMap::setCacheMapInfo("WorldMap1.tmx", mapInfo1);
            }
            else {
                mapInfo1->release();
            }
            //});
            
            CCLOG("loadingRestAsyn end");
        }
        
    }
    int loadCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("day_load_count");
    loadCount+=1;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("day_load_count", loadCount);
//    CCUserDefault::sharedUserDefault()->flush();
}

void LoadingScene::showLoading()
{
    schedule(schedule_selector(LoadingScene::loadingAni), 0.167f);
//    selectLogin(0.0);
}

void LoadingScene::loadingAni(float t)
{
    
    
    if (m_steps == LOADING_STEP) {
        SoundController::sharedSound()->playBGMusic(Music_Sfx_logo_loading);
    }
    if(m_steps == 75 && (showServiceTerms() || waitingForLogin()))
        return;
    if (m_steps <= LOADING_STEP/2)
    {
        // lzy  内部服务器打点
        CCCommonUtils::postEventLogToServer("loadingScene_loadingHalf_1");
        unschedule(schedule_selector(LoadingScene::loadingAni));

    } else {
        float pro = (LOADING_STEP-m_steps+1)*1.0/(LOADING_STEP*1.0);
        string proInfo = CC_ITOA(pro*100);
        m_loadPerLabel->setString(proInfo+"%");
        float curX = m_barSize.width*pro;
        m_clipNode->setContentSize(CCSizeMake(curX, m_barSize.height));
        m_barClipNode->setContentSize(CCSizeMake(curX, m_barSize.height));
        if (CCCommonUtils::isIosAndroidPad())
        {
            curX *= 2.f;
        }
        m_headParticleNode->setPositionX(m_clipNode->getPositionX()+curX);
        if (m_steps == 65)
        {
            unschedule(schedule_selector(LoadingScene::loadingAni));
            schedule(schedule_selector(LoadingScene::loadingAni), 0.167f);
        }
        --m_steps;
        if(m_steps == 18 && m_platformLogo){
            m_platformLogo->removeFromParent();
            m_platformLogo = NULL;
        }
    }
}

void LoadingScene::parseXML() {
    return;
//    LocalController::shared()->init();
//    GlobalData::shared()->isXMLInitFlag = true;
//    doLoginInit();
}

void LoadingScene::stepLoading(CCObject* p)
{
    //loadingLog统计
    time_t worldTime = GlobalData::shared()->getWorldTime();
    GameController::getInstance()->setLoadingLog("LoadingScene", CCString::createWithFormat("stepLoading:serverTime:%s ",CCCommonUtils::timeStampToDate(worldTime).c_str())->getCString());
    unschedule(schedule_selector(LoadingScene::doLoginInit));
    //提示更新版本
    std::string str;
    GlobalData::shared()->lang = cocos2d::extension::CCDevice::getLanguage();
    GlobalData::shared()->platform = "ios";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    GlobalData::shared()->platform = "android";
    //根据平台获得提示语
    if(GlobalData::shared()->analyticID == "market_global"){
        str = "GOOGLE";
    }else{
        /**
         tstore
         nstore
         amazon
         cafebazaar
         **/
        str = GlobalData::shared()->analyticID;
    }
#else
#ifdef Channel_91
    str = "91Store";
    GlobalData::shared()->analyticID = "91Store";
#else
    str = "AppStore";
    GlobalData::shared()->analyticID = "AppStore";
#endif
#endif
    if (GlobalData::shared()->updateType==1)
    {
        auto _dialog = YesNoDialog::show(CCString::createWithFormat(_lang("login_error_client_ver_must").c_str(), str.c_str())->getCString(),
                                         CCCallFuncO::create(this, callfuncO_selector(LoadingScene::forceUpdate), NULL),0,
                                         true,
                                         CCCallFuncO::create(this, callfuncO_selector(LoadingScene::offerUpdate), NULL));
        _dialog->showCancelButton();
        return;
    }
    if (GlobalData::shared()->updateType==2)
    {
        YesNoDialog::showVariableTitle(CCString::createWithFormat(_lang("login_error_client_ver_must").c_str(), str.c_str())->getCString(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::forceUpdate),NULL), _lang("confirm").c_str(),true);
        return;
    }
//
//    isWallCallBackDone = false;
//    isBuildCallBackDone = false;
//
//    CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(LoadingScene::buildingCallBack), NULL), 2+10*GlobalData::shared()->contryResType);
//    CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(LoadingScene::wallCallBack), NULL), 8+10*GlobalData::shared()->contryResType);

    gotoMainScene(0.1);
}

static int LoadingScene_STEPS = 3;

void LoadingScene::prepareImperialSceneEnterView(float time) {
//    if( LoadingScene_STEPS == 3 ) {
//        RechargeACTVCell::createDailyRwd(100);
//    } else if( LoadingScene_STEPS == 2 ) {
//        //tips: if don't call addPupupView and removePopupView,
//        // the first time enter imperialscene,
//        // the background of node would be transparent
//        auto node = RechargeACTVCell::create(100);
//        PopupViewController::getInstance()->addPopupView(node);
//        PopupViewController::getInstance()->removePopupView(node);
//    } else if ( LoadingScene_STEPS == 1 ) {
//        RechargeACTVCell::createMonthCard(100);
//    }
    
    LoadingScene_STEPS--;
    if( LoadingScene_STEPS == 0 ) {
        unschedule(schedule_selector(LoadingScene::prepareImperialSceneEnterView));
        gotoMainScene(0.1);
    }
}

void LoadingScene::prepareImperialSceneEnterView() {
    
    LoadingScene_STEPS = 3;
    schedule(schedule_selector(LoadingScene::prepareImperialSceneEnterView), 0.001);
}

void LoadingScene::buildingCallBack(CCObject* params) {
    isBuildCallBackDone = true;
    if( isBuildCallBackDone && isWallCallBackDone ) {
        prepareImperialSceneEnterView();
        //gotoMainScene(0.1);
    }
}

void LoadingScene::wallCallBack(CCObject* params) {
    isWallCallBackDone = true;
    if( isBuildCallBackDone && isWallCallBackDone ) {
        prepareImperialSceneEnterView();
        //gotoMainScene(0.1);
    }

}

void LoadingScene::gotoMainScene(float t)
{

    unschedule(schedule_selector(LoadingScene::loadingAni));
    if (m_steps > 1) {
        float pro = (LOADING_STEP-m_steps+1)*1.0/(LOADING_STEP*1.0);
        string proInfo = CC_ITOA(pro*100);
        m_loadPerLabel->setString(proInfo+"%");
        
        float curX = m_barSize.width*pro;
        m_clipNode->setContentSize(CCSizeMake(curX, m_barSize.height+30));
        m_barClipNode->setContentSize(CCSizeMake(curX, m_barSize.height+30));
        if (CCCommonUtils::isIosAndroidPad())
        {
            curX *= 2.f;
        }
        m_headParticleNode->setPositionX(m_clipNode->getPositionX()+curX);
        
        --m_steps;
        //m_steps -= 10;
        if ( m_steps <= 1 ) m_steps = 1;
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        schedule(schedule_selector(LoadingScene::gotoMainScene), 0.005f);
    }
    else if(!GlobalData::shared()->isInitFlag){
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        schedule(schedule_selector(LoadingScene::gotoMainScene), 0.01f);
    }
    else {
        //loadingLog统计
        GameController::getInstance()->setLoadingLog("LoadingScene", "gotoMainScene");
        unschedule(schedule_selector(LoadingScene::doLoginInit));
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        m_barClipNode->setContentSize(m_barSize);
        m_clipNode->setContentSize(m_barSize);
        m_loadPerLabel->setString("100%");
        m_headParticleNode->setPositionX(m_clipNode->getPositionX()+m_barSize.width);
        
        unschedule(schedule_selector(LoadingScene::onLoginTimeout));
        
        
         //        3Dtouch 调用的功能
        //        GlobalData::shared()->shortItemType = shortItemType_chat;
        std::string _shortItemType = GlobalData::shared()->shortItemType;
        bool _is3Dtouch = false;
        if(GlobalData::shared()->shortItem_switch && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0)))
        {
             if (!_shortItemType.compare(shortItemType_chat))
            {
                GlobalData::shared()->isUiInti = false;

                UIComponent::getInstance()->showAllianceChat();
                CCDirector::sharedDirector()->setVisitFlag(true);
                schedule(schedule_selector(LoadingScene::onEnterMainScene), 0.1);
                return;
            }
            
            if (!_shortItemType.compare(shortItemType_mail))
            {
                GlobalData::shared()->isUiInti = false;
                CCDirector::sharedDirector()->setVisitFlag(false);
                UIComponent::getInstance()->showMailUI();
                CCDirector::sharedDirector()->setVisitFlag(true);
                schedule(schedule_selector(LoadingScene::onEnterMainScene), 0.1);
                return;
            }
        }
        
        // lzy
        CCCommonUtils::postEventLogToServer("loading_loadingFinish_1");
        
        //行云log
        GameController::getInstance()->trackVisit();
        SceneController::getInstance()->addGUI();

        if(GlobalData::shared()->shortItem_switch && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0)))
        {
            if (!_shortItemType.compare(shortItemType_world))
            {
                _is3Dtouch = true;
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
            }
        }

        if(SceneController::getInstance()->showBG){
            SceneController::getInstance()->showBG->release();
            SceneController::getInstance()->showBG = NULL;
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
        }else if(!_is3Dtouch)
        {
            showMainScene();
        }
        CCCommonUtils::recordStep("5");
        

    }
}

void LoadingScene::showMainScene()
{
    bool showServiceTerms = false;
    showServiceTerms = CCUserDefault::sharedUserDefault()->getStringForKey("service_terms_check","") == "";
    if (showServiceTerms) {
        if (GlobalData::shared()->permitiCountry != "") {
            vector<std::string> mTmpV;
            bool showService =true;
            CCCommonUtils::splitString(GlobalData::shared()->permitiCountry, "|", mTmpV);
            for (int i = 0; i < mTmpV.size(); i++) {
                if (GlobalData::shared()->playerInfo.regCountry == mTmpV[i]) {
                    showService = false;
                    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
                    UIComponent::getInstance()->setVisible(false);
                    PopupViewController::getInstance()->addPopupView(PermitionForServicePopupView::create());
                    break;
                }
            }
            if (showService) {
                SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
            }
        }
        else
            SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
    }
    else
        SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
    
    
    // lzy
    CCCommonUtils::finishLoadingTotalTime();
    CCCommonUtils::postEventLogToServer("loading_enterMainScene_1");
}

void LoadingScene::onEnterMainScene(float t)
{
    unschedule(schedule_selector(LoadingScene::onEnterMainScene));
    //行云log
    GameController::getInstance()->trackVisit();
    SceneController::getInstance()->addGUI();
    
    if(SceneController::getInstance()->showBG){
        SceneController::getInstance()->showBG->release();
        SceneController::getInstance()->showBG = NULL;
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
    }else{
        showMainScene();
    }
    
    CCDirector::sharedDirector()->setVisitFlag(true);
    
    CCCommonUtils::recordStep("5");
}

void LoadingScene::forceUpdate(CCObject* p) {
    //服务器下发地址为空
    bool isUrlNull =  GlobalData::shared()->downloadUrl.empty() || GlobalData::shared()->downloadUrl=="null";
    
    if (isUrlNull)
        GameController::getInstance()->quitGame(NULL);
    else
        GameController::getInstance()->updateVersion();
}

void LoadingScene::onConnectionLost(cocos2d::CCObject *obj)
{
    CCLOG("ConnectionLost");
    auto ret = dynamic_cast<NetResult*>(obj);
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onLoginFail");
    NetController::shared()->setIslogining(false);
    if (Error_Network_Lost==ret->getErrorCode() && !GlobalData::shared()->isPause) {
//        LocalController::shared()->init();
//        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::doLogin), NULL),_lang("105248").c_str(),true);
        NetController::shared()->checkNetWork("login 1");
    };
}

void LoadingScene::doResourceUpdate(CCObject* p) {
    unschedule(schedule_selector(LoadingScene::onLoginTimeout));
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "doResourceUpdate");
    
    //zym 增长时间
    if( GlobalData::shared()->analyticID != "cn1" )
        schedule(schedule_selector(LoadingScene::doLoginInit), 25.0f);

}

void LoadingScene::onNetworkAvailable(CCObject* p)
{
    CCLog("jinpeng post onNetworkAvailable");
    CCCommonUtils::onNetworkAvailable(p);
}

void LoadingScene::offerUpdate(CCObject* p)
{
    GlobalData::shared()->updateType=0;
    stepLoading(NULL);
}

bool LoadingScene::isChina(){
    bool isChinaPlatForm = (GlobalData::shared()->isChinaPlatForm() || GlobalData::shared()->analyticID == "common");
    bool isChinaLan = (CCCommonUtils::getLanguage() == "zh_CN");
    return isChinaPlatForm || isChinaLan;
}

void LoadingScene::checkCanGotoMainScene(CCObject* p)
{
    UIComponent::getInstance()->setVisible(true);
    SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
}
//---------

RollingSprite *RollingSprite::create(std::string picName, float speed){
    RollingSprite *ret = new RollingSprite();
    if(ret && ret->init(picName, speed)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RollingSprite::onEnter(){
    CCNode::onEnter();

    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void RollingSprite::onExit(){
    this->removeAllChildren();
    m_batch = NULL;
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCNode::onExit();
}

bool RollingSprite::init(std::string picName, float speed){
    m_speed = speed;
    m_picName = picName;
    m_batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(m_picName.c_str())->getTexture(), 5);
    this->addChild(m_batch);
    auto size = CCDirector::sharedDirector()->getWinSize();
    int w = 0;
    
    auto addSprite = [&](){
        auto sprite = CCLoadSprite::createSprite(m_picName.c_str());
        m_batch->addChild(sprite);
        sprite->setAnchorPoint(ccp(0, 1));
        sprite->setPosition(ccp(w, 0));
        w += sprite->getContentSize().width;
    };
    
    while(w < size.width){
        addSprite();
        if(w > size.width){
            addSprite();
        }
    }
    return true;
}

void RollingSprite::update(float time){
    if(!m_batch){
        return;
    }
    auto &arr = m_batch->getChildren();
    if(arr.size() > 0){
        auto node = dynamic_cast<CCNode*>(arr.at(0));
        if(node){
            int cellW = node->getContentSize().width;
            float batchX = m_batch->getPositionX();
            float nextX = batchX - time * m_speed;
            if(nextX < -cellW){
                nextX += int(abs(nextX) / cellW) * cellW;
            }
            m_batch->setPositionX(nextX);
        }
    }
}
