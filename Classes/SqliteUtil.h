//
//  Sqlite.h
//  Slot
//
//  Created by huxf on 14-9-10.
//
//

#ifndef __Slot__Sqlite__
#define __Slot__Sqlite__

#include "sqlite3.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class SqliteUtil{
    
    
public:
    
    static void openDB(sqlite3**,string dbName);
    
    static bool execute(sqlite3*,string sql);
    static vector<string> select(sqlite3*,string sql,int& columns);
    
    static void close(sqlite3*&pdb);
    
};

#endif /* defined(__Slot__Sqlite__) */
