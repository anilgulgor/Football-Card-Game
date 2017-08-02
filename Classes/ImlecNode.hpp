//
//  ImlecNode.hpp
//  Santra
//
//  Created by Anıl Gülgör on 27/03/16.
//
//

#ifndef ImlecNode_hpp
#define ImlecNode_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AppManager.hpp"

using namespace cocos2d;

USING_NS_CC;


class ImlecNode : public cocos2d::Node {
  
public:

    static ImlecNode *create(std::string name);
    
    void addImageToNode();
    
    AppManager *appManager;
    
    
private:
    
    
    ImlecNode(std::string name);
    
    std::string name;
};

#endif /* ImlecNode_hpp */
