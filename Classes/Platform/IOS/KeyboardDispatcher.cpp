/*!
 * @brief	处理软键盘状态的变化
 * @author	Zhou Lingfei
 * @date	2013-1-23
 * @ingroup	Common
 */

#include "Common/KeyboardDispatcher.h"
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

}

void KeyboardDispatcher::removeDelegate(KeyboardDelegate* delegate)
{

}

void KeyboardDispatcher::notifyKeyboardPositionChanged()
{
}
