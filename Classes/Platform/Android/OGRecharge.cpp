#include "Network/OGRecharge.h"
#include "Network/NetworkConstants.h"
#include "UI/ProgressHUD.h"
#include "Common/Localization.h"
#include "cocos2d.h"
#include "Events/EventManager.h"
#include "Data/DataManager.h"
#include "Common/Utilities.h"
#include "Common/Logger.h"
//#include "UmengSDK/OGUmeng.h"
#include "ui/OGDialog.h"
#include "jni/JniHelper.h"
#include "../AppGlobal.h"
#include "../Scenes/ShopPayLayer.h"
#include "Common/globaldefine.h"
#include "MainMenuXLayer.h"
#include "GamePlayLayer.h"
#include "TableListLayer.h"

USING_NS_CC;
//超时时间
#define Time_Out  20
#define CLASS_NAME  "com/ourgame/common/SysUtil"

OGRecharge& OGRecharge::getInstance()
{
    static OGRecharge trnRecharge;
    return trnRecharge;
}

OGRecharge::OGRecharge()
{
}

bool OGRecharge::isRecharging() const
{
    return m_isRecharging;
}

void OGRecharge::setRechargeUI(OGRechargeDelegate* ui)
{
    m_delegate = ui;
}
/*
// 向服务器请求支付产品列表
void OGRecharge::sendProductsListRequest()
{
}

// 接收到产品列表
void OGRecharge::onProductsList(const char* message, int size)
{
}
*/
// 向苹果验证产品列表
void OGRecharge::sendAppleVerifyProducts()
{
}

// 向服务器发送充值请求
void OGRecharge::sendRecharge(const std::string& product,      // 产品ID
                              const std::string& account,      // 帐号名
                              const std::string& role,		   // 角色名
							  const std::string& payType,
							  const std::string& goodsbepid)      // payType  
{
	std::string udid = System::getUniqueID();
	std::string channelID = System::getChannelID();
	std::ostringstream ostring;
	ostring<<role<<GAME_ID<<channelID;
	std::string token = System::getMD5String(ostring.str());

	REQThranPayConn payConn;
	payConn.set_username(account);
	payConn.set_rolename(role);
	payConn.set_gameid(GAME_ID);
	payConn.set_channelid(channelID);
	payConn.set_token(token);
	payConn.set_paytype(payType);
	payConn.set_payinfo1("");
	payConn.set_payinfo2("");
	payConn.set_payinfo3(product);
	payConn.set_goodsbepid(goodsbepid.c_str());
	writeRechargeUser(account, role);

//	connect(THRAN_SERVER_IP, THRAN_SERVER_PORT);
//	send(&payConn, THRANID_SVR_PAYCONN);

	//CCLog("[send] Thran pay request:product = %s, accoun = %s, server ip = %s, payType = %s", product.c_str(), account.c_str(),THRAN_SERVER_IP, payType.c_str());
}

// 服务器收到充值请求后生成订单
void OGRecharge::onPayRequest(const char* message, int size)
{
	ProgressHUD::hide();
    
	ACKThranPayConn ackPayConn;
	ackPayConn.ParseFromArray(message, size);
	const std::string& statement = Utilities::GBKToUTF8(ackPayConn.statement());
    const std::string& paytypestr = Utilities::GBKToUTF8(ackPayConn.paytype());
    const std::string& paycode = Utilities::GBKToUTF8(ackPayConn.paycode());
    const std::string& productdetails = Utilities::GBKToUTF8(ackPayConn.productdetails());
    const std::string& paydate = Utilities::GBKToUTF8(ackPayConn.paydate());
    
    long long coins = ackPayConn.coins();
    float cost = ackPayConn.cost();
    std::string thirdAccessToken =  DataManager::sharedDataManager()->getUser()->thirdKey;
    std::string thirdDigitalName =  DataManager::sharedDataManager()->getUser()->thirdDigitalName;
	//CCLog("[ack] onPayRequest: statement = %s, paytypestr = %s,paycode = %s，productdetails = %s", statement.c_str(),paytypestr.c_str(),paycode.c_str(),productdetails.c_str());
	
	
	JniMethodInfo method;
	if (paytypestr.compare("UMPAY") == 0 )//话付宝
	{
		if (JniHelper::getStaticMethodInfo(method, "com/og/common/OGMainActivity", "showPayViewMore", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;JFLjava/lang/String;Ljava/lang/String;)V"))
		{
			JNIEnv* env = method.env;
			jint  type = 2;
			jstring state = env->NewStringUTF(statement.c_str());
			jstring paytstr = env->NewStringUTF(paytypestr.c_str());
			jstring paycodestr =  env->NewStringUTF(paycode.c_str());
			jstring productdetailsstr =  env->NewStringUTF(productdetails.c_str());
			jstring paydatestr =  env->NewStringUTF(paydate.c_str());

			jlong coinsL = coins;
			jfloat costF = cost;
			jstring thirdAccessTokenStr =  env->NewStringUTF(thirdAccessToken.c_str());
			jstring thirdDigitalNameStr =  env->NewStringUTF(thirdDigitalName.c_str());
			env->CallStaticObjectMethod(method.classID, method.methodID, type, state,paytstr,paycodestr,coinsL,costF,productdetailsstr,paydatestr);
			env->DeleteLocalRef(state);
			env->DeleteLocalRef(paytstr);
			env->DeleteLocalRef(paycodestr);
			env->DeleteLocalRef(productdetailsstr);
			env->DeleteLocalRef(paydatestr);
			env->DeleteLocalRef(thirdAccessTokenStr);
			env->DeleteLocalRef(thirdDigitalNameStr);
			env->DeleteLocalRef(method.classID);
		}
	}
	else// 支付宝
	{
		if (JniHelper::getStaticMethodInfo(method, "com/og/common/OGMainActivity", "showPayView", "(ILjava/lang/String;)V"))
		{
			jint  type = 0;
			JNIEnv* env = method.env;
			jstring state = env->NewStringUTF(statement.c_str());
			env->CallStaticObjectMethod(method.classID, method.methodID, type, state);
			env->DeleteLocalRef(state);
			env->DeleteLocalRef(method.classID);
		}
	}
}

// 付费跟踪
void OGRecharge::sendPaymentTrack(const std::string& payment)
{
	/*REQThranPayTrack payTrack;
	payTrack.set_statement(payment);

	connect(THRAN_SERVER_IP, THRAN_SERVER_PORT);
	send(&payTrack, THRANID_SVR_PAYTRACK);

	OGUmeng::onEvent("pay_alipay");*/
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OGRecharge::trackCurrentStatement), this);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OGRecharge::trackCurrentStatement), this, 60.0, false, 3, 5);
}

//付费跟踪状态 "0"成功 "1"还未到账 "-1"卡密错误 或面额不符 "2"账单不存在 "3"元宝不足 "4"未知错误
void OGRecharge::onPaymentTrack(const char* message, int size)
{
	ACKThranPayTrack ackPayTrack;
	ackPayTrack.ParseFromArray(message, size);

	const std::string statement = ackPayTrack.statement();
	long long coins = ackPayTrack.coins();
	const std::string role = ackPayTrack.rolename();

	if (m_delegate != NULL)
	{
		if (ackPayTrack.ackresult() == 0)
		{
			// 充值成功，删除订单
			m_delegate->rechargeResult(RECHARGE_SUCCEEDED, role, coins);

			if (statement.compare(readRechargeStatement()) == 0)
			{
				writeRechargeStatement("");
			}
		}
		else if (ackPayTrack.ackresult() == 1)
		{
			// 还未到帐
			CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OGRecharge::trackCurrentStatement), this, 0, false, 0, 10);
		}
		else
		{
			// 充值失败，删除订单
			m_delegate->rechargeResult(RECHARGE_FAILED, role, coins);
			if (statement.compare(readRechargeStatement()) == 0)
			{
				writeRechargeStatement("");
			}
		}
	}
}

void OGRecharge::trackCurrentStatement(float t)
{
	DataManager::sharedDataManager()->getUser()->updateCoin = true;
	EventManager::sharedEventManager()->startEvent(EventTypeUpdateUserCoin);
	//std::string statement = readRechargeStatement();
	//if (statement.length() > 0)
	//{
	//	sendPaymentTrack(statement);
	//}
}
/*
// 向索蓝服务器发送苹果的收据
void OGRecharge::sendAppleReceipt(const std::string& receipt, const std::string& uid, const std::string& account, const std::string& role)
{               
}
*/
// 向索蓝服务器发送苹果的收据
void OGRecharge::sendAppleReceipt(const std::string& receipt, const std::string& uid)
{
}

// 服务器收到收据后返回充值结果
void OGRecharge::onAppleReceipt(const char* message, int size)
{ 
}

// 支付失败
void OGRecharge::transactionFailed(int code, const std::string& error)
{
}

// 充值时需将用户名和角色名写入文件。
void OGRecharge::writeRechargeUser(const std::string& account, const std::string& role)
{
	std::string roleName = Utilities::GBKToUTF8(role);
	CCUserDefault::sharedUserDefault()->setStringForKey("recharge_account", account);
	CCUserDefault::sharedUserDefault()->setStringForKey("recharge_rolename", roleName);
}

void OGRecharge::writeRechargeStatement(const std::string& statement)
{
	CCUserDefault::sharedUserDefault()->setStringForKey("recharge_statement", statement);
}

std::string OGRecharge::readRechargeAccount()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("recharge_account");
}

std::string OGRecharge::readRechargeRole()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("recharge_rolename");
}

std::string OGRecharge::readRechargeStatement()
{
	return CCUserDefault::sharedUserDefault()->getStringForKey("recharge_statement");
}

void OGRecharge::onReceiveData(int messageID, const char* message, int size)
{
	disconnect();

	switch (messageID)
	{
	case (OGID_ACK | THRANID_SVR_PAYCONN):
		onPayRequest(message, size);
		break;
	case (OGID_ACK | THRANID_SVR_PAYTRACK):
		onPaymentTrack(message, size);
		break;
/*	case (OGID_ACK | THRANID_SVR_GETAPPLEPRO):
		onProductsList(message, size);
		break;
	case (OGID_ACK | THRANID_SVR_VERIFYRECEIPT):
		onAppleReceipt(message, size);
		break;
	case (OGID_ACK | THRANID_SVR_PRODUCTCONN):
		ackYDSmsRecharge(message, size);
		break;
*/	default:
		break;
	}
}

void OGRecharge::startTimer()
{
	stopTimer();
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
	std::string channelID = System::getChannelID();
	UserData* user = DataManager::sharedDataManager()->getUser();
	 if (user == NULL)
	 {
		 return;
	 }
	std::string tmp_role = user->roleName;
	REQProductConn payConn;
	payConn.set_rolename(tmp_role);
	payConn.set_gameid(GAME_ID);
	payConn.set_channelid(channelID);
	payConn.set_version(CURRENT_VERSION);
	payConn.set_paytype(payType.c_str());
	payConn.set_productid(product);
	payConn.set_clientversion(System::getAppVersion());

	payConn.set_coins(user->coin);
	payConn.set_imsi(System::getIMSI());
	payConn.set_mobile(System::getMobileNum());
	payConn.set_goodsbepid(goodsbepid.c_str());

//	if (connect(THRAN_SERVER_IP, THRAN_SERVER_PORT))
//	{
//		if (send(&payConn, THRANID_SVR_PRODUCTCONN))
//		{
//			CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(OGRecharge::sendSmsRechargeTimeout), this, 0, false, 0, 10);
//		}
//	}
}

void OGRecharge::ackYDSmsRecharge(const char* message, int size)
{
	//required string productId = 1; 		//产品ID
	//optional string payCode = 2; 			//计费点代码
	//optional int32  count =3 [default=1]; //计费数量
	//required string token = 4;			//MD5(productId+payCode+count+md5code),其中md5code为服务器定义的字符串
	//optional int32 isRight = 5; 			//0有权限，1无权限。
	//optional string rightDesc = 6; 		//无权限时的描述。
	//optional int64 coins = 7;				// 购买的金币数量
	//optional float cost = 8;				//购买的花费，货币类型可为人民币或第三方货币。
	//optional string sendCode = 9;			//移动发送的指令码
	//optional string payType = 10;			

	//OGLog(LogTypeDiamondPay ,"[OGRecharge::ackSmsRecharge]");

	ProgressHUD::hide();
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OGRecharge::sendSmsRechargeTimeout), this);

	// 消息
	ACKProductConn ackPayConn;
	ackPayConn.ParseFromArray(message, size);

	const std::string& paycode = Utilities::GBKToUTF8(ackPayConn.paycode());
	const std::string& token = ackPayConn.token();
	int isRight = ackPayConn.isright();
	const std::string& rightDesc = ackPayConn.rightdesc();
	long long coins = ackPayConn.coins();
	float cost = ackPayConn.cost();
	const std::string& sendCode = ackPayConn.sendcode();
	const std::string& payType = Utilities::GBKToUTF8(ackPayConn.paytype());
	const std::string& serverStatement  =  Utilities::GBKToUTF8(ackPayConn.statement());

	//CCLog("--------------[receive] TackSmsRecharge:paycode = %s, payType = %s  , coins= %lld, cost=%f  sendCode=%s serverStatement=%s", paycode.c_str(), payType.c_str(),coins,cost,sendCode.c_str(),serverStatement.c_str());

	if (isRight != 0)
	{
		//OGLog(LogTypeDiamondPay ,"no root================================");
		// 无权限
		OGDialog::show(LocalizationString("but_by_sms_no_right").c_str());
	}
	else
	{
		int iLen = paycode.size();


		if ((!serverStatement.empty()) && (serverStatement.length() > 0))
		{
			m_statement = serverStatement;
		}
		else
		{
			m_statement = payType + paycode.substr(iLen - 16);
		}


		//OGLog(LogTypeDiamondPay ,"[ackSmsRecharge].paycode=%s, st = %s", paycode.c_str(), m_statement.c_str());

		//OGLog(LogTypeDiamondPay ,"[receive] TackSmsRecharge:paycode = %s, payType = %s  count = %d, coins= %lld, cost=%f  sendCode=%s", paycode.c_str(), payType.c_str(),count,coins,cost,sendCode.c_str());

		JniMethodInfo method;

		if (JniHelper::getStaticMethodInfo(method, "com/og/common/OGMainActivity", "buyWithYDSms", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JF)V"))
		{
			JNIEnv* env = method.env;
			jstring sendCodeStr = env->NewStringUTF(sendCode.c_str());
			jstring paycodeStr =  env->NewStringUTF(paycode.c_str());
			jstring sName = env->NewStringUTF(Localization::localizationString("produce_name_diamond").c_str());
			jlong coinsL = coins;
			jfloat costF = cost;

			env->CallStaticObjectMethod(method.classID, method.methodID, paycodeStr, sendCodeStr, sName, coinsL, costF);

			env->DeleteLocalRef(paycodeStr);
			env->DeleteLocalRef(sendCodeStr);
			env->DeleteLocalRef(method.classID);
		}
	}
}

// 向服务器发送充值请求超时
void OGRecharge::sendSmsRechargeTimeout(float t)
{
	CCLog("[OGRecharge::sendSmsRechargeTimeout]");

	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(OGRecharge::sendSmsRechargeTimeout), this);
	ProgressHUD::hide();

	OGDialog::show(Localization::localizationString("pay_Timeout").c_str());
}

void OGRecharge::processPendingTransactions()
{
}

//移动支付
void OGRecharge::pay(const std::string& rolename,const std::string& orderId,const std::string& productId,const std::string& strPayType)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t,CLASS_NAME,"rtnObject","()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID,t.methodID);
		if(JniHelper::getMethodInfo(t,CLASS_NAME,"pay","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
		{
			jstring jstrRoleName = t.env->NewStringUTF(rolename.c_str());
			jstring jstrOrderId = t.env->NewStringUTF(orderId.c_str());
			jstring jstrProductId = t.env->NewStringUTF(productId.c_str());
			jstring jstrPayType = t.env->NewStringUTF(strPayType.c_str());
			t.env->CallVoidMethod(jobj,t.methodID,jstrRoleName,jstrOrderId,jstrProductId,jstrPayType);
			t.env->DeleteLocalRef(jstrRoleName);
			t.env->DeleteLocalRef(jstrOrderId);
			t.env->DeleteLocalRef(jstrProductId);
			t.env->DeleteLocalRef(jstrPayType);
			
		}
	}
}

//支付回调
void OGRecharge::OnPayCallback(const std::string& orderId)
{
	CCLog("OnPayCallback  %s",orderId.c_str());
	AppGlobal::sharedInstance()->DelayOrderConfirm(orderId,1);
}

//通过sdk获取商品列表
void OGRecharge::getShopList(const std::string& username)
{

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t,CLASS_NAME,"rtnObject","()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID,t.methodID);
		if(JniHelper::getMethodInfo(t,CLASS_NAME,"getShopList","(Ljava/lang/String;)V"))
		{
			JNIEnv *env = t.env;
			jstring jstrUsername = t.env->NewStringUTF(username.c_str());
			env->CallVoidMethod(jobj,t.methodID,jstrUsername);
			env->DeleteLocalRef(jstrUsername);
			
		}
	}

}

void OGRecharge::getPayTypesByProduct(const std::string& orderId,const std::string& productId)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t,CLASS_NAME,"rtnObject","()Ljava/lang/Object;"))
	{
		jobject jobj;
		jobj = t.env->CallStaticObjectMethod(t.classID,t.methodID);
		if (JniHelper::getMethodInfo(t,CLASS_NAME,"getPayTypesByProduct","(Ljava/lang/String;Ljava/lang/String;)V"))
		{
			JNIEnv *env = t.env;
			jstring jstrOrderId = t.env->NewStringUTF(orderId.c_str());
			jstring jstrProductId = t.env->NewStringUTF(productId.c_str());
			env->CallVoidMethod(jobj,t.methodID,jstrOrderId,jstrProductId);
			env->DeleteLocalRef(jstrProductId);
		}
	}
}

void OGRecharge::OnPayTypesCallback(int array[],const std::string& orderId,const std::string& productId)
{
	
	std::vector<int>& v = AppGlobal::sharedInstance()->getPayTypes();
	v.clear();
	for(int i = 0;i < 10;i++)
	{
		if (array[i] != 0)
		{
			v.push_back(array[i]);
			CCLog("GoodsPayType:%d",array[i]);
		}
		
	}
	//支付方式排序
	sort(v.begin(),v.end());

	for(int i=0;i<v.size();i++)
	{
		CCLog("PayType:%d",v[i]);
	}

	if (AppGlobal::sharedInstance()->bIsQuickPay)
	{
		//直接支付，不显示支付方式
		CCLog("直接支付 %s  %s",orderId.c_str(),productId.c_str());
		for(int i = 0 ; i < v.size();i++)
		{
			if (v[i] == 128)//短信
			{
				stringstream payType;
    			payType<<v[i];
				pay(AppGlobal::sharedInstance()->LoginRole(), orderId,productId,payType.str());
				return;
			}
		}
		for(int i = 0; i < v.size();i++)
		{
			if (v[i] == 1)//支付宝
			{
				stringstream payType;
    			payType<<v[i];
				pay(AppGlobal::sharedInstance()->LoginRole(), orderId,productId,payType.str());
				return;
			}
		}
		for(int i = 0; i < v.size();i++)
		{
			if (v[i] == 8)//掌上汇通的银行卡支付
			{
				stringstream payType;
    			payType<<v[i];
				pay(AppGlobal::sharedInstance()->LoginRole(), orderId,productId,payType.str());
				return;
			}
		}
		
		for(int i = 0; i < v.size();i++)
		{
			if (v[i] == 16)//掌上汇通的信用卡支付
			{
				stringstream payType;
    			payType<<v[i];
				pay(AppGlobal::sharedInstance()->LoginRole(), orderId,productId,payType.str());
				return;
			}
		}
		for(int i = 0; i < v.size();i++)
		{
			if (v[i] == 4)//工行e支付
			{
				stringstream payType;
    			payType<<v[i];
				pay(AppGlobal::sharedInstance()->LoginRole(), orderId,productId,payType.str());
				return;
			}
		}
		for(int i = 0; i < v.size();i++)
		{
			if (v[i] == 64)//话付宝
			{
				stringstream payType;
    			payType<<v[i];
				pay(AppGlobal::sharedInstance()->LoginRole(), orderId,productId,payType.str());
				return;
			}
		}
	}
	else
	{
		//显示支付方式列表
		ShopPayLayer* paylist = ShopPayLayer::create(orderId,productId);
		paylist->setTag(TAG_GLOBAL_SHOPPAYLAYER);
    	CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
    	if (pScene != NULL)
    	{
    		if (AppGlobal::sharedInstance()->bIsInGame)
    		{
    			CCNode* pMainLyr = pScene->getChildByTag(TAG_GLOBAL_GAMELAYER);
    			if(pMainLyr )
    			{
     				GamePlayLayer * pMainLyr1 = (GamePlayLayer*)pMainLyr;
     				pMainLyr1->addChild(paylist);
     				
    			}
    			
    		}
    		else
    		{
    			CCNode* pMainLyr = pScene->getChildByTag(TAG_GLOBAL_MAINMENULAYER);
            	if(pMainLyr )
        		{
        	  	 	MainMenuXLayer* pMainLyr1 = (MainMenuXLayer*)pMainLyr;
        	   		pMainLyr1->addChild(paylist);
            
            		return;
        		}
        		CCNode* node = pScene->getChildByTag(TAG_GLOBAL_TABLELISTLAYER);
            	if (node != NULL) {
                	TableListLayer* pLayer = (TableListLayer*)node;
                	pLayer->addChild(paylist);
            	}
    		}
    		
    	}
	}
	
    // if(pScene)
    // {
    // 	if (AppGlobal::sharedInstance()->bPayIsInGame)
    // 	{
    // 		CCLog("1");
    // 		CCNode* pMainLyr = pScene->getChildByTag(TAG_GLOBAL_GAMELAYER);
    //         if(pMainLyr )
    //         {
    //             GamePlayLayer * pMainLyr1 = (GamePlayLayer*)pMainLyr;
    //             pMainLyr1->addChild(paylist);
                
                
    //         }
    // 	}
    // 	else
    // 	{
    // 		CCLog("2");
    		
    // 	}
        

        
    // }
}

extern "C"
{
	JNIEXPORT void JNICALL Java_com_og_third_AliPayThird_onAliPaySucceeded(JNIEnv* env, jclass obj, jstring statement)
	{
		OGRecharge::getInstance().sendPaymentTrack("");
	}

	JNIEXPORT void JNICALL Java_com_og_common_OGMainActivity_nativeSmsRechargeResult(JNIEnv* env, jclass obj, jstring sRet, jint iRet)
	{
		OGRecharge::getInstance().sendPaymentTrack("");
	}

	JNIEXPORT void JNICALL Java_com_ourgame_common_SysUtil_OnPayCallback(JNIEnv *env,jobject thiz,jstring orderId)
	{
		const char* pText = env->GetStringUTFChars(orderId,NULL);
		std::string order_Id(pText);
		OGRecharge::getInstance().OnPayCallback(order_Id);
	}

	JNIEXPORT void JNICALL Java_com_ourgame_common_SysUtil_OnPayTypesCallback(JNIEnv *env,jobject thiz,jintArray arr,jstring orderId,jstring productId)
	{
		jint *carr;
		carr = env->GetIntArrayElements(arr,false);
		const char* orderId_Str = env->GetStringUTFChars(orderId,NULL);
		const char* productId_Str = env->GetStringUTFChars(productId,NULL);
		std::string order_Id(orderId_Str);
		std::string product_Id(productId_Str);
		if (carr == NULL)
		{
			return;
		}
		// int array[10];
		// for(int i = 0;i<10;i++)
		// {
		// 	// array[i] = carr[i];
		// 	CCLog("Int:%d",carr[i]);
		// }
		OGRecharge::getInstance().OnPayTypesCallback(carr,order_Id,product_Id); 
		env->ReleaseIntArrayElements(arr,carr,0);
		env->ReleaseStringUTFChars(orderId, orderId_Str);
		env->ReleaseStringUTFChars(productId,productId_Str);
	}


};
