/*!
 * @brief	处理软键盘状态的变化
 * @author	Zhou Lingfei
 * @date	2013-1-23
 * @ingroup	Common
 */

#include "Common/KeyboardDispatcher.h"
#include "platform/android/jni/JniHelper.h"
#include "string.h"

static KeyboardDispatcher* s_sharedDispatcher = NULL;

KeyboardDispatcher* KeyboardDispatcher::sharedDispatcher()
{
	if (s_sharedDispatcher == NULL)
	{
		s_sharedDispatcher = new KeyboardDispatcher();
	}
	return s_sharedDispatcher;
}

KeyboardDispatcher::KeyboardDispatcher()
{

}

KeyboardDispatcher::~KeyboardDispatcher()
{

}

void KeyboardDispatcher::addDelegate(KeyboardDelegate* delegate)
{
	for (std::list<KeyboardDelegate*>::iterator it = m_delegates.begin(); it != m_delegates.end(); ++it)
	{
		if ((*it) == delegate)
		{
			return;
		}
	}
	m_delegates.push_back(delegate);
}

void KeyboardDispatcher::removeDelegate(KeyboardDelegate* delegate)
{
	m_delegates.remove(delegate);
}

void KeyboardDispatcher::notifyKeyboardPositionChanged()
{
	for (std::list<KeyboardDelegate*>::iterator it = m_delegates.begin(); it != m_delegates.end(); ++it)
	{
		(*it)->onKeyboardPositionChanged();
	}
}

extern "C"
{
	JNIEXPORT void JNICALL Java_com_og_common_OGMainLayout_onKeyboardPositionChanged(JNIEnv* env, jobject obj)
	{
		KeyboardDispatcher::sharedDispatcher()->notifyKeyboardPositionChanged();
	}
};
