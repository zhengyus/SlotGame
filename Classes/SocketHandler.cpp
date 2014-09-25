/*!
 * @brief	π‹¿ÌøÕªß∂ÀÀ˘”–socket
 * @author	Zhou Lingfei
 * @date	2012-9-10
 * @ingroup	Network
 * @note     π”√libcurlµƒmulti interface µœ÷°£
 */
#include "SocketHandler.h"
#include "CCScheduler.h"
#include "TcpSocket.h"

#include <signal.h>

static SocketHandler* s_socketHandler = NULL;

SocketHandler::SocketHandler()
{
    //struct sigaction sa;
    //sa.sa_handler = SIG_IGN;
    //sigaction(SIGPIPE, &sa, NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

SocketHandler::~SocketHandler()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
}

SocketHandler* SocketHandler::sharedSocketHandler()
{
    if (s_socketHandler == NULL)
    {
        s_socketHandler = new SocketHandler;
    }
    return s_socketHandler;
}

void SocketHandler::addSockect(TcpSocket *socket)
{
	for (SocketList::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
	{
		if ((*it) == socket)
		{
			return;
		}
	}
    
    m_sockets.push_back(socket);
}

void SocketHandler::removeSocket(TcpSocket *socket)
{
    m_sockets.remove(socket);
}

void SocketHandler::update(float dt)
{
    int maxfd = 0;
    for (SocketList::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        SOCKET s = (*it)->getSocket();
        if (s != INVALID_SOCKET && maxfd < s)
        {
            maxfd = s;
        }
    }
    if (maxfd <= 0)
    {
        return;
    }
    
    fd_set fs_read;
    fd_set fs_write;
    fd_set fs_except;
    
    FD_ZERO(&fs_read);
    FD_ZERO(&fs_write);
    FD_ZERO(&fs_except);
    
    for (SocketList::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        SOCKET s = (*it)->getSocket();
        if (s != INVALID_SOCKET)
        {
            FD_SET(s, &fs_read);
            FD_SET(s, &fs_write);
            FD_SET(s, &fs_except);
        }
    }
    
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    int result = select(maxfd + 1, &fs_read, &fs_write, &fs_except, &tv);
	if (result == SOCKET_ERROR)
	{
		//OGLog(LogTypeNetwork,"[socket] select error!");
        CCLog("ERR socket.........");
	}
	else
	{
		for (SocketList::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
		{
			TcpSocket* socket = *it;
            SOCKET s = socket->getSocket();
            if (s != INVALID_SOCKET)
            {
                if (FD_ISSET(s, &fs_read))
                {
                    if (socket->isConnected())
                    {
                        socket->receive();
                    }
                }
                
                if (FD_ISSET(s, &fs_write))
                {
                    int value = 0;
                    socklen_t length = sizeof(value);
                    getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&value, &length);

//                    CCLog("send0");
                    //不等于0代表有错误发生
                    if (value != 0)
                    {
//                        CCLog("send1");
                        socket->disconnect();
                        socket->onError();
                        continue;
                    }
                    else
                    {
//                        CCLog("send2");
                        if (socket->isConnecting())
                        {
//                            CCLog("send3");
                            socket->onConnect();
                        }
                        
//                        CCLog("send4");
						socket->send();
//                        CCLog("send5");
                    }
                }
                
                if (FD_ISSET(s, &fs_except))
                {
					socket->disconnect();
                    socket->onError();
                }
				else
				{
					if (socket->isConnected())
					{
						socket->onIdle();
					}
				}
            }
		}
	}
    
//    re();
}

void SocketHandler::re()
{
    for (SocketList::iterator it = m_sockets.begin(); it != m_sockets.end(); it++)
    {
        TcpSocket* socket = *it;
        if (socket->m_bDiscard){
            removeSocket(socket);
            break;
        }
    }
}
