#ifndef __LOGINMAGCODE_H__
#define __LOGINMAGCODE_H__

namespace Protocol
{
	enum
	{
		/*LOGIN_SC_BEGIN = 7000,
		LOGIN_CS_BEGIN = 7050,*/

		REGISTER_SC_BEGIN = 7100,
		REGISTER_CS_BEGIN = 7150,
	};

	enum _MsgTypeLogin
	{
		
		///////////////////////////////////////// 登录服务器相关 /////////////////////////////////////////
		MT_LOGIN_ACK_SC = 7000,						// 登陆请求返回
		MT_ROLE_LIST_SC = 7001,						// 角色列表
		MT_GET_THREAD_ACK_SC = 7002,				// 查看服务器状态返回
		MT_ANTI_WALLOW_EX_RET_SC = 7003,			// 服务器验证防沉迷返回
		MT_ACCOUNT_KEY_ERROR_SC = 7004,				// KEY不对通知
		MT_MERGE_ROLE_LIST_SC = 7005,				// 合服后的角色列表
		MT_PROF_NUM_INFO_SC = 7006,					// 职业人数信息、
		MT_L_HEARTBEAT_SC = 7007,					// 心跳包返回

		//MT_LOGIN_REQ_CS = 7050,					// 登陆请求
		//MT_ROLE_REQ_CS = 7051,					// 角色登录请求
		MT_L_HEARTBEAT_CS = 7052,					// 心跳包
		MT_GET_TREAD_CS = 7053,						// 查看服务器分线信息
		MT_ANTI_WALLOW_CS = 7054,					// 通过防沉迷
		MT_ANTI_WALLOW_EX_CS = 7055,				// 防沉迷检验
		MT_NEW_LOGIN_REQ_CS = 7056,					// 新登录请求
		MT_NEW_ROLE_REQ_CS = 7057,					// 新角色登录请求
		
		///////////////////////////////////////// 角色相关 /////////////////////////////////////////
		MT_CREATE_ROLE_ACK_SC = 7100,				// 创建角色返回
		MT_DESTROY_ROLE_ACK_SC = 7101,				// 销毁角色返回
		MT_CHANGE_PLATNAME_ACK_SC = 7102,			// 修改平台名返回 （特定模式下使用）

		MT_CREATE_ROLE_REQ_CS = 7150,				// 创建角色
		MT_DESTROY_ROLE_REQ_CS = 7151,				// 销毁角色
		MT_CHANGE_PLATNAME_REQ_CS = 7152,			// 修改平台名
	};
}

#endif // __LOGINMAGCODE_H__
