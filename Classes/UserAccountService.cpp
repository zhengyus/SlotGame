//
//  UserAccountService.cpp
//  Slot
//
//  Created by huxf on 14-9-11.
//
//

#include "UserAccountService.h"
#include "cocos2d.h"

USING_NS_CC;

void UserAccountService::createTable()
{
    SqliteUtil::openDB(&_pdb, "Slot");
    SqliteUtil::execute(_pdb, "create table Account (id integer PRIMARY KEY autoincrement,account TEXT,password TEXT)");
    SqliteUtil::close(_pdb);
}

vector<UserAccount> UserAccountService::queryAllUserAccount()
{
    int columns;
    vector<UserAccount> accounts;
    
    SqliteUtil::openDB(&_pdb, "Slot");
    vector<string>table = SqliteUtil::select(_pdb, "select * from Account",columns);
    SqliteUtil::close(_pdb);
    
    for (int i = 0; i < table.size();++i)
    {
        UserAccount user;
        user.id = table[i++];
        user.account = table[i++];
        user.password = table[i];
        accounts.push_back(user);
    }
    return accounts;
}

bool UserAccountService::existsAccount(sqlite3* pdb,string account)
{
    int columns;
    
    string sql = "select * from Account where account ='%s'";
    vector<string>table = SqliteUtil::select(pdb, CCString::createWithFormat(sql.c_str(),account.c_str())->getCString(),columns);
    
    return table.size() > 0;
}

bool UserAccountService::addUserAccount(const char* account,const char* password)
{
    SqliteUtil::openDB(&_pdb, "Slot");
    
    if (existsAccount(_pdb,account))
    {
        return true;
    }
    
    string sql = "insert into Account (account,password) values('%s','%s')";

    bool ret = SqliteUtil::execute(_pdb,CCString::createWithFormat(sql.c_str(),account,password)->getCString());
    
    SqliteUtil::close(_pdb);
    return ret;
}

bool UserAccountService::deleteUserAccount(int id)
{
    SqliteUtil::openDB(&_pdb, "Slot");
    bool ret = SqliteUtil::execute(_pdb,CCString::createWithFormat("DELETE FROM Account WHERE id = %d",id)->getCString());
    SqliteUtil::close(_pdb);
    return ret;
}