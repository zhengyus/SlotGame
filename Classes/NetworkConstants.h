/*!
 * @brief	网络模块用到的常量
 * @author	Zhou Lingfei
 * @date	2012-9-12
 * @ingroup	Network
 * @note    包括网络消息号，服务器IP地址等。
 */

#ifndef __NETWORK_CONSTANTS_H__
#define __NETWORK_CONSTANTS_H__


//---------------------------------------------------------------------------
//	游戏和平台相关的配置
//---------------------------------------------------------------------------

#define GAME_ID                 25022
#define LANGUAGE_ID             "cn"
#define CURRENT_VERSION         100201
#define GAME_NAME				"texas-poker"

enum ErrorCode
{
	Right,
    PasswordError,
    CertError,
    TokenError,
    CreteRolenameError,
    CoinChangeError,
    NullError,
    OpenIdError,
    SystemError,
    NetError
};

//typedef enum
//{
//	PLATFORM_ANDROID_PHONE	= 1,
//	PLATFORM_IPHONE			= 2,
//	PLATFORM_IPAD			= 3,
//	PLATFORM_ANDROID_PAD	= 4,
//	PLATFORM_TV				= 5
//}PlatformType;

//--------------------------------------------------------
// 临时
//--------------------------------------------------------
//有登录界面之后再去掉
//#define tmpUID "thrdz"


//---------------------------------------------------------------------------
//	索蓝服务器IP和端口
//---------------------------------------------------------------------------

//#ifdef DEBUG
//#define THRAN_SERVER_IP			"172.28.14.56"//211.154.165.84(外网正式环境）//"172.28.14.56"(内网测试环境)//"211.154.165.218"(仿真服务器)//"202.108.36.94"//索蓝服务器(内网)
//#define MATCHLIST_SERVER_IP		"172.28.14.195"//"172.28.14.195"//"172.28.26.164"//"172.28.14.196"     //matchlist服务器(内网)
//#define MATCHLIST_SERVER_PORT   22104
//#else
//#define THRAN_SERVER_IP			"172.28.14.56"//"172.28.14.61"//"172.28.19.30"//"172.28.19.76"//"172.28.14.56"//"211.154.165.218"//"202.108.36.94"	//索蓝服务器(外网)
//#define MATCHLIST_SERVER_IP		"172.28.14.195"//"172.28.14.195"//"172.28.14.196"//"172.28.14.191"		//matchlist服务器(内网)
//#define MATCHLIST_SERVER_PORT	22104
//#endif

// 商城的地址
//#define SHOP_URL		"http://172.28.14.80/ddz/Mobile/static_page/dezhoumall/mall.html"	// QA
//#define SHOP_URL		"http://202.108.32.15/mddz/Mobile/static_page/dezhoumall/mall.html" // 仿真
//#define SHOP_URL		"http://mddz.lianzhong.com/Mobile/static_page/dezhoumall/mall.html" // 生产


//#define THRAN_SERVER_PORT       22101//22599//22399//22599//

//---------------------------------------------------------------------------
//	索蓝服务器消息号
//---------------------------------------------------------------------------

// 成功应答时的返回值
#define THRAN_RESULT_OK 0

// 索蓝游戏版本消息
#define THRANID_SVR_VERSION 0x00003001

// 获取苹果产品列表
#define THRANID_SVR_GETAPPLEPRO 0x00002004

// 苹果收据验证
#define THRANID_SVR_VERIFYRECEIPT 0x00002005

// 获取验证码信息
#define THRANID_SVR_NEWVERIFY 0x00003003

// 注册消息
#define THRANID_SVR_REG 0x00001003

// 快速登录消息
#define THRANID_SVR_QUICKCONN 0x00001006

// 短证书登录消息
#define THRANID_SVR_SHORTCERT 0x00001005

// 发送微博
#define THRANID_SVR_WEIBO 0x00003005

// 新浪微游戏相关
#define THRANID_SVR_SINAGAME 0x00003007

// 付费消息
#define THRANID_SVR_PAYCONN 0x00002001

// 付费跟踪消息
#define THRANID_SVR_PAYTRACK 0x00002002

// 获取苹果产品列表
#define THRANID_SVR_GETAPPLEPRO 0x00002004

// 苹果收据验证
#define THRANID_SVR_VERIFYRECEIPT 0x00002005

#define THRANID_SVR_PRODUCTCONN 0x00002008

#define THRANID_SVR_GAMEREGISTER 0x00000262

#define THRANID_SVR_GAMELOGIN 0x00000263

//---------------------------------------------------------------------------
//	比赛列表
//---------------------------------------------------------------------------
//比赛列表的idle时间
#define MATCHLIST_IDLE       60//000

// 成功应答时的返回值
#define MATCHLIST_RESULT_OK 0

//基础号w
#define OGID_MSGBASE_PK_MATCHLIST                           0x00003F00

// 用户发送确认连接消息
#define OGID_TEXASPOKER_MATCHLIST_CONFIRMCONNECT            OGID_MSGBASE_PK_MATCHLIST + 0x000000001

// 用户发送登录消息
#define OGID_TEXASPOKER_MATCHLIST_LOGIN                     OGID_MSGBASE_PK_MATCHLIST + 0x000000002

//比赛列表
#define OGID_TEXASPOKER_MATCHLIST_SEARCHMATCH               OGID_MSGBASE_PK_MATCHLIST + 0x000000003

// 进入比赛详细信息窗口
#define OGID_TEXASPOKER_MATCHLIST_ENTERMATCH                OGID_MSGBASE_PK_MATCHLIST + 0x000000004

// 离开比赛信息窗口
#define OGID_TEXASPOKER_MATCHLIST_LEAVEMATCH                OGID_MSGBASE_PK_MATCHLIST + 0x000000005

//matchlist.idle
#define OGID_TEXASPOKER_MATCHLIST_IDLE                      OGID_MSGBASE_PK_MATCHLIST + 0x000000006

// 下发奖金信息
#define OGID_TEXASPOKER_MATCHLIST_SENDREWARD                OGID_MSGBASE_PK_MATCHLIST + 0x00000000B

//下发比赛变化信息
#define OGID_TEXASPOKER_MATCHLIST_SENDCHANGEINFO			(OGID_MSGBASE_PK_MATCHLIST + 0x00000000C)
//class MatchList_SendMatchChangeInfo;

//下发筹码变化信息
#define OGID_TEXASPOKER_MATCHLIST_SENDMATCHCOININFO			(OGID_MSGBASE_PK_MATCHLIST + 0x00000000D)
//class MatchList_SendMatchCoinInfo;

//下发比赛选手信息
#define OGID_TEXASPOKER_MATCHLIST_SENDPLAYERINFO			(OGID_MSGBASE_PK_MATCHLIST + 0x00000000E)
//class MatchList_SendPlayerInfo;

//下发桌子信息
#define OGID_TEXASPOKER_MATCHLIST_SENDTABLEINFO				(OGID_MSGBASE_PK_MATCHLIST + 0x00000000F)
//class MatchList_SendTableInfo;

//报名，退赛
#define OGID_TEXASPOKER_MATCHLIST_SIGNMATCH					(OGID_MSGBASE_PK_MATCHLIST + 0x000000010)
//class MatchList_SignMatch;
//class MatchList_AckSignMatch;

//通知DB本服务器人数变化
#define OGID_TEXASPOKER_MATCHLIST_REPORTPLAYER				(OGID_MSGBASE_PK_MATCHLIST + 0x000000011)
//class MatchList_ReportPlayer;

//通知用户开始比赛
#define OGID_TEXASPOKER_MATCHLIST_STARTMATCH				(OGID_MSGBASE_PK_MATCHLIST + 0x000000012)
//class MatchList_NotifyStartMatch;

//通知用户已报名比赛人数的变化
#define OGID_TEXASPOKER_MATCHLIST_NOTIFYMATCHPLAYER			(OGID_MSGBASE_PK_MATCHLIST + 0x000000013)
//class MatchList_NotifyMatchPlayer;

//从DB获取用户信息
#define OGID_TEXASPOKER_MATCHLIST_GETUSERINFO				(OGID_MSGBASE_PK_MATCHLIST + 0x000000014)
//class MatchList_GetUserInfo;
//class MatchList_AckGetUserInfo;

//从DB获取和自己相关的比赛ID
#define OGID_TEXASPOKER_MATCHLIST_GETSELFMATCH				(OGID_MSGBASE_PK_MATCHLIST + 0x000000015)
//class MatchList_GetSelfMatch;
//class MatchList_AckGetSelfMatch;

// 发送和自己相关的报名信息
#define OGID_TEXASPOKER_MATCHLIST_SENDSIGNINFO              OGID_MSGBASE_PK_MATCHLIST + 0x000000016

#define OGID_TEXASPOKER_MATCHLIST_WATCHMATCH				(OGID_MSGBASE_PK_MATCHLIST + 0x000000017)
//class MatchList_WatchMatch;

//通知比赛状态变化消息
#define OGID_TEXASPOKER_MATCHLIST_NOTIFYMATCHSTATUS			(OGID_MSGBASE_PK_MATCHLIST + 0x000000018)
//class MatchList_NotifyMatchStatus;

// 通知新增比赛
#define OGID_TEXASPOKER_MATCHLIST_NOTIFYNEWMATCH            OGID_MSGBASE_PK_MATCHLIST + 0x000000019

//通知服务器的时间
#define OGID_TEXASPOKER_MATCHLIST_SENDSERVERTIME			(OGID_MSGBASE_PK_MATCHLIST + 0x00000001A)
//class MatchList_SendServerTime;

//通知服务器在线人数
#define OGID_TEXASPOKER_MATCHLIST_SENDONLINECOUNT			(OGID_MSGBASE_PK_MATCHLIST + 0x00000001B)
//class MatchList_SendOnLineCount;

// 用户取自己的账户信息
#define OGID_TEXASPOKER_MATCHLIST_GETUSERCOININFO           OGID_MSGBASE_PK_MATCHLIST + 0x00000001C

//用户定时从服务器获取在线人数，服务器时间，比赛列表等相关信息
#define	OGID_TEXASPOKER_MATCHLIST_GETMATCHINFOINTIME		(OGID_MSGBASE_PK_MATCHLIST + 0x00000001D)

// 服务器下发通知消息给客户端（废弃不用）
#define OGID_TEXASPOKER_MATCHLIST_BROADCASTMSG              OGID_MSGBASE_PK_MATCHLIST + 0x00000001F

//通知用户没有角色名
#define	OGID_TEXASPOKER_MATCHLIST_GETROLERESULT				(OGID_MSGBASE_PK_MATCHLIST + 0x000000021)
//class MatchList_GetRoleResult;

//检查角色名
#define OGID_TEXASPOKER_MATCHLIST_CHECKROLENAME				(OGID_MSGBASE_PK_MATCHLIST + 0x000000022)
//class MatchList_CheckRoleName;		//发送检查角色名消息时使用
//class MatchList_CheckRoleNameResult;	//检查角色名的结果

#define OGID_TEXASPOKER_MATCHLIST_CREATEROLENAME			(OGID_MSGBASE_PK_MATCHLIST + 0x000000023)
//class MatchList_CreateRole;
//class MatchList_CreateRoleResult;

// 删除比赛的时候通知客户端
#define OGID_TEXASPOKER_MATCHLIST_DELMATCH                  OGID_MSGBASE_PK_MATCHLIST + 0x000000024

#define OGID_TEXASPOKER_MATCHLIST_GETGAMEUSERINFO			(OGID_MSGBASE_PK_MATCHLIST + 0x000000025)
//class MatchList_GetGameUserInfo;
//class MatchList_AckGetGameUserInfo;

//定时获取比赛状态信息（含人数，状态，进行时间等等)
//发送的时候发空消息，回应的消息结构是MatchList_NotifyMatchPlayer
#define OGID_TEXASPOKER_MATCHLIST_GETMATCHPLAYER			(OGID_MSGBASE_PK_MATCHLIST + 0x000000026)
//class MatchList_NotifyMatchPlayer;

//用户取比赛信息或者新增比赛时，服务器主动下发给用户该信息
#define OGID_TEXASPOKER_MATCHLIST_SENDMATCHTOPINFO			(OGID_MSGBASE_PK_MATCHLIST + 0x000000027)
//class MatchList_MatchTopInfo;

// 修改配置
#define OGID_TEXASPOKER_MATCHLIST_MODIFYCONFIG              OGID_MSGBASE_PK_MATCHLIST + 0x000000028

//通知 常规桌和朋友桌的属性 到 客户端
#define OGID_TEXASPOKER_MATCHLIST_MATCHINFO					(OGID_MSGBASE_PK_MATCHLIST + 0x000000029)
//class OGMatchList_MatchInfos;

//客户端需要知道的部分配置信息
#define OGID_TEXASPOKER_MATCHLIST_CONFIGINFO                OGID_MSGBASE_PK_MATCHLIST + 0x000000030

//赠送礼物
#define OGID_TEXASPOKER_MATCHLIST_GIVEGIFT					(OGID_MSGBASE_PK_MATCHLIST + 0x000000031)
//class OGMatchList_GiveGift;

#define OGID_TEXASPOKER_MATCHLIST_GIVEITEM					(OGID_MSGBASE_PK_MATCHLIST + 0x000000032)
//class OGMatchList_GiveItem;

//请求排行榜信息
#define OGID_TEXASPOKER_MATCHLIST_USER_COIN_RNK_LIST_REQ	(OGID_MSGBASE_PK_MATCHLIST + 0x000000033)
#define OGID_TEXASPOKER_MATCHLIST_USER_COIN_RNK_LIST_ACK	(OGID_MSGBASE_PK_MATCHLIST + 0x000000034)
//class OGMatchList_UserCoinRnkListAck;

//请求新闻信息
#define OGID_TEXASPOKER_MATCHLIST_BROADCAST_NEWS_REQ		(OGID_MSGBASE_PK_MATCHLIST + 0x000000035)
#define OGID_TEXASPOKER_MATCHLIST_BROADCAST_NEWS_ACK		(OGID_MSGBASE_PK_MATCHLIST + 0x000000036)
//class OGMatchList_BroadCast_News;

// 通知客户端 常规桌盲注限制列表(add 2012-5-31 yanhp)
#define OGID_TEXASPOKER_MATCHLIST_CGZENTERLIMITINFOLIST		(OGID_MSGBASE_PK_MATCHLIST + 0x000000037)

//请求、通知 加入常规桌新手区(add 2012-5-31 yanhp)
#define OGID_TEXASPOKER_MATCHLIST_ENTERNOVICE				(OGID_MSGBASE_PK_MATCHLIST + 0x000000038)
//class MatchList_ReqEnterNoviceMatch;
//class MatchList_AckEnterNoviceMatch;

//客户端请求、通知新手区在线人数
#define OGID_TEXASPOKER_MATCHLIST_NOVICEONLINEPLYCNT		(OGID_MSGBASE_PK_MATCHLIST + 0x000000039)
//class Match_ReqNoviceOnLinePlyCnt;
//class Match_AckNoviceOnLinePlyCnt;

//客户端登陆后，给列表服发送渠道昵称
#define OGID_TEXASPOKER_MATCHLIST_CFUSERNAME				(OGID_MSGBASE_PK_MATCHLIST + 0x00000003A)
//class MatchList_CFUserName;

////比赛列表头的图片连接（废弃不用）
//#define OGID_TEXASPOKER_MATCHLIST_PROPERTY  				(OGID_MSGBASE_PK_MATCHLIST + 0x00000003B)
//class OGMatchList_Property;

//客户端登陆后，选择该客户端已经报名并未结束的比赛进入
#define OGID_TEXASPOKER_MATCHLIST_ENTERSIGNEDMATCH			(OGID_MSGBASE_PK_MATCHLIST + 0x00000003C)
//class MatchList_SendEnterSignedMatch;

////客户端请求、通知All In场列表（废弃不用）
//#define OGID_TEXASPOKER_MATCHLIST_ALLINMATCHLIST			(OGID_MSGBASE_PK_MATCHLIST + 0x00000003D)
//class OGMatchList_AllInMatchListReq;
//class OGMatchList_AllInMatchListAck;

//客户端请求兑换短证书
#define OGID_USER_LONGCERT_TO_SHORT							(OGID_MSGBASE_PK_MATCHLIST + 0x00000003E)
//class OGMatchList_UserLongCertToShortReq;
//class OGMatchList_UserLongCertToShortAck;

//认证服务长证书换取短证书
#define OGID_USER_GET_SHORTCERT_REQ							(OGID_MSGBASE_GAMEUSERDBSVR + 20)
#define OGID_USER_GET_SHORTCERT_ACK							(OGID_MSGBASE_GAMEUSERDBSVR + 6)
//class OGReqUserGetShortCert;
//class OGAckUserGetShortCert;

//客户端定时请求match的玩家、牌桌信息
#define OGID_TEXASPOKER_MATCHLIST_MATCHSTATEINFO			(OGID_MSGBASE_PK_MATCHLIST + 0x00000003F)
//class OGReqMatchStateInfo;

//客户端请求自己比赛的TableId
#define OGID_TEXASPOKER_MATCHLIST_SELFTABLEID				(OGID_MSGBASE_PK_MATCHLIST + 0x000000040)
//class OGReqSelfTableId;
//class OGAckSelfTableId;

//网页端请求当前的玩家排行榜
#define OGID_TEXASPOKER_MATCHLIST_WEB_USER_COIN_RANK_LIST	(OGID_MSGBASE_PK_MATCHLIST + 0x000000041)
//class OGMatchList_UserCoinRnkListAck;

//移动类型客户端请求盲注列表
#define OGID_TEXASPOKER_MATCHLIST_MOBILEBLINDLIST           OGID_MSGBASE_PK_MATCHLIST + 0x000000042

//移动类型客户端请求比赛列表
#define OGID_PK_MATCHLIST_MOBILEMATCHLIST                   OGID_MSGBASE_PK_MATCHLIST + 0x000000043

//移动guest
#define OGID_PK_MATCHLIST_LOGINSPE                          OGID_MSGBASE_PK_MATCHLIST + 0x000000044

//客户端请求快速开始
#define OGID_TEXASPOKER_MATCHLIST_FASTJOIN					(OGID_MSGBASE_PK_MATCHLIST + 0x000000045)
//class MatchList_FastJoinReq;
//class MatchList_FastJoinAck;

//移动端请求朋友桌列表
#define OGID_PK_MATCHLIST_MB_FRIENDMATCHLIST                OGID_MSGBASE_PK_MATCHLIST + 0x000000046

//移动端定时请求朋友桌信息
#define OGID_PK_MATCHLIST_MB_FRIENDMATCHINFINTIME           OGID_MSGBASE_PK_MATCHLIST + 0x000000047

//QOS记录
#define OGID_TEXASPOKER_MATCHLIST_QOS						(OGID_MSGBASE_PK_MATCHLIST + 0x000000048)
//class MatchList_QOSRecord;

// 移动端，client请求match列表 （只发送非毕业、非体验、非all in的常规桌）
#define	OGID_TEXASPOKER_MATCHLIST_MB_MATCHLIST						(OGID_MSGBASE_PK_MATCHLIST + 0x00000004E)

// 移动端，client 请求match详细
#define	OGID_TEXASPOKER_MATCHLIST_MB_MATCHDETAILINFO						(OGID_MSGBASE_PK_MATCHLIST + 0x00000004F)

// 向移动端推送某牌桌玩家变化信息
#define	OGID_TEXASPOKER_MATCHLIST_MB_MATCHPLAYERCHANGE						(OGID_MSGBASE_PK_MATCHLIST + 0x000000050)

//请求排行榜大师分信息
#define OGID_TEXASPOKER_MATCHLIST_USER_MASTER_RNK_LIST_REQ	(OGID_MSGBASE_PK_MATCHLIST + 0x000000051)
#define OGID_TEXASPOKER_MATCHLIST_USER_MASTER_RNK_LIST_ACK	(OGID_MSGBASE_PK_MATCHLIST + 0x000000052)

//---------------------------------------------------------------------------
//	Jss服务（背包）
//---------------------------------------------------------------------------
//比赛列表的idle时间
#define JSS_IDLE       60//000
// 基础消息号
#define OGID_MSGBASE_TEXASPOKER_JSSCLIENT					0x00003F00 + 0x0000000A0

// 客户端登陆JSS
#define OGID_MSGBASE_TEXASPOKER_JSS_LOGIN_REQ				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 1

// 客户端登陆结果
#define OGID_MSGBASE_TEXASPOKER_JSS_LOGIN_ACK				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 2

// 道具信息
#define OGID_MSGBASE_TEXASPOKER_JSS_ITEMINFO				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 3

// 礼物信息
#define OGID_MSGBASE_TEXASPOKER_JSS_GIFTINFO				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 4

// 请求背包列表
#define OGID_MSGBASE_TEXASPOKER_JSS_PACK					OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 5

// 返回道具列表
#define OGID_MSGBASE_TEXASPOKER_JSS_ITEMLIST				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 6

// 返回礼物列表
#define OGID_MSGBASE_TEXASPOKER_JSS_GIFTLIST				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 7

// 进入桌子，客户端查询在座玩家的礼品状态 和 NB卡标志
#define OGID_MSGBASE_TEXASPOKER_JSS_TABLEPLAYERSTATUS_REQ	OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 8

// 返回桌子内所有玩家的礼品栏状态 和 NB卡标志
#define OGID_MSGBASE_TEXASPOKER_JSS_TABLEPLAYERSTATUS_ACK	OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 9

// 玩家坐下，此时需要向桌内其他玩家广播此玩家的礼品状态
#define OGID_MSGBASE_TEXASPOKER_JSS_BOAST_PLAYERSTATUS		OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0xa

// 给某人赠送礼物
#define OGID_MSGBASE_TEXASPOKER_JSS_GIVEGIFT				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0xb

// 广播表情
#define OGID_MSGBASE_TEXASPOKER_JSS_FACE_REQ				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0xc

// 系统提示消息
#define OGID_MSGBASE_TEXASPOKER_JSS_ALERT					OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0xd

// 收费表情包
#define OGID_MSGBASE_TEXASPOKER_JSS_EXPRESSION				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0xe

// JSS发送到客户端的金币礼包发放提示消息
#define OGID_MSGBASE_TEXASPOKER_JSS_GOLDPACKAGE_ADDGOLD		OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0xf

// 客户端发送到JSS服务器置换礼品状态
#define OGID_MSGBASE_TEXASPOKER_JSS_CHANGE_GIFT				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x10

// 客户端发送到JSS服务器通知道具或礼物过期
#define OGID_MSGBASE_TEXASPOKER_JSS_ITEM_GIFT				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x11

// Jss服务器通知客户端剩余免充次数和剩余免充金币量
#define OGID_MSGBASE_TEXASPOKER_JSS_FREE_COIN_BOAST			OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x12

// 客户端向JSS服务器请求免充
#define OGID_MSGBASE_TEXASPOKER_JSS_FREE_COIN_REQ			OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x13

// 通知客户端免充结果
#define OGID_MSGBASE_TEXASPOKER_JSS_FREE_COIN_ACK			OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x14

// 返回门票列表
#define OGID_MSGBASE_TEXASPOKER_JSS_ACK_TICKETLIST			OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x15

// 门票信息
#define OGID_MSGBASE_TEXASPOKER_JSS_TICKETINFO				OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x16

// 心跳
#define OGID_MSGBASE_TEXASPOKER_JSS_IDLE					OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x17
/* 未添加功能
 //购买单个贵族卡信息
 #define OGID_MSGBASE_TEXASPOKER_JSS_NOBLECARDONLY_ACK       OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x18;
 //贵族卡图片资源
 #define OGID_MSGBASE_TEXASPOKER_JSS_NOBLECARDURL_ACK        OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x19;
 //贵族卡列表信息
 #define OGID_MSGBASE_TEXASPOKER_JSS_NOBLECARDLIST_ACK       OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 0x20;
 */
// 大师分评级，应答客户端玩家大师分评级等级信息
#define OGID_MSGBASE_TEXASPOKER_JSS_PLAYERMASTERLEVINFO      OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 47

// 大师分评级，应答客户端大师分评级等级配置
#define OGID_MSGBASE_TEXASPOKER_JSS_MASTERLEVCONFIG         OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 48


// client根据自身的渠道号请求对应的活动id列表，client->jss
#define  OGID_MSGBASE_TEXASPOKER_JSS_ACTLIST                 OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 53

// client请求活动配置
#define OGID_MSGBASE_TEXASPOKER_JSS_ACTCONFIG                OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 54

// client 请求玩家参与的所有活动的进度信息
#define OGID_MSGBASE_TEXASPOKER_JSS_ACTPROGRESS              OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 55

//领取奖励
#define OGID_MSGBASE_TEXASPOKER_JSS_ACTGETREWARD_REQ            OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 31

#define OGID_MSGBASE_TEXASPOKER_JSS_ACTGETREWARD_ACK             OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 32

//玩家游戏进度变化通知
#define OGID_MSGBASE_TEXASPOKER_JSS_ACTPROGRESS_CHANGED      OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 33

//查询当前签到活动配置
#define OGID_MSGBASE_TEXASPOKER_JSS_SIGNCONFIG    OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 56

//查询当前签到活动进度
#define OGID_MSGBASE_TEXASPOKER_JSS_SIGNPROGRESS    OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 57

//玩家请求签到
#define OGID_MSGBASE_TEXASPOKER_JSS_DOSIGN      OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 58

//请求会员信息
#define OGID_MSGBASE_TEXASPOKER_JSS_USER_VIP_INFO  OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 59

//获取玩家个人信息
#define OGID_MSGBASE_TEXASPOKER_JSS_USER_INFO     OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 60

//定时取排行榜（间隔3分钟请求一次，排行榜窗口切换到当前Table时定时取，否则不请求）
#define OGID_MSGBASE_TEXASPOKER_JSS_USER_RANK_LIST    OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 61



//jinyaox
//请求广告信息
#define OGID_MSGBASE_TEXASPOKER_JSS_MBBASE_CONF     OGID_MSGBASE_TEXASPOKER_JSSCLIENT + 62
/*
typedef enum
{
	FR_UNKNOWN		= 0,		//未知
	FR_NEWFREE		= 1,		//新手引导免充
	FR_NORMAL		= 2,		//常规免充
	FR_GOLDPACKAGE	= 3,		//金币礼包免充
}FreeReason;
 */

//---------------------------------------------------------------------------
//	游戏
//---------------------------------------------------------------------------
//游戏的idle时间
#define GAME_IDLE       60//00//0
//基础号w
#define OGID_MSGBASE_TEXASPOKER_GAMEBASET		134217728

//用户发送连接确认消息
#define OGID_TEXASPOKER_GAMECONFIRMCONNECT		OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000001

//用户发送登录游戏消息
#define OGID_TEXASPOKER_LOGINGAME				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000002

//进入游戏
#define OGID_TEXASPOKER_ENTERMATCH				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000003

//[PC未使用]
#define OGID_TEXASPOKER_ADDHANDCOIN				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000004

//发牌消息
#define OGID_TEXASPOKER_INITCARD				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000005

//加注消息[广播]
#define OGID_TEXASPOKER_RAISEBET				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000006

//跟注消息[广播]
#define OGID_TEXASPOKER_FOLLOWBET				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000007

//弃牌消息[广播]
#define OGID_TEXASPOKER_FOLDBET					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000008

//让牌消息[广播]
#define OGID_TEXASPOKER_CHECKBET				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000009

//底牌首次发牌消息
#define OGID_TEXASPOKER_FLOPCARD				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000000a

//底牌第二次发牌消息
#define OGID_TEXASPOKER_TURNCARD				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000000b

//底牌第三次发牌消息
#define OGID_TEXASPOKER_RIVERCARD				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000000c

//一轮下注结束消息
#define OGID_TEXASPOKER_SENDOVERTURN			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000000d

//IDLE消息
#define OGID_TEXASPOKER_IDLE					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000000e

//一局游戏结果消息
#define OGID_TEXASPOKER_TESTRESULT				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000000f

//用户离开比赛发送该消息
#define OGID_TEXASPOKER_LEAVEGAME				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000010

//发送用户信息
#define OGID_TEXASPOKER_SENDUSERINFO			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000011

//发送成绩消息
#define OGID_TEXASPOKER_SENDRANK				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000012

//[未使用]
#define OGID_TEXASPOKER_RESTMATCH				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000013

//通知用户换桌
#define OGID_TEXASPOKER_CHANGETABLE				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000014

//发送游戏相关信息
#define OGID_TEXASPOKER_SENDGAMEINFO			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000015

//报告人数
#define OGID_TEXASPOKER_REPORTPLAYER			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000016

//玩家发送亮牌（或盖牌）消息
#define OGID_TEXASPOKER_SHOWCARD				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000017

//用户使用延时时长
#define OGID_TEXASPOKER_USEDELAYTIME			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000018

//离座消息
#define OGID_TEXASPOKER_LEAVESEAT				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000019

//广播比赛筹码消息
#define OGID_TEXASPOKER_MATCHCHIP				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000001A

//发送用户名次变化消息
#define OGID_TEXASPOKER_SENDUSERRANK			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000001B

//发送盲注变化信息
#define OGID_TEXASPOKER_BLINDCHANGE				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000001C

//发送下一级盲注信息
#define OGID_TEXASPOKER_BLINDINFO				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000001D

//发送开赛准备时间和中场休息时间
#define OGID_TEXASPOKER_SENDRESTTIME			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000001E

//发送用户离桌消息，当用户桌号发生调整时，给原桌用户发送该消息
#define OGID_TEXASPOKER_LEAVETABLE				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000001F

//服务器通知客户端下前注
#define OGID_TEXASPOKER_PUSHPRECOIN				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000020

//服务器通知客户端下盲注
#define OGID_TEXASPOKER_PUSHBLINDCOIN			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000021

//给游戏客户端发送防沉迷的消息
#define OGID_TEXASPOKER_NOWELTER				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000062

//坐下的消息
#define OGID_TEXASPOKER_SITDOWN					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000063

//带入消息
#define OGID_TEXASPOKER_BUYCHIP					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000064

//由于筹码不够，需要向客户端发送带入的消息
#define OGID_TEXASPOKER_NEETBUYCHIP				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000065

//离桌消息
#define OGID_TEXASPOKER_LIVE					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000066

//游戏规则
#define OGID_TEXASPOKER_GAMERULE				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000067

//等待大盲 c->s
#define OGID_TEXASPOKER_WAITBLING				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000068

//下一个大盲旁观 c->s
#define OGID_TEXASPOKER_NEXTBIGBLINGWATCH		OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000069



//聊天 c<->s
#define OGID_TEXASPOKER_CHATMSG					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000070

//建议 c->s
#define OGID_TEXASPOKER_PROPOSAL				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000071

//付费消息 c<->s
#define OGID_TEXASPOKER_PAYMSG					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000072



//======================以下为朋友桌新增消息======================
//兑换活跃度
#define OGID_TEXASPOKER_EXCHANGEHYD				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000073

// 通知活跃度变化
#define OGID_TEXASPOKER_CHANGEHYD				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000074

//赠送金币
#define OGID_TEXASPOKER_PRESENTCOINS			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000075

//私聊消息
#define OGID_TEXASPOKER_PRIVATECHAT				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000076

//禁言和解除禁言
#define OGID_TEXASPOKER_ABLEORUNABLECHAT		OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000077

//踢人
#define OGID_TEXASPOKER_KICK					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000078

//发送玩家列表,包含旁观者和游戏者
#define OGID_TEXASPOKER_USERLIST				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000079

//修改配置
#define OGID_TEXASPOKER_MODIFYCONFIG			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000080

//向玩家发送禁言者的列表
#define OGID_TEXASPOKER_UNABLECHATLIST			OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000081

//随机显示图片
#define OGID_TEXASPOKER_SHOWPICTRUE				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000082

//新玩家加入玩家列表
#define OGID_TEXASPOKER_ADDUSER					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000083

//玩家从玩家列表中删除
#define OGID_TEXASPOKER_DELETEUSER				OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000084

//JSS通过GS广播玩家的礼品状态
#define OGID_BROADCAST_PLAYER_GIFT_STATUS		OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000085

//赠送礼品(客户端接到此消息后应仅作播放动画操作)
#define OGID_GIVEGIFT							OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000086

//广播表情
#define OGID_BROADCAST_FACE                     OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000087

//rebuy
#define OGID_TEXASPOKER_REBUY					OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000088

//赠送道具(客户端接收到此消息后重新发送查询玩家礼品状态消息,这样贵族卡才能及时更新)
#define OGID_GIVEITEM                           OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000089

//广播比赛名次，奖励
#define OGID_BROADCAST_MATCHRANK                OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x0000008A

//常规桌中相同账号玩家被顶掉，给被顶掉的client发送通知
#define OGID_TEXASPOKER_CGZ_REPLACEBYSELF        (OGID_MSGBASE_TEXASPOKER_GAMEBASET + 0x00000097)

//======================以下为聊天新增消息======================
#define OGID_MSGBASE_OGMESSAGE                          0x00007440
#define OGID_CHAT_IDLE                                  ( OGID_MSGBASE_OGMESSAGE + 0 )
#define OGID_CHAT_GET_CHATSVR_LIST                      ( OGID_MSGBASE_OGMESSAGE + 50 )
#define OGID_CHAT_LOGIN                                 ( OGID_MSGBASE_OGMESSAGE + 1 )
#define OGID_CHAT_CHANNEL_LOGIN                         ( OGID_MSGBASE_OGMESSAGE + 2 )
#define OGID_CHAT_MSG                                   ( OGID_MSGBASE_OGMESSAGE + 3 )
#define OGID_CHAT_CHANNEL_MSG                           ( OGID_MSGBASE_OGMESSAGE + 4 )
#define OGID_CHAT_STATUSCHANGE                          ( OGID_MSGBASE_OGMESSAGE + 5 )
#define OGID_CHAT_SYSMSG                                ( OGID_MSGBASE_OGMESSAGE + 51 )

//---------------------------------------------------------------------------
//	错误代码
//---------------------------------------------------------------------------
#define ERR_ENTERMATCH_OVER_FLOW			1	//入游戏场数大于限制数
#define ERR_ENTERMATCH_PASSWORD				2	//输入密码失败(朋友桌添加)
#define ERR_ENTERMATCH_GUEST				3	//不允许游客进入(朋友桌添加)



//---------------------------------------------------------------------------
//	操作类型定义 共9种
//	28位至21位，共8位一个字节，说明消息操作类型，最多可以表示9种类型
//	0000 1111 1111 0000 0000 0000 0000 0000
//---------------------------------------------------------------------------
#define OGID_REQ					0x00000000		//请求消息类型
#define OGID_ACK					0x08000000		//应答消息类型
#define	OGID_ENC					0x04000000		//加密消息类型
#define	OGID_CPS					0x02000000		//压缩消息类型

#endif
