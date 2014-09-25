/*!
 * @brief	����ͻ�������socket
 * @author	Zhou Lingfei
 * @date	2012-9-10
 * @ingroup	Network
 * @note    ʹ��libcurl��multi interfaceʵ�֡�
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
     * @brief		����SocketHandler������  
     */
    static SocketHandler* sharedSocketHandler();
    
    /*!
     * @brief		���һ��socket���б�  
     */
    void addSockect(TcpSocket* socket);
    
    /*!
     * @brief		���б���ɾ��һ��socket��  
     */
    void removeSocket(TcpSocket* socket);
    
public:
    /*!
     * @brief		����CCObject���update������
     */
    virtual void update(float dt);
    
private:
    void re();
    
private:
	typedef std::list<TcpSocket*> SocketList;
	
    /*!
     * @brief		����socket�б�
     */
    SocketList m_sockets;
};

#endif
