//
//  VipDetailCell.h
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#ifndef __IF__VipDetailCell__
#define __IF__VipDetailCell__


#include "CommonInclude.h"


class VipDetailCell :
public CCTableViewCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static const float CELL_WIDTH;
    static const float CELL_HEIGHT;
    
    //static VipDetailCell* create(int index, int type = 0, int VIP_type = 1);
    //VipDetailCell(int index, int type, int VIP_type):m_index(index),m_type(type),m_VIPtype(VIP_type) {};
    
    static VipDetailCell* create(int index, int type = 0, int VIP_type = 1, int curLevel = 1); //a by ljf
    VipDetailCell(int index, int type, int VIP_type, int curLevel):m_index(index),m_type(type),m_VIPtype(VIP_type), mCurrentVipLevel(curLevel) {}; //a by ljf

    
    
    ~VipDetailCell()
    {
        
    }
    
    //void setData(int index);
    void setData(int index, int curLevel);  //ljf

    
    float getHG();
    float getTopCellHeight();  //ljf

private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
   

    CCSafeObject<CCLabelIF> m_leftText;
    CCSafeObject<CCLabelIF> m_rightText;
    CCSafeObject<CCNode> m_leftPicNode;
    CCSafeObject<CCNode> m_rightPicNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_listNode;
    
    
    //CCScrollView* m_infoScrollView;
    int m_index;
    int m_type;
    int m_VIPtype;//1-vip,2-svip
    
    //b y ljf
    float mTopCellHeight;
    int mCurrentVipLevel;
    //e y ljf

};

class VipEffectCell :public CCNode
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:

    //static VipEffectCell* create(string type1,int value1,string type2,int value2,int guide = 0);
    //VipEffectCell(string type1,int value1,string type2,int value2,int guide):m_type1(type1),m_value1(value1),m_type2(type2),m_value2(value2),m_guide(guide){};
    
    //b a ljf
    static VipEffectCell* create(string type1,int value1,string type2,int value2, int guide,  string posType, int index, int curLevel);
    VipEffectCell(string type1,int value1,string type2,int value2,  int guide, string posType, int index, int curLevel):m_type1(type1),m_value1(value1),m_type2(type2),m_value2(value2), m_guide(guide), mPositionType(posType), mIndex(index), mCurrentLevel(curLevel){};
    //e a ljf
    
    ~VipEffectCell()
    {
        
    }
    
    //void setData(string type1,int value1,string type2,int value2);
    
    void setData(string type1,int value1,string type2,int value2, int index, int curLevel); //ljf

    float getHG();
    void setGuideAnim();
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_leftValueText;
    CCSafeObject<CCLabelIF> m_rightValueText;
    CCSafeObject<CCLabelIF> m_leftText;
    CCSafeObject<CCLabelIF> m_rightText;
    CCSafeObject<CCNode> m_leftPicNode;
    CCSafeObject<CCNode> m_rightPicNode;
    CCSafeObject<CCNode> m_showNode1;
    CCSafeObject<CCNode> m_showNode2;
   // CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCSprite> m_rightAddFlag;
    CCSafeObject<Node> m_activeGuideNode;
    //CCScrollView* m_infoScrollView;
    string m_type1;
    int m_value1;
    int m_value2;
    string m_type2;
    int m_guide;
    
    
    
    //begin a by ljf
    string mPositionType;
    int mIndex;
    int mCurrentLevel;
    CCSafeObject<CCLabelBMFont> m_curVIPLevel;
    CCSafeObject<CCLabelBMFont> m_nextVIPLevel;
    CCSafeObject<CCSprite> m_currentLVBg;
    //end a by ljf

};

class VipEffectSVIPCell :public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    
    static VipEffectSVIPCell* create(string type,int itemID,int nextItemID);
    VipEffectSVIPCell(string type,int itemID,int nextItemID):m_type(type),m_itemID(itemID),m_nextItemID(nextItemID){};
    
    ~VipEffectSVIPCell()
    {
        
    }
    
    void setData();
    float getHG();
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_leftValueText;
    CCSafeObject<CCLabelIF> m_rightValueText;
    CCSafeObject<CCLabelIF> m_leftText;
    CCSafeObject<CCLabelIF> m_rightText;
    CCSafeObject<CCNode> m_leftPicNode;
    CCSafeObject<CCNode> m_rightPicNode;
    CCSafeObject<CCNode> m_showNode1;
    CCSafeObject<CCNode> m_showNode2;
    CCSafeObject<CCSprite> m_rightAddFlag;
    CCSafeObject<Node> m_activeGuideNode;
    string m_type;
    int m_itemID;
    int m_nextItemID;
};
#endif /* defined(__IF__VipDetailCell__) */
