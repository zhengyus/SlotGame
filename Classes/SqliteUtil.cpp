//
//  Sqlite.cpp
//  Slot
//
//  Created by huxf on 14-9-10.
//
//

#include "SqliteUtil.h"

void SqliteUtil::openDB(sqlite3** pdb,string dbName)
{
    string path = CCFileUtils::sharedFileUtils()->getWritablePath() + dbName + ".db";
    
    int result;
    result = sqlite3_open(path.c_str() , pdb);//3
    if(result != SQLITE_OK)
    {
        CCLOG("open database failed,  number%d",result);
        CCAssert(result != SQLITE_OK, "open database failed");
    }
}



bool SqliteUtil::execute(sqlite3* pdb,string sql)
{
    int result = sqlite3_exec(pdb, sql.c_str(), NULL, NULL, NULL);
    
    if (SQLITE_OK != result)
    {
        CCLOG("execute sql failed,  sql: %s",sql.c_str());
    }
    
    return SQLITE_OK == result;
}

vector<string> SqliteUtil::select(sqlite3* pdb,string sql,int& columns)
{
    vector<string> table;
    
    char **result;  //查询结果
    int row,col;    //行、列
    
    sqlite3_get_table(pdb , sql.c_str() , &result , &row , &col , NULL);//1
    CCLOG("row is %d,column is %d" , row , col);
    columns = col;
    
    for(int i=1; i <= row; i++)//2
    {
        for(int j=0; j < col; j++)
        {
            table.push_back(result[i * col + j]);
        }
    }
    
    sqlite3_free_table(result);
    return table;
}

void SqliteUtil::close(sqlite3 *&pdb)
{
    sqlite3_close(pdb);
    pdb = NULL;
}
