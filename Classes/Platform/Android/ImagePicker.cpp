//
//  ImagePicker.mm
//  ImagePicker
//
//  Created by Chaim on 14-4-15.
//
//

#include "ImagePicker.h"
#include "ImagePickerAndroid.h"

ImagePicker::ImagePicker()
{
    
}

void ImagePicker::setCallback(CCObject* pObject, IMAGEPICKER_OnImageData pFunc)
{
	if(m_pObjectOnData)
	{
		m_pObjectOnData->release();
		m_pObjectOnData = NULL;
	}
	m_pObjectOnData = pObject;
	m_pFuncOnData = pFunc;
	if(m_pObjectOnData)
		m_pObjectOnData->retain();
}

void ImagePicker::pickImageFromAlbum(CCObject* pObject, IMAGEPICKER_OnImageData pFunc)
{
	setCallback(pObject, pFunc);
	
	ImagePickerAndroid::sharedInstance()->SetDelegate(this);
	ImagePickerAndroid::sharedInstance()->PickPhotoFromGallery();
}

//从相机选取图片
void ImagePicker::pickImageFromCamera(CCObject* pObject, IMAGEPICKER_OnImageData pFunc)
{
	setCallback(pObject, pFunc);
	
	ImagePickerAndroid::sharedInstance()->SetDelegate(this);
	ImagePickerAndroid::sharedInstance()->PickImageFromCamera();
}

void ImagePicker::onImageData(char *data, int len)
{
	if(m_pObjectOnData && m_pFuncOnData)
	{
		(m_pObjectOnData->*m_pFuncOnData)(0, data, len);
//		m_pObjectOnData->release();
		m_pObjectOnData = NULL;
		m_pFuncOnData = NULL;
	}
}

CCSprite* createSpriteWithImageData(char* data, int len)
{
    if(len == 0)
        return NULL;
    
    const char * path = CCFileUtils::sharedFileUtils()->getWritablePath().c_str();
    
    const char * imagepath = CCString::createWithFormat("%s/ImagePicker_Image.png", path)->getCString();
    
    //将文件保存到imagepath
    FILE *fp = fopen(imagepath, "wb+");
    if(fp)
    {
        if( fwrite(data, len, 1, fp) )
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("ImagePicker_Image", imagepath);
            CCUserDefault::sharedUserDefault()->flush();
        }
        fclose(fp);
    }
    
    
    return CCSprite::createWithTexture( CustomCCTexture2D::create(data, len) );
}

const char* ImagePicker::pickerImageFile()
{
    const char *path = CCUserDefault::sharedUserDefault()->getStringForKey("ImagePicker_Image", "").c_str();
    
    if( strcmp(path, "") )
    {
        return path;
    }
    return NULL;
}


