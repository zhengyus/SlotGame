/*!
 * @brief	管理客户端所有socket
 * @author	Zhou Lingfei
 * @date	2012-9-10
 * @ingroup	Network
 * @note    使用libcurl的multi interface实现。
 */

#ifndef __SOCKET_HANDLER_H__
#define __SOCKET_HANDLER_H__

#include "cocos2d.h"

#include <list>

USING_NS_CC;

class TcpSocket;

class SocketHandler : public CCObject
{
private:
    SocketHandler();
    ~SocketHandler();
    
public:
    /*!
     * @brief		返回SocketHandler单例。  
     */
    static SocketHandler* sharedSocketHandler();
    
    /*!
     * @brief		添加一个socket到列表。  
     */
    void addSockect(TcpSocket* socket);
    
    /*!
     * @brief		从列表中删除一个socket。  
     */
    void removeSocket(TcpSocket* socket);
    
public:
    /*!
     * @brief		覆盖CCObject类的update方法。
     */
    virtual void update(float dt);
    
private:
    void re();
    
private:
	typedef std::list<TcpSocket*> SocketList;
	
    /*!
     * @brief		所有socket列表。
     */
    SocketList m_sockets;
};

#endif
