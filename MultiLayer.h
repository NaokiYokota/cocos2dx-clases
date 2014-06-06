//
//  MultiLayer.h
//  MyProject2
//
//  Created by 横田 直輝 on 2014/06/04.
//
//

#ifndef __MyProject2__MultiLayer__
#define __MyProject2__MultiLayer__

#include "cocos2d.h"

class MultiLayer : public cocos2d::Layer{
  
public:
    
    virtual bool init();
    static cocos2d::Scene *scene();
    CREATE_FUNC(MultiLayer);
    
    void startMulti();
    
    void update_therad_progress(float delta);

};

#endif /* defined(__MyProject2__MultiLayer__) */
