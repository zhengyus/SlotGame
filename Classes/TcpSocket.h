/*!
 * @brief	客户端socket类
 * @author	Zhou Lingfei
 * @date	2012-9-6
 * @ingroup	Network
 */

#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include <list>
#include <string>

#ifdef _WIN32
#include <winsock.h>
typedef	int socklen_t;
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
#endif

#include <google/protobuf/message_lite.h>

#define SOCKET_BUFFER_SIZE  327680

/*!
 * @brief		消息头。
 */
typedef struct MessageHeader
{
    int messageID;
    int length;
    int sequenceID;
}
MessageHeader;

/*!
 * @brief		socket数据缓存。
 */
typedef struct SocketBuffer
{
    char data[SOCKET_BUFFER_SIZE];
    int length;
    
    SocketBuffer()
    {
        length = 0;
    }
    
    void setBuffer(const char* data, size_t length)
    {
        memcpy(this->data, data, length);
        this->length = length;
    }
    
    void clear()
    {
        length = 0;
    }

	/*
	 *	截取ori后的数据
	 */
	void setOffset(size_t ori)
	{
        if (length > ori)
        {
            memmove(data, data + ori, length - ori);
            length = length - ori;
        }
	}
    
    MessageHeader* getHeader()
    {
        return (MessageHeader*)data;
    }
    
    int getMessageID()
    {
        return getHeader()->messageID;
    }
    
    char* getBody()
    {
        return (data + sizeof(MessageHeader));
    }

	//char* getBodys(int iLen)
	//{
	//	char dataTmp[SOCKET_BUFFER_SIZE];
	//	memcpy(dataTmp, data + sizeof(MessageHeader), iLen);
	//	return dataTmp;

	//	return (data + sizeof(MessageHeader));
	//}
    
    int getBodySize()
    {
        return length - sizeof(MessageHeader);
    }
}
SocketBuffer;

class TcpSocket
{
public:
    TcpSocket();
    ~TcpSocket();
    
public:
    /*!
     * @brief		连接服务器。
     * @param       ip      IP字符串
     * @param       port    端口   
     */
    bool connect(const char* ip, int port);
    
    /*!
     * @brief		断开连接。
     */
    bool disconnect();
    
    /*!
     * @brief		发送数据。
     * @param       data      数据
     * @param       length    数据长度   
     */
    virtual bool send(const char* data, int length);
    
    /*!
     * @brief		发送带消息头的protobuf消息。
     * @param       msg       protobuf消息
     * @param       msgID     消息号
     */
    virtual bool send(google::protobuf::MessageLite* msg, int msgID);
    
    /*!
     * @brief		发送存储在发送缓存中的数据。
     */
    virtual bool send();
    
    /*!
     * @brief		接收数据并存储到缓存。
     */
    virtual bool receive();
    
    /*!
     * @brief		判断是否已连接上服务器。
     */
    virtual bool isConnected() const;
    
    /*!
     * @brief		判断是否正在连接服务器。
     */
    virtual bool isConnecting() const;
    
    /*!
     * @brief		获取内部socket。
     */
    SOCKET getSocket() const;
    
    /*!
     * @brief		获取IP。
     */
    const char* getIP() const;
    
    /*!
     * @brief		获取端口。
     */
    int getPort() const;

	/*
	 *	设置idle的间隔时间
	 *
	 *	@param interval	间隔时间
	 */
	void setIdel(long interval);

	/*
	 *	获取链接时间
	 */
	long getConnTime();

	/*
	 *	设置链接时间
	 */
	void setConnTime(long long currTime);

	/*
	 *	idle
	 */
	virtual void onIdle();
    
private:
    /*!
     * @brief		设置socket为非阻塞模式。
     */
    bool setNonBlocking();
    
    /*!
     * @brief		判断是否有正在进行的非阻塞任务。
     */
    bool isBusy();
    
public:
    /*!
     * @brief		网络出错时的回调。 
     */
    virtual void onError();
    
    /*!
     * @brief		连接服务器成功时的回调。 
     */
    virtual void onConnect();
    
    /*!
     * @brief		断开连接后的回调。 
     */
    virtual void onDisconnect();
    
    /*!
     * @brief		发送成功后的回调。 
     */
    virtual void onSendData();
    
    /*!
     * @brief		接收数据后的回调。 
     * @param       messageID   消息号
     * @param       message     消息体
     * @param       size        消息体长度
     */
    virtual void onReceiveData(int messageID, const char* message, int size) = 0;
    
public:
    bool m_bDiscard;
    
protected:
    /*!
     * @brief		获取内部socket。
     */
    SOCKET m_socket;
    
    /*!
     * @brief		是否已经连接上服务器。
     */
    bool m_isConnected;
    
    /*!
     * @brief		是否正在尝试连接服务器。
     */
    bool m_isConnecting;
    
    /*!
     * @brief		IP。
     */
    std::string m_ip;
    
    /*!
     * @brief		端口。
     */
    int m_port;
    
    /*!
     * @brief		存储待发送数据。
     */
    SocketBuffer m_sendBuffer;
    
    /*!
     * @brief		存储接收到的数据。
     */
    SocketBuffer m_recvBuffer;

	/*
	 *	socket建立连接时间
	 */
	long long m_ConnTime;
    
    /*
	 *	idle的间隔时间
	 */
	long m_Idle;
    /*
     *  特定的socket name
     */
    const char * m_socket_name;
};
unsigned long get_clock_ms();

#endif
