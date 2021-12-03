
#ifndef MSGPRESENT_H
#define MSGPRESENT_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/chatserverdef.h"

#include "servercommon/maildef.hpp"
#include "servercommon/lovecontractdef.hpp"
#include "servercommon/yaojiangdef.hpp"
#include "servercommon/marrydef.hpp"
#include "servercommon/struct/discountbuyparam.hpp"
#include "other/turntable/turntableconfig.h"
#include "servercommon/roleshopdef.hpp"
#include "other/rollmoney/rollmoney.h"
#include "servercommon/struct/rolegoalparam.hpp"
#include "global/redpaper/redpapermanager.h"
#include "global/marriageseeking/marriageseekingmanager.h"
#include "servercommon/struct/shizhuangparam.hpp"
#include "servercommon/wingdef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/struct/cardparam.hpp"
#include "servercommon/struct/zhibaoparam.hpp"
#include "servercommon/guilddef.hpp"
#include "global/worldboss/worldbossmanager.hpp"
#include "servercommon/struct/rolegoalparam.hpp"
#include "servercommon/struct/zerogiftparam.hpp"
#include "gameworld/other/wabao/wabaoconfig.hpp"
#include "servercommon/shenyindef.hpp"
#include "other/upgraderush/upgraderushconfig.hpp"
#include "servercommon/talentdef.hpp"
#include "servercommon/droploglistdef.hpp"
#include "servercommon/bosscarddef.hpp"
#include "gameworld/global/luckylog/luckylogmanager.hpp"
#include "config/touzijihuaconfig.hpp"
#include "servercommon/struct/rolebigsmallgoalparam.hpp"
#include "servercommon/struct/global/worldstatus2param.hpp"
#include "servercommon/struct/orangeequipcollectdef.hpp"
#include "servercommon/struct/redequipcollectdef.hpp"
#include "servercommon/xianzunkadef.hpp"
#include "servercommon/equipbaptizedef.hpp"
#include "servercommon/commondata.hpp"
#include "servercommon/huguozhilidef.hpp"
#include "servercommon/crossgoaldef.hpp"
#include "servercommon/baizhanequipdef.hpp"
#include "global/guildrarelog/guildrarelogmanager.hpp"
#include "config/littlehelperconfig.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSSpeaker		// 2750
	{
	public:
		CSSpeaker();
		MessageHeader		header;

		enum SPEAKER_TYPE 
		{
			SPEAKER_TYPE_LOCAL,
			SPEAKER_TYPE_CROSS,

			SSPEAKER_TYPE_MAX,
		};

		void ResetSpeakMsg()
		{
			memset(speaker_msg, 0, sizeof(speaker_msg));
		}

		char				content_type;
		char				speaker_type;
		short				reserve_sh;
		int					is_auto_buy;
		unsigned int		speaker_msg_length;
		char				speaker_msg[MAX_SPEAKER_MSG_LENGTH];
	};

	class CSEvaluateRole
	{
	public:
		CSEvaluateRole();
		MessageHeader		header;

		int					uid;
		int					reserve_int;
	};


	class SCFunctionSuccNotice
	{
	public:
		SCFunctionSuccNotice();
		MessageHeader		header;

		enum FUNCTION_SUCC_TYPE
		{
			FST_EQUIP_STRENGTHEN = 0,
			FST_EQUIP_UPQUALITY,
			FST_EQUIP_UPLEVEL,
			FST_EQUIP_DECOMPOSE,
			FST_EQUIP_FLUSH,
			FST_GEMSTONE_COMPOSE,
			FST_EQUIP_STRENGTHEN_FAIL,
			FST_EQUIP_REFINE,

			FST_PET_LIFT_WUXING_CTRI = 20,
			FST_PET_LIFT_LINGXING,

			FST_MOUNT_STRENGTHEN = 40,
			FST_MOUNT_STRENGTHEN_FAIL,
		};
		short				succ_type;
		short				flag;
	};

	class SCFunctionFailNotice
	{
	public:
		SCFunctionFailNotice();
		MessageHeader		header;

		enum FUNCTION_SUCC_TYPE
		{
			FST_EQUIP_STRENGTHEN = 0,

			FST_MOUNT_STRENGTHEN = 40,
		};

		short				fail_type;
		short				flag;
	};


	class SCRoleSundryInfo
	{
	public:
		SCRoleSundryInfo();
		MessageHeader		header;

		int					has_favorited;
		int					can_fetch_weekend_reward;
	};

	class CSClientSpecialOperate
	{
	public:
		CSClientSpecialOperate();
		MessageHeader		header;

		enum
		{
			CLIENT_OPERATE_TYPE_INVALID = 0,
			CLIENT_OPERATE_TYPE_GUILD_GUIDE,		// 军团引导
			CLIENT_OPERATE_TYPE_MAX
		};

		int					client_operate_type;
	};

	class SCOperateSucc
	{
	public:
		SCOperateSucc();
		MessageHeader		header;

		enum SUCC_REASON
		{
			SUCC_REASON_EQUIP_STRENGTH = 1,										// 装备强化
		};

		short				reason;												// 原因
		short				param;												// 参数预留
	};

	class CSRoleTaskTransmit
	{
	public:
		CSRoleTaskTransmit();
		MessageHeader		header;

		int pos_x;
		int pos_y;
	};

	class CSTaskOpenPanel
	{
	public:
		CSTaskOpenPanel();
		MessageHeader		header;

		enum OPEN_TYPE
		{
			OPEN_TYPE_PET_SKILL_PANEL = 0,										// 宠物技能面板
			OPEN_TYPE_CAPABILITY_PANEL = 1,										// 战斗力面板
		};

		int open_type;
	};

	/////////////////////////////////// 投资计划 ///////////////////////////////////////
	class SCTouZiJiHuaInfo				// 2720 投资计划信息
	{
	public:
		SCTouZiJiHuaInfo();
		MessageHeader		header;

		char active_plan_0;				// 等级投资1激活标志
		char active_plan_1;				// 等级投资2激活标志
		char active_plan_2;				// 等级投资3激活标志
		char new_plan_first_reward_flag;	// 周卡投资立返标志

		short plan_0_reward_flag;		// 等级投资1奖励标志
		short plan_1_reward_flag;		// 等级投资2奖励标志
		short plan_2_reward_flag;		// 等级投资3奖励标志
		char active_highest_plan;		// 当前激活最高等级投资计划
		char reserve_sh;

		char reserve;
		char active_plan_3;				// 等级投资4激活标志
		short plan_3_reward_flag;		// 等级投资4奖励标志

		unsigned int buy_time;			// 周卡投资购买时间
		int reward_flag;
		int vip_reward_flag;
		int reward_gold_bind_flag;		// 周卡投资奖励标志

		int list_len;
		char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE];	//成长基金奖励领取次数,0代表没购买

		UNSTD_STATIC_CHECK(TouzijihuaConfig::MAX_FOUNDATION_TYPE % 4 == 0)
	};

	class CSFetchTouZiJiHuaReward		// 2780 领取投资计划奖励请求
	{
	public:
		CSFetchTouZiJiHuaReward();
		MessageHeader		header;

		short plan_type;
		short seq;
	};

	class CSTouzijihuaActive			// 2786 激活投资计划请求
	{
	public:
		CSTouzijihuaActive();
		MessageHeader		header;

		short plan_type;
		short reserve_sh;
	};

	enum NEW_TOUZIJIHUA_OPERATE_TYPE
	{
		NEW_TOUZIJIHUA_OPERATE_BUY = 0,		// 购买周卡投资
		NEW_TOUZIJIHUA_OPERATE_FETCH,		// 获取周卡奖励
		NEW_TOUZIJIHUA_OPERATE_FIRST,		// 获取周卡立返
		NEW_TOUZIJIHUA_OPERATE_VIP_FETCH,
		NEW_TOUZIJIHUA_OPERATE_FOUNDATION_FETCH,	//领取成长基金
	};
	class CSNewTouzijihuaOperate		// 2787 新投资计划操作请求
	{
	public:
		CSNewTouzijihuaOperate();
		MessageHeader		header;

		short operate_type;
		short param;
	};

	enum TOUZIJIHUA_FB_BOSS_OPERATE_TYPE
	{
		TOUZIJIHUA_FB_BOSS_OPERATE_FB_BUY = 0,			// 购买副本投资计划
		TOUZIJIHUA_FB_BOSS_OPERATE_FB_REWARD =1,		// 获取副本投资计划奖励，param：index

		TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_BUY =2,			// 购买boss投资计划
		TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_REWARD =3,		// 获取boss投资计划，param：index

		TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_BUY = 4,			// 购买神域boss投资计划
		TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_REWARD = 5,		// 获取神域boss投资计划，param：index
	};

	class CSTouzijihuaFbBossOperate			// 2731 副本、boss投资计划请求
	{
	public:
		CSTouzijihuaFbBossOperate();
		MessageHeader	header;

		short operate_type;
		short param;
	};

	class SCTouzijihuaFbBossInfo		  // 2732 副本、boss投资计划信息
	{
	public:
		SCTouzijihuaFbBossInfo();
		MessageHeader	header;

		short active_fb_plan;
		short fb_plan_reward_flag;
		short fb_pass_level;

		short active_boss_plan;
		short boss_plan_reward_flag;
		short kill_boss_num;

		short active_shenyu_boss_plan;
		short shenyu_boss_plan_reward_flag;
		short kill_shenyu_boss_num;
		short reserve_sh;
	};
	//////////////////////////////////////////////////////////////////////////
	class CSYaojiangReq			// 2807
	{
	public:
		CSYaojiangReq();
		MessageHeader		header;

		int yaojiang_type;
	};

	class SCYaojiangInfo		// 2745
	{
	public:
		SCYaojiangInfo();
		MessageHeader		header;

		int rand_index;
		ItemID item_id;
		short num;
	};

	///////////////////////////////// 结婚系统 /////////////////////////////////////////

	//-------------------------------- 通用 -----------------------------------------
	class CSMarryXunyouOpera  // 2793 结婚巡游
	{
	public:
		CSMarryXunyouOpera();
		MessageHeader		header;

		int					opera_type;
	};

	class CSResetLoverName	// 2796 重设仙侣名字
	{
	public:
		CSResetLoverName();
		MessageHeader		header;

		GameName			new_lover_name;
	};

	class SCMarryXunyouFirework	 // 6006 巡游烟花(暂无用)
	{
	public:
		SCMarryXunyouFirework();
		MessageHeader		header;

		ObjID				obj_id;
		short				reserved;
	};

	class SCMarryReqRoute	// 6004 求婚信息转发对方
	{
	public:
		SCMarryReqRoute();
		MessageHeader		header;

		int					hunli_type;
		int					req_uid;
		GameName			req_name;
	};

	class SCMarrySpecialEffect	// 6003结婚特效
	{
	public:
		SCMarrySpecialEffect();
		MessageHeader		header;

		int					marry_type;
	};

	enum MARRY_REQ_TYPE
	{
		MARRY_REQ_TYPE_PROPOSE = 0,		// 求婚 p1:婚礼类型 p2: 对方uid
		MARRY_CHOSE_SHICI_REQ,			// 选誓词 p1:誓词类型
		MARRY_PRESS_FINGER_REQ,         // 摁手指
	};

	class CSMarryReq	// 2778 结婚请求
	{
	public:
		CSMarryReq();
		MessageHeader		header;

		int				ope_type;
		int				param_1;
		int				param_2;
	};

	class CSMarryRet	// 2779 求婚回复请求
	{
	public:
		CSMarryRet();
		MessageHeader		header;

		int					marry_type;
		int					req_uid;
		int					is_accept;
	};

	class SCIsAcceptMarry	// 6616 是否同意求婚
	{
	public:
		SCIsAcceptMarry();
		MessageHeader		header;

		int is_accept;		// 同意：不同意  1：0
	};

	enum MARRY_RET_TYPE
	{
		MARRY_AGGRE = 0,			// 点击我愿意
		MARRY_CHOSE_SHICI,			// 选誓词
		MARRY_PRESS_FINGER,         // 摁手指
		MARRY_CANCEL,				// 取消结婚
	};

	class SCMarryRetInfo			// 2864 结婚操作回馈  
	{
	public:
		SCMarryRetInfo();
		MessageHeader header;

		int ret_type;				// 返回类型（MARRY_RET_TYPE）
		int ret_val;				// 返回值 0：对象的role_id，1：誓词类型 2：双方是否摁手指（1代表自己已按，2代表双方已按）
	};

	class SCMarryInformation	// 6604 结婚信息
	{
	public:
		SCMarryInformation();
		MessageHeader		header;

		int				lover_uid;
		UserName		lover_name;
	};

	class CSQingyuanDivorceReq
	{
	public:
		CSQingyuanDivorceReq();
		MessageHeader header;

		int is_forced_divorce;  // 1：强制离婚, 0：非强制离婚
	};

	class SCDivorceReqRoute	// 6005 离婚信息转发
	{
	public:
		SCDivorceReqRoute();
		MessageHeader		header;

		int					req_uid;
		GameName			req_name;
	};

	class CSDivorceRet	// 6603 离婚回复
	{
	public:
		CSDivorceRet();
		MessageHeader		header;

		int					req_uid;
		int					is_accept;
	};

	//-------------------------------- 情缘新加协议 ----------------------
	enum QINGYUAN_OPERA_TYPE
	{
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE = 0,		// 婚礼预约 param1 预约下标 param2 婚宴类型
		QINGYUAN_OPERA_TYPE_WEDDING_INVITE_GUEST,	// 邀请宾客 param1 宾客uid
		QINGYUAN_OPERA_TYPE_WEDDING_REMOVE_GUEST,	// 移除宾客 param1 宾客uid
		QINGYUAN_OPERA_TYPE_WEDDING_BUY_GUEST_NUM,	// 购买宾客数量
		QINGYUAN_OPERA_TYPE_WEDDING_GET_YUYUE_INFO,	// 获取预约信息
		QINGYUAN_OPERA_TYPE_WEDDING_GET_ROLE_INFO,	// 获取玩家信息
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_FLAG,		// 获取婚礼预约标记
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_RESULT,	// 是否同意婚礼预约时间 param1 seq param2 是否同意
		QINGYUAN_OPERA_TYPE_LOVER_INFO_REQ,			// 请求伴侣信息
		QINGYUAN_OPERA_TYPE_XUNYOU_ROLE_INFO,		// 获取玩家巡游信息
		QINGYUAN_OPERA_TYPE_XUNYOU_SA_HONGBAO,		// 巡游撒红包 param1:is_buy
		QINGYUAN_OPERA_TYPE_XUNYOU_GIVE_FLOWER,		// 巡游购买送花次数  
		QINGYUAN_OPERA_TYPE_XUNYOU_OBJ_POS,			// 获取巡游对象当前坐标 param1:seq
		QINGYUAN_OPERA_TYPE_LOVER_TITLE_INFO,		// 仙侣称号信息 （暂无用）
		QINGYUAN_OPERA_TYPE_FETCH_LOVER_TITLE,		// 领取仙侣称号 param1 index （暂无用）
		QINGYUAN_OPERA_TYPE_BUY_AND_PUTON_EQUIP,	// 购买且穿戴装备 param1 index （暂无用）

		QINGYUAN_OPERA_TYPE_MAX,
	};

	class CSQingYuanOperaReq		// 2795 情缘操作请求
	{
	public:
		CSQingYuanOperaReq();
		MessageHeader header;

		int opera_type;
		int param1;
		int param2;
	};

	enum QINGYUAN_INFO_TYPE
	{
		QINGYUAN_INFO_TYPE_WEDDING_YUYUE = 0,	// 婚礼预约
		QINGYUAN_INFO_TYPE_WEDDING_STANDBY,		// 婚礼准备
		QINGYUAN_INFO_TYPE_GET_BLESSING,		// 收到祝福 param_ch1 祝福类型 param2 参数
		QINGYUAN_INFO_TYPE_BAITANG_RET,			// 拜堂请求
		QINGYUAN_INFO_TYPE_BAITANG_EFFECT,		// 拜堂特效 param_ch1 是否已经拜堂
		QINGYUAN_INFO_TYPE_LIVENESS_ADD,		// 婚礼热度增加 param2 当前热度
		QINGYUAN_INFO_TYPE_HAVE_APPLICANT,		// 婚礼申请者 param2 申请者uid 
		QINGYUAN_INFO_TYPE_APPLY_RESULT,		// 申请结果 param2 1:同意 0:拒绝
		QINGYUAN_INFO_TYPE_ROLE_INFO,			// 玩家信息 param_ch1 婚姻类型 param_ch2 是否有婚礼次数 param_ch3 当前婚礼状态 param_ch4 婚礼预约seq param_ch5 求婚奖励标记
		QINGYUAN_INFO_TYPE_WEDDING_YUYUE_FLAG,	// 婚礼预约标记 param6
		QINGYUAN_INFO_TYPE_YUYUE_RET,			// 婚礼预约请求 param_ch1 seq
		QINGYUAN_INFO_TYPE_YUYUE_POPUP,			// 婚礼预约弹窗
		QINGYUAN_OPERA_TYPE_BUY_QINGYUAN_FB_RET,// 收到购买次数请求
		QINGYUAN_INFO_TYPE_YUYUE_SUCC,			// 婚礼预约成功
		QINGYUAN_INFO_TYPE_LOVER_INFO,			// 伴侣信息 param5 伴侣uid param_ch1 伴侣阵营 role_name 伴侣名字
		QINGYUAN_INFO_TYPE_LOVER_TITLE_INFO,	// 仙侣称号信息 param2 领取flag （暂无用）
		QINGYUAN_INFO_TYPE_WEDDING_BEGIN_NOTICE,// 婚宴开启通知
		QINGYUAN_INFO_TYPE_XUNYOU_INFO,			// 婚宴巡游信息 p_c1:撒红包次数 p_c2:撒红包购买次数 p_c3:送花次数 p_c4:送花购买次数 p_c5:采集巡游红包次数 p_ch6:婚姻类型
		QINGYUAN_INFO_TYPE_XUNYOU_OBJ_POS,		// 婚宴巡游对象坐标 p_c1:是否是巡游主人 p2:x p3:y p4:obj_id p5:scene_id
	};

	class SCQingYuanAllInfo  // 2784 情缘返回信息
	{
	public:
		SCQingYuanAllInfo();
		MessageHeader header;

		int info_type;
		char param_ch1;
		char param_ch2;
		char param_ch3;
		char param_ch4;
		char param_ch5;
		char param_ch6;
		short param2;
		short param3;
		short param4;
		int param5;
		long long param6;
		GameName role_name;    
	};

	struct WeddingGuestInfo
	{
		WeddingGuestInfo() : user_id(0)
		{
			memset(name, 0, sizeof(GameName));
		}

		int user_id;
		GameName name;
	};

	class SCQingYuanWeddingAllInfo	//  6007 情缘婚宴预约信息返回
	{
	public:
		SCQingYuanWeddingAllInfo();
		MessageHeader header;

		int role_id;						// 被求婚玩家id
		GameName role_name;					// 被求婚玩家名字
		int lover_role_id;					// 求婚玩家id
		GameName lover_role_name;			// 求婚玩家名字
		char wedding_type;					// 结婚类型
		char has_invite_guests_num;			// 已邀请宾客数量
		char can_invite_guest_num;			// 能够邀请的宾客最大数
		char wedding_yuyue_seq;				// 婚宴预约时间段
		int wedding_index;					// 全服第几对情侣
		char role_prof;						// 被求婚玩家职业
		char lover_role_prof;				// 求婚玩家职业
		short reserve_sh;	
		int count;
		WeddingGuestInfo guests_list[WEDDING_GUESTS_MAX_NUM];
	};

	//-------------------------------- 表白墙 ----------------------

	enum PROFESS_WALL_REQ_TYPE
	{
		PROFESS_WALL_REQ_INFO = 0,			// 表白墙信息（p1:0自己 1对方 2公共；p2:时间戳） 公共信息无需时间戳
		PROFESS_WALL_REQ_LEVEL_INFO = 1,	// 表白等级信息
		PROFESS_WALL_REQ_DELETE = 2,		// 删除表白（p1:墙类型；p2:时间戳；p3:角色id） 结果由1145返回，operate为75,p1时间戳 p2:role_id

		PROFESS_WALL_REQ_TYPE_MAX,
	};

	// 表白墙通用请求 2865
	class CSProfessWallReq
	{
	public:
		CSProfessWallReq();
		MessageHeader header;

		int oper_type;
		unsigned int param_1;
		unsigned int param_2;
		unsigned int param_3;
	};

	// 表白请求 2866
	class CSProfessToReq
	{
	public:
		CSProfessToReq();
		MessageHeader header;

		int target_role_id;		// 对方id
		short gift_id;			// 礼物类型
		short is_auto_buy;		// 是否自动购买
		ProfessContent	content;// 表白内容
	};


	// 公共表白墙信息 2867
	class SCGlobalProfessWallInfo
	{
	public:
		SCGlobalProfessWallInfo();
		MessageHeader header;

		struct ProfessItem
		{
			int				role_id_from;		// 表白者
			int				role_id_to;			// 被表白者
			int				gife_type;			// 礼物类型
			unsigned int	profess_time;		// 表白时间
			long long		avatar_timestamp;	// 表白者头像
			long long		avatar_timestamp_to;// 被表白者头像
			GameName		role_name_from;		// 表白者名字
			GameName		role_name_to;		// 被表白者名字
			ProfessContent	content;			// 表白内容
		};

		int profess_count;						// 表白数量
		unsigned int timestamp;					// 更新时间戳（避免发重复数据）
		ProfessItem items[GLOBAL_PROFESS_MAX_NUM];	//最大30个
	};

	enum PROFESS_INFO_NOTIFY_TYPE
	{
		PROFESS_INFO_NOTIFY_TYPE_NORMAL = 0,
		PROFESS_INFO_NOTIFY_TYPE_PROFESS_FROM,       // 被表白通知 
	};

	// 个人表白墙信息 2868
	class SCPersonProfessWallInfo
	{
	public:
		SCPersonProfessWallInfo();
		MessageHeader header;

		struct ProfessItem
		{
			int				other_role_id;		// 对方id
			int				gife_type;			// 礼物类型
			unsigned int	profess_time;		// 表白时间
			long long		avatar_timestamp;	// 对方头像
			GameName		other_name;			// 对方名字
			ProfessContent	content;			// 表白内容
		};

		char notify_type;						// 通知类型
		char profess_type;						// 表白墙类型
		short profess_count;					// 表白数量
		unsigned int timestamp;					// 更新时间戳（避免发重复数据）
		ProfessItem items[PERSON_PROFESS_MAX_NUM];	//最大12个
	};

	// 表白特效	2869
	class SCProfessWallEffect
	{
	public:
		SCProfessWallEffect();
		MessageHeader header;

		int effect_type;
	};

	// 表白等级信息	2870
	class SCProfessLevelInfo
	{
	public:
		SCProfessLevelInfo();
		MessageHeader header;

		unsigned short my_grade;		// 自己的表白等级
		unsigned short other_grade;		// 对方的表白等级
		unsigned int my_exp;			// 自己的表白经验
		unsigned int other_exp;			// 对方的表白经验
	};

	//-------------------------------- 婚宴副本 -----------------------------------------
	class CSMarryHunyanBless  // 2783 婚宴祝福
	{
	public:
		CSMarryHunyanBless();
		MessageHeader		header;

		void ResetMailContentTxt()
		{
			memset(contenttxt, 0, sizeof(contenttxt));
		}

		int					merry_uid;
		int					zhufu_type;

		int					contenttxt_len;
		MailContentTxt		contenttxt;
	};

	class CSMarryHunyanGetBlessNews  // 2794 获取婚宴祝福消息 (暂时无用)
	{
	public:
		CSMarryHunyanGetBlessNews();
		MessageHeader		header;
	};

	class CSMarryHunyanOpera   // 2782 婚宴操作请求
	{
	public:
		CSMarryHunyanOpera();
		MessageHeader		header;

		int					opera_type;
		int					param_1;
		int					param_2;
	};

	enum SPECIAL_PARAM_CHANGE_TYPE
	{
		SPECIAL_PARAM_CHANGE_TYPE_VALID = 0,
		SPECIAL_PARAM_CHANGE_TYPE_XUNYOU,			// 巡游
	};

	class SCSpecialParamChange	// 2729 特殊状态改变
	{
	public:
		SCSpecialParamChange();
		MessageHeader		header;

		short change_type;
		ObjID obj_id;
		int param;
	};

	class SCMarryHunyanOpera	// 6008 婚宴操作返回信息
	{
	public:
		SCMarryHunyanOpera();
		MessageHeader		header;

		ObjID				obj_id;
		short				opera_type;
		short				opera_param;
		short				reserve;
	};

	class SCWeddingBlessingRecordInfo // 6010 婚宴祝福记录
	{
	public:
		SCWeddingBlessingRecordInfo();
		MessageHeader header;

		int count;
		WeddingBlessingRecord record_info_list[WEDDING_BLESSSING_MAX_RECORD_NUM];
	};

	class SCWeddingApplicantInfo  // 6016  婚宴申请列表返回
	{
	public:
		SCWeddingApplicantInfo();
		MessageHeader header;

		int count;
		WeddingApplicantInfo applicant_list[WEDDING_APPLICANT_MAX_NUM];
	};

	class SCHunYanCurWeddingAllInfo  // 6000 当前婚宴场景信息请求返回
	{
	public:
		SCHunYanCurWeddingAllInfo();
		MessageHeader header;

		int role_id;						// 被求婚玩家id
		GameName role_name;					// 被求婚玩家名字
		int lover_role_id;					// 求婚玩家id
		GameName lover_role_name;			// 求婚玩家名字
		char role_prof;						// 被求婚玩家职业
		char lover_role_prof;				// 求婚玩家职业
		char reserve_ch;
		char cur_wedding_seq;				// 婚宴时间段序号
		int wedding_index;					// 全服第几对情侣
		int count;
		int guests_uid[WEDDING_GUESTS_MAX_NUM]; // 宾客列表
	};

	class SCWeddingRoleInfo   // 5440 婚宴场景个人信息
	{
	public:
		SCWeddingRoleInfo();
		MessageHeader header;

		void Reset()
		{
			wedding_liveness = 0;
			is_baitang = 0;
			is_in_red_bag_fulsh_time = 0;
			banquet_has_gather_num = 0;
			cur_turn_has_gather_red_bag = 0;
			total_exp = 0;
			hunyan_food_id_count = 0;

			memset(hunyan_food_id_list, 0, sizeof(hunyan_food_id_list));
		}

		static const int MAX_GATHR_FOOD_COUNT = 20;

		short wedding_liveness;
		char is_baitang;
		char is_in_red_bag_fulsh_time;
		short banquet_has_gather_num;
		short cur_turn_has_gather_red_bag;
		long long total_exp;

		short hunyan_food_id_count;							// 已采婚宴酒席数量
		ObjID hunyan_food_id_list[MAX_GATHR_FOOD_COUNT];	// 已采婚宴酒席objid列表
	};

	enum HUNYAN_NOTIFY_REASON
	{
		HUNYAN_NOTIFY_REASON_STATE_CHANGE = 0,			// 状态改变
		HUNYAN_NOTIFY_REASON_ENTER_HUNYAN,				// 进入婚宴
		HUNYAN_NOTIFY_REASON_LOGIN,						// 上线
		HUNYAN_NOTIFY_REASON_INVITE_FRIEND,				// 邀请好友
		HUNYAN_NOTIFY_REASON_INVITE_GUILD,				// 邀请仙盟好友
		HUNYAN_NOTIFY_REASON_GATHER,					// 采集
		HUNYAN_NOTIFY_REASON_PAOHUAQIU,					// 抛花球
		HUNYAN_NOTIFY_REASON_LEVEL_OR_ENTER,			// 婚宴人数变动
		HUNYAN_NOTIFY_REASON_LEAVE,						// 离开婚宴
	};

	struct HunyanMarryUserInfo
	{
		HunyanMarryUserInfo()
			: marry_uid(0), sex(0), prof(0), camp(0), hongbao_count(0), avator_timestamp(0)
		{
			memset(marry_name, 0, sizeof(marry_name));
		}

		int marry_uid;
		GameName marry_name;
		char sex;
		char prof;
		char camp;
		char hongbao_count;
		long long avator_timestamp;
	};

	const int HUNYAN_MARRY_USER_COUNT = 2;

	class SCHunyanStateInfo	// 6009 婚宴场景状态信息
	{
	public:
		SCHunyanStateInfo();
		MessageHeader		header;

		int notify_reason;  

		int hunyan_state;												// 婚宴状态
		unsigned int next_state_timestmp;
		int fb_key;														// 场景key
		int yanhui_type;												// 婚宴类型
		int remainder_eat_food_num;										// 还可以采集多少食物
		int roleuid_count;												// 
		int reserve_int;												// 是否第一次钻石宴会
		int is_self_hunyan;												// 是否是自己开的婚宴
		unsigned int paohuoqiu_timestmp;                                // 最后一次开始抛花球的时间
		int total_gather_times;											// 采集酒席次数

		HunyanMarryUserInfo marryuser_list[HUNYAN_MARRY_USER_COUNT];	// 两夫妻
		int roleuid_list[MAX_ROLE_IN_YANHUI];							// 宾客的list
	};

	class SCQingyuanHunyanInviteInfo // 5439 婚宴邀请信息（暂无用）
	{
	public:
		SCQingyuanHunyanInviteInfo();
		MessageHeader header;

		struct InviteItem
		{
			InviteItem()
			{
				this->Reset();
			}

			void Reset()
			{
				memset(mem_name, 0, sizeof(mem_name));
				memset(women_name, 0, sizeof(women_name));
				yanhui_fb_key = -1;
				yanhui_type = -1;
				gather_num = 0;
			}

			GameName mem_name;
			GameName women_name;
			int yanhui_fb_key;
			int yanhui_type;
			int gather_num;
		};

		static const int INVITE_NUM = 10;
		int invite_count;
		InviteItem scq_invite_list[INVITE_NUM];
	};
	
	// 6013 通知全服结婚
	class SCMarryNotic
	{
	public:
		SCMarryNotic();
		MessageHeader		header;
		int uid1;
		GameName name1;
		long long avator_timestamp1;
		int prof1;

		int uid2;
		GameName name2;
		long long avator_timestamp2;
		int prof2;

		int server_marry_times;
	};

	enum MARRY_ZHUHE_TYPE
	{
		MARRY_ZHUHE_TYPE0 = 0,			// 祝福
		MARRY_ZHUHE_TYPE1,				// 送花
	};

	// 6014 祝贺新人
	class CSMarryZhuheSend
	{
	public:
		CSMarryZhuheSend();
		MessageHeader		header;

		int uid;
		int type;
	};

	// 6015 新人收到祝贺
	class SCMarryZhuheShou
	{
	public:
		SCMarryZhuheShou();
		MessageHeader		header;
		
		int uid;
		GameName name;
		int type;
	};

	//-------------------------------- 情缘圣地 -----------------------------------------

	enum QYSD_OPERA_TYPE
	{
		QYSD_OPERA_TYPE_FETCH_TASK_REWARD = 0,			// 领取任务奖励
		QYSD_OPERA_TYPE_FETCH_OTHER_REWARD,				// 领取额外奖励
	};

	// 情缘圣地操作请求 6020
	class CSQingYuanShengDiOperaReq
	{
	public:
		CSQingYuanShengDiOperaReq();
		MessageHeader		header;

		int opera_type;
		int param;
	};

	// 情缘圣地任务信息 6021
	class SCQingYuanShengDiTaskInfo
	{
	public:
		SCQingYuanShengDiTaskInfo();
		MessageHeader		header;

		char is_fetched_task_other_reward;
		char lover_is_all_task_complete;
		short task_count;
		QingYuanShengDiTask task_info_list[QING_YUAN_SHENG_DI_TASK_MAX_COUNT];
	};

	// 情缘圣地boss信息 6022
	static const int QING_YUAN_SHENG_DI_BOSS_COUNT_MAX = 20;
	class SCQingYuanShengDiBossInfo
	{
	public:
		SCQingYuanShengDiBossInfo();
		MessageHeader		header;

		struct BossInfo
		{
			int boss_id;
			unsigned short pos_x;
			unsigned short pos_y;
			unsigned int next_refresh_time;
		};

		int boss_count;
		BossInfo boss_list[QING_YUAN_SHENG_DI_BOSS_COUNT_MAX];
	};

	//-------------------------------- 情缘副本-----------------------------------------

	enum QINGYUAN_FB_OPERA_TYPE
	{
		QINGYUAN_FB_OPERA_TYPE_BASE_INFO = 0,		  // 请求副本基本信息
		QINGYUAN_FB_OPERA_TYPE_BUY_TIMES,			  // 购买进入次数
		QINGYUAN_FB_OPERA_TYPE_BUY_BUFF,			  // 购买buff
		QINGYUAN_FB_OPERA_TYPE_BUY_DOUBLE_REWARD,	  // 购买双倍奖励
	};

	class CSQingyuanFBOperaReq // 5469 情缘副本操作请求
	{
	public:
		CSQingyuanFBOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
	};

	class SCQingyuanBaseFBInfo		// 5425 副本基本信息返回
	{
	public:
		SCQingyuanBaseFBInfo();
		MessageHeader header;

		char join_fb_times;
		char buy_fb_join_times;
		char is_hunyan_already_open;
		char reserve_ch;
		int qingyuan_value;
		int yanhui_fb_key;
	};

	class SCQingyuanFBInfo		// 5426 副本场景信息
	{
	public:
		SCQingyuanFBInfo();
		MessageHeader header;

		char curr_wave;					// 当前波数
		char max_wave_count;			// 最大波数
		char is_pass;					// 是否通过
		char is_finish;					// 是否完成
		int next_refresh_monster_time;	// 下次刷怪时间
		short add_qingyuan_value;
		short buy_buff_times;			// 
		int buff_failure_timestamp;
		int per_wave_remain_time;		// 每波剩余时间
		int total_get_uplevel_stuffs;
		long long total_get_exps;
		int kick_out_timestamp;			// 踢出副本时间
		char male_is_buy_double_reward;	// 男方是否购买双倍奖励
		char female_is_buy_double_reward; // 女方是否购买双倍奖励
		short reserve_sh;					
	};

	class SCQingyuanFBRewardRecordInfo				//	5442 副本奖励记录
	{
	public:
		SCQingyuanFBRewardRecordInfo();
		MessageHeader header;

		static const int MAX_ITEM_COUNT = 4;
		struct ItemInfo
		{
			ItemInfo() : reward_item_id(0), reward_item_num(0) {}
			ItemID reward_item_id;
			short reward_item_num;
		};

		int item_count;
		ItemInfo item_list[MAX_ITEM_COUNT];
	};

	//-------------------------------- 情缘装备-----------------------------------------
	class CSHunjieUpLevel
	{
	public:
		CSHunjieUpLevel();
		MessageHeader header;

		ItemID stuff_id;
		short repeat_times;
		int is_auto_buy;
	};

	class CSHunjieReqEquipInfo
	{
	public:
		CSHunjieReqEquipInfo();
		MessageHeader header;
	};

	class CSQingyuanTakeOffEquip
	{
	public:
		CSQingyuanTakeOffEquip();
		MessageHeader header;
	};

	class SCHunjieEquipInfo
	{
	public:
		SCHunjieEquipInfo();
		MessageHeader header;

		int consume_num;
		int baoji_num;
		int exp;
		int star;

		int lover_level;
		ItemID lover_hunjie_item_id;
		ItemID hunjie_item_id;
		short lover_hunjie_star_level;
		short lover_prof;
	};

	class SCHunjieEquipUpLevelSendLover
	{
	public:
		SCHunjieEquipUpLevelSendLover();
		MessageHeader header;

		int level;
		ItemID hunjie_item_id;
		short hunjie_star_level;
	};

	class SCQingyuanBlessInfo
	{
	public:
		SCQingyuanBlessInfo();
		MessageHeader header;

		int is_fetch_bless_reward;
		int bless_days;
		int lover_bless_days;
	};


	class CSQingyuanQueryMateQingyuanValue
	{
	public:
		CSQingyuanQueryMateQingyuanValue();
		MessageHeader header;
	};

	class SCQingyuanMateQingyuanValue
	{
	public:
		SCQingyuanMateQingyuanValue();
		MessageHeader header;

		int mate_qingyuan_value;
	};

	class SCQingyuanCardLevelList											// 情缘卡牌等级列表  4724
	{
	public:
		SCQingyuanCardLevelList();
		MessageHeader header;

		char card_level_list[QINGYUAN_CARD_MAX_ID + 1];
	};

	class SCQingyuanCardUpdate												// 情缘卡牌等级变更通知 4725
	{
	public:
		SCQingyuanCardUpdate();
		MessageHeader header;

		char card_id;
		char card_level;
		short reserve_sh;
	};

	class CSQingyuanCardUpgradeReq											// 请求升级情缘卡牌 4780
	{
	public:
		CSQingyuanCardUpgradeReq();
		MessageHeader header;

		char card_id;
		char reserve_ch;
		short reserve_sh;
	};

	class CSQingyuanFetchBlessRewardReq										// 获取情缘祝福奖励
	{
	public:
		CSQingyuanFetchBlessRewardReq();
		MessageHeader header;
	};

	class CSQingyuanAddBlessDaysReq										// 增加祝福天数
	{
	public:
		CSQingyuanAddBlessDaysReq();
		MessageHeader header;
	};

	enum LOVE_CONTRACT_REQ_TYPE
	{
		LC_REQ_TYPE_INFO = 0,					// 信息请求
		LC_REQ_TYPE_BUY_LOVE_CONTRACT,		    // 购买爱情契约
		LC_REQ_TYPE_NOTICE_LOVER_BUY_CONTRACT,   // 提醒对方购买爱情契约
	};

	class CSLoveContractOperaReq			// 4792 购买爱情契约
	{
	public:
		CSLoveContractOperaReq();
		MessageHeader header;

		short opera_type;
		short param1;
	};

	enum LOVE_CONTRACT_NOTICE_TYPE
	{
		LC_NOTICE_TYPE_FETCH = 0,					// 领取爱情契约奖励
		LC_NOTICE_TYPE_NOTICE,						// 爱情契约留言
	};

	class CSQingyuanFetchLoveContract		// 4793 领取爱情契约奖励
	{
	public:
		CSQingyuanFetchLoveContract();
		MessageHeader header;

		short opera_type;
		short day_num;
		LoveContractNotice love_contract_notice;
	};

	struct ContractNoticePack
	{
		unsigned int time;
		LoveContractNotice contract_notice;
	};

	enum LOVE_CONTRACT_INFO_TYPE
	{
		LOVE_CONTRACT_INFO_TYPE_NORMAL = 0,					// 一般
		LOVE_CONTRACT_INFO_TYPE_NOTICE_BUY_CONTRACT,	  // 提醒对方购买契约
	};

	class SCQingyuanLoveContractInfo		// 4794 爱情契约信息
	{
	public:
		SCQingyuanLoveContractInfo();
		MessageHeader header;

		int info_type;									// 消息类型
		int self_love_contract_reward_flag;
		int can_receive_day_num;						// -1 表示不可以领
		unsigned int lover_love_contract_timestamp;		// 0 表示没有帮伴侣购买

		int self_notice_list_count;
		ContractNoticePack self_notice_list[MAX_NOTICE_COUNT];

		int lover_notice_list_count;
		ContractNoticePack lover_notice_list[MAX_NOTICE_COUNT];
	};

	//-------------------------------- 情缘光环-----------------------------------------

	enum QINGYUAN_COUPLE_HALO_REQ_TYPE
	{
		QINGYUAN_COUPLE_REQ_TYPE_INFO = 0,				// 请求信息
		QINGYUAN_COUPLE_REQ_TYPE_USE,					// 装备光环 param_1光环类型
		QINGYUAN_COUPLE_REQ_TYPE_UP_LEVEL,				// 光环升级 param_1光环类型 param_2 是否自动购买
		QINGYUAN_COUPLE_REQ_TYPE_BUY_HALO,				// 光环特卖 param_1 光环类型

		QINGYUAN_COUPLE_REQ_TYPE_MAX,
	};

	class CSQingyuanCoupleHaloOperaReq								// 4785
	{
	public:
		CSQingyuanCoupleHaloOperaReq();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;
	};

	class SCQingyuanCoupleHaloInfo										// 4783
	{
	public:
		SCQingyuanCoupleHaloInfo();
		MessageHeader header;

		char equiped_couple_halo_type;														// 已装备的夫妻光环
		char  cur_tejia_halo_type;															// 当前特价光环类型
		short couple_halo_max_type_count;													// 夫妻光环类型数
		unsigned int tejie_halo_invalid_time;												// 当前光环特价失效时间
		short couple_halo_level_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];						// 夫妻光环等级
		short couple_halo_exp_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];							// 夫妻光环经验
	};

	class SCQingyuanCoupleHaloTrigger									// 4784
	{
	public:
		SCQingyuanCoupleHaloTrigger();
		MessageHeader header;

		int role1_uid;
		int role2_uid;

		int halo_type;
	};

	//-------------------------------- 情缘全局-----------------------------------------
	enum QINGYUAN_EQUIP_REQ_TYPE
	{
		QINGYUAN_EQUIP_REQ_SELF_EQUIP_INFO = 0,			// 请求自己装备信息
		QINGYUAN_EQUIP_REQ_OTHER_EQUIP_INFO,			// 请求伴侣装备信息
		QINGYUAN_EQUIP_REQ_ACTIVE_SUIT,					// 请求激活套装 param_1套装类型，param_2 套装槽, param_3背包索引
		QINGYUAN_EQUIP_REQ_TAKE_OFF,					// 请求脱装备, param_1 装备索引

		QINGYUAN_EQUIP_REQ_ACTIVE_SPECIAL_BABY,			// 请求激活特殊宝宝 param_1 类型 param_2 品质

		QINGYUAN_EQUIP_REQ_EQUIP2_UPLEVEL = 10,			// 请求升级情比金坚装备 param_1 装备部位
		QINGYUAN_EQUIP_REQ_BUY_GIFT,					// 购买礼包 param_1 礼包序号

		QINGYUAN_EQUIP_REQ_TYPE_MAX,
	};

	// 情缘全局操作 8392
	class CSQingyuanEquipOperate
	{
	public:
		CSQingyuanEquipOperate();
		MessageHeader header;

		int operate_type;
		int param_1;
		int param_2;
		int param_3;
	};

	// 情缘全局信息 8394
	class SCQingyuanEquipInfo
	{
	public:
		SCQingyuanEquipInfo();
		MessageHeader header;

		int is_self;    // 自己1 伴侣 0
		int marry_level;
		int marry_level_exp;
		unsigned char qingyuan_suit_flag[QINGYUAN_EQUIPSUIT_MAX_COUNT];		// 情缘套装标记
		char buy_gift_flag;													// 今日购买礼包标记
		char reserve;
		ItemDataWrapper qingyuan_equip_list[QINGYUAN_EQUIP_TYPE_MAX];		// 情缘装备信息
		QingyuanEquip2Item qingyuan_equip2_list[QINGYUAN_EQUIP_TYPE_MAX];	// 情比金坚装备信息
		QingyuanSpecialBaby special_baby_list[SPECIAL_BABY_TYPE_MAX];		// 特殊宝宝信息
	};

	//////////////////////////////// 约会 //////////////////////////////////////////
	class CSDatingInviteReq
	{
	public:
		CSDatingInviteReq();
		MessageHeader		header;

		int					target_uid;
		int					is_specific_invite;
	};

	class CSDatingInviteAck
	{
	public:
		CSDatingInviteAck();
		MessageHeader	header;

		int				req_uid;
		int				is_agree;
	};

	//////////////////////////////////// 世界BOSS /////////////////////////////////
	class CSGetWorldBossInfo					// 10151请求世界boss信息
	{
	public:
		CSGetWorldBossInfo();
		MessageHeader	header;

	};

	class SCWorldBossInfoToAll						// 10100 发送世界boss广播信息
	{
	public:
		SCWorldBossInfoToAll();
		MessageHeader header;

		int boss_id;
		int scene_id;
		int status;
		unsigned int next_refresh_time;
		int killer_uid;
	};

	class SCWorldBossInfoList						// 10101 下发世界boss信息
	{
	public:
		SCWorldBossInfoList();
		MessageHeader	header;

		unsigned int next_refresh_time;
		static const int MAX_BOSS_COUNT = 100;
		int count;

		struct BossInfo
		{
			int boss_id;
			int status;

			WorldBossManager::KillerInfo killer_info_list[WORLD_KILLER_LIST_MAX_COUNT];
		};

		BossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCWorldBossBorn						// 10102世界boss出生
	{
	public:
		SCWorldBossBorn();
		MessageHeader	header;
	};

	class CSWorldBossPersonalHurtInfoReq				// 10152世界boss个人伤害排名请求
	{
	public:
		CSWorldBossPersonalHurtInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossSendPersonalHurtInfo				// 10103返回世界boss个人伤害排名信息
	{
	public:
		SCWorldBossSendPersonalHurtInfo();
		MessageHeader header;

		long long my_hurt;
		int self_rank;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				uid = 0;
				memset(name, 0, sizeof(name));
				hurt = 0;
			}


			int uid;
			GameName name;
			long long hurt;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class CSWorldBossGuildHurtInfoReq					// 10153世界boss公会伤害排名请求
	{
	public:
		CSWorldBossGuildHurtInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossSendGuildHurtInfo					// 10104返回世界boss公会伤害排名信息
	{
	public:
		SCWorldBossSendGuildHurtInfo();
		MessageHeader header;

		long long my_guild_hurt;
		int my_guild_rank;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = 0;
				memset(guild_name, 0, sizeof(guild_name));
				guild_hurt = 0;
			}


			GuildID guild_id;
			GuildName guild_name;
			long long guild_hurt;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class CSWorldBossWeekRankInfoReq			// 10154世界boss击杀数量周榜排名请求
	{
	public:
		CSWorldBossWeekRankInfoReq();
		MessageHeader header;

	};

	class SCWorldBossWeekRankInfo				// 10105返回世界boss击杀数量周榜排名信息
	{
	public:
		SCWorldBossWeekRankInfo();
		MessageHeader header;

		short my_guild_kill_count;
		short my_guild_rank;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = 0;
				memset(guild_name, 0, sizeof(guild_name));
				guild_kill_count = 0;
			}


			GuildID guild_id;
			GuildName guild_name;
			int guild_kill_count;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};
	
	class SCWorldBossCanRoll				// 10106世界boss护盾被击破，通知玩家可摇点
	{
	public:
		SCWorldBossCanRoll();
		MessageHeader header;

		int boss_id;
		int hudun_index;					// 第一次护盾1，第二次护盾2
	};

	class CSWorldBossKillerInfoReq				// 10150 请求世界boss击杀列表
	{
	public:
		CSWorldBossKillerInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossKillerList						// 10109 发送boss击杀列表
	{
	public:
		SCWorldBossKillerList();
		MessageHeader header;

		WorldBossManager::KillerInfo killer_info_list[WORLD_KILLER_LIST_MAX_COUNT];
	};

	class CSWorldBossRollReq				// 10155玩家请求摇点
	{
	public:
		CSWorldBossRollReq();
		MessageHeader header;

		int boss_id;
		int hudun_index;
	};

	class SCWorldBossRollInfo				// 10107返回玩家摇点点数
	{
	public:
		SCWorldBossRollInfo();
		MessageHeader header;

		int roll_point;
		int hudun_index;
	};

	class SCWorldBossRollTopPointInfo     // 10108返回最高点信息
	{
	public:
		SCWorldBossRollTopPointInfo();
		MessageHeader header;

		int boss_id;
		int hudun_index;
		int top_roll_point;
		GameName top_roll_name;
	};

	class SCWorldBossHPInfo				 // 10111世界boss血量信息
	{
	public:
		SCWorldBossHPInfo();
		MessageHeader header;

		int boss_id;
		long long cur_hp;
		long long max_hp;
		int shield_hp;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCActivityFindInfo				// 6005 副本找回信息下发
	{
	public:
		SCActivityFindInfo();
		MessageHeader header;

		enum  
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_REQ_INFO,
			NOTIFY_REASON_FIND_REQ,
			NOTIFY_REASON_AUTO_ACTIVITY,
			NOTIFY_REASON_JOIN_ACTIVITY,
		};

		int activity_open_flag;				// 已开启标记
		int activity_reward_flag;			// 已奖励标记
		int activity_day_enter_flag;		// 已进入标记
		int notify_reason;					// 通知原因
	};

	//////////////////////////////////////////////////////////////////////////
	class CSSupplyBuyItem				// 2772补给购买
	{
	public:
		CSSupplyBuyItem();
		MessageHeader header;

		int supply_type;
		int index;

		char is_use_no_bind_gold;
		char reserver_1;
		short reserve_2;
	};

	class CSSupplySetRecoverRangePer	// 2773设置补给百分比
	{
	public:
		CSSupplySetRecoverRangePer();
		MessageHeader header;

		int supply_type;
		int recover_range_per;
	};

	class SCSupplyInfo					// 2726下发补给信息
	{
	public:
		SCSupplyInfo();
		MessageHeader header;

		struct SupplyItem
		{
			long long supply_left_value;
			int supply_range_per;
		};

		SupplyItem supply_info_list[SUPPLY_TYPE_MAX];
	};

	class CSFindRoleByName
	{
	public:
		CSFindRoleByName();
		MessageHeader header;

		GameName name;
		int msg_identify;			// 标记（仅客户端用）
	};

	class SCFindRoleByNameRet
	{
	public:
		SCFindRoleByNameRet();
		MessageHeader header;

		long long uuserverid;
		long long uuid;
		int role_id;
		GameName role_name;
		char is_online;
		char sex;
		char prof;
		char camp;
		int level;
		int msg_identify;			// 标记（仅客户端用）
		long long avatar_timestamp;
		char at_cross;
		char reserve;
		short reserve_s;
		int capability;
	};

	//////////////////////////////////////////////////////////////////////////

// 	class SCWingInfo		// 4703
// 	{
// 	public:
// 		SCWingInfo();
// 		MessageHeader header;
// 
// 		short select_jinhua_img;				// 选择的形象对应的进化值
// 		short jinhua;							// 进化值
// 		int jinhua_bless;						// 进化祝福值	
// 		short special_img_flag;					// 特殊形象标记
// 		char select_special_img;				// 选中的特殊形象
// 		char reserve_ch;	
// 		short grade;							// 阶级
// 		short use_grade;						// 使用阶级
// 		int grade_bless_val;					// 阶级祝福值
// 
// 		int shuxingdan_list[WingParam::SHUXINGDAN_MAX_TYPE];			// 属性丹
// 		char special_img_grade_list[WingParam::MAX_SPECIAL_IMG_COUNT];	// 特殊形象阶数
// 		short fumo_level_list[WingParam::MAX_WING_FUMO_TYPE];
// 	};
// 
// 	class SCWingGrowExpType		// 4705
// 	{
// 	public:
// 		SCWingGrowExpType();
// 		MessageHeader header;
// 
// 		enum WGET
// 		{
// 			WGET_NORMAL = 0,
// 			WGET_BAOJI = 1,
// 		};
// 
// 		short grow_type;
// 		short reserve;
// 		int increase_point;
// 	};
// 
// 	class CSWingPromoteJinghua	// 4757
// 	{
// 	public:
// 		CSWingPromoteJinghua();
// 		MessageHeader header;
// 
// 		short is_auto_buy;						// 1 自动购买，0 否
// 		short repeat_times;
// 	};
// 
// 	class CSWingUseImage		// 4758
// 	{
// 	public:
// 		CSWingUseImage();
// 		MessageHeader header;
// 
// 		short img_id;						// 形象id
// 		short is_special_img;				// 是否特殊形象
// 	};
// 
// 	class CSWingUpgrade
// 	{
// 	public:
// 		CSWingUpgrade();				
// 		MessageHeader header;
// 
// 		short repeat_times;
// 		short auto_buy;
// 	};
// 
// 	class CSWingUseGrade
// 	{
// 	public:
// 		CSWingUseGrade();				
// 		MessageHeader header;
// 
// 		int grade;
// 	};
// 
// 	class CSWingSpecialUpgrade
// 	{
// 	public:
// 		CSWingSpecialUpgrade();
// 		MessageHeader header;
// 
// 		int img_id;
// 	};
// 
// 	class CSWingFumoUplevel		// 4787
// 	{
// 	public:
// 		CSWingFumoUplevel();
// 		MessageHeader header;
// 
// 		int type;
// 		int is_auto_buy;
// 	};

	//////////////////////////////////////////////////////////////////////////
	class CSDiscountBuyGetInfo			// 4760 获得一折抢购信息
	{
	public:
		CSDiscountBuyGetInfo();
		MessageHeader header;
	};

	class CSDiscountBuyReqBuy			// 4761 一折抢购 购买
	{
	public:
		CSDiscountBuyReqBuy();
		MessageHeader header;

		short seq;
		short reserve_sh;
	};

	class SCDiscountBuyInfo				// 4706 一折抢购信息
	{
	public:
		SCDiscountBuyInfo();
		MessageHeader header;

		struct Phase
		{
			unsigned int close_timestamp;
			short buy_count_list[DISCOUNT_BUY_ITEM_PER_PHASE];
		};

		Phase phase_list[DISCOUNT_BUY_PHASE_MAX_COUNT];
	};

	//////////////////////////////////////////////////////////////////////////
	class CSZeroGiftOperate				// 4797零元礼包操作
	{
	public:
		CSZeroGiftOperate();
		MessageHeader header;

		enum ZERO_GIFT_OPERATE_TYPE
		{
			ZERO_GIFT_GET_INFO = 0,
			ZERO_GIFT_BUY,
			ZERO_GIFT_FETCH_REWARD_ITEM,     //参数1 seq   参数2 天数
			ZERO_GIFT_BUY_GOD_COSTUME,					//购买四阶神装
			ZERO_GIFT_FETCH_GOD_COSTUME_REWARD_ITEM,     //领取四阶神装奖励，参数1 seq   参数2 等级阶段
		};

		int operate_type;
		int param_1;
		int param_2;
	};

	class SCZeroGiftInfo				//  4796 零元礼包信息
	{
	public:
		SCZeroGiftInfo();
		MessageHeader header;

		struct Phase
		{
			int state;
			unsigned int timestamp;
			int reward_flag;
		};

		Phase phase_list[ZERO_GIFT_PHASE_MAX];
	};

	//////////////////////////////////////////////////////////////////////////
	class CSAdvanceNoitceOperate				// 4798功能预告操作
	{
	public:
		CSAdvanceNoitceOperate();
		MessageHeader header;

		enum ADVANCE_NOTICE_OPERATE_TYPE
		{
			ADVANCE_NOTICE_GET_INFO = 0,
			ADVANCE_NOTICE_FETCH_REWARD,

			ADVANCE_NOTICE_DAY_GET_INFO = 2,		// 天数预告
			ADVANCE_NOTICE_DAY_FETCH_REWARD,		// 领取奖励
		};

		int operate_type;
		int param_1;
	};

	class SCAdvanceNoticeInfo				//  4799 功能预告信息信息
	{
	public:
		SCAdvanceNoticeInfo();
		MessageHeader header;

		enum ADVANCE_NOTICE_TYPE
		{
			ADVANCE_NOTICE_TYPE_LEVEL,				// 等级
			ADVANCE_NOTICE_TYPE_DAY,				// 天数
		};

		int notice_type;							// 通知类型
		int last_fecth_id;							// 上次领取天数id
		unsigned char notice_day_fectch_flag_list[CommonDataParam::NOTICE_DAY_MAX_NUM]; // 天数预告领取标记
	};

	//////////////////////////////////////////////////////////////////////////
	class CSGoldTurntableOperateReq				// 8425 元宝转盘操作请求
	{
	public:
		CSGoldTurntableOperateReq();
		MessageHeader header;

		enum
		{
			INFO_REQ = 0,						// 奖池信息请求
			CHOU_JIANG = 1,						// 抽奖
			
		};

		int operate_type;
	};

	class SCGoldTurntableReward					// 8426 元宝转盘 奖品信息
	{
	public:
		SCGoldTurntableReward();
		MessageHeader header;

		int index;
	};

	class SCGoldTurntableInfo					//8427 元宝奖池数
	{
	public:
		SCGoldTurntableInfo();
		MessageHeader header;

		long long gold_num;
		int chou_jiang_times;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCMonthCardInfo
	{
	public:
		SCMonthCardInfo();
		MessageHeader header;

		unsigned int active_timestamp;
		int reserve_int;
		int next_reward_day_idx;
		int buy_times;
	};

	class CSMonthCardFetchDayReward 
	{
		CSMonthCardFetchDayReward();
		MessageHeader header;
	};

	static const int MAX_RANDOM_ROLE_LIST_NUM = 8;

	struct RoleInfo
	{
		int uid;
		GameName role_name;
		char avatar;
		char sex;
		char prof;
		char camp;
		int capability;
		int level;
		long long avatar_timestamp;
	};

	class SCRandomRoleListRet
	{
	public:
		SCRandomRoleListRet();
		MessageHeader header;

		int count;
		RoleInfo role_list[MAX_RANDOM_ROLE_LIST_NUM];
	};

	class CSGetRandomRoleList
	{
	public:
		CSGetRandomRoleList();
		MessageHeader header;
	};

	class CSTurnTableReq							// 5257请求转盘转动
	{
	public:
		CSTurnTableReq();
		MessageHeader header;

		char type;
		char is_roll;
		short reserve_sh;
	};

	class SCTurnTableInfo							// 5205转盘基本信息
	{
	public:
		SCTurnTableInfo();
		MessageHeader header;

		short lucky_point[TURNTABLE_TYPE_MAX_COUNT];
	};

	class SCTurnTableReward							// 5204转盘奖励下发
	{
	public:
		SCTurnTableReward();
		MessageHeader header;

		char type;
		char rewards_index;
		short reserve_sh;
	};

	class SCTurnTableMillionaireView				// 5201大富豪形象广播
	{
	public:
		SCTurnTableMillionaireView();
		MessageHeader header;

		ObjID obj_id;
		short is_millionaire;

	};

	//////////////////////////////////////////////////////////////////////////
	class SCWabaoInfo
	{
	public:
		SCWabaoInfo();
		MessageHeader header;

		int baozang_scene_id;
		int baozang_pos_x;
		int baozang_pos_y;
		int baotu_count;				// 宝图数目
		int wabao_reward_type;			// 奖励类型
		int wabao_reward_count;
		unsigned int shouhuzhe_time;
		short wabao_reward_list[WabaoBaozangRewardConfig::MAX_REWARD_LIMIT];
		int is_quick_complete;
	};

	class CSWabaoOperaReq
	{
	public:
		enum OPERA_TYPE
		{
			OPERA_TYPE_START = 0,
			OPERA_TYPE_DIG = 1,
			OPERA_TYPE_QUICK_COMPLETE = 2, 
			OPERA_TYPE_GET_INFO = 3,
			OPERA_TYPE_SHOUHUZHE_TIME = 4,

			OPERA_TYPE_MAX,
		};

		CSWabaoOperaReq();
		MessageHeader header;

		short opera_type;
		short is_killed;
	};

	//////////////////////////////////////////////////////////////////////////
	class CSJilianOpera
	{
	public:
		enum OPERA_TYPE
		{
			OPERA_TYPE_JILIAN,
			OPERA_TYPE_CONTINUE,

			OPERA_TYPE_MAX,
		};

		CSJilianOpera();
		MessageHeader header;

		short opera_type;
		short param;
	};

	class SCJilianInfo
	{
	public:
		SCJilianInfo();
		MessageHeader header;

		char jilian_type;
		char is_working;
		short progress;
	};

	class SCJianlianViewChange
	{
	public:
		SCJianlianViewChange();
		MessageHeader header;

		ObjID obj_id;
		char jilian_type;
		char reserve_ch;
	};

//////////////////////////////////////////////////////////////////////////////////

	class SCZhanShenDianFBInfo
	{
	public:
		SCZhanShenDianFBInfo();
		MessageHeader header;

		short pass_level;
		short yesterday_pass_level;
		unsigned int fetch_day_reward_timestamp;
	};

	class SCZhanShenDianFBResult
	{
	public:
		SCZhanShenDianFBResult();
		MessageHeader header;

		char is_pass;
		char reseve_ch;
		unsigned short time_cost;
	};

	class SCTiangongShilianFBResult
	{
	public:
		SCTiangongShilianFBResult();
		MessageHeader header;

		char is_pass;
		char reseve_ch;
		unsigned short time_cost;
	};

	class SCTiangongShilianFBInfo
	{
	public:
		SCTiangongShilianFBInfo();
		MessageHeader header;

		int pass_level;
	};

	class CSFBReqNextLevel
	{
	public:
		CSFBReqNextLevel();
		MessageHeader header;
	};

	class SCRollMoneyInfo
	{
	public:
		SCRollMoneyInfo();
		MessageHeader header;

		short fetch_gold_reward_times;
		short fetch_coin_reward_times;

		char day_roll_one_gold_times;
		char reserve_ch;
		short reserve_sh;

		char gold_roll_times;
		char gold_roll_num_list[ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT];
		

		char coin_roll_times;
		char coin_roll_num_list[ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT];
	};

	enum ROLL_MONEY_OPERA_TYPE
	{
		ROLL_MONEY_OPERA_TYPE_QUERY = 0,
		ROLL_MONEY_OPERA_TYPE_ROLL_GOLD = 1,
		ROLL_MONEY_OPERA_TYPE_ROLL_COIN = 2,
		ROLL_MONEY_OPERA_TYPE_FETCH_GOLD_REWARD = 3,
		ROLL_MONEY_OPERA_TYPE_FETCH_COIN_REWARD = 4,	

		ROLL_MONEY_OPERA_TYPE_MAX, 
	};

	class CSRollMoneyOperaReq
	{
	public:
		CSRollMoneyOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
	};

	class CSZhanshendianFetchDayReward
	{
	public:
		CSZhanshendianFetchDayReward();
		MessageHeader header;
	};

	class CSRoleGoalOperaReq
	{
	public:

		enum OPERA_TYPE
		{
			PERSONAL_GOAL_INFO_REQ = 0,
			FETCH_PERSONAL_GOAL_REWARD_REQ = 1,
			FETCH_BATTLE_FIELD_GOAL_REWARD_REQ = 2,
			FINISH_GOLE_REQ = 3,
			UPLEVEL_SKILL = 4,
		};

		CSRoleGoalOperaReq();
		MessageHeader header;

		int opera_type;
		int param;
	};

	class SCRoleGoalInfo
	{
	public:
		SCRoleGoalInfo();
		MessageHeader header;

		int cur_chapter;
		int old_chapter;
		int data_list[PERSONAL_GOAL_COND_MAX];

		int field_goal_can_fetch_flag;
		int field_goal_fetch_flag;
		short skill_level_list[FIELD_GOAL_SKILL_TYPE_MAX];
	};

	class SCRoleGoalChange
	{
	public:
		SCRoleGoalChange();
		MessageHeader header;

		int gold_id;
		int param;
	};

	class CSRedPaperCreateReq
	{
	public:
		CSRedPaperCreateReq();
		MessageHeader header;

		int type;
		int gold_num;
		int can_fetch_times;
	};

	class CSRedPaperFetchReq
	{
	public:
		CSRedPaperFetchReq();
		MessageHeader header;

		int red_paper_id;
	};

	class CSRedPaperQueryDetailReq
	{
	public:
		CSRedPaperQueryDetailReq();
		MessageHeader header;

		int red_paper_id;
	};

	class SCRedPaperRoleInfo
	{
	public:
		SCRedPaperRoleInfo();
		MessageHeader header;

		int day_guild_red_paper_consume_gold_num;
	};

	class SCRedPaperDetailInfo
	{
	public:
		SCRedPaperDetailInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_CREATE = 1,

			NOTIFY_REASON_MAX,
		};

		struct FetchUserInfo
		{
			int uid;
			int gold_num;
			long long avator_timestamp;
			GameName fetch_name;
		};

		int notify_reason;

		int id;
		int type;
		int total_gold_num;
		int fetch_gold_num;
		int can_fetch_times;
		unsigned int timeount_timestamp;

		int creater_uid;
		GameName creater_name;
		int creater_guild_id;
		long long creater_avator_timestamp;
		char creater_sex;
		char creater_prof;
		short reserve;
		int boss_id;

		int fetch_user_count;
		FetchUserInfo fetch_user_info_list[RED_PAPER_MAX_CAN_FETCH_TIMES];
	};

	class SCRedPaperFetchResult
	{
	public:
		SCRedPaperFetchResult();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_FETCH_SUCC = 0,
			NOTIFY_REASON_HAS_FETCH,
			NOTIFY_REASON_NO_FETCH_TIMES,
			NOTIFY_REASON_FAIL,

			NOTIFY_REASON_MAX,
		};

		int notify_reason;
		int fetch_gold;
		GameName creater_name;
		int red_paper_type;
		int red_paper_id;
	};

	class CSCreateCommandRedPaper				//4727
	{
	public:
		CSCreateCommandRedPaper();
		MessageHeader header;

		int msg_length;							//消息长度
		//后面紧跟消息内容
	};

	class CSFetchCommandRedPaper				//4728
	{
	public:
		CSFetchCommandRedPaper();
		MessageHeader header;

		int red_paper_id;						//红包id
	};

	class CSCommandRedPaperCheckInfo			//4729
	{
	public:
		CSCommandRedPaperCheckInfo();
		MessageHeader header;

		int red_paper_id;						//红包id
	};

	class SCCommandRedPaperSendInfo				//4730
	{
	public:
		SCCommandRedPaperSendInfo();
		MessageHeader header;

		int creater_uid;
		GameName creater_name;
		long long creater_avator_timestamp;
		int msg_len;							//消息长度
		char creater_sex;
		char creater_prof;
		short reserve;
		int red_paper_id;
		//后面紧跟消息
	};

	struct MarriageSeekingInfo
	{
		GameName			gamename;
		int					user_id;
		char				camp;
		char				sex;
		char				prof;
		char				is_online;
		short				level;
		short				reserve_sh;
		unsigned int		timestamp;
		MarriageSeekingNotice marriage_seeking_notice;
	};

	class SCMarriageSeekingInfo
	{
	public:
		SCMarriageSeekingInfo();							// 4715
		MessageHeader header;
		
		int count;
		MarriageSeekingInfo	marriage_seeking_list[MARRIAGE_SEEKING_MAX_COUNT];
	};

	class CSMarriageSeekingOpera
	{
	public:
		CSMarriageSeekingOpera();							// 4768
		MessageHeader header;

		short type;
		short reserve_sh;
		MarriageSeekingNotice marriage_seeking_notice;
	};

	class SCFakePrivateChat
	{
	public:
		SCFakePrivateChat();
		MessageHeader header;

		int	from_uid;
		UserName username;		
		
		char sex;
		char camp;
		char vip_level;
		char prof;
		short level;
		short reserve_ch;
		long long avatar_timestamp;
	};

	enum LINGYUSHUISHOU_OPERA_TYPE
	{
		LINGYUSHUISHOU_OPERA_TYPE_INVALID, 
		LINGYUSHUISHOU_OPERA_TYPE_FETCH_XMZ_REWARD,
		LINGYUSHUISHOU_OPERA_TYPE_FETCH_GCZ_REWARD,
		LINGYUSHUISHOU_OPERA_TYPE_MOBAI,

		LINGYUSHUISHOU_OPERA_TYPE_MAX,
	};

	class CSShizhuangUseReq
	{
	public:
		CSShizhuangUseReq();
		MessageHeader header;

		char shizhuang_type;	// 0-武器，1-时装
		char img_type;			// 0-普通形象，1-特殊形象
		short img_id;			// 形象ID
	};

	class CSShizhuangUpgradeReq
	{
	public:
		CSShizhuangUpgradeReq();
		MessageHeader header;
	
		short shizhuang_type;
		char is_auto_buy;								// 是否自动购买进阶物品
		char repeat_times;								// 当前次请求，连续进阶次数
	};

	class CSShizhuangSpecialImgUpgradeReq				// 请求升级时装特殊形象  4809
	{
	public:
		CSShizhuangSpecialImgUpgradeReq();
		MessageHeader header;

		short shizhuang_type;
		char special_img_id;							// 特殊形象ID
		char reserve_ch;
	};

	class SCShizhuangInfo
	{
	public:
		SCShizhuangInfo();
		MessageHeader header;
		
		struct ShizhuangItemMsg 
		{
			char use_img;					// 当前使用的形象ID
			char use_special_img;			// 当前使用的特殊形象ID（这个优先级高）
			char grade;						// 阶数
			char is_use_special_img;        // 是否使用特殊形象(幻化形象),0不使用,1使用
			int grade_bless;				// 进阶祝福值
			unsigned char active_flag[SHIZHUANG_MAX_ACTIVE_IMAGE_BYTE];
			unsigned char special_img_active_flag[SHIZHUANG_MAX_ACTIVE_SPECIAL_IMAGE_BYTE]; // 特殊形象激活标记
			short special_img_grade_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT]; // 特殊形象等级列表
			unsigned int valid_timestamp_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT];
			short shuxingdan_count;         // 使用属性丹数量(资质丹)
			short reserve;
			short skill_level_list[ShizhuangItem::SKILL_COUNT];              //技能等级
			short equip_level_list[ShizhuangItem::EQUIP_COUNT];              //装备等级
			int equip_skill_level;                                           //装备技能等级
			unsigned int clear_bless_value_time;                             //清空祝福值的时间
		};

		ShizhuangItemMsg item_list[SHIZHUANG_TYPE_MAX];
	};

	enum CARD_OPERATE_TYPE
	{
		CARD_OPERATE_TYPE_REQ = 0,
		CARD_OPERATE_TYPE_INLAY,
		CARD_OPERATE_TYPE_UPLEVEL,
		CARD_OPERATE_TYPE_KEY_UPLEVEL,
	};

	class CSCardOperate
	{
	public:
		CSCardOperate();
		MessageHeader header;
			
		int operate_type;

		int param1;
		int param2;
		int param3;
		int param4;
	};

	class SCCardInfo
	{
	public:
		SCCardInfo();
		MessageHeader header;

		struct CardItem 
		{
			char slot_color[SLOT_PER_CARD];
			short level;
			short reserve;
			int exp;
		};

		CardItem card_item_list[CARD_MAX_COUNT];
	};

	class SCCardUplevelSucc
	{
	public:
		SCCardUplevelSucc();
		MessageHeader header;
	};

	class CSDisconnectReq
	{
	public:
		CSDisconnectReq();
		MessageHeader header;
	};

	class CSZhuanzhiReq
	{
	public:
		CSZhuanzhiReq();
		MessageHeader header;
	};

	class SCMolongInfo      // 6104
	{
	public:
		SCMolongInfo();
		MessageHeader header;

		int accumulate_consume_gold;
		int today_consume_gold;
		short today_move_step;
		short total_move_step;
		int curr_loop;


		int rank_grade;
		int rank_cumulate_gold;
	};

	class SCOtherCapabilityInfo		// 2427
	{
	public:
		SCOtherCapabilityInfo();
		MessageHeader header;

		int active_flag;
	};

	class SCAllZhiBaoInfo								// 至宝所有信息
	{
	public:
		SCAllZhiBaoInfo();
		MessageHeader header;

		int exp;
		short level;
		short use_image;
		short reserve;
		short grade_max;
		short grade[MAX_ZHIBAO_HUANHUA_TPYE_COUNT];
		short revese;
	};

	class CSFriendExpBottleOP							// 好友经验瓶操作请求 1790
	{
	public:
		CSFriendExpBottleOP();
		MessageHeader header;

		int type;										// 0领取经验,1发送传闻
	};

	class SCFriendExpBottleAddExp						// 好友经验瓶增加累计经验 1791
	{
	public:
		SCFriendExpBottleAddExp();
		MessageHeader header;

		int next_broadcast_time;						// 下次广播时间
		int get_exp_count;								// 领取经验瓶数
		long long exp;									// 当前累计经验
		int auto_add_friend_count;						// 自动增加的好友数量
	};

	class CSSkipReq
	{
	public:
		CSSkipReq();
		MessageHeader header;

		enum SKIP_TYPE
		{
			SKIP_TYPE_CHALLENGE = 0,					//决斗场，附近的人
			SKIP_TYPE_SAILING,							//决斗场，航海
			SKIP_TYPE_MINE,								//决斗场，挖矿
			SKIP_TYPE_FISH,								//捕鱼
			SKIP_TYPE_JINGLING_ADVANTAGE,				//精灵奇遇
			SKIP_TYPE_SHENZHOU_WEAPON,					//上古遗迹
			SKIP_TYPE_XINGZUOYIJI,						//星座遗迹
			SKIP_TYPE_QYSD,								//情缘圣地
			SKIP_TYPE_PRECIOUS_BOSS,					//秘藏boss
			SKIP_TYPE_PAOHUAN_TASK,						//跑环任务
			SKIP_TYPE_CROSS_GUIDE_BATTLE,						//跨服争霸
		};

		int type;										//类型
		int param;										//品质,从0开始(没有品质需求的就随便填)
	};

	//神印协议
	class SCShenYinScoreInfo // 8444
	{
	public:
		SCShenYinScoreInfo();
		MessageHeader header;

		int notify_type;
		int add_score;

		int score;				// 可用印记币积分	
		int chouhun_score;		//神印抽魂积分
	};

	struct ShenYinGridParamMsg
	{
		int param1; // type 0 为背包索引  type 1 为印位
		short v_item_id;
		short item_num;
		short is_bind;
		short level;
		int grade;

		ShenYinPastureSpiritGridParam::AttrItem attr_param;
		ShenYinPastureSpiritGridParam::AttrItem new_attr_param;
	};


	class SCShenYinBagInfo			// 虚拟背包信息8445
	{
	public:
		enum TYPE
		{
			BAG,			// param1  为背包索引
			IMPRINT_SLOT,	// param1  为印位
		};

		SCShenYinBagInfo();
		MessageHeader header;


		char type;
		char count;
		short imprint_grade;
		ShenYinGridParamMsg grid_list[SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT];
	};

	class SCShenYinShopInfo	// 8446
	{
	public:
		struct Shop
		{
			char index;
			char buy_count;
			short reserve_sh;
			unsigned int timestamp;
		};

		SCShenYinShopInfo();
		MessageHeader header;

		int count;
		Shop shop_list[SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT];
	};

	class SCShenYinLieMingBagInfo		// 8447
	{
	public:
		SCShenYinLieMingBagInfo();
		MessageHeader header;

		int liehun_color;
		short liehun_pool[SHEN_YIN_LIEHUN_POOL_MAX_COUNT];
	};

	struct VirtualBagData
	{
		VirtualBagData() { this->Reset(); }

		void Reset()
		{
			virtual_bag_index = -1;
			num = 0;
		}

		int virtual_bag_index;
		int num;
	};

	class CSShenYinOneKeyRecyleReq
	{
	public:
		CSShenYinOneKeyRecyleReq();
		MessageHeader header;

		int count;
		short bag_index_list[SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT];
	};

	class SCUpgradeRushInfo
	{
	public:
		SCUpgradeRushInfo();
		MessageHeader header;

		unsigned int upgrade_return_end_time_list[UPGRADE_RUSH_MAX_TYPE_COUNT];
	};

	enum DROP_LOG_TYPE
	{
		DOPE_LOG_TYPE_BOSS =0,
		DOPE_LOG_TYPE_FB = 1,
	};
	class CSGetDropLog                 // 8521
	{
	public:
		CSGetDropLog();
		MessageHeader header;

		short type;
		short param;
	};

	class SCDropLogRet                 // 8522
	{
	public:
		SCDropLogRet();
		MessageHeader header;

		short type;
		short count;
		DropLogItem log_item[DROP_LOG_NUM_MAX];
	};


	enum ROLE_TALENT_OPERATE_TYPE
	{
		ROLE_TALENT_OPERATE_TYPE_INFO = 0,		// 请求天赋信息
		ROLE_TALENT_OPERATE_TYPE_UPLEVEL,		// 升级天赋
		ROLE_TALENT_OPERATE_TYPE_RESET,			// 重置天赋点
	};

	class CSRoleTelentOperate
	{
	public:
		CSRoleTelentOperate();
		MessageHeader header;

		int opera_type;
		int param_1;
	};

	class SCRoleTelentInfo
	{
	public:
		SCRoleTelentInfo();
		MessageHeader header;

		char talent_level_list[MAX_TELENT_TYPE_COUT][MAX_TELENT_INDEX_COUT];
		int talent_point;
	};


	static const int TALENT_SKILL_TARGET_MAX = 5;

	class SCTalentSkillTargetList						// 7252
	{
	public:
		SCTalentSkillTargetList();
		MessageHeader header;

		long long injure;
		int target_count;
		unsigned short deliver_obj_id;
		unsigned short target_objid_list[TALENT_SKILL_TARGET_MAX];
	};


	//////////////////////////////////////////////////////////////////////////////////
	struct SCWorldBossItemInfo
	{
		static const int HISTORY_KILLER_MAX_COUNT = 5;      

		int boss_id;
		unsigned int next_refresh_time;

		char is_concern;	// 是否关注了该BOSS
		char reserve_ch;
		short reserve_sh;
	};

	///////////////////////////////// 上古boss ///////////////////////////////////////
	static const int SC_SHANGGU_BOSS_LAYER_MAX_COUNT = 10;	// 上古BOSS - 每一层最大boss数量
	static const int SC_SHANGGU_BOSS_MAX_LAYER = 6;			// 上古BOSS - 最大层数
	enum SHANGGU_BOSS_OPERA_TYPE
	{
		SHANGGU_BOSS_OPERA_TYPE_ENTER = 0,				// 进入请求，param1 层数，param2 bossID
		SHANGGU_BOSS_OPERA_TYPE_ALL_BOSS_INFO = 1,		// 所有信息
		SHANGGU_BOSS_OPERA_TYPE_LAYER_BOSS_INFO = 2,	// 单层信息，param1 层数
		SHANGGU_BOSS_OPERA_TYPE_SET_CONCERN = 3,		// 设置关注, param1 层数 param2 BOSS id  param3 是否关注（1或0）
	};

	class CSShangGuBossReq    // 操作请求 8545
	{
	public:
		CSShangGuBossReq();
		MessageHeader header;

		short opera_type;
		short param1;
		short param2;
		short param3;
	};

	class SCShangGuBossAllInfo        // 所有信息 8546
	{
	public:
		SCShangGuBossAllInfo();
		MessageHeader header;

		struct LayerBossInfo
		{
			short reserve_sh;
			short boss_count;	// 当前层boss数
			SCWorldBossItemInfo boss_info_list[SC_SHANGGU_BOSS_LAYER_MAX_COUNT];
		};

		int tire_value;		// 疲劳值
		short enter_times;	// 已进入次数
		short layer_count;	// 层数
		LayerBossInfo layer_list[SC_SHANGGU_BOSS_MAX_LAYER];
	};

	class SCShangGuBossLayerInfo   // 单层信息 8547
	{
	public:
		SCShangGuBossLayerInfo();
		MessageHeader header;

		short cur_layer;
		short boss_count;	// 当前层boss数
		SCWorldBossItemInfo boss_info_list[SC_SHANGGU_BOSS_LAYER_MAX_COUNT];
	};

	class SCShangGuBossSceneInfo				// BOSS场景内信息 8548
	{
	public:
		SCShangGuBossSceneInfo();
		MessageHeader	header;

		short angry_value;				// 怒气值
		short reserve_sh;
		unsigned int kick_out_time;		// 踢出副本的时间戳
	};

	class SCShangGuBossSceneOtherInfo	// 场景内信息黄金怪、隐藏boss、大宝箱、小宝箱 8549
	{
	public:
		SCShangGuBossSceneOtherInfo();
		MessageHeader	header;

		struct LayerInfo
		{
			LayerInfo() : gold_monster_num(0), hide_boss_num(0), max_boss_num(0), min_boss_num(0) {}

			short gold_monster_num;
			short hide_boss_num;
			short max_boss_num;
			short min_boss_num;
		};

		int layer_count;	// 层数
		LayerInfo layer_info_list[SC_SHANGGU_BOSS_MAX_LAYER];
	};

	/////////////////////////////// BOSS图鉴 //////////////////////////////////////
	enum BOSS_CARD_OPERA_TYPE
	{
		BOSS_CARD_OPERA_TYPE_ALL_INFO = 0,       //所有信息
		BOSS_CARD_OPERA_TYPE_ACTIVE,			 // 激活 param1 序号seq
		BOSS_CARD_OPERA_TYPE_FETCH,              // 领取集齐礼包 param1 卡组type

		BOSS_CARD_OPERA_TYPE_MAX,
	};

	class CSBossCardReq			// 8775
	{
	public:
		CSBossCardReq();
		MessageHeader header;

		short req_type;
		short param1;
		short param2;
	};

	class SCBossCardAllInfo			// 8776 
	{
	public:
		SCBossCardAllInfo();
		MessageHeader header;

		unsigned long long card_group_reward_fetch_flag;                              // 集齐奖励领取标记
		unsigned char card_can_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// 可激活标记
		unsigned char card_has_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// 已激活标记
	};

	class SCReliveTire				          //  复活疲劳   5740
	{
	public:
		SCReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// 复活次数
		unsigned int tire_buff_end_time;		// 复活疲劳BUFF结束时间
		unsigned int tire_can_relive_time;		// 下次允许复活时间
	};

	class CSGetLuckyLog                 // 8620
	{
	public:
		CSGetLuckyLog();
		MessageHeader header;

		int activity_type;
	};

	class SCLuckyLogRet                 // 8621
	{
	public:
		SCLuckyLogRet();
		MessageHeader header;

		int activity_type;
		int count;
		LuckyLogItem log_item[LOG_NUM_MAX];
	};

	////////////////////////// 角色大小目标 //////////////////////////
	enum ROLE_BIG_SMALL_GOAL_OPERA_TYPE
	{
		ROLE_BIG_SMALL_GOAL_OPERA_INFO = 0,      // 请求信息   param_1 系统类型
		ROLE_BIG_SMALL_GOAL_OPERA_FETCH,         // 领取       param_1 系统类型  param_2 是否使用元宝
	};

	class CSRoleBigSmallGoalOper            // 8821
	{
	public:
		CSRoleBigSmallGoalOper();
		MessageHeader header;

		int oper_type;                    // 操作类型
		int param_1;
		int param_2;
	};

	class SCRoleBigSmallGoalInfo           // 8822
	{
	public:
		SCRoleBigSmallGoalInfo();
		MessageHeader header;

		struct GoalInfoItem
		{
			GoalInfoItem() : active_flag(0), fetch_flag(0){}
			char active_flag;                                               // 激活标记
			char fetch_flag;                                                // 领取标记
		};

		short system_type;                                                   // 系统类型
		char res_ch;
		char active_special_attr_flag;                                       // 激活特殊属性标记
		unsigned int open_system_timestamp;                                  // 系统开启的时间戳
		GoalInfoItem goal_item_info[ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX];    // 2   0 小目标  1 大目标
	};

	//-------------------------------------------------------------------------------------------

	// 红装收集           8766
	class SCRedEquipCollect
	{
	public:
		SCRedEquipCollect();
		MessageHeader header;

		int seq;
		ItemDataWrapper equip_slot[RED_EQUIP_COLLECT_SLOT_COUNT];
	};

	// 红装收集-其他信息        8767
	class SCRedEquipCollectOther
	{
	public:
		SCRedEquipCollectOther();
		MessageHeader header;

		UInt32 seq_active_flag;					// 套装激活标记（已激活才可穿戴）
		int collect_count;						// 已集齐的套装数
		UInt32 act_reward_can_fetch_flag;		// 开服活动可领取标记
		UInt32 active_reward_fetch_flag;		// 激活奖励领取标记（称号领取标记）
		RedEquipCollectInfo collect_info_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT];		// 套装收集个数
	};

	//-------------------------------------------------------------------------------------------

	// 橙装收集               8768
	class SCOrangeEquipCollect
	{
	public:
		SCOrangeEquipCollect();
		MessageHeader header;

		int seq;
		ItemDataWrapper equip_slot[ORANGE_EQUIP_COLLECT_SLOT_COUNT];
	};

	// 橙装收集-其他信息            8769
	class SCOrangeEquipCollectOther
	{
	public:
		SCOrangeEquipCollectOther();
		MessageHeader header;

		UInt32 seq_active_flag;					// 套装激活标记（已激活才可穿戴）
		int collect_count;						// 已集齐的套装数
		UInt32 act_reward_can_fetch_flag;		// 开服活动可领取标记
		UInt32 active_reward_fetch_flag;		// 激活奖励领取标记（称号领取标记）
		OrangeEquipCollectInfo collect_info_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT];		// 套装收集个数
	};

	//---------------------------仙尊卡--------------------------------------------------------
	enum XIANZUNKA_OPERA_REQ_TYPE
	{
		XIANZUNKA_OPERA_REQ_TYPE_ALL_INFO = 0,				// 请求所有信息
		XIANZUNKA_OPERA_REQ_TYPE_BUY_CARD,					// 购买仙尊卡 param_1 : 仙尊卡类型
		XIANZUNKA_OPERA_REQ_TYPE_FETCH_DAILY_REWARD,		// 拿取每日奖励 param_1 :仙尊卡类型
		XIANZUNKA_OPERA_REQ_TYPE_ACTIVE,					// 激活永久仙尊 param_1:仙尊卡类型

		XIANZUNKA_OPERA_REQ_TYPE_MAX
	};

	struct CSXianZunKaOperaReq			// 8839
	{
		CSXianZunKaOperaReq();
		MessageHeader header;

		short opera_req_type;
		short param_1;
		int param_2;
	};

	struct SCXianZunKaAllInfo		//8840
	{
		SCXianZunKaAllInfo();
		MessageHeader header;

		short forever_active_flag;						// 永久激活位标记	
		char first_active_reward_flag;					// 首次激活奖励标记
		char daily_reward_fetch_flag;					// 每日奖励拿取标记

		unsigned int temporary_valid_end_timestamp_list[XIANZUNKA_TYPE_MAX];		// 临时激活结束时间戳
	};

	///////////////////// 装备洗炼 ////////////////////////////////
	enum EQUIP_BAPTIZE_OPERA_TYPE
	{
		EQUIP_BAPTIZE_OPERA_TYPE_ALL_INFO = 0,		// 所有信息
		EQUIP_BAPTIZE_OPERA_TYPE_OPEN_SLOT,			// 开启槽          param_1 装备部位   param_2  洗炼槽索引
		EQUIP_BAPTIZE_OPERA_TYPE_LOCK_OR_UNLOCK,	// 加锁 or 去锁    param_1 装备部位   param_2  洗炼槽索引
		EQUIP_BAPTIZE_OPERA_TYPE_BEGIN_BAPTIZE,		// 开始洗炼        param_1 洗练位置   param_2  是否使用元宝   param_3  特殊洗练石

		EQUIP_BAPTIZE_OPERA_TYPE_MAX,
	};

	class CSEquipBaptizeOperaReq            // 8847
	{
	public:
		CSEquipBaptizeOperaReq();
		MessageHeader header;

		int opera_type;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCEquipBaptizeAllInfo             // 8848
	{
	public:
		SCEquipBaptizeAllInfo();
		MessageHeader header;

		EquipBaptizePartInfo part_info_list[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];

		unsigned char open_flag[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];
		unsigned char lock_flag[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];
		unsigned char used_free_times;
		unsigned char reserve_ch;
		unsigned short reserve_sh;
	};

	class SCMiZangBossReliveTire				          //  秘藏复活疲劳   8860
	{
	public:
		SCMiZangBossReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// 复活次数
		unsigned int tire_buff_end_time;		// 复活疲劳BUFF结束时间
		unsigned int tire_can_relive_time;		// 下次允许复活时间
	};

	class SCYouMingBossReliveTire				          //  幽冥复活疲劳 8855
	{
	public:
		SCYouMingBossReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// 复活次数
		unsigned int tire_buff_end_time;		// 复活疲劳BUFF结束时间
		unsigned int tire_can_relive_time;		// 下次允许复活时间
	};

	class SCUpgradeCardBuyInfo										// 8878 购买信息
	{
	public:
		SCUpgradeCardBuyInfo();
		MessageHeader header;

		short activity_id;
		short grade;												// 当前阶数
		short is_already_buy;	                                    // 0 没有购买  1已经购买
		short reserve_sh;
	};

	class CSUpgradeCardBuyReq										// 8879 购买请求
	{
	public:
		CSUpgradeCardBuyReq();
		MessageHeader header;

		short activity_id;
		ItemID item_id;												// unsigned short
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	class SCHuguozhiliInfo											// 8610  天神护体 - 信息
	{
	public:
		SCHuguozhiliInfo();
		MessageHeader header;

		HuguozhiliParam info;
	};

	class CSHuguozhiliReq											// 8611  天神护体 - 请求
	{
	public:
		enum
		{
			REQ_INFO,				// 请求信息
			REQ_ACTIVE_HUGUOZHILI,	// 请求激活护国之力
		};

		CSHuguozhiliReq();
		MessageHeader header;

		int req_type;
	};

	class CSTodayThemeRewardReq			//8916
	{
	public:
		CSTodayThemeRewardReq();
		MessageHeader header;

		int seq;				//领取哪个奖励，读配表
	};

	class SCTodayThemeRewardFlagInfo	//8917
	{
	public:
		SCTodayThemeRewardFlagInfo();
		MessageHeader header;

		unsigned char fetch_flag[TODAY_THEME_REWARD_MAX_BYTE];	//领取标记
	};

	//////////////////////////////////////////////////////////////////////////
	class SCWorldAcquisitionLog										// 8612  下发全服收购记录
	{
	public:
		SCWorldAcquisitionLog();
		MessageHeader header;

		enum NOTICE_TYPE
		{
			NOTICE_TYPE_ADD = 0, // 添加一条
			NOTICE_TYPE_ALL = 1, // 下发所有
		};

		struct LogItem
		{
			LogItem() { this->Reset(); }

			void Reset()
			{
				log_type = 0;
				log_str_id = 0;
				item_id = 0;
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));

				timestamp = 0;
				memset(role_name, 0, sizeof(role_name));
				uuid = 0;
			}

			char log_type;			// 类型
			char log_str_id;		// 日志字符串ID
			ItemID item_id;			// 物品ID
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // 仙品属性类型

			unsigned int timestamp;	// 记录时间戳
			GameName	role_name;	// 角色
			long long uuid;
		};

		short notice_type;		// 通知类型，枚举NOTICE_TYPE
		short count;
		LogItem log_List[MAX_SERVER_ACQUISITION_RECORD_NUM];  // 最多100条
	};

	class CSWorldAcquisitionLogReq									// 8613  请求添加收购记录
	{
	public:
		CSWorldAcquisitionLogReq();
		MessageHeader header;

		char log_type;			// 类型
		char log_str_id;		// 日志字符串ID
		ItemID item_id;			// 物品ID
		unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // 仙品属性类型
	};

	class CSCrossGoalOperaReq	 //8625
	{
	public:
		enum OPERA_TYPE
		{
			CROSS_GOAL_INFO_REQ = 0,		//获取信息
			FETCH_CROSS_GOAL_REWARD_REQ = 1,//获取奖励
			FETCH_GUILD_GOAL_REWAED_REQ = 2,//获取公会目标奖励
		};

		CSCrossGoalOperaReq();
		MessageHeader header;

		int opera_type;
		int param;
	};

	class SCCrossGoalInfo //8626
	{
	public:
		SCCrossGoalInfo();
		MessageHeader header;

		char fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // 任务状态标记
		unsigned short kill_cross_boss_num;		//击杀神域boss
		unsigned short cross_boss_role_killer;	//神域boss场景杀人数
		unsigned short kill_baizhan_boss_num;		//击杀百战boss数
		unsigned short finish_baizhan_task_num;		//完成百战boss数
		char guild_fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // 公会任务状态标记 0 未完成 1 可领取 2 已领取
		unsigned short guild_kill_cross_boss;
		unsigned short guild_kill_baizhan_boss;
		int reserve;
	};

	class SCCrossGoalGuildNotify //8627
	{
	public:
		SCCrossGoalGuildNotify();
		MessageHeader header;

		int flag;
		unsigned short guild_kill_cross_boss;
		unsigned short guild_kill_baizhan_boss;
	};

	class SCZeroGiftGodCostumeInfo				//  8922 零元礼包四阶神装信息
	{
	public:
		SCZeroGiftGodCostumeInfo();
		MessageHeader header;

		ZeroGiftGodCostumeInfo zero_gift_god_costume_info[ZERO_GIFT_GOD_COSTUME_MAX];
	};

	class SCSceneActivityRewardInfo				//  8925 活动奖励展示
	{
	public:

		enum REWARD_TYPE
		{
			REWARD_TYPE_SHIZHUANG = 0,		//时装
			REWARD_TYPE_WEAPON = 1,			//武器
			REWARD_TYPE_MOUNT = 2,			//坐骑
			REWARD_TYPE_TITLE = 3,		    //称号
			REWARD_TYPE_WING = 4,		    //羽翼
		};

		enum REWARD_SOURCE_ID				//仅用于客户端识别非活动号奖励来源
		{
			REWARD_SOURCE_ID_CROSS_ADD_CAP = 10000,					//跨服增战榜
			REWARD_SOURCE_ID_CROSS_ADD_CHARM = 10001,				//跨服增魅榜
			REWARD_SOURCE_ID_CROSS_QINGYUAN_CAP = 10002,			//跨服情缘榜
			REWARD_SOURCE_ID_CROSS_GUILD_KILL_BOSSP = 10003,		//跨服公会击杀榜
			REWARD_SOURCE_ID_CHALLENGEFIELD = 10004,				//开服竞技场
		};

		SCSceneActivityRewardInfo();
		MessageHeader header;

		int activity_id;
		int reward_type;
		int reward_id;
		int param;
	};

	class SCEquipUplevelSuitInfo				//  8924 装备等级套装
	{
	public:

		SCEquipUplevelSuitInfo();
		MessageHeader header;

		int suit_level;
		int suit_active_flag;
	};

	class CSEquipUplevelSuitActive				//  8937 装备等级套装
	{
	public:

		CSEquipUplevelSuitActive();
		MessageHeader header;

		int active_suit_level;
	};

	/////////////////////////////////////// 百战装备 ///////////////////////////////////////////////////
	// 百战装备操作类型
	enum BAIZHAN_EQUIP_OPERATE_TYPE
	{
		BAIZHAN_EQUIP_OPERATE_TYPE_ALL_INFO = 0,		// 请求装备所有信息

		BAIZHAN_EQUIP_OPERATE_TYPE_TAKE_OFF,			// 脱下 p1: part_index

		BAIZHAN_EQUIP_OPERATE_TYPE_UP_LEVEL,			// 升级装备	p1: part_index p2:item_id
	};

	class CSBaizhanEquipOpera		// 8926
	{
	public:
		CSBaizhanEquipOpera();
		MessageHeader header;

		short operate;                                  // 操作类型（0 = 请求信息，1 = 脱下...）
		short param1;
		int param2;
	};

	class SCBaizhanEquipAllInfo			// 8927 百战装备信息
	{
	public:
		SCBaizhanEquipAllInfo();
		MessageHeader header;

		ItemDataWrapper baizhan_equip_list[E_INDEX_BAIZHAN_MAX];

		char part_order_list[E_INDEX_BAIZHAN_MAX];			// 阶数列表
		short reserve_sh;
	};


	enum LITTLE_HELPER_COMPLETE_TYPE	//CSLittleHelperOpera::type
	{
		LITTLE_HELPER_COMPLETE_TYPE_ALL = -1,
		LITTLE_HELPER_COMPLETE_TYPE_EASY_BOSS = 0,					//简单boss
		LITTLE_HELPER_COMPLETE_TYPE_DIFFICULT_BOSS = 1,				//困难boss
		LITTLE_HELPER_COMPLETE_TYPE_BABY_BOSS = 2,					//宝宝boss
		LITTLE_HELPER_COMPLETE_TYPE_SUIT_BOSS = 3,					//套装boss
		LITTLE_HELPER_COMPLETE_TYPE_DEMON_BOSS = 4,					//神魔boss
		LITTLE_HELPER_COMPLETE_TYPE_PET_ADVENTURE = 5,				//宠物奇遇
		LITTLE_HELPER_COMPLETE_TYPE_ESCORT_FAIRY = 6,				//护送仙女
		LITTLE_HELPER_COMPLETE_TYPE_EXP_FB = 7,						//经验副本 p0: 重复次数 p1: 经验副本的类型
		LITTLE_HELPER_COMPLETE_TYPE_TOWER_DEFENSE_FB = 8,			//塔防副本 p0: 重复次数 p1: 召唤怪物数量
		LITTLE_HELPER_COMPLETE_TYPE_CYCLE_TASK = 9,					//跑环任务
		LITTLE_HELPER_COMPLETE_TYPE_EXP_TASK = 10,					//经验任务
		LITTLE_HELPER_COMPLETE_TYPE_GUILD_TASK = 11,				//仙盟任务

		LITTLE_HELPER_COMPLETE_TYPE_MAX
	};

	class CSLittleHelperOpera		// 8932
	{
	public:
		CSLittleHelperOpera();
		MessageHeader header;

		struct Req
		{
			Req() : type(0), param_0(0), param_1(0), param_2(0) {}

			short type;									 // 完成类型
			short param_0;
			short param_1;
			short param_2;
		};
		Req req;
	};

	class CSLittleHelperRepeatOpera		// 8958
	{
	public:
		CSLittleHelperRepeatOpera();
		MessageHeader header;

		int count;										// 请求次数
		CSLittleHelperOpera::Req req_list[Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX];		// 请求
	};

	class SCLittleHelperInfo
	{
	public:
		SCLittleHelperInfo();
		MessageHeader header;

		short type;									// 通知客户端这个类型需要刷新
		short reverse_sh;
	};

	class CSGetGuildRareLog                 // 8930  请求同步日志
	{
	public:
		CSGetGuildRareLog();
		MessageHeader header;
	};

	class SCGuildRareLogRet                 // 8931  珍稀日志信息
	{
	public:
		SCGuildRareLogRet();
		MessageHeader header;

		int count;
		GuildRareLogItem log_item[GUILD_RARE_LOG_MAX_NUM];
	};

	class SCTianShuXZInfo	// 8935
	{
	public:
		SCTianShuXZInfo();
		MessageHeader header;

		static const int TIANSHU_XUNZHU_MAX_TYPE_COUNT = 8;

		short equip_level_50;
		short equip_level_100;
		long long zhuanzhi_equip_fangyu;									//转职装防御
		unsigned int fetch_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];		// 目标领取标记列表
		unsigned int act_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];			// 目标激活标记列表
		short baizhan_equip_num;
		short reserve_sh;
	};

	enum CS_TIANSHUXZ_SEQ_TYPE
	{
		CS_TIANSHUXZ_SEQ_TYPE_INFO = 0,	//获取信息
		CS_TIANSHUXZ_SEQ_TYPE_FETCH = 1,//领取奖励
	};

	class CSTianShuXZFetchReward	// 8936
	{
	public:
		CSTianShuXZFetchReward();
		MessageHeader header;

		int type;				// 领取类型
		short tianshu_type;		// 天数类型
		short seq;				// 天书领取索引
	};

	class SCLittleHelperItemInfo
	{
	public:
		SCLittleHelperItemInfo();
		MessageHeader		header;


		struct RewardItem
		{
			RewardItem() : item_id(0), is_bind(0), num(0)
			{
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
			}

			ItemID item_id;
			short is_bind;
			int num;
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// 仙品属性类型
		};

		int count;
		RewardItem	item_list[lhns::MAX_DROP_ITEM_COUNT];
	};

}

#pragma pack(pop)

#endif

