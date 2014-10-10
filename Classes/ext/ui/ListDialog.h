//
//  ListDialog.h
//  Slot
//
//  Created by huxf on 14-9-2.
//
//

#ifndef __Slot__ListDialog__
#define __Slot__ListDialog__

#include "ext/ui/Dialog.h"
#include "Hdefine.h"

template <typename T>
class ListDialog : public Dialog
{
    
public:
    
    ListDialog(string bgPath):Dialog(bgPath)
    {
        _initListItem = NULL;
    }
    
    ~ListDialog(){}
    
public:
    bool initWithJson(string listJson,string itemJson,vector<T>&data)
    {
        if ( !Dialog::initContentWithJsonFile(listJson.c_str()))
        {
            return false;
        }
        
        _data = &data;
        
//        initContentWithJsonFile(listJson.c_str());
        
        _listView = static_cast<ListView*>(getWidgetByName("ListView_list"));
        Widget* item = GUIReader::shareReader()->widgetFromJsonFile(itemJson.c_str());
        _listView->setItemModel(item);
        
        return true;
    }
    
    static ListDialog<T>* createWithJson(string listJson,string itemJson,string bgPath,vector<T>&data)
    {
        ListDialog<T> *pRet = new ListDialog<T>(data,bgPath);
        if (pRet && pRet->initWithJson(listJson,itemJson))
        {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        
        return NULL;
    }
    
public:
    
    void setInitListItem(void (*initListItem)(int,Widget*,T))
    {
        _initListItem = initListItem;
    }
    
    void show()
    {
        Dialog::show();
        
        if (NULL == _initListItem)
        {
            return;
        }
        
        _listView->removeAllItems();
        Widget* item = NULL;
        for (int i = 0; i < _data->size(); ++i)
        {
            _listView->pushBackDefaultItem();
            item = _listView->getItem(i);
            _initListItem(i,item,_data->at(i));
        }
    }
    
    void refresh(vector<T> data)
    {
        _listView->removeAllItems();
        Widget* item = NULL;
        for (int i = 0; i < data.size(); ++i)
        {
            _listView->pushBackDefaultItem();
            item = _listView->getItem(i);
            _initListItem(i,item,data[i]);
        }
        _listView->jumpToTop();
    }
    
protected:
    vector<T>*  _data;
    ListView*   _listView;
    
    void (*_initListItem)(int,Widget*,T);
};

#endif /* defined(__Slot__ListDialog__) */
