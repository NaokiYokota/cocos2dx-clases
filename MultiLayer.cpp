//
//  MultiLayer.cpp
//  MyProject2
//
//　マルチスレッドの実験(レシピ本)
//
//  Created by albatrus on 2014/06/04.
//
//





#include "MultiLayer.h"
#include <semaphore.h>
#include <unistd.h>

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#define MY_USE_NAMED_SEMAPHORE 1

#else

#define MY_USE_NAMED_SEMAPHORE 0


#endif


#if MY_USE_NAMED_SEMAPHORE

#define MY_SEMAPHORE "cocosThreadSample.sem"

#else

static sem_t s_semaphore;

#endif

static sem_t* s_pSemaphore = NULL;
static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t s_thread;

struct threadData{
    
    //スレッド作業データ格納配列
    cocos2d::Vector<cocos2d::__String*> vec;
};

static threadData *s_threadData;


/*
 
 マルチスレッド のワーカスレッド
 
 */

void *thread_function(void *arg){
    
    threadData *td = (threadData*)arg;
    for (int i = 0; i < 20; i++) {
        
        cocos2d::__String *str = new cocos2d::__String();
        str->initWithFormat("%d",i);
        
        //
        CCLOG("ワーカスレッド : %s",str->getCString());

        //0.5秒に一度データを投入
        pthread_mutex_lock(&s_mutex);
        td->vec.pushBack(str);
        pthread_mutex_unlock(&s_mutex);
        
        usleep(500000); // wait 0.5sec
        
        
    }
    
    sem_post(s_pSemaphore);
    return NULL;
    
}













Scene *MultiLayer :: scene(){
    
    auto _scene = Scene::create();
    auto _layer = MultiLayer::create();
    _scene->addChild(_layer);
    
    return _scene;
}


bool MultiLayer::init(){
    
    if (!Layer::init()) {
        
        return false;
    }
    
    Size winSize = CCDirector::getInstance()->getWinSize();
    Size viSize = CCDirector::getInstance()->getVisibleSize();
    

    //背景
    auto bg = LayerColor::create(Color4B(0, 128, 64, 255), winSize.width, winSize.height);
    this->addChild(bg);

    auto bg2 = LayerColor::create(Color4B(128, 0, 64, 255), viSize.width/2, viSize.height/2);
    bg2->setPosition(Vec2(viSize.width/4, viSize.height/4));
    this->addChild(bg2);
    
    
    
    //ラベル
    auto label = CCLabelTTF::create("テスト", "Arial", 48);
    label->setPosition(Vec2(viSize.width/2,viSize.height/2));
    this->addChild(label, 1,1);

    
    this->startMulti();
    
    return true;

}

void MultiLayer::startMulti(){

    //マルチスレッド
    CCLOG("マルチスレッド startMulti");
#if MY_USE_NAMED_SEMAPHORE
    

    s_pSemaphore = sem_open(MY_SEMAPHORE, O_CREAT,0644,0);
    if (s_pSemaphore == SEM_FAILED) {
        
        return;
        
    }
    
#else

    if (sem_init(&s_semaphore, 0, 0) < 0) {
        
        return;
        
    }
    
    s_pSemaphore = &s_semaphore;
    
#endif
    
    //スレッド用の作業データを作成
    s_threadData = new threadData();
    //s_threadData->vec = new Vector<__String*>;
    
    //スレッド起動
    pthread_mutex_init(&s_mutex, NULL);
    pthread_create(&s_thread, NULL,thread_function, (void*)s_threadData);
    pthread_detach(s_thread);
    
    //スレッド表示ルート開始
    this->schedule(schedule_selector(MultiLayer::update_therad_progress));
    

    
    
}




/*
 
 メインスレッドのUpdate(ルート)
 
 */

void MultiLayer::update_therad_progress(float delta){
    
    //Ref *obg = NULL;
    std::string str = "";
    
    //スレッドからデータを取得
    pthread_mutex_lock(&s_mutex);
    Vector<__String*> _ver  = s_threadData->vec;
    
    for (int i =0; i < _ver.size(); i++) {
        
        __String *ccstr = (__String*)_ver.at(i);
        str += ccstr->getCString();
        
    }
    
    pthread_mutex_unlock(&s_mutex);
    
    
    //ラベル表示
    //Label *_label = (Label*)this->getChildByTag(1);
    CCLabelTTF *_label = (CCLabelTTF*)this->getChildByTag(1);
    const std::string _current = _label->getString();

    if (strcmp(_current.c_str(), str.c_str()) != 0) {
        CCLOG("Update : %s",_current.c_str());
        _label->setString(str);
        
    }
    
    
    //マルチスレッド終了
    int ret = sem_trywait(s_pSemaphore);
    if (ret == 0) {
        
        //カウント終了
        CCLOG("マルチスレッド カウント終了");

        this->unschedule(schedule_selector(MultiLayer::update_therad_progress));
        
        s_threadData->vec.clear(); // removeAllObject
        delete s_threadData;
        s_threadData = NULL;
        
#if MY_NAMED_SEMAPHORE
        
        sem_close(s_pSemaphore);
        sem_unlink(MY_SEMAPHORE);
        
#else
        
        sem_destroy(s_pSemaphore);
        
#endif
        
        
    }
    
    
}









