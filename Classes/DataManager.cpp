/*!
 * @brief	数据管理类，负责管理游戏中的所有数据。
 * @author	bug kang
 * @date	2014-8-21
 * @ingroup	models
 */

#include "DataManager.h"


static DataManager* s_dataManager = NULL;

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

DataManager* DataManager::sharedDataManager()
{
    if (s_dataManager == NULL)
    {
        s_dataManager = new DataManager;
    }
    return s_dataManager;
}

