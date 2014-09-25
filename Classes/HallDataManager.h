//
//  HDataManager.h
//  Slot
//
//  Created by huxf on 14-9-2.
//
//

#ifndef __Slot__HDataManager__
#define __Slot__HDataManager__

#include "Hdefine.h"

typedef struct
{
    std::string title;
    std::string name;
    unsigned long long gold;    
} Notice;

class HallDataManager {
    
public:
    static HallDataManager* getInstance();
    
public:
    
    std::vector<MyMailMeg>      _mails;
    std::vector<MyRankList>     _ranks[4];
    std::vector<MyJPRankList>   _JPRanks[2];
    std::vector<MyPetList>      _pets;
    std::vector<Notice>         _notices;
    
private:
    
    HallDataManager();
    
};

#endif /* defined(__Slot__HDataManager__) */
