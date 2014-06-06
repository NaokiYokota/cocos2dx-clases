//
//  NetworkLayer.cpp
//  MyProject2
//
//  Created by albatrus on 2014/06/04.
//http://is4cafe.hatenablog.com/entry/20140327/1395848377
//http://programming-ios.com/cocos2dx-cchttprequest-ccsprite/
//http://befool.co.jp/blog/8823-scholar/cocos2d-x-study-010-save-file-to-local-strage/

/*

 
 http://blog.livedoor.jp/seitokai_honbu/archives/52163762.html
 
 
 
 
 
 
 
 */

#include "NetworkLayer.h"
#include <curl/curl.h>
#include "picojson.h"



#if(CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)

#include "extensions/cocos-ext.h"
#include "tinyxml2/tinyxml2.h"


#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "external/tinyxml2/tinyxml2.h"

#endif


//using namespace cocos2d::extension;
using namespace tinyxml2;


//USING_NS_CC;
//using namespace cocos2d::network;


Scene *NetworkLayer :: scene(){
    
    auto _scene = Scene::create();
    auto _layer = NetworkLayer::create();
    _scene->addChild(_layer);
    
    return _scene;
}


bool NetworkLayer::init(){
    
    if (!Layer::init()) {
        
        return false;
        
    }
    
    Size winSize = CCDirector::getInstance()->getWinSize();
    Size viSize = CCDirector::getInstance()->getVisibleSize();
    
    auto bg = LayerColor::create(Color4B(40, 128, 40, 255), winSize.width, winSize.height);
    this->addChild(bg);
    
    auto label = CCLabelTTF::create("ネットワーク", "Arial", 64);
    label->setPosition(Vec2(viSize.width/2,viSize.height/3*2));
    this->addChild(label, 1,1);

    auto _menuItem = MenuItemImage::create();
    _menuItem->setNormalImage(Sprite::create("b-yes-button.png"));
    _menuItem->setSelectedImage(Sprite::create("b-yes-button-hover.png"));
    _menuItem->setCallback(CC_CALLBACK_0(NetworkLayer::actionButton, this));
    
    _menuItem->setPosition(Vec2(winSize.width/2,winSize.height/3));
    
    
    auto _menuItem2 = MenuItemImage::create();
    _menuItem2->setNormalImage(Sprite::create("no-button.png"));
    _menuItem2->setSelectedImage(Sprite::create("no-button-hover.png"));
    _menuItem2->setCallback(CC_CALLBACK_0(NetworkLayer::actionButton2, this));
    
    _menuItem2->setPosition(Vec2(winSize.width/2,winSize.height/5));

    
    
    auto _menu = Menu::create(_menuItem,_menuItem2,NULL);
    _menu->setPosition(Vec2::ZERO);
    this->addChild(_menu);
    
    
    getProfilePictureFromServer("https://dl.dropboxusercontent.com/u/22515015/test-logo.jpg");


    return true;
}


void NetworkLayer::actionButton(){
    
    this->getJsonValue("http://dl.dropboxusercontent.com/u/22515015/cocos2dx-json.json");
}


void NetworkLayer::actionButton2(){
    
    this->getXmlValue("https://dl.dropboxusercontent.com/u/22515015/test-xml.xml");
}

#pragma mark -
#pragma mark CCSprite
void NetworkLayer::getProfilePictureFromServer(const char *url){
    
    CCLOG("%s",url);
    cocos2d::network::HttpRequest *request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    //request->setResponseCallback(this, httpresponse_selector(NetworkLayer::callback));
    request->setResponseCallback(CC_CALLBACK_2(NetworkLayer::callback,this));
    HttpClient::getInstance()->send(request);
    request->release();
    
}

void NetworkLayer::callback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
    
    Size viSize = CCDirector::getInstance()->getVisibleSize();


    if (!response->isSucceed()) {
        
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    
    auto _image = new Image();
    _image->initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), buffer->size());
    
    auto _tex  = new Texture2D();
    _tex->initWithImage(_image);
    
    auto _sprite = Sprite::createWithTexture(_tex);
    _sprite->setPosition(Vec2(viSize.width/2,viSize.height/2));
    this->addChild(_sprite);
    
    
    
    
}

#pragma mark -
#pragma mark JSON

static size_t onCurlWriteData(char *ptr,size_t size,size_t nmemb,void*userdata){
    
    std::vector<char> *buff = (std::vector<char>*)userdata;
    size_t sizes = size * nmemb;
    buff->insert(buff->end(), (char*)ptr,(char*)ptr+sizes);
    return sizes;
    
}

bool NetworkLayer::getTextFromUrl(const char *url,std::vector<char> *response){
    

    CURL *curl;
    CURLcode res = CURLE_FAILED_INIT;
    
    curl = curl_easy_init();
    
    if (curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,onCurlWriteData);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,response);
        res=curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        

        
        
    }
    
    if (res == CURLE_OK) {
        
        CCLOG("res == CURLE_OK");

        
        if(response->size()){
            
            CCLOG("response -> size()");

            
            return true;

        }
        
    
    }
    
    return false;
    
}


void NetworkLayer::getJsonValue(const char *url){
    

    std::vector<char> responseData;
    
    if (getTextFromUrl(url,&responseData)) {
        
        std::string err;
        
        picojson::value va;
        picojson::parse(va, responseData.begin(), responseData.end(),&err);
        
        CCLOG("get json 1 %s",err.c_str());

        
        if (err.empty()) {
            
            picojson::object &root = va.get<picojson::object>();
            
            picojson::array &results = root["test"].get<picojson::array>();
            picojson::array::iterator it;
            
            CCLOG("get json 2");

            
            for (it = results.begin(); it != results.end(); it++) {
                
                picojson::object& text = it->get<picojson::object>();
                
                std::string& age = text["x"].get<std::string>();
                std::string& name = text["y"].get<std::string>();
                
                
                auto label = (LabelTTF*)this->getChildByTag(1);
                if (label) {
                    
                    CCLOG("JSON 成功 %s",name.c_str());

                    label->setString(age.c_str());
                    
                    
                }


            }
            
            
            
            
        }
        
    }
    
    
    
}

#pragma mark -
#pragma mark XML

void NetworkLayer::getXmlValue(const char *url){
    
    
    
    CCLOG("%s",url);
    cocos2d::network::HttpRequest *request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(NetworkLayer::callback2,this));
    HttpClient::getInstance()->send(request);
    request->release();

}


void NetworkLayer::callback2(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response){
    

    CCLOG("XML テスト　ボタン　ON");

    if (!response->isSucceed()) {
        
        return;
    }
    

   
    std::vector<char> *buffer = response->getResponseData();
    
    std::string _xml = std::string(buffer->begin(), buffer->end());

    
    
    
    XMLDocument doc;
    if (doc.Parse(_xml.c_str()) == XML_NO_ERROR) {
        

        const XMLElement *test = doc.FirstChildElement();
        for (;test;test = test->NextSiblingElement()) {
            
            const XMLElement *_x = test->FirstChildElement("x");
            __String *xStr = __String::create(_x->GetText());
            
            const XMLElement *_y = test->FirstChildElement("y");
            __String *yStr = __String::create(_y->GetText());
            
            CCLOG("x:%s y:%s",xStr->getCString(),yStr->getCString());
            
            auto label = (LabelTTF*)this->getChildByTag(1);
            if (label) {
                
                CCLOG("XML 成功 %s",xStr->getCString());

                label->setString(xStr->getCString());
                
                
            }
        }
        
    }
    
    
    
    
}

