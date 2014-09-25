//
//  UIControl.h
//  Texas-iPhone2
//
//  Created by Chaim on 14-7-1.
//
//

#ifndef __Texas_iPhone2__UIControl__
#define __Texas_iPhone2__UIControl__


#include "cocos2d.h"

USING_NS_CC;

class UISystemControl : public cocos2d::CCObject
{
public:
	UISystemControl(void) {};
	~UISystemControl(void) {};
	
	int attachEdit(CCRect r, float fScaleX = 1.0f, float fScaleY = 1.0f);
	void detachEdit(int nObjID);
	
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	
	void setVisible(int nObjID, bool bVisible);
	void openIME(int nObjID);
	void closeIME(int nObjID);
	void setMaxLimit(int nObjID, int nMaxLimit);
	void clear(int nObjID);
	std::string getText(int nObjID);

	void setHeight(int nObjID, float fHeight);
	
	static UISystemControl* sharedInstance();

	static UISystemControl* s_inst;
};


#endif /* defined(__Texas_iPhone2__UIControl__) */
