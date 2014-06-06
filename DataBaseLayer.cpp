//
//  DataBaseLayer.cpp
//
/*
 
 データベースを使った読み込み・書き込みのテスト
 
 
 
 
 
 
 */
//
//  Created by albatrus on 2014/06/04.
//
//

#include "DataBaseLayer.h"


USING_NS_CC;


Scene *DataBaseLayer :: scene(){
    
    auto _scene = Scene::create();
    auto _layer = DataBaseLayer::create();
    _scene->addChild(_layer);
    
    return _scene;
}


bool DataBaseLayer::init(){
    
    if (!Layer::init()) {
        
        return false;
 
    }
    
    Size winSize = CCDirector::getInstance()->getWinSize();
    Size viSize = CCDirector::getInstance()->getVisibleSize();
    
    //背景
    auto bg = LayerColor::create(Color4B(0, 0, 188, 255), winSize.width, winSize.height);
    this->addChild(bg);
    
    //ラベル
    auto label = CCLabelTTF::create("データベースのテスト", "Arial", 64);
    label->setPosition(Vec2(viSize.width/2,viSize.height/3*2));
    this->addChild(label, 1,1);
    
    //メニュー
    auto _menuItem = MenuItemImage::create();
    _menuItem->setNormalImage(Sprite::create("b-yes-button.png"));
    _menuItem->setSelectedImage(Sprite::create("b-yes-button-hover.png"));
    _menuItem->setCallback(CC_CALLBACK_0(DataBaseLayer::actionButton, this));
    
    auto _menuItem2 = MenuItemImage::create();
    _menuItem2->setNormalImage(Sprite::create("no-button.png"));
    _menuItem2->setSelectedImage(Sprite::create("no-button-hover.png"));
    _menuItem2->setCallback(CC_CALLBACK_0(DataBaseLayer::actionButton2, this));
    
    auto _menuItem3 = MenuItemImage::create();
    _menuItem3->setNormalImage(Sprite::create("b-back-button.png"));
    _menuItem3->setSelectedImage(Sprite::create("b-back-button-hover.png"));
    _menuItem3->setCallback(CC_CALLBACK_0(DataBaseLayer::actionButton3, this));
    
    
    _menuItem->setPosition(Vec2(winSize.width/2, winSize.height/3));
    _menuItem2->setPosition(Vec2(winSize.width/2, winSize.height/5));
    _menuItem3->setPosition(Vec2(winSize.width/2, winSize.height/8));

    auto menu = Menu::create(_menuItem,_menuItem2,_menuItem3,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    
    /*
     
     
     データベース作成
     
     
    
    //データベースの保存先のパス
    auto filePath = FileUtils::getInstance()->getWritablePath();
    filePath.append("Test.db");
    
    //Open
    auto status = sqlite3_open(filePath.c_str(), &useDataBase);
    
    
    //ステータス確認
    if(status != SQLITE_OK){
        
        CCLOG("Opne failed 失敗 %s",errorMessage);
        
    }else{
        
        CCLOG("Open sucessed");
        
    }
    
    
    //データベースのテーブルの作成
    auto create_sql = "CREATE TABLE user( id integer primary key autoincrement, name navarchar(32),age int(2) )";
    status = sqlite3_exec(useDataBase, create_sql, NULL, NULL, &errorMessage);
    if(status != SQLITE_OK){
        
        CCLOG("create table failed : %s",errorMessage);
    }
    
    //インサート（値を入れる)
    auto insert_sql = "INSERT INTO user(name,age) VALUES('albatrus','32')";
    status = sqlite3_exec(useDataBase, insert_sql, NULL, NULL, &errorMessage);
    
    
    //閉める
    sqlite3_close(useDataBase);
    
*/
    
    //初期設定
    sqliteCreateTable();
    sqliteSetValueForKey("first", "cocos2dx");
    sqliteSetValueForKey("second", "cocos2d-x");
    
    return true;

}


//データベースの保存先を呼び出す
void DataBaseLayer::actionButton(){
    
    CCLOG("start");

    const char *string = sqliteGetValueForKey("first");
    
    if (string) {
        
        CCLOG("first 取り出し %s",string);
        auto label = (CCLabelTTF*)this->getChildByTag(1);
        if (label) {
            
            label->setString(string);
            
        }
    }else{
        
        CCLOG("失敗");
        
    }

    
    
}

//データベースの保存先を呼び出す
void DataBaseLayer::actionButton2(){
    
    /*
    //データベースの呼び出し
    auto filePath = FileUtils::getInstance()->getWritablePath();
    filePath.append("Test.db");
    
    CCLOG("%s",filePath.c_str());
    */
    
    const char *string = sqliteGetValueForKey("second");
    
    if (string) {
        
        CCLOG("second 取り出し2 %s",string);
        
        auto label = (CCLabelTTF*)this->getChildByTag(1);
        if (label) {
            
            label->setString(string);
            
        }

    }else{
        
        CCLOG("失敗");

    }
    
}

void DataBaseLayer::actionButton3(){
    
    
    int random = arc4random()%5;
    const char *randamStr = NULL;
    
    switch (random) {
        case 0:randamStr = "Unity 0";break;
        case 1:randamStr = "cocos2d 1";break;
        case 2:randamStr = "Object-c 2";break;
        case 3:randamStr = "Java 3";break;
        case 4:randamStr = "xcode 4";break;

        default:
            break;
    }
    
    CCLOG("置き換える key =first value = %s",randamStr);
    sqliteUpdateValueForKey("first", randamStr);
    
    
}

#pragma mark -
#pragma mark データベース関係


//データベースを開ける
int DataBaseLayer::sqliteOpen(sqlite3 **db){
    
    auto filePath = FileUtils::getInstance()->getWritablePath();
    filePath += "sample.db";
    
    
    return sqlite3_open(filePath.c_str(),db);


}


//テーブルの作成
void DataBaseLayer::sqliteCreateTable(){
    
    CCLOG("create table");

    //データベースファイルを作成する
    sqlite3*db = NULL;
    if (sqliteOpen(&db) == SQLITE_OK) {
        
        const char *sql_createtable = "CREATE TABLE test1(key TEXT,value TEXT)";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_createtable, -1, &stmt, NULL) == SQLITE_OK) {
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
                CCLOG("create table done");
            }
            
            sqlite3_reset(stmt);
            
            
        }
        
        sqlite3_finalize(stmt);
        
        
        
    }
    
    sqlite3_close(db);

    
    
}

//キーから値を取り出す
const char *DataBaseLayer::sqliteGetValueForKey(const char *key){
    
    __String *strValue = NULL;
    sqlite3 *db = NULL;
    if (sqliteOpen(&db) == SQLITE_OK) {
        
        const char *sql_select = "SELECT value FROM test1 WHERE key=?";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {

            sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT);
            int ret = sqlite3_step(stmt);
            if (ret == SQLITE_DONE || ret == SQLITE_ROW) {
                
                const char *val = (const char*)sqlite3_column_text(stmt, 0);
                strValue = __String::create(val);
                
            }
            
            sqlite3_reset(stmt);

        }
        
        sqlite3_finalize(stmt);

    }

    sqlite3_close(db);

    
    if (strValue != NULL) {
        
        return strValue->getCString();
    }
    
    return NULL;
}


//値をセットする(REPLACE INTO[データベース名]テーブル名(カラムのならび) VALUES(値のならび)
void DataBaseLayer::sqliteSetValueForKey(const char *key,const char *value){
    
    
    std::string fullpath = CCFileUtils::getInstance()->getWritablePath();
    fullpath += "sample.db";
    sqlite3 *db = NULL;
    
    if (sqlite3_open(fullpath.c_str(), &db) == SQLITE_OK) {
        
        const char *sql_select = "REPLACE INTO test1(key,value)VALUES(?,?)";
        sqlite3_stmt *stmt = NULL;
        
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
            
            sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, value, -1, SQLITE_TRANSIENT);
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
                CCLOG("replace key:%s value:%s",key,value);
                
            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    

}


//値を置き換える
void DataBaseLayer::sqliteUpdateValueForKey(const char *key,const char *value){
    
    
    std::string fullpath = CCFileUtils::getInstance()->getWritablePath();
    fullpath += "sample.db";
    sqlite3 *db = NULL;
    
    if (sqlite3_open(fullpath.c_str(), &db) == SQLITE_OK) {
        
        const char *sql_select = "UPDATE test1 SET value =? WHERE key =? ";
        sqlite3_stmt *stmt = NULL;
        
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
            
            sqlite3_bind_text(stmt, 1, value, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, key, -1, SQLITE_TRANSIENT);
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
                CCLOG("change key:%s value:%s",key,value);
                
            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    
    
}


