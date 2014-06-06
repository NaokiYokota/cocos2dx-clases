//
//  NetworkLayer.h
//  MyProject2
//
//  Created by 横田 直輝 on 2014/06/04.
//
//


#ifndef __MyProject2__NetworkLayer__
#define __MyProject2__NetworkLayer__

#include "cocos2d.h"
#include "network/HttpClient.h"


USING_NS_CC;
using namespace cocos2d::network;


class NetworkLayer : public cocos2d::Layer{
    
public:
    
    virtual bool init();
    static cocos2d::Scene *scene();
    CREATE_FUNC(NetworkLayer);
    
    
    void getProfilePictureFromServer(const char *url);
    void callback(HttpClient* sender, HttpResponse* response);
    
    bool getTextFromUrl(const char *url,std::vector<char> *response);
    
    
    void getJsonValue(const char *url);
    void getXmlValue(const char *url);

    void actionButton();
    void actionButton2();

    void callback2(HttpClient* sender, HttpResponse* response);

    
};

#endif /* defined(__MyProject2__NetworkLayer__) */
