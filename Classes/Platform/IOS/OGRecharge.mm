#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <StoreKit/SKPayment.h>
#import <StoreKit/SKPaymentQueue.h>
#import <StoreKit/SKProductsRequest.h>
#import <StoreKit/SkError.h>

#include "cocos2d.h"
#include "support/base64.h"
#include "json.h"


#include "Network/OGRecharge.h"
#include "OGRechargeObserver.h"
#include "NetworkConstants.h"
#include "UI/ProgressHUD.h"
#include "Common/Localization.h"
#include "Events/EventManager.h"
#include "Common/Utilities.h"
#include "Common/SaveDataManger.h"

#include "Common/Logger.h"
#include "AppGlobal.h"

USING_NS_CC;

//超时时间
#define Time_Out  20


OGRecharge& OGRecharge::getInstance()
{
    static OGRecharge trnRecharge;
    return trnRecharge;
}

OGRecharge::OGRecharge()
{
    m_socket_name = "thran charge";
    [OGRechargeObserver sharedRechargeObserver];
}

bool OGRecharge::isRecharging() const
{
    return m_isRecharging;
}

void OGRecharge::setRechargeUI(OGRechargeDelegate* ui)
{
    m_delegate = ui;
}

void OGRecharge::trackCurrentStatement(float t)
{
}
/*
// 向服务器请求支付产品列表
void OGRecharge::sendProductsListRequest()
{
    REQAppleProList req;

    req.set_gameid(GAME_ID);
    req.set_devicename("ipad");
//    connect(THRAN_SERVER_IP, THRAN_SERVER_PORT);
	connect(AppGlobal::sharedInstance()->ThranHost().c_str(), AppGlobal::sharedInstance()->ThranPort());
    send(&req, THRANID_SVR_GETAPPLEPRO);
    
    OGLog(LogTypeNetwork, "[req] Request for products list.\n");
}

// 接收到产品列表
void OGRecharge::onProductsList(const char* message, int size)
{
    ACKAppleProList proList;
    proList.ParseFromArray(message, size);
        
    m_products.clear();
    
    std::string products;
    for (int i = 0; i < proList.appleprolist_size(); i++)
    {
        AppleProItem* item = proList.mutable_appleprolist(i);
        m_products.push_back(*item);
        products.append(item->productid());
        products.append(", ");
    }
    
    if (m_delegate != NULL)
    {
        m_delegate->showProductsList(m_products);
    }

    OGLog(LogTypeNetwork, "[ack] Products list: %s\n", products.c_str());
}
*/

// 向苹果验证产品列表
void OGRecharge::sendAppleVerifyProducts()
{
    NSSet* productSet = [NSSet setWithObjects:nil];
	[[OGRechargeObserver sharedRechargeObserver] verifyProducts:productSet];
}

// 向苹果服务器发送充值请求
void OGRecharge::sendRecharge(const std::string& product,		// 产品ID
							  const std::string& account,		// 帐号名
							  const std::string& role,
							  const std::string& payType,		// payType
                              const std::string& goodsbepid)         // 角色名
{
    NSString* productStr = [NSString stringWithCString:product.c_str() encoding:NSUTF8StringEncoding];
    [[OGRechargeObserver sharedRechargeObserver] buyProduct:productStr];
    
    m_isRecharging = true;
    writeRechargeUser(account, role);
    startTimer();
    OGLog(LogTypeNetwork, "[IGP] User %s recharge for product: %s.", account.c_str(), product.c_str());
}

void OGRecharge::processPendingTransactions()
{
	[[OGRechargeObserver sharedRechargeObserver] processPendingTransactions];
}

/*
// 向索蓝服务器发送苹果的收据
void OGRecharge::sendAppleReceipt(const std::string& receipt, const std::string& uid, const std::string& account, const std::string& role)
{               
    REQAppleReceipt req;
    req.set_receipt(receipt);
    req.set_uid(uid);
    req.set_username(account);
    req.set_rolename(role);
    req.set_gameid(GAME_ID);
    req.set_channelid(System::getChannelID());
    //对于ios原来的ChannelID只在登陆时使用
    string purchaseInfo=getItemFromJson(receipt, "purchase-info");
    
    //OGLog(LogTypeNetwork,"purchaseInfo %s\n", purchaseInfo.c_str());
    unsigned char * buf;        //注意释放
    int decLen;
    decLen = base64Decode((unsigned char *)(purchaseInfo.c_str()),(unsigned int)purchaseInfo.size(),&buf);
    string decstr((const char *)buf, decLen);
    
    OGLog(LogTypeNetwork,"dec result %s\n, %d", decstr.c_str(), decLen);
    string pid=getItemFromJson(decstr, "product-id");
    
    CC_SAFE_DELETE_ARRAY(buf);

    req.set_producttype(SaveDataManger::sharedSaveDataManger()->getBuyedProductType(pid));
    OGLog(LogTypeNetwork, "[req] Receipt for apple. Role name %s,channelid %s", role.c_str(), SaveDataManger::sharedSaveDataManger()->getBuyedProductType(pid).c_str());
//    connect(THRAN_SERVER_IP, THRAN_SERVER_PORT);
    connect(AppGlobal::sharedInstance()->ThranHost().c_str(), AppGlobal::sharedInstance()->ThranPort());
    send(&req, THRANID_SVR_VERIFYRECEIPT);
}
*/
string OGRecharge::getItemFromJson(string jsonStr,const string& key){
    Json::Reader reader;
    Json::Value root;
    //调整为正常的json格式
    reader.Item_separator=';';
    reader.KeyValue_separator='=';
    jsonStr.erase(jsonStr.rfind(";"),1);        //去掉多余的
    if(!reader.parse((const char*)jsonStr.c_str(),
                     (const char*)(jsonStr.c_str() + jsonStr.size()),
                     root,
                     false)){
        OGLog(LogTypeNetwork,"Json::Reader Parse error!\n");
    }
    OGLog(LogTypeNetwork,"%s= %s", key.c_str(), root[key].asString().c_str());
    return root[key].asString();
}

// 向索蓝服务器发送苹果的收据
void OGRecharge::sendAppleReceipt(const std::string& receipt, const std::string& uid)
{
    std::string account = readRechargeAccount();
    std::string role = readRechargeRole();
    if (account.size() == 0||role.size() == 0)
    {
        [[OGRechargeObserver sharedRechargeObserver] finishCurrentTransaction];
        return;
    }
    OGLog(LogTypeNetwork, "sendAppleReceipt receipt%s,uid%s,account%s, role%s",receipt.c_str(),uid.c_str(),account.c_str(),role.c_str());

//    sendAppleReceipt(receipt, uid, account, role);
	
	//去WEB服务器请求验证
	std::string strUrl = AppGlobal::sharedInstance()->WebRootUrl() + "/iospay/purchase";
	WebDelegate::sharedInstance()->IOSPay_Purchase(strUrl, account, role, receipt.c_str(), this, (WEB_OnIospayPurchase)&OGRecharge::OnPay);
}

//web付费结果
void OGRecharge::OnPay(int nResult, const std::string& strUserId, const std::string& strOutTradeNo, int nPurchased)
{
    stopTimer();
    m_isRecharging = false;
    
    std::string roleName = readRechargeRole();
	
	int nAckStatus = (0 == nResult) ? RECHARGE_SUCCEEDED : RECHARGE_FAILED;
    if (m_delegate)
    {
        m_delegate->rechargeResult(nAckStatus, roleName, nPurchased);
    }

    if (nAckStatus == RECHARGE_SUCCEEDED)
    {
        char message[256] = {0};
        sprintf(message, "充值成功，账号%s充值万能豆%lld。", roleName.c_str(), nPurchased);
        NSString* messageStr = [NSString stringWithCString:message encoding:NSUTF8StringEncoding];
//        [SVProgressHUD dismissWithSuccess:messageStr afterDelay:3];
    }
//    else if (appleReceipt.ackresult() == RECHAEGE_BUSY)
//    {
//        [SVProgressHUD dismissWithError:@"由于网络问题，您充值的金币将在几分钟内到账，请稍侯。" afterDelay:3];
//    }
    else
    {
//        [SVProgressHUD dismissWithError:@"对不起，您的充值操作失败，请稍后再试！" afterDelay:3];
    }
	
    if(nAckStatus == RECHARGE_SUCCEEDED){
        /*
         Your application should call finishTransaction: only after it has successfully processed the transaction and unlocked the functionality purchased by the user.
         https://developer.apple.com/library/mac/documentation/StoreKit/Reference/SKPaymentQueue_Class/Reference/Reference.html#//apple_ref/occ/instm/SKPaymentQueue/finishTransaction:
         */
        [[OGRechargeObserver sharedRechargeObserver] finishCurrentTransaction];
    }
    
    OGLog(LogTypeNetwork, "[ack] Recharge result: %d, Account: %s, Coins: %lld.\n",
		  nAckStatus,
		  roleName.c_str(),
		  nPurchased);
}
/*
// 服务器收到苹果收据后返回充值结果
void OGRecharge::onAppleReceipt(const char* message, int size)
{
    stopTimer();
    ACKAppleReceipt appleReceipt;
    appleReceipt.ParseFromArray(message, size);
    
    m_isRecharging = false;
    
    std::string roleName = readRechargeRole();

    if (m_delegate)
    {
        m_delegate->rechargeResult(appleReceipt.ackresult(), roleName, appleReceipt.coins());
    }
    
//    if (appleReceipt.ackresult() == RECHARGE_SUCCEEDED)
//    {
//        char message[256] = {0};
//        sprintf(message, "充值成功，账号%s充值金币%lld。", account.c_str(), appleReceipt.coins());
//        NSString* messageStr = [NSString stringWithCString:message encoding:NSUTF8StringEncoding];
//        [SVProgressHUD dismissWithSuccess:messageStr afterDelay:3];
//    }
//    else if (appleReceipt.ackresult() == RECHAEGE_BUSY)
//    {
//        [SVProgressHUD dismissWithError:@"由于网络问题，您充值的金币将在几分钟内到账，请稍侯。" afterDelay:3];
//    }
//    else
//    {
//        [SVProgressHUD dismissWithError:@"对不起，您的充值操作失败，请稍后再试！" afterDelay:3];
//    }    
    
    if(appleReceipt.ackresult()==RECHARGE_SUCCEEDED){
        [[OGRechargeObserver sharedRechargeObserver] finishCurrentTransaction];
    }
    
    OGLog(LogTypeNetwork, "[ack] Recharge result: %d, Account: %s, Coins: %lld.\n", 
         appleReceipt.ackresult(),
         roleName.c_str(),
         appleReceipt.coins());
}
*/

// 苹果支付失败
void OGRecharge::transactionFailed(int code, const std::string& error)
{
    stopTimer();
    m_isRecharging = false;
    EventManager::sharedEventManager()->notifyEventFinished(EventTypeStopIndicator);
    OGLog(LogTypeNetwork, "recharge failed %d", code);
    if (code != SKErrorPaymentCancelled && m_delegate != NULL && !error.empty())
    {
        m_delegate->rechargeFailed(code, error);
    }
}

// 充值时需将用户名和角色名写入文件。//zzd：因为可能是appstore服务器发起的未完成交易，要用到上次的信息
void OGRecharge::writeRechargeUser(const std::string& account, const std::string& role)
{
    std::string roleName = Utilities::GBKToUTF8(role);
    NSString* accountStr = [NSString stringWithCString:account.c_str() encoding:NSUTF8StringEncoding];
    NSString* roleStr = [NSString stringWithCString:roleName.c_str() encoding:NSUTF8StringEncoding];
    [[NSUserDefaults standardUserDefaults] setObject:accountStr forKey:@"recharge_account"];
    [[NSUserDefaults standardUserDefaults] setObject:roleStr forKey:@"recharge_rolename"];
}

std::string OGRecharge::readRechargeAccount()
{
    NSString* accountStr = [[NSUserDefaults standardUserDefaults] stringForKey:@"recharge_account"];
    if (!accountStr)
    {
        return "";
    }
    return std::string([accountStr UTF8String]);
}

std::string OGRecharge::readRechargeRole()
{
    NSString* roleStr = [[NSUserDefaults standardUserDefaults] stringForKey:@"recharge_rolename"];
    if (!roleStr)
    {
        return "";
    }
    return std::string([roleStr UTF8String]);
}

void OGRecharge::onReceiveData(int messageID, const char* message, int size)
{
	disconnect();
    OGLog(LogTypeNetwork,"[thran charge.onReceiveData]. 0x%x, size=%d", messageID, size);
	switch (messageID)
    {
		case (OGID_ACK | THRANID_SVR_PAYCONN):
			break;
        case (OGID_ACK | THRANID_SVR_PAYTRACK):
			break;
//		case (OGID_ACK | THRANID_SVR_GETAPPLEPRO):
//			onProductsList(message, size);
//			break;
//        case (OGID_ACK | THRANID_SVR_VERIFYRECEIPT):
//			onAppleReceipt(message, size);
//			break;
		default:
			OGLog(LogTypeNetwork, "[thran] Unknown message id: 0X%08X", messageID);
			break;
	}
}

void OGRecharge::startTimer()
{
    stopTimer();
    ProgressHUD::show(LocalizationString("rechargeing").c_str());
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void OGRecharge::stopTimer()
{
    ProgressHUD::hide();
    m_reqTime = 0;
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
}

void OGRecharge::update(float t)
{
    m_reqTime+=t;
    if (m_reqTime >= Time_Out)
    {
        m_delegate->requestTimeout();
    }
}


void OGRecharge::sendYDSmsRecharge(const std::string& product, const std::string& payType,const std::string& goodsbepid)
{
}

void OGRecharge::ackYDSmsRecharge(const char* message, int size)
{

}

void OGRecharge::pay(const string& rolename,const string& orderId,const string& productId,const std::string& payType)
{
    
}

void OGRecharge::OnPayCallback(const string& orderId)
{
    
}

void OGRecharge::getShopList(const std::string &username)
{
    
}

void OGRecharge::getPayTypesByProduct(const string& orderId,const std::string &productId)
{
    
}

void OGRecharge::OnPayTypesCallback(int array[],const std::string& orderId,const std::string& productId)
{
    
}



