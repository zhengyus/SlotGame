//
//  HDataManager.cpp
//  Slot
//
//  Created by huxf on 14-9-2.
//
//

#include "HallDataManager.h"

static HallDataManager* dm = NULL;

HallDataManager::HallDataManager()
{
    
}

HallDataManager* HallDataManager::getInstance()
{
    if (NULL == dm)
    {
        dm = new HallDataManager();
    }
    return dm;
}