//
//  SysUtil.h
//  Texas-iPhone2
//
//  Created by Chaim on 14-5-3.
//
//

#ifndef __Texas_iPhone2__ImagePickerAndroid__
#define __Texas_iPhone2__ImagePickerAndroid__

#include "cocos2d.h"
#include "Utility/ImagePickerDelegate.h"

USING_NS_CC;

class ImagePickerAndroid : public cocos2d::CCObject
{
public:
	ImagePickerAndroid(void) {};
	~ImagePickerAndroid(void) {};
	
	bool Init();
	
	//设置通知接口
	void SetDelegate(ImagePickerDelegate* pDelegate) { m_pDelegate = pDelegate; }
	
	bool PickImageFromCamera();
	bool PickImageFromCamera(const char* szFile);
	bool PickPhotoFromGallery();
	bool PickPhotoFromGallery(const char* szFile);
	bool CropImage(const char* szSrcFile, const char* szDstFile);
	bool CropImage(const char* szSrcFile);

	
public:
	void OnPickImageFromCamera(int nResult, const char* pszFile);
	void OnPickPhotoFromGallery(int nResult, const char* pszFile);
	void OnCropImage(int nResult, const char* pszFile);

	
protected:
	void OnImageFile(const char* pszFile);
	
public:
	static ImagePickerAndroid* sharedInstance();
	
public:
	static ImagePickerAndroid* s_inst;
	static std::string s_strCameraFile;
	
protected:
	ImagePickerDelegate* m_pDelegate = NULL;
};

#endif /* defined(__Texas_iPhone2__ImagePickerAndroid__) */
