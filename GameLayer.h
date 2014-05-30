//
//  GameLayer.h
//  MyProject2
//
//  Created by 横田 直輝 on 2014/05/30.
//
//

#ifndef __MyProject2__GameLayer__
#define __MyProject2__GameLayer__

#include "cocos2d.h"

class GameLayer : public cocos2d::Layer{
  
public:
    
    static cocos2d::Scene *secne();
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
    
};


#endif /* defined(__MyProject2__GameLayer__) */
