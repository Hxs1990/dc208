//
//  Pickaroon.cpp
//  DragonClans
//
//  Created by fusheng on 16/2/4.
//
//

#include "Pickaroon.h"

using namespace cocos2d;


Pickaroon::Pickaroon( Node* boundingBox,int seq, NBSprite3D * p3D, Node * pParticleNode, int level) : m_boundingBox(boundingBox), mSeq(seq),  m_pickaroon3D(p3D), mParticleNode(pParticleNode), mLevel(level)
{
}



Pickaroon * Pickaroon::create(Node* boundingBox, int seq, NBSprite3D * p3D, Node * pParticleNode, int level)
{
    Pickaroon* ret = new Pickaroon( boundingBox, seq, p3D, pParticleNode, level);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void Pickaroon::clearResources()
{
    if (m_boundingBox) {
        m_boundingBox->removeFromParent();
    }
    if (m_pickaroon3D) {
        m_pickaroon3D->removeFromParent();
    }
    if (mParticleNode) {
        mParticleNode->removeFromParent();
    }
}
bool Pickaroon::init()
{
    
    if (!Node::init())
    {
        return false;
    }
    /* code */

    mActionStatus = PickaroonActionStatus::PICKAROON_ACTION_NORMAL_IDLE;
    nextStatus = PickaroonActionStatus::PICKAROON_ACTION_NONE;
    
    
    return true;
}

void Pickaroon::playIdle()
{

    if(m_pickaroon3D)
    {

        string modelPath = "3d/ship/pickaroon_normal_idle.c3b";

        auto anim_stand = Animation3D::create(modelPath);
        if (anim_stand) {
            auto pAnim = Animate3D::create(anim_stand);
            if (pAnim) {
                auto act = Sequence::createWithTwoActions(pAnim, CallFunc::create(this, callfunc_selector(Pickaroon::handleActionComplete)));
                m_pickaroon3D->stopAllActions();
                act->setTag(PickaroonActionStatus::PICKAROON_ACTION_NORMAL_IDLE);
                m_pickaroon3D->runAction(act);
                mActionStatus = PickaroonActionStatus::PICKAROON_ACTION_NORMAL_IDLE;
            }
        }
    }
    
}

void Pickaroon::playMove()
{
    if(m_pickaroon3D)
    {

        string modelPath = "3d/ship/pickaroon_normal_walk.c3b";

        auto animation= Animation3D::create(modelPath);
        
        if (animation) {
            auto pAnim = Animate3D::create(animation);
            if (pAnim) {
                auto act = Sequence::createWithTwoActions(pAnim, CallFunc::create(this, callfunc_selector(Pickaroon::handleActionComplete)));
                m_pickaroon3D->stopAllActions();
                act->setTag(PickaroonActionStatus::PICKAROON_ACTION_NORMAL_MOVE);
                m_pickaroon3D->runAction(act);
               
                mActionStatus = PickaroonActionStatus::PICKAROON_ACTION_NORMAL_MOVE;
            }
        }
    }
}


void Pickaroon::playBomb()
{
    if(m_pickaroon3D)
    {
        
        string modelPath = "3d/ship/pickaroon_boom.c3b";
        
        auto animation= Animation3D::create(modelPath);
        
        if (animation) {
            auto pAnim = Animate3D::create(animation);
            if (pAnim) {
                auto act = Sequence::createWithTwoActions(pAnim, CallFunc::create(this, callfunc_selector(Pickaroon::handleActionComplete)));
                m_pickaroon3D->stopAllActions();
                act->setTag(PickaroonActionStatus::PICKAROON_ACTION_BOMB);
                m_pickaroon3D->runAction(act);
                
                mActionStatus = PickaroonActionStatus::PICKAROON_ACTION_BOMB;
            }
        }
    }


}
void Pickaroon::playBrokenIdle()
{
    if(m_pickaroon3D)
    {
        
        string modelPath = "3d/ship/pickaroon_broken_idle.c3b";
        
        auto animation= Animation3D::create(modelPath);
        
        if (animation) {
            auto pAnim = Animate3D::create(animation);
            if (pAnim) {
                auto act = Sequence::createWithTwoActions(pAnim, CallFunc::create(this, callfunc_selector(Pickaroon::handleActionComplete)));
                m_pickaroon3D->stopAllActions();
                act->setTag(PickaroonActionStatus::PICKAROON_ACTION_BROKEN_IDLE);
                m_pickaroon3D->runAction(act);
                
                mActionStatus = PickaroonActionStatus::PICKAROON_ACTION_BROKEN_IDLE;
            }
        }
    }


}
void Pickaroon::playBrokenMove()
{
    if(m_pickaroon3D)
    {
        
        string modelPath = "3d/ship/pickaroon_broken_walk.c3b";
        
        auto animation= Animation3D::create(modelPath);
        
        if (animation) {
            auto pAnim = Animate3D::create(animation);
            if (pAnim) {
                auto act = Sequence::createWithTwoActions(pAnim, CallFunc::create(this, callfunc_selector(Pickaroon::handleActionComplete)));
                m_pickaroon3D->stopAllActions();
                act->setTag(PickaroonActionStatus::PICKAROON_ACTION_BROKEN_MOVE);
                m_pickaroon3D->runAction(act);
                
                mActionStatus = PickaroonActionStatus::PICKAROON_ACTION_BROKEN_MOVE;
            }
        }
    }
}

void Pickaroon::handleActionComplete()
{
    if(nextStatus != PickaroonActionStatus::PICKAROON_ACTION_NONE)
    {

        changeActionByStatus(nextStatus);
        nextStatus = PickaroonActionStatus::PICKAROON_ACTION_NONE;

    }
    else
    {
        changeActionByStatus(mActionStatus);
    }
}

void Pickaroon::changeActionByStatus(PickaroonActionStatus status)
{
    switch (status) {
        case PICKAROON_ACTION_NONE:
            
            break;
        case PICKAROON_ACTION_NORMAL_IDLE:
            playIdle();
            break;
        case PICKAROON_ACTION_NORMAL_MOVE:
            playMove();
            break;
        case PICKAROON_ACTION_BOMB:
            playBrokenIdle();//播完爆炸 播放brokenIdle
            break;
        case PICKAROON_ACTION_BROKEN_IDLE:
            playBrokenIdle();
            break;
        case PICKAROON_ACTION_BROKEN_MOVE:
            playBrokenMove();
            break;
            
        default:
            break;
    }
}

void Pickaroon::setNextAnimationStatus(PickaroonActionStatus status)
{
    nextStatus = status;
}
