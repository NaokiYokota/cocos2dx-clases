//
//  GameLayer.cpp
//  MyProject2
//
//  Created by 横田 直輝 on 2014/05/30.
//
//

#include "GameLayer.h"

USING_NS_CC;

Scene *GameLayer::secne(){
    
    auto scene = Scene::create();
    auto glayer = GameLayer::create();
    scene->addChild(glayer);
    
    return scene;
    
}


bool GameLayer::init(){
    
    if (!Layer::init()) {
        
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    
    //背景
    auto bg = LayerColor::create(Color4B(0, 128, 64, 255), winSize.width, winSize.height);
    this->addChild(bg);
    
    return true;
}