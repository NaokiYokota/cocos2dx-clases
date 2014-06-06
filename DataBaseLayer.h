//
//  DataBaseLayer.h
//  MyProject2
//
//  Created by 横田 直輝 on 2014/06/04.
//
//

#ifndef __MyProject2__DataBaseLayer__
#define __MyProject2__DataBaseLayer__

#include "cocos2d.h"
#include "sqlite/sqlite3.h"

class DataBaseLayer : public cocos2d::Layer{
    
public:
    
    virtual bool init();
    static cocos2d::Scene *scene();
    CREATE_FUNC(DataBaseLayer);
    
    void actionButton();
    void actionButton2();
    void actionButton3();

    //sqlite
    sqlite3 *useDataBase = NULL;
    char *errorMessage = NULL;
    
    int sqliteOpen(sqlite3 **db);
    void sqliteCreateTable();
    const char *sqliteGetValueForKey(const char *key);
    void sqliteSetValueForKey(const char *key,const char *value);
    void sqliteUpdateValueForKey(const char *key,const char *value);

};

#endif /* defined(__MyProject2__DataBaseLayer__) */
