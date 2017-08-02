//
//  ImlecNode.cpp
//  Santra
//
//  Created by Anıl Gülgör on 27/03/16.
//
//

#include "ImlecNode.hpp"


ImlecNode::ImlecNode(std::string name){
    
    this->name = name;
    
}


ImlecNode *ImlecNode::create(std::string name){
    
    ImlecNode *imlecNode = new ImlecNode(name);

    return imlecNode;
    
}

void ImlecNode::addImageToNode() {
    
    Sprite *imlec = Sprite::create(this->name);
    
    FIT_H(imlec, GET_HEIGHT*.15);
    
    this->addChild(imlec);
    
    auto anim1 = MoveBy::create(.5, Vec2(this->getPositionX() + 15,0));
    
    auto anim2 = MoveBy::create(.5, Vec2(this->getPositionX() - 15,0));
    
    auto seq = Sequence::create(anim1,anim2, NULL);
    
    auto infiniteAction = RepeatForever::create(seq);
    
    imlec->runAction(infiniteAction);
    
}