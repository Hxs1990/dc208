//
//  YuanJunTipView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-11.
//
//

#include "YuanJunTipView.h"

#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicView.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldMapView.h"
#include "ChangeNickNameView.h"
#include "BattleView.h"
#include "AllianceReinforceSoldier.h"
#include "TroopsController.h"


YuanJunTipView *YuanJunTipView::create(int index){
    YuanJunTipView *ret = new YuanJunTipView(index);
    if(ret && ret->init(-1)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

YuanJunTipView *YuanJunTipView::createByType(int index,int cityType,int haveOwner)
{
    YuanJunTipView *ret = new YuanJunTipView(index);
    if(ret && ret->init(cityType)){
        ret->m_haveOwner=haveOwner;
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void YuanJunTipView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void YuanJunTipView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_pic_click");
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool YuanJunTipView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void YuanJunTipView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_bg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool YuanJunTipView::init(int cityType){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        CCBLoadFile("YuanJunTipView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());

        m_titleText->setString(_lang("115151"));
        m_tipTxt1->setString(_lang("115152"));
        
        
        m_cityType=cityType;
        m_haveOwner=0;
        this->setTipTxtShow();
        
        
        
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("cancel_btn_label").c_str());
        CCCommonUtils::setButtonTitle(m_sendBtn, _lang("115154").c_str());
        m_current = 0;
        m_maxSoldier = 0;
        m_numNode->setVisible(false);
        m_sendBtn->setEnabled(false);
        
        
        std::string reinforceType=ALLIANCE_AL_REINFORCE_SOLDIER;
        std::string paramKey="point";
        if (cityType!=-1) {
            reinforceType=ALLIANCE_AL_REINFORCE_THRONE_TREBUCHET;
            paramKey="pointId";
        }
        AllianceReinforceSoldier* cmd = new AllianceReinforceSoldier(reinforceType);
        cmd->putParam(paramKey, CCInteger::create(m_index));
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(YuanJunTipView::getData), NULL));
        cmd->sendAndRelease();
        ret = true;
    }
    return ret;
}

void YuanJunTipView::setTipTxtShow()
{
    switch (m_cityType) {
        case -1:
        {
            m_tipTxt2->setString(_lang("115153"));
            break;
        }
        case 10:  //Throne 王座
        {
            m_tipTxt2->setString(_lang_1("140188", _lang("110172").c_str()));
            break;
        }
        case 12:   //Trebuchet  投石机
        {
            m_tipTxt2->setString(_lang_1("140188", _lang("110081").c_str()));
            break;
        }
        case 14:   //Tile_allianceArea  联盟堡垒
        {
            m_tipTxt2->setString(_lang_1("140186", _lang("115357").c_str()));
            break;
        }
        case 18:   //tile_superMine  超级矿
        {
            m_tipTxt2->setString(_lang_1("140186", _lang("115373").c_str()));
            break;
        }
        case 20:   //tile_tower  联盟箭塔
        {
            m_tipTxt2->setString(_lang_1("140186", _lang("115582").c_str()));
            break;
        }
        case 21:   //tile_wareHouse     联盟仓库
        {
            m_tipTxt2->setString(_lang_1("140186", _lang("115364").c_str()));
            break;
        }
        case 23:   //tile_banner     联盟国旗
        {
            m_tipTxt2->setString(_lang_1("140186", _lang("115534").c_str()));
            break;
        }
        default:
            m_tipTxt2->setString(_lang("115153"));
            break;
    }
}

void YuanJunTipView::getData(CCObject *param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        if (m_cityType!=-1) {
            m_current = dic->valueForKey("currSoldiers")->intValue();
            m_maxSoldier = dic->valueForKey("maxSoldiers")->intValue();
            if (m_maxSoldier==0) {
                m_maxSoldier=TroopsController::getInstance()->getMaxSoilder();
            }
        }
        else{
            m_current = dic->valueForKey("forces")->intValue();
            m_maxSoldier = dic->valueForKey("max")->intValue();
        }
        
        std::string str = CC_CMDITOA(m_current);
        str.append("/");
        str.append(CC_CMDITOA(m_maxSoldier));
        m_numTxt->setString(str);
        if(m_maxSoldier<=0){
            m_numNode->setVisible(false);
            m_sendBtn->setEnabled(false);
            m_tipTxt1->setString(_lang("115174"));
            m_tipTxt1->setColor(ccRED);
        }else{
            m_numNode->setVisible(true);
        }
        int gapNum = m_maxSoldier - m_current;
        m_sendBtn->setEnabled(gapNum>0);
    }
}

void YuanJunTipView::onSendClick(CCObject *pSender, CCControlEvent event){
    int pointId = m_index;
    
    int gapNum = m_maxSoldier - m_current;
    
    if (m_cityType==-1) {
        WorldController::getInstance()->openMarchDeploy(pointId,1,gapNum,MethodYuanSolider);
    }
    else{
        WorldController::getInstance()->openMarchDeploy(pointId, m_haveOwner,gapNum);
    }
    
    
    
    
    
    PopupViewController::getInstance()->removePopupView(this);
}

void YuanJunTipView::onCancelClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}

bool YuanJunTipView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numNode", CCNode*, this->m_numNode);
    return false;
}

SEL_CCControlHandler YuanJunTipView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelClick", YuanJunTipView::onCancelClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendClick", YuanJunTipView::onSendClick);
    return NULL;
}

void YuanJunTipView::openYuanYunView(int pointId,int cityType,int haveOwner)
{
    if (pointId<0) {
        return;
    }
    PopupViewController::getInstance()->addPopupView(YuanJunTipView::createByType(pointId, cityType,haveOwner));
}