//
//  Pickaroon.h
//  DragonClans
//
//  Created by fusheng on 16/2/4.
//
//

#ifndef Pickaroon_h
#define Pickaroon_h

#include "CommonInclude.h"
#include "CCLoadSprite.h"
#include "SpriteSheetAni.h"
#include "CCMathUtils.h"
#include "NBSprite3D.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum PickaroonActionStatus
{
    PICKAROON_ACTION_NONE,
    PICKAROON_ACTION_NORMAL_IDLE,
    PICKAROON_ACTION_NORMAL_MOVE,
    PICKAROON_ACTION_BROKEN_IDLE,
    PICKAROON_ACTION_BROKEN_MOVE,
    PICKAROON_ACTION_BOMB
};

class Pickaroon : public CCNode
{
public:
    Pickaroon( Node* boundingBox,int seq, NBSprite3D * p3D, Node * pParticleNode, int level);
    
    static Pickaroon * create(Node* boundingBox,int seq, NBSprite3D * p3D, Node * pParticleNode, int level = 1);
    
    bool init();
    
//    Node * getPosCCBNode(){return mPosCCBNode; }
//    
//    Node * getTouchCCBNode(){return mTouchCCBNode;}
    Node * getPickaroonBoundingBox(){return m_boundingBox;}
    
    int getSeq(){return mSeq;}
    
    NBSprite3D * getPickaroon3D(){return m_pickaroon3D;}
    
    void setPickaroon3D(NBSprite3D * p3D){m_pickaroon3D = p3D;}

    Node * getParticleNode(){return mParticleNode;}

    PickaroonActionStatus getActionStatus(){return mActionStatus;}
    
    void setNextAnimationStatus(PickaroonActionStatus status);
    
    void clearResources();
    
    void playIdle();
    void playMove();
    void playBomb();
    void playBrokenIdle();
    void playBrokenMove();
    
    void handleActionComplete();
    
    void changeActionByStatus(PickaroonActionStatus status);
    
private:
    NBSprite3D * m_pickaroon3D;
    Node * m_boundingBox;
    CCNode * mParticleNode;
    int mSeq;
    int mLevel;
    PickaroonActionStatus mActionStatus;
    
    PickaroonActionStatus nextStatus;
    
    LayerColor* layerColor;
    
};

#endif /* Pickaroon_h */
