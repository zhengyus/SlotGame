/*!
 * @brief	øÕªß∂Àsocket¿‡
 * @author	Zhou Lingfei
 * @date	2012-9-6
 * @ingroup	Network
 */

#include "TcpSocket.h"
#include "SocketHandler.h"

#include <iostream>

TcpSocket::TcpSocket()
{
	m_socket        = INVALID_SOCKET;
    m_isConnected   = false;
    m_isConnecting  = false;
    m_bDiscard      = false;
}

TcpSocket::~TcpSocket()
{
    disconnect();
	SocketHandler::sharedSocketHandler()->removeSocket(this);
}

bool TcpSocket::connect(const char* ip, int port)
{
    m_ip = ip;
    m_port = port;

	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    m_isConnected = false;
    m_isConnecting = false;

	if (m_socket == INVALID_SOCKET)
	{
//		OGLog(LogTypeNetwork,"[socket] Invalid Socket!");
		return false;
	}
    
    if (!setNonBlocking())
    {
//        OGLog(LogTypeNetwork,"[socket] Failed to set socket in nonblocking mode!");
        return false;
    }
//	OGLog(LogTypeNetwork,"[%s socket] connect to server: ip = %s, port = %d", m_socket_name, ip, port);
    
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    sa.sin_port = htons(port);
    
    int result = ::connect(m_socket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa));
    if (result == SOCKET_ERROR)
    {
        if (isBusy())
        {
            m_isConnecting = true;
        }
        else
        {
//            OGLog(LogTypeNetwork,"[socket] Failed to connect to server!");
            return false;
        }
    }
    
    m_recvBuffer.clear();

	SocketHandler::sharedSocketHandler()->addSockect(this);

    return true;
}

bool TcpSocket::disconnect()
{
	if (m_socket != INVALID_SOCKET)
    {
        int result = ::closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        
        
		return (result != SOCKET_ERROR);
	}
       m_isConnected = false;
    m_isConnecting = false;
    
    m_recvBuffer.clear();
    
	return true;
}

bool TcpSocket::isConnected() const
{
    return (m_socket != INVALID_SOCKET) && m_isConnected;
}

bool TcpSocket::isConnecting() const
{
    return (m_socket != INVALID_SOCKET) && m_isConnecting;
}

SOCKET TcpSocket::getSocket() const
{
    return m_socket;
}

const char* TcpSocket::getIP() const
{
    return m_ip.c_str();
}

int TcpSocket::getPort() const
{
    return m_port;
}

bool TcpSocket::send(const char *data, int length)
{
    // »Áπ˚“—¡¨Ω”£¨‘Ú÷±Ω”∑¢ÀÕ
    if (isConnected())
    {
        //OGLog(LogTypeNetwork,"%lu[%s send].isConnected socket = %d length = %d", get_clock_ms(), m_socket_name, m_socket, length);
        
//        length = strlen(data);
        
        int result = 0;
        try
        {
            result = ::send(m_socket, data, length, 0);
        }
        catch (...)
        {
            disconnect();
            onError();
            return false;
        }
        
//        CCLog("Socket length = %d", length);
        

        if (result == SOCKET_ERROR)
        {
			CCLog("send fail:%d,%d", result, errno);
            return isBusy();
        }
        else if (result == 0)
        {
			CCLog("send fail:%d", result);
            disconnect();
            onError();
            return false;
        }
    }
    // »Áπ˚’˝‘⁄¡¨Ω”£¨‘ÚÕ∆≥Ÿ∑¢ÀÕ
    else if (isConnecting())
    {
        //OGLog(LogTypeNetwork,"%lu[%s send].isConnecting socket = %d length = %d",get_clock_ms(), m_socket_name, m_socket, length);
        CCLog("conecting.........");
        m_sendBuffer.setBuffer(data, length);
    }
    else
    {
        CCLog("[TcpSocket::send].unknown socket = %d length = %d", m_socket, length);
        disconnect();
        connect(m_ip.c_str(), m_port);
        m_sendBuffer.setBuffer(data, length);
//        return false;
    }
    
    return true;
}

bool TcpSocket::send()
{
	if (m_sendBuffer.length > 0)
	{
		bool result = send(m_sendBuffer.data, m_sendBuffer.length);
        m_sendBuffer.length = 0;
        return result;
	}
    return true;
}

bool TcpSocket::send(google::protobuf::MessageLite* msg, int msgID)
{
	int size = sizeof(MessageHeader);
	int msgLen = 0;
	if (msg != NULL)
	{
		msgLen = msg->ByteSize();
		size += msgLen;
	}
	//OGLog(LogTypeNetwork,"[%s send].msgid = 0x%x, msglen = %d, size = %d", m_socket_name, msgID, msgLen, size);
    
    MessageHeader header;
    header.length = msgLen;
    header.messageID = msgID;
    header.sequenceID = 0;
    
    // œ˚œ¢Õ∑
    char* buffer = (char*)malloc(size);
    memcpy(buffer, &header, sizeof(header));
    
    // œ˚œ¢ÃÂ
    char* msgBuffer = buffer + sizeof(header);
    
//    buffer[0] = msgID>>24;
//    buffer[1] = msgID>>16;
//    buffer[2] = msgID>>8;
//    buffer[3] = msgID;
//    
//    buffer[4] = msgLen>>24;
//    buffer[5] = msgLen>>16;
//    buffer[6] = msgLen>>8;
//    buffer[7] = msgLen;

	if (msg != NULL)
	{
		msg->SerializeToArray(msgBuffer, msgLen);
	}
    
    int result = send(buffer, size);
    
    free(buffer);

    return (result > 0);
}

bool TcpSocket::receive()
{
    if (!isConnected())
    {
        return false;
    }
    
    // ø…ƒ‹–Ë“™◊È∞¸£¨À˘“‘√ø¥ŒΩ´ ’µΩµƒ ˝æ›ÃÌº”µΩ‘≠ ˝æ›ª∫¥Ê÷Æ∫Û
    char* data = m_recvBuffer.data + m_recvBuffer.length;
    int size = SOCKET_BUFFER_SIZE - m_recvBuffer.length;
    
    int result = ::recv(m_socket, data, size, 0);
    if (result == SOCKET_ERROR)
    {
        return isBusy();
    }
    else if (result <= 0)
    {
        disconnect();
        onError();
        return false;
    }
    else
    {
        m_recvBuffer.length += result;
        
        if (m_recvBuffer.length >= sizeof(MessageHeader))
        {
            // µ•∏ˆœ˚œ¢ÃÂµƒ≥§∂»
            int msgLen = m_recvBuffer.getHeader()->length;
            
            // Œﬁ–ßµƒœ˚œ¢°£π˚∂œ…æ°£
            if (msgLen < 0 || msgLen > SOCKET_BUFFER_SIZE)
            {
                m_recvBuffer.clear();
                return false;
            }
            
            // ≤∞¸
            while (m_recvBuffer.getBodySize() > msgLen)
            {
                onReceiveData(m_recvBuffer.getMessageID(), m_recvBuffer.getBody(), msgLen);
                
                if (m_recvBuffer.length < sizeof(MessageHeader))
                {
                    break;
                }

                m_recvBuffer.setOffset(msgLen + sizeof(MessageHeader));
                if (m_recvBuffer.length < sizeof(MessageHeader))
                {
                    break;
                }
                msgLen = m_recvBuffer.getHeader()->length;
            }
        }
        
        // µ»¥˝◊È∞¸
        if (m_recvBuffer.length < sizeof(MessageHeader) || m_recvBuffer.getHeader()->length > m_recvBuffer.getBodySize())
        {
            return true;
        }
        else
        {
            onReceiveData(m_recvBuffer.getMessageID(), m_recvBuffer.getBody(), m_recvBuffer.getBodySize());
            m_recvBuffer.clear();
        }
    }
    return true;
}

bool TcpSocket::isBusy()
{
#ifdef _WIN32
    return WSAGetLastError() == WSAEWOULDBLOCK;
#else
    int err = errno;
    return err == EAGAIN || err == EINPROGRESS;
#endif
    return false;
}

bool TcpSocket::setNonBlocking()
{
#ifdef _WIN32
    unsigned long l = 1;
    int result = ioctlsocket(m_socket, FIONBIO, &l);
    return result != SOCKET_ERROR;
#else
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == SOCKET_ERROR)
    {
        return false;
    }
    int result = fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
    return result != SOCKET_ERROR;
#endif
    return false;
}

void TcpSocket::onError()
{
}

void TcpSocket::onConnect()
{
    //OGLog(LogTypeNetwork,"[%s onconnect]", m_socket_name);
    m_isConnected = true;
    m_isConnecting = false;
}

void TcpSocket::onDisconnect()
{
    m_isConnected = false;
    m_isConnecting = false;
    m_socket = INVALID_SOCKET;
}

void TcpSocket::onSendData()
{
}

/*
 *	…Ë÷√idleµƒº‰∏Ù ±º‰
 */
void TcpSocket::setIdel(long interval)
{
	m_Idle = interval;
}

/*
 *	ªÒ»°Ω®¡¢¡¨Ω”µƒ ±º‰
 */
long TcpSocket::getConnTime()
{
	return m_ConnTime;
}

/*
 *	…Ë÷√¡¥Ω” ±º‰
 */
void TcpSocket::setConnTime(long long currTime)
{
	m_ConnTime = currTime;
}

void TcpSocket::onIdle()
{
}

unsigned long get_clock_ms(){
    return clock()/10000;
}

