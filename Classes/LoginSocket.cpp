//
//  LoginSocket.cpp
//  Slot
//
//  Created by zhengyu on 14-8-21.
//
//

#include "LoginSocket.h"
#include "NetworkConstants.h"
#include "ThranConn.pb.h"
#include "OGTexasSlotsProtocol.pb.h"

#include "DataManager.h"
#include <iostream>
#include <sstream>
#include <System.h>
#include "Utils.h"
#include "base64.h"


LoginSocket::LoginSocket()
{
//    m_socket_name = "LoginSocket";
    m_ActionType = ACTION_TYPE_UNKNOWN;
    
    m_tlogconfig.logtype = LOGIN_TYPE;
    m_tlogconfig.logthirdName = LOGIN_THIRD_D_NAME;
    m_tlogconfig.logthirdKey = LOGIN_THIRD_KEY;
    m_tlogconfig.logthirdAppID = LOGIN_THIRD_APPID;
    
}

LoginSocket::~LoginSocket()
{
    CCLog("delete loginSocket");
    
}

void LoginSocket::onError()
{
    if(m_ActionType == ACTION_TYPE_REG)
    {
        if( System::isNetAvailable())
        {
            CCLog("login err0-0");
        }
        else
        {
            CCLog("login err0-1");
        }
    }
    else if(m_ActionType == ACTION_TYPE_LOGIN)
    {
        if( System::isNetAvailable())
        {
            CCLog("login err1-0");
        }
        else
        {
            CCLog("login err1-1");
        }
    }
}

void LoginSocket::onReceiveData(int messageID, const char* message, int size)
{
	//不能断开，断开后再连会需要时间，这段时间send不出去
//	disconnect();
#if (OPEN_SOCKET_OR_HTTP == 0)
    CCLog("in LoginSocket Ret");
    
	switch (messageID)
    {
		case (OGID_ACK | THRANID_SVR_GAMELOGIN):
        {
			onLogin(message, size);
            break;
        }
        case (OGID_ACK | THRANID_SVR_GAMEREGISTER):
        {
            onReg(message, size);
            break;
		}
        default:
        {
            CCLog("in LoginSocket Ret err!!!!!!");
			break;
        }
	}
#endif
}

bool LoginSocket::sendLogin(const char* name, const char* password,int loginType, const ::std::string& thirdDigitalName, const ::std::string& thirdKey, const ::std::string& thirdAppID)
{
    
    std::string mac = System::getMacAddress();
    std::string udid = System::getUDID();
	std::string channelID = System::getLoginChannelID();
    std::ostringstream ostring;
    ostring<<udid<<GAME_ID<<channelID;
    std::string token = System::getMD5String(ostring.str());
    std::string username = name;
	std::string despw = System::getDESString(password);
    
    ReqGameLogin quickConn;
    quickConn.set_username(name);
	quickConn.set_password(despw);
    quickConn.set_terminal((int)System::getDeviceType());
    quickConn.set_devicenumber(udid);
	quickConn.set_comefrom(channelID);
    quickConn.set_gameid(GAME_ID);
    quickConn.set_token(token);
    
    CCLog("kkk username = %s", quickConn.username().c_str());
    CCLog("kkk psw = %s", despw.c_str());
    CCLog("kkk terminal = %d", quickConn.terminal());
    CCLog("kkk devicenumber = %s", quickConn.devicenumber().c_str());
    CCLog("kkk comefrom = %s", quickConn.comefrom().c_str());
    CCLog("kkk gameid = %d", quickConn.gameid());
    CCLog("kkk token = %s", quickConn.token().c_str());
  
#if (OPEN_SOCKET_OR_HTTP == 0)
    return send(&quickConn, THRANID_SVR_GAMELOGIN);
#else
  
    
    
    CCHttpRequest* request = new CCHttpRequest();
    std::string Url = "http://";
    Url.append(URLIP_PORT);
    Url.append("/game/userLogin.do");
    request->setUrl(Url.c_str());
//    request->setUrl("http://172.28.14.86:22447/game/userLogin.do");
//    request->setUrl("http://202.108.0.83:22447/game/userLogin.do");
//    request->setUrl("http://192.168.1.158:22447/game/userLogin.do");
    
    request->setRequestType(CCHttpRequest::kHttpPost);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    request->setResponseCallback(this, httpresponse_selector(LoginSocket::onHttpRequestCompleted));
    
    //网络密码用户名转义
    username = zhuanYi(username);
    despw = zhuanYi(despw);
    
    // write the post data
    char tmpStr[200];
    std::string postData = "account=";
    postData.append(username.c_str());
    postData.append("&accountType=0");
    postData.append("&password=");
    postData.append(despw.c_str());
    postData.append("&terminal=");
    sprintf(tmpStr, "%d", quickConn.terminal());
    postData.append(tmpStr);
    postData.append("&deviceNumber=");
    postData.append(quickConn.devicenumber().c_str());
    postData.append("&gameId=");
    sprintf(tmpStr, "%d", quickConn.gameid());
    postData.append(tmpStr);
    postData.append("&comeFrom=");
    postData.append(quickConn.comefrom().c_str());
    postData.append("&token=");
    postData.append(quickConn.token().c_str());
    
    request->setRequestData(postData.c_str(), postData.length());
    
    CCLog("postStr=%s", postData.c_str());
    
    request->setTag("POST_Login");
    CCHttpClient::getInstance()->send(request);
    request->release();
    return true;
    
#endif
    
}

#if (OPEN_SOCKET_OR_HTTP == 0)
void LoginSocket::onLogin(const char* message, int size)
#else
void LoginSocket::onLogin(AckGameLogin ackLogin)
#endif
{
#if (OPEN_SOCKET_OR_HTTP == 0)
	AckGameLogin ackLogin;
	ackLogin.ParseFromArray(message, size);
#endif
    CCLog("kkk Rhavecert = %s", ackLogin.certhaverole().c_str());
    CCLog("kkk Rnocert= %s", ackLogin.certnorole().c_str());
    CCLog("kkk Rusername = %s", ackLogin.username().c_str());
    CCLog("kkk Rrolename = %s", ackLogin.rolename().c_str());
    CCLog("kkk ret = %d", ackLogin.result());
  
   
    Ret2UILoginMeg ret2UI;
    ret2UI.ret = ackLogin.result();
    ret2UI.errmeg = DataManager::sharedDataManager()->errmeg;
    CCLog("pingtai=%d", ret2UI.ret);
    
#if (OPEN_SOCKET_OR_HTTP == 0)
    //将共有数据存入此处
    DataManager::sharedDataManager()->userid = Utils::CharConversion::GBKToUTF8(ackLogin.rolename());
#else
    //将共有数据存入此处
    DataManager::sharedDataManager()->userid = ackLogin.rolename();
#endif
//    DataManager::sharedDataManager()->cert = ackLogin.certnorole().c_str();//证书
    DataManager::sharedDataManager()->cert = ackLogin.certhaverole().c_str();//证书
    DataManager::sharedDataManager()->gameid = 25022;
    DataManager::sharedDataManager()->comfromid = 0;
    DataManager::sharedDataManager()->entrance = "0";
    DataManager::sharedDataManager()->nickname = "";
    DataManager::sharedDataManager()->username = ackLogin.username();
    DataManager::sharedDataManager()->come = "yd";
#if (OPEN_DEBUG == 1)
    //如果登入成功，开启linkgamesocket，连接下个服务器ip，端口
    if(RET_SUCCESS == ret2UI.ret)
    {
        LoginMeg2 logmeg2;
        logmeg2.login1.set_userid(DataManager::sharedDataManager()->userid);

        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LINKGAME2MSG, &logmeg2);
        
//        this->disconnect();
    }
    
#endif
    
    //向UI发送登入平台返回信息
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_LOGINMES_FROM_MSG, &ret2UI);
    
}

bool LoginSocket::sendReg(const char* name, const char* password)
{
	std::string udid = System::getUDID();
	std::string channelID = System::getLoginChannelID();
	std::ostringstream ostring;
	ostring<<udid<<GAME_ID<<channelID;
	std::string token = System::getMD5String(ostring.str());
    std::string username = name;
    std::string despw = password;
//	std::string despw = System::getDESString(password);
    
	ReqGameRegister reg;
	reg.set_username(name);
//    reg.set_username("fishtest012");
	reg.set_password(password);
    reg.set_terminal((int)System::getDeviceType());
	reg.set_devicenumber(udid);
	reg.set_comefrom(channelID);
    reg.set_gameid(GAME_ID);
	reg.set_token(token);
    
#if (OPEN_SOCKET_OR_HTTP == 0)
    return send(&reg, THRANID_SVR_GAMEREGISTER);
#else
    CCHttpRequest* request = new CCHttpRequest();
    std::string Url = "http://";
    Url.append(URLIP_PORT);
    Url.append("/game/userRegister.do");
    request->setUrl(Url.c_str());
//    request->setUrl("http://172.28.14.86:22447/game/userRegister.do");
//    request->setUrl("http://202.108.0.83:22447/game/userRegister.do");
    
    request->setRequestType(CCHttpRequest::kHttpPost);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    request->setResponseCallback(this, httpresponse_selector(LoginSocket::onHttpRequestCompleted));
    
    //网络密码用户名转义
    username = zhuanYi(username);
    despw = zhuanYi(despw);
    
    // write the post data
    char tmpStr[200];
    std::string postData = "userName=";
    postData.append(username.c_str());
    postData.append("&password=");
    postData.append(despw.c_str());
    postData.append("&terminal=");
    sprintf(tmpStr, "%d", (int)System::getDeviceType());
    postData.append(tmpStr);
    postData.append("&deviceNumber=");
    postData.append(reg.devicenumber().c_str());
    postData.append("&gameId=");
    sprintf(tmpStr, "%d", reg.gameid());
    postData.append(tmpStr);
    postData.append("&comeFrom=");
    postData.append(reg.comefrom().c_str());
    postData.append("&token=");
    postData.append(reg.token().c_str());
    
    request->setRequestData(postData.c_str(), postData.length());
    
    request->setTag("POST_Reg");
    CCHttpClient::getInstance()->send(request);
    request->release();
    return true;
#endif
}

#if (OPEN_SOCKET_OR_HTTP == 0)
void LoginSocket::onReg(const char* message, int size)
#else
void LoginSocket::onReg(AckGameRegister ackReg)
#endif
{
#if (OPEN_SOCKET_OR_HTTP == 0)
    AckGameRegister ackReg;
	ackReg.ParseFromArray(message, size);
#endif
    
    Ret2UIRegMeg meg;
    meg.ret = ackReg.result();
    meg.errmessage = ackReg.errormsg();
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_REGMES_FROM_MSG, &meg);
    
    
    CCLog("kkk err= %s", ackReg.errormsg().c_str());
    CCLog("kkk ret = %d", ackReg.result());
    
}

bool LoginSocket::sendCreateRole(const char* userName, const char* roleName, int maleid)
{
    CCHttpRequest* request = new CCHttpRequest();
    std::string Url = "http://";
    Url.append(URLIP_PORT);
    Url.append("/game/createRoleByRandom.do");
    request->setUrl(Url.c_str());
//    request->setUrl("http://172.28.14.86:22447/game/createRoleByRandom.do");
//    request->setUrl("http://202.108.0.83:22447/game/createRoleByRandom.do");
    
    request->setRequestType(CCHttpRequest::kHttpPost);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    
#if (OPEN_SOCKET_OR_HTTP == 1)
    request->setResponseCallback(this, httpresponse_selector(LoginSocket::onHttpRequestCompleted));
#endif
    
    //网络密码用户名转义
    std::string Susername = userName;
    std::string Srolename = roleName;
    
    Susername = zhuanYi(Susername);
    Srolename = zhuanYi(Srolename);
    
    // write the post data
    char strTmp[5];
    std::string postData = "userName=";
    postData.append(Susername.c_str());
    postData.append("&roleName=");
    postData.append(Srolename.c_str());
    postData.append("&accountCert=");
    CCLog("cccccc=%s", DataManager::sharedDataManager()->cert16.c_str());
    postData.append(DataManager::sharedDataManager()->cert16.c_str());
    postData.append("&sex=");
    sprintf(strTmp, "%d", maleid);
    postData.append(strTmp);
    postData.append("&random=0");
    postData.append("&gameId=");
    sprintf(strTmp, "%d", GAME_ID);
    postData.append(strTmp);
    
    
    request->setRequestData(postData.c_str(), postData.length());
    
    request->setTag("POST_CreateRole");
    CCHttpClient::getInstance()->send(request);
    request->release();
    return true;
}

void LoginSocket::onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
  
    CCLog("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        
        AckGameLogin ackLogin;
        ackLogin.set_result(-1);
#if (OPEN_SOCKET_OR_HTTP == 1)
        onLogin(ackLogin);
#endif
        
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    printf("Http Test, dump data: ");
    
    std::string retDate;
    
    char tmpStr[5000];
    
    for(int i = 0; i < 5000; i++)
    {
        tmpStr[i] = '\0';
    }
    
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%c", (*buffer)[i]);
        tmpStr[i] = (*buffer)[i];
    }
    
    printf("\n");
    retDate = tmpStr;
    CCLog("DAET = %s", retDate.c_str());

    Json* myjson = Json_create(retDate.c_str());
    
    if(strcmp(response->getHttpRequest()->getTag(), "POST_Login") == 0)
    {
    
        const char* errorCode = Json_getString(myjson, "errorCode", "");
        const char* errorMessage = Json_getString(myjson, "errorMessage", "");
        CCLog("errorCode = %s", errorCode);
        CCLog("errorMessage = %s", errorMessage);
        
        const char* certhaverole = Json_getString(myjson, "accountCert", "");
        const char* certnorole = Json_getString(myjson, "accountCert", "");
        
        //证书转义
        std::string Scert = certhaverole;
        Scert = zhuanYi(Scert);
//        CCLog("ZhuanYi0=%s", Scert.c_str());
        
        //16进制证书
//        const char* certhaverole = Json_getString(myjson, "accountCert", "");
//        const char* certnorole = Json_getString(myjson, "accountCert", "");
        
        //16进制证书转字节数组
//        std::string ScertScert = fromHexString(certnorole);
//        CCLog("ZhuanYi1=%s", ScertScert.c_str());
        
        DataManager::sharedDataManager()->cert16 = Scert;
        
        const char* username = Json_getString(myjson, "userName", "");
        const char* rolename = Json_getString(myjson, "roleName", "");
        DataManager::sharedDataManager()->errmeg = Json_getString (myjson, "message", "");
        DataManager::sharedDataManager()->sex = Json_getInt (myjson, "sex", 0);
        int ret = Json_getInt (myjson, "result", -1);
        
//        if(ret != -1)
//        {
        
            AckGameLogin ackLogin;
            ackLogin.set_certhaverole(certhaverole);
            ackLogin.set_certnorole(certnorole);
            ackLogin.set_username(username);
            ackLogin.set_rolename(rolename);
            ackLogin.set_result(ret);
            
//            CCLog("HTTP Rhavecert = %s", ackLogin.certhaverole().c_str());
//            CCLog("HTTP Rnocert = %s", ackLogin.certnorole().c_str());
            CCLog("HTTP Rusername = %s", ackLogin.username().c_str());
            CCLog("HTTP Rrolename = %s", ackLogin.rolename().c_str());
            CCLog("HTTP ret = %d", ackLogin.result());
#if (OPEN_SOCKET_OR_HTTP == 1)
            CCLog("hhhhhh11119~~~~~~~");
            onLogin(ackLogin);
#endif
//        }
        
    }
    else if(strcmp(response->getHttpRequest()->getTag(), "POST_Reg") == 0)
    {

        const char* errorMessage = Json_getString(myjson, "message", "");
        CCLog("message = %s", errorMessage);
        int ret = Json_getInt(myjson, "result", -119);
        CCLog("ret = %d", ret);
            
        AckGameRegister ackReg;
        ackReg.set_errormsg(errorMessage);
        ackReg.set_result(ret);
            
#if (OPEN_SOCKET_OR_HTTP == 1)
        onReg(ackReg);
#endif
        
    }
    else if(strcmp(response->getHttpRequest()->getTag(), "POST_CreateRole") == 0)
    {
//        onReturn();
        
        int ret = Json_getInt(myjson, "result", -119);
        const char* errorMessage = Json_getString(myjson, "message", "");
        
        CCLog("ret=%d", ret);
        CCLog("err Meg=%s", errorMessage);
        
        Ret2UIRegMeg meg;
        meg.ret = ret;
        meg.errmessage = errorMessage;
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(CREATE_ROLE_EVENT_RET, &meg);

    }
    
    Json_dispose(myjson);

}

std::string LoginSocket::zhuanYi(const std::string mystr)
{
    std::string strRet = mystr;
    
    while (1)
    {
        if(strRet.find('&', 0) != string::npos)
        {
            strRet.replace(strRet.find('&', 0), 1, "%26");
        }
        else
        {
            break;
        }
    }
    
    while (1)
    {
        if(strRet.find('+', 0) != string::npos)
        {
            strRet.replace(strRet.find('+', 0), 1, "%2B");
        }
        else
        {
            break;
        }
    }
    
//    Json::Value tmpValue;
//    if (username != null && username != "")
//    {
//    	username=username.replace(/\+/g, "%2B");
//    	username=username.replace(/\&/g, "%26");
//    }
//    
//    if (password != null && password != "")
//    {
//    	password=password.replace(/\+/g, "%2B");
//    	password=password.replace(/&/g, "%26");
//    }

    return strRet;
}

std::string LoginSocket::zhuanYiFan(const std::string mystr)
{
    std::string strRet = mystr;
    
    while (1)
    {
        if(strRet.find("%26", 0) != string::npos)
        {
            strRet.replace(strRet.find("%26", 0), 1, "&");
        }
        else
        {
            break;
        }
    }
    
    while (1)
    {
        if(strRet.find("%2B", 0) != string::npos)
        {
            strRet.replace(strRet.find("%2B", 0), 1, "+");
        }
        else
        {
            break;
        }
    }
    
    //    Json::Value tmpValue;
    //    if (username != null && username != "")
    //    {
    //    	username=username.replace(/\+/g, "%2B");
    //    	username=username.replace(/\&/g, "%26");
    //    }
    //
    //    if (password != null && password != "")
    //    {
    //    	password=password.replace(/\+/g, "%2B");
    //    	password=password.replace(/&/g, "%26");
    //    }
    
    return strRet;
}

const char *LoginSocket::hexStringToByte(std::string hex)
{
    //获取传入16进制字符串长度
    int len = (hex.length() / 2);
    //申请字符存储空间
    char* result = new char[len];
    
    const char* achar = hex.c_str();
    
    //转码
    for (int i = 0; i < len; i++)
    {
        int pos = i * 2;
        result[i] = (char) ((unsigned char)(achar[pos]) << 4 | ((unsigned char)(achar[pos + 1])));
    }
    
    return result;
}

const char *LoginSocket::fromHex(const char * sc, int len)
{
    
    char * res = new char[len / 2];
    
    for (int i = 0; i < len; i++)
    {
        
        char c1 = (char) (sc[i] - 48 < 17 ? sc[i] - 48 : sc[i] - 55);
        
        i++;
        
        char c2 = (char) (sc[i] - 48 < 17 ? sc[i] - 48 : sc[i] - 55);
        
        res[i / 2] = (char) (c1 * 16 + c2);
        
    }
    
    return res;
    
}

const char *LoginSocket::fromHexString(std::string hex)
{
    return fromHex(hex.c_str(), hex.length());
}
