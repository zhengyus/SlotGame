/*!
 * @brief	�ͻ���socket��
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
 * @brief		��Ϣͷ��
 */
typedef struct MessageHeader
{
    int messageID;
    int length;
    int sequenceID;
}
MessageHeader;

/*!
 * @brief		socket���ݻ��档
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
	 *	��ȡori�������
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
     * @brief		���ӷ�������
     * @param       ip      IP�ַ���
     * @param       port    �˿�   
     */
    bool connect(const char* ip, int port);
    
    /*!
     * @brief		�Ͽ����ӡ�
     */
    bool disconnect();
    
    /*!
     * @brief		�������ݡ�
     * @param       data      ����
     * @param       length    ���ݳ���   
     */
    virtual bool send(const char* data, int length);
    
    /*!
     * @brief		���ʹ���Ϣͷ��protobuf��Ϣ��
     * @param       msg       protobuf��Ϣ
     * @param       msgID     ��Ϣ��
     */
    virtual bool send(google::protobuf::MessageLite* msg, int msgID);
    
    /*!
     * @brief		���ʹ洢�ڷ��ͻ����е����ݡ�
     */
    virtual bool send();
    
    /*!
     * @brief		�������ݲ��洢�����档
     */
    virtual bool receive();
    
    /*!
     * @brief		�ж��Ƿ��������Ϸ�������
     */
    virtual bool isConnected() const;
    
    /*!
     * @brief		�ж��Ƿ��������ӷ�������
     */
    virtual bool isConnecting() const;
    
    /*!
     * @brief		��ȡ�ڲ�socket��
     */
    SOCKET getSocket() const;
    
    /*!
     * @brief		��ȡIP��
     */
    const char* getIP() const;
    
    /*!
     * @brief		��ȡ�˿ڡ�
     */
    int getPort() const;

	/*
	 *	����idle�ļ��ʱ��
	 *
	 *	@param interval	���ʱ��
	 */
	void setIdel(long interval);

	/*
	 *	��ȡ����ʱ��
	 */
	long getConnTime();

	/*
	 *	��������ʱ��
	 */
	void setConnTime(long long currTime);

	/*
	 *	idle
	 */
	virtual void onIdle();
    
private:
    /*!
     * @brief		����socketΪ������ģʽ��
     */
    bool setNonBlocking();
    
    /*!
     * @brief		�ж��Ƿ������ڽ��еķ���������
     */
    bool isBusy();
    
public:
    /*!
     * @brief		�������ʱ�Ļص��� 
     */
    virtual void onError();
    
    /*!
     * @brief		���ӷ������ɹ�ʱ�Ļص��� 
     */
    virtual void onConnect();
    
    /*!
     * @brief		�Ͽ����Ӻ�Ļص��� 
     */
    virtual void onDisconnect();
    
    /*!
     * @brief		���ͳɹ���Ļص��� 
     */
    virtual void onSendData();
    
    /*!
     * @brief		�������ݺ�Ļص��� 
     * @param       messageID   ��Ϣ��
     * @param       message     ��Ϣ��
     * @param       size        ��Ϣ�峤��
     */
    virtual void onReceiveData(int messageID, const char* message, int size) = 0;
    
public:
    bool m_bDiscard;
    
protected:
    /*!
     * @brief		��ȡ�ڲ�socket��
     */
    SOCKET m_socket;
    
    /*!
     * @brief		�Ƿ��Ѿ������Ϸ�������
     */
    bool m_isConnected;
    
    /*!
     * @brief		�Ƿ����ڳ������ӷ�������
     */
    bool m_isConnecting;
    
    /*!
     * @brief		IP��
     */
    std::string m_ip;
    
    /*!
     * @brief		�˿ڡ�
     */
    int m_port;
    
    /*!
     * @brief		�洢���������ݡ�
     */
    SocketBuffer m_sendBuffer;
    
    /*!
     * @brief		�洢���յ������ݡ�
     */
    SocketBuffer m_recvBuffer;

	/*
	 *	socket��������ʱ��
	 */
	long long m_ConnTime;
    
    /*
	 *	idle�ļ��ʱ��
	 */
	long m_Idle;
    /*
     *  �ض���socket name
     */
    const char * m_socket_name;
};
unsigned long get_clock_ms();

#endif
