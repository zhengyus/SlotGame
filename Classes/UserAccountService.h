//
//  UserAccountService.h
//  Slot
//
//  Created by huxf on 14-9-11.
//
//

#ifndef __Slot__UserAccountService__
#define __Slot__UserAccountService__

#include <iostream>
#include "Hdefine.h"
#include "SqliteUtil.h"

using namespace std;

class UserAccountService {
    
    
    
public:
    
    void createTable();
    
    vector<UserAccount> queryAllUserAccount();
    bool existsAccount(sqlite3* pdb,string account);
    bool addUserAccount(string account,string password);
    bool deleteUserAccount(int id);
    
private:
    sqlite3* _pdb;
    
};

#endif /* defined(__Slot__UserAccountService__) */
