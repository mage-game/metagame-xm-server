
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
			CLIENT_OPERATE_TYPE_GUILD_GUIDE,		// ????????
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
			SUCC_REASON_EQUIP_STRENGTH = 1,										// ????????
		};

		short				reason;												// ????
		short				param;												// ????????
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
			OPEN_TYPE_PET_SKILL_PANEL = 0,										// ????????????
			OPEN_TYPE_CAPABILITY_PANEL = 1,										// ??????????
		};

		int open_type;
	};

	/////////////////////////////////// ???????? ///////////////////////////////////////
	class SCTouZiJiHuaInfo				// 2720 ????????????
	{
	public:
		SCTouZiJiHuaInfo();
		MessageHeader		header;

		char active_plan_0;				// ????????1????????
		char active_plan_1;				// ????????2????????
		char active_plan_2;				// ????????3????????
		char new_plan_first_reward_flag;	// ????????????????

		short plan_0_reward_flag;		// ????????1????????
		short plan_1_reward_flag;		// ????????2????????
		short plan_2_reward_flag;		// ????????3????????
		char active_highest_plan;		// ????????????????????????
		char reserve_sh;

		char reserve;
		char active_plan_3;				// ????????4????????
		short plan_3_reward_flag;		// ????????4????????

		unsigned int buy_time;			// ????????????????
		int reward_flag;
		int vip_reward_flag;
		int reward_gold_bind_flag;		// ????????????????

		int list_len;
		char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE];	//????????????????????,0??????????

		UNSTD_STATIC_CHECK(TouzijihuaConfig::MAX_FOUNDATION_TYPE % 4 == 0)
	};

	class CSFetchTouZiJiHuaReward		// 2780 ????????????????????
	{
	public:
		CSFetchTouZiJiHuaReward();
		MessageHeader		header;

		short plan_type;
		short seq;
	};

	class CSTouzijihuaActive			// 2786 ????????????????
	{
	public:
		CSTouzijihuaActive();
		MessageHeader		header;

		short plan_type;
		short reserve_sh;
	};

	enum NEW_TOUZIJIHUA_OPERATE_TYPE
	{
		NEW_TOUZIJIHUA_OPERATE_BUY = 0,		// ????????????
		NEW_TOUZIJIHUA_OPERATE_FETCH,		// ????????????
		NEW_TOUZIJIHUA_OPERATE_FIRST,		// ????????????
		NEW_TOUZIJIHUA_OPERATE_VIP_FETCH,
		NEW_TOUZIJIHUA_OPERATE_FOUNDATION_FETCH,	//????????????
	};
	class CSNewTouzijihuaOperate		// 2787 ??????????????????
	{
	public:
		CSNewTouzijihuaOperate();
		MessageHeader		header;

		short operate_type;
		short param;
	};

	enum TOUZIJIHUA_FB_BOSS_OPERATE_TYPE
	{
		TOUZIJIHUA_FB_BOSS_OPERATE_FB_BUY = 0,			// ????????????????
		TOUZIJIHUA_FB_BOSS_OPERATE_FB_REWARD =1,		// ??????????????????????param??index

		TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_BUY =2,			// ????boss????????
		TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_REWARD =3,		// ????boss??????????param??index

		TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_BUY = 4,			// ????????boss????????
		TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_REWARD = 5,		// ????????boss??????????param??index
	};

	class CSTouzijihuaFbBossOperate			// 2731 ??????boss????????????
	{
	public:
		CSTouzijihuaFbBossOperate();
		MessageHeader	header;

		short operate_type;
		short param;
	};

	class SCTouzijihuaFbBossInfo		  // 2732 ??????boss????????????
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

	///////////////////////////////// ???????? /////////////////////////////////////////

	//-------------------------------- ???? -----------------------------------------
	class CSMarryXunyouOpera  // 2793 ????????
	{
	public:
		CSMarryXunyouOpera();
		MessageHeader		header;

		int					opera_type;
	};

	class CSResetLoverName	// 2796 ????????????
	{
	public:
		CSResetLoverName();
		MessageHeader		header;

		GameName			new_lover_name;
	};

	class SCMarryXunyouFirework	 // 6006 ????????(??????)
	{
	public:
		SCMarryXunyouFirework();
		MessageHeader		header;

		ObjID				obj_id;
		short				reserved;
	};

	class SCMarryReqRoute	// 6004 ????????????????
	{
	public:
		SCMarryReqRoute();
		MessageHeader		header;

		int					hunli_type;
		int					req_uid;
		GameName			req_name;
	};

	class SCMarrySpecialEffect	// 6003????????
	{
	public:
		SCMarrySpecialEffect();
		MessageHeader		header;

		int					marry_type;
	};

	enum MARRY_REQ_TYPE
	{
		MARRY_REQ_TYPE_PROPOSE = 0,		// ???? p1:???????? p2: ????uid
		MARRY_CHOSE_SHICI_REQ,			// ?????? p1:????????
		MARRY_PRESS_FINGER_REQ,         // ??????
	};

	class CSMarryReq	// 2778 ????????
	{
	public:
		CSMarryReq();
		MessageHeader		header;

		int				ope_type;
		int				param_1;
		int				param_2;
	};

	class CSMarryRet	// 2779 ????????????
	{
	public:
		CSMarryRet();
		MessageHeader		header;

		int					marry_type;
		int					req_uid;
		int					is_accept;
	};

	class SCIsAcceptMarry	// 6616 ????????????
	{
	public:
		SCIsAcceptMarry();
		MessageHeader		header;

		int is_accept;		// ????????????  1??0
	};

	enum MARRY_RET_TYPE
	{
		MARRY_AGGRE = 0,			// ??????????
		MARRY_CHOSE_SHICI,			// ??????
		MARRY_PRESS_FINGER,         // ??????
		MARRY_CANCEL,				// ????????
	};

	class SCMarryRetInfo			// 2864 ????????????  
	{
	public:
		SCMarryRetInfo();
		MessageHeader header;

		int ret_type;				// ??????????MARRY_RET_TYPE??
		int ret_val;				// ?????? 0????????role_id??1?????????? 2??????????????????1??????????????2??????????????
	};

	class SCMarryInformation	// 6604 ????????
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

		int is_forced_divorce;  // 1??????????, 0????????????
	};

	class SCDivorceReqRoute	// 6005 ????????????
	{
	public:
		SCDivorceReqRoute();
		MessageHeader		header;

		int					req_uid;
		GameName			req_name;
	};

	class CSDivorceRet	// 6603 ????????
	{
	public:
		CSDivorceRet();
		MessageHeader		header;

		int					req_uid;
		int					is_accept;
	};

	//-------------------------------- ???????????? ----------------------
	enum QINGYUAN_OPERA_TYPE
	{
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE = 0,		// ???????? param1 ???????? param2 ????????
		QINGYUAN_OPERA_TYPE_WEDDING_INVITE_GUEST,	// ???????? param1 ????uid
		QINGYUAN_OPERA_TYPE_WEDDING_REMOVE_GUEST,	// ???????? param1 ????uid
		QINGYUAN_OPERA_TYPE_WEDDING_BUY_GUEST_NUM,	// ????????????
		QINGYUAN_OPERA_TYPE_WEDDING_GET_YUYUE_INFO,	// ????????????
		QINGYUAN_OPERA_TYPE_WEDDING_GET_ROLE_INFO,	// ????????????
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_FLAG,		// ????????????????
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_RESULT,	// ???????????????????? param1 seq param2 ????????
		QINGYUAN_OPERA_TYPE_LOVER_INFO_REQ,			// ????????????
		QINGYUAN_OPERA_TYPE_XUNYOU_ROLE_INFO,		// ????????????????
		QINGYUAN_OPERA_TYPE_XUNYOU_SA_HONGBAO,		// ?????????? param1:is_buy
		QINGYUAN_OPERA_TYPE_XUNYOU_GIVE_FLOWER,		// ????????????????  
		QINGYUAN_OPERA_TYPE_XUNYOU_OBJ_POS,			// ???????????????????? param1:seq
		QINGYUAN_OPERA_TYPE_LOVER_TITLE_INFO,		// ???????????? ??????????
		QINGYUAN_OPERA_TYPE_FETCH_LOVER_TITLE,		// ???????????? param1 index ??????????
		QINGYUAN_OPERA_TYPE_BUY_AND_PUTON_EQUIP,	// ?????????????? param1 index ??????????

		QINGYUAN_OPERA_TYPE_MAX,
	};

	class CSQingYuanOperaReq		// 2795 ????????????
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
		QINGYUAN_INFO_TYPE_WEDDING_YUYUE = 0,	// ????????
		QINGYUAN_INFO_TYPE_WEDDING_STANDBY,		// ????????
		QINGYUAN_INFO_TYPE_GET_BLESSING,		// ???????? param_ch1 ???????? param2 ????
		QINGYUAN_INFO_TYPE_BAITANG_RET,			// ????????
		QINGYUAN_INFO_TYPE_BAITANG_EFFECT,		// ???????? param_ch1 ????????????
		QINGYUAN_INFO_TYPE_LIVENESS_ADD,		// ???????????? param2 ????????
		QINGYUAN_INFO_TYPE_HAVE_APPLICANT,		// ?????????? param2 ??????uid 
		QINGYUAN_INFO_TYPE_APPLY_RESULT,		// ???????? param2 1:???? 0:????
		QINGYUAN_INFO_TYPE_ROLE_INFO,			// ???????? param_ch1 ???????? param_ch2 ?????????????? param_ch3 ???????????? param_ch4 ????????seq param_ch5 ????????????
		QINGYUAN_INFO_TYPE_WEDDING_YUYUE_FLAG,	// ???????????? param6
		QINGYUAN_INFO_TYPE_YUYUE_RET,			// ???????????? param_ch1 seq
		QINGYUAN_INFO_TYPE_YUYUE_POPUP,			// ????????????
		QINGYUAN_OPERA_TYPE_BUY_QINGYUAN_FB_RET,// ????????????????
		QINGYUAN_INFO_TYPE_YUYUE_SUCC,			// ????????????
		QINGYUAN_INFO_TYPE_LOVER_INFO,			// ???????? param5 ????uid param_ch1 ???????? role_name ????????
		QINGYUAN_INFO_TYPE_LOVER_TITLE_INFO,	// ???????????? param2 ????flag ??????????
		QINGYUAN_INFO_TYPE_WEDDING_BEGIN_NOTICE,// ????????????
		QINGYUAN_INFO_TYPE_XUNYOU_INFO,			// ???????????? p_c1:?????????? p_c2:?????????????? p_c3:???????? p_c4:???????????? p_c5:???????????????? p_ch6:????????
		QINGYUAN_INFO_TYPE_XUNYOU_OBJ_POS,		// ???????????????? p_c1:?????????????? p2:x p3:y p4:obj_id p5:scene_id
	};

	class SCQingYuanAllInfo  // 2784 ????????????
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

	class SCQingYuanWeddingAllInfo	//  6007 ????????????????????
	{
	public:
		SCQingYuanWeddingAllInfo();
		MessageHeader header;

		int role_id;						// ??????????id
		GameName role_name;					// ??????????????
		int lover_role_id;					// ????????id
		GameName lover_role_name;			// ????????????
		char wedding_type;					// ????????
		char has_invite_guests_num;			// ??????????????
		char can_invite_guest_num;			// ????????????????????
		char wedding_yuyue_seq;				// ??????????????
		int wedding_index;					// ??????????????
		char role_prof;						// ??????????????
		char lover_role_prof;				// ????????????
		short reserve_sh;	
		int count;
		WeddingGuestInfo guests_list[WEDDING_GUESTS_MAX_NUM];
	};

	//-------------------------------- ?????? ----------------------

	enum PROFESS_WALL_REQ_TYPE
	{
		PROFESS_WALL_REQ_INFO = 0,			// ????????????p1:0???? 1???? 2??????p2:???????? ??????????????????
		PROFESS_WALL_REQ_LEVEL_INFO = 1,	// ????????????
		PROFESS_WALL_REQ_DELETE = 2,		// ??????????p1:????????p2:????????p3:????id?? ??????1145??????operate??75,p1?????? p2:role_id

		PROFESS_WALL_REQ_TYPE_MAX,
	};

	// ?????????????? 2865
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

	// ???????? 2866
	class CSProfessToReq
	{
	public:
		CSProfessToReq();
		MessageHeader header;

		int target_role_id;		// ????id
		short gift_id;			// ????????
		short is_auto_buy;		// ????????????
		ProfessContent	content;// ????????
	};


	// ?????????????? 2867
	class SCGlobalProfessWallInfo
	{
	public:
		SCGlobalProfessWallInfo();
		MessageHeader header;

		struct ProfessItem
		{
			int				role_id_from;		// ??????
			int				role_id_to;			// ????????
			int				gife_type;			// ????????
			unsigned int	profess_time;		// ????????
			long long		avatar_timestamp;	// ??????????
			long long		avatar_timestamp_to;// ????????????
			GameName		role_name_from;		// ??????????
			GameName		role_name_to;		// ????????????
			ProfessContent	content;			// ????????
		};

		int profess_count;						// ????????
		unsigned int timestamp;					// ????????????????????????????
		ProfessItem items[GLOBAL_PROFESS_MAX_NUM];	//????30??
	};

	enum PROFESS_INFO_NOTIFY_TYPE
	{
		PROFESS_INFO_NOTIFY_TYPE_NORMAL = 0,
		PROFESS_INFO_NOTIFY_TYPE_PROFESS_FROM,       // ?????????? 
	};

	// ?????????????? 2868
	class SCPersonProfessWallInfo
	{
	public:
		SCPersonProfessWallInfo();
		MessageHeader header;

		struct ProfessItem
		{
			int				other_role_id;		// ????id
			int				gife_type;			// ????????
			unsigned int	profess_time;		// ????????
			long long		avatar_timestamp;	// ????????
			GameName		other_name;			// ????????
			ProfessContent	content;			// ????????
		};

		char notify_type;						// ????????
		char profess_type;						// ??????????
		short profess_count;					// ????????
		unsigned int timestamp;					// ????????????????????????????
		ProfessItem items[PERSON_PROFESS_MAX_NUM];	//????12??
	};

	// ????????	2869
	class SCProfessWallEffect
	{
	public:
		SCProfessWallEffect();
		MessageHeader header;

		int effect_type;
	};

	// ????????????	2870
	class SCProfessLevelInfo
	{
	public:
		SCProfessLevelInfo();
		MessageHeader header;

		unsigned short my_grade;		// ??????????????
		unsigned short other_grade;		// ??????????????
		unsigned int my_exp;			// ??????????????
		unsigned int other_exp;			// ??????????????
	};

	//-------------------------------- ???????? -----------------------------------------
	class CSMarryHunyanBless  // 2783 ????????
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

	class CSMarryHunyanGetBlessNews  // 2794 ???????????????? (????????)
	{
	public:
		CSMarryHunyanGetBlessNews();
		MessageHeader		header;
	};

	class CSMarryHunyanOpera   // 2782 ????????????
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
		SPECIAL_PARAM_CHANGE_TYPE_XUNYOU,			// ????
	};

	class SCSpecialParamChange	// 2729 ????????????
	{
	public:
		SCSpecialParamChange();
		MessageHeader		header;

		short change_type;
		ObjID obj_id;
		int param;
	};

	class SCMarryHunyanOpera	// 6008 ????????????????
	{
	public:
		SCMarryHunyanOpera();
		MessageHeader		header;

		ObjID				obj_id;
		short				opera_type;
		short				opera_param;
		short				reserve;
	};

	class SCWeddingBlessingRecordInfo // 6010 ????????????
	{
	public:
		SCWeddingBlessingRecordInfo();
		MessageHeader header;

		int count;
		WeddingBlessingRecord record_info_list[WEDDING_BLESSSING_MAX_RECORD_NUM];
	};

	class SCWeddingApplicantInfo  // 6016  ????????????????
	{
	public:
		SCWeddingApplicantInfo();
		MessageHeader header;

		int count;
		WeddingApplicantInfo applicant_list[WEDDING_APPLICANT_MAX_NUM];
	};

	class SCHunYanCurWeddingAllInfo  // 6000 ????????????????????????
	{
	public:
		SCHunYanCurWeddingAllInfo();
		MessageHeader header;

		int role_id;						// ??????????id
		GameName role_name;					// ??????????????
		int lover_role_id;					// ????????id
		GameName lover_role_name;			// ????????????
		char role_prof;						// ??????????????
		char lover_role_prof;				// ????????????
		char reserve_ch;
		char cur_wedding_seq;				// ??????????????
		int wedding_index;					// ??????????????
		int count;
		int guests_uid[WEDDING_GUESTS_MAX_NUM]; // ????????
	};

	class SCWeddingRoleInfo   // 5440 ????????????????
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

		short hunyan_food_id_count;							// ????????????????
		ObjID hunyan_food_id_list[MAX_GATHR_FOOD_COUNT];	// ????????????objid????
	};

	enum HUNYAN_NOTIFY_REASON
	{
		HUNYAN_NOTIFY_REASON_STATE_CHANGE = 0,			// ????????
		HUNYAN_NOTIFY_REASON_ENTER_HUNYAN,				// ????????
		HUNYAN_NOTIFY_REASON_LOGIN,						// ????
		HUNYAN_NOTIFY_REASON_INVITE_FRIEND,				// ????????
		HUNYAN_NOTIFY_REASON_INVITE_GUILD,				// ????????????
		HUNYAN_NOTIFY_REASON_GATHER,					// ????
		HUNYAN_NOTIFY_REASON_PAOHUAQIU,					// ??????
		HUNYAN_NOTIFY_REASON_LEVEL_OR_ENTER,			// ????????????
		HUNYAN_NOTIFY_REASON_LEAVE,						// ????????
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

	class SCHunyanStateInfo	// 6009 ????????????????
	{
	public:
		SCHunyanStateInfo();
		MessageHeader		header;

		int notify_reason;  

		int hunyan_state;												// ????????
		unsigned int next_state_timestmp;
		int fb_key;														// ????key
		int yanhui_type;												// ????????
		int remainder_eat_food_num;										// ??????????????????
		int roleuid_count;												// 
		int reserve_int;												// ??????????????????
		int is_self_hunyan;												// ??????????????????
		unsigned int paohuoqiu_timestmp;                                // ????????????????????????
		int total_gather_times;											// ????????????

		HunyanMarryUserInfo marryuser_list[HUNYAN_MARRY_USER_COUNT];	// ??????
		int roleuid_list[MAX_ROLE_IN_YANHUI];							// ??????list
	};

	class SCQingyuanHunyanInviteInfo // 5439 ??????????????????????
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
	
	// 6013 ????????????
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
		MARRY_ZHUHE_TYPE0 = 0,			// ????
		MARRY_ZHUHE_TYPE1,				// ????
	};

	// 6014 ????????
	class CSMarryZhuheSend
	{
	public:
		CSMarryZhuheSend();
		MessageHeader		header;

		int uid;
		int type;
	};

	// 6015 ????????????
	class SCMarryZhuheShou
	{
	public:
		SCMarryZhuheShou();
		MessageHeader		header;
		
		int uid;
		GameName name;
		int type;
	};

	//-------------------------------- ???????? -----------------------------------------

	enum QYSD_OPERA_TYPE
	{
		QYSD_OPERA_TYPE_FETCH_TASK_REWARD = 0,			// ????????????
		QYSD_OPERA_TYPE_FETCH_OTHER_REWARD,				// ????????????
	};

	// ???????????????? 6020
	class CSQingYuanShengDiOperaReq
	{
	public:
		CSQingYuanShengDiOperaReq();
		MessageHeader		header;

		int opera_type;
		int param;
	};

	// ???????????????? 6021
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

	// ????????boss???? 6022
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

	//-------------------------------- ????????-----------------------------------------

	enum QINGYUAN_FB_OPERA_TYPE
	{
		QINGYUAN_FB_OPERA_TYPE_BASE_INFO = 0,		  // ????????????????
		QINGYUAN_FB_OPERA_TYPE_BUY_TIMES,			  // ????????????
		QINGYUAN_FB_OPERA_TYPE_BUY_BUFF,			  // ????buff
		QINGYUAN_FB_OPERA_TYPE_BUY_DOUBLE_REWARD,	  // ????????????
	};

	class CSQingyuanFBOperaReq // 5469 ????????????????
	{
	public:
		CSQingyuanFBOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
	};

	class SCQingyuanBaseFBInfo		// 5425 ????????????????
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

	class SCQingyuanFBInfo		// 5426 ????????????
	{
	public:
		SCQingyuanFBInfo();
		MessageHeader header;

		char curr_wave;					// ????????
		char max_wave_count;			// ????????
		char is_pass;					// ????????
		char is_finish;					// ????????
		int next_refresh_monster_time;	// ????????????
		short add_qingyuan_value;
		short buy_buff_times;			// 
		int buff_failure_timestamp;
		int per_wave_remain_time;		// ????????????
		int total_get_uplevel_stuffs;
		long long total_get_exps;
		int kick_out_timestamp;			// ????????????
		char male_is_buy_double_reward;	// ????????????????????
		char female_is_buy_double_reward; // ????????????????????
		short reserve_sh;					
	};

	class SCQingyuanFBRewardRecordInfo				//	5442 ????????????
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

	//-------------------------------- ????????-----------------------------------------
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

	class SCQingyuanCardLevelList											// ????????????????  4724
	{
	public:
		SCQingyuanCardLevelList();
		MessageHeader header;

		char card_level_list[QINGYUAN_CARD_MAX_ID + 1];
	};

	class SCQingyuanCardUpdate												// ???????????????????? 4725
	{
	public:
		SCQingyuanCardUpdate();
		MessageHeader header;

		char card_id;
		char card_level;
		short reserve_sh;
	};

	class CSQingyuanCardUpgradeReq											// ???????????????? 4780
	{
	public:
		CSQingyuanCardUpgradeReq();
		MessageHeader header;

		char card_id;
		char reserve_ch;
		short reserve_sh;
	};

	class CSQingyuanFetchBlessRewardReq										// ????????????????
	{
	public:
		CSQingyuanFetchBlessRewardReq();
		MessageHeader header;
	};

	class CSQingyuanAddBlessDaysReq										// ????????????
	{
	public:
		CSQingyuanAddBlessDaysReq();
		MessageHeader header;
	};

	enum LOVE_CONTRACT_REQ_TYPE
	{
		LC_REQ_TYPE_INFO = 0,					// ????????
		LC_REQ_TYPE_BUY_LOVE_CONTRACT,		    // ????????????
		LC_REQ_TYPE_NOTICE_LOVER_BUY_CONTRACT,   // ????????????????????
	};

	class CSLoveContractOperaReq			// 4792 ????????????
	{
	public:
		CSLoveContractOperaReq();
		MessageHeader header;

		short opera_type;
		short param1;
	};

	enum LOVE_CONTRACT_NOTICE_TYPE
	{
		LC_NOTICE_TYPE_FETCH = 0,					// ????????????????
		LC_NOTICE_TYPE_NOTICE,						// ????????????
	};

	class CSQingyuanFetchLoveContract		// 4793 ????????????????
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
		LOVE_CONTRACT_INFO_TYPE_NORMAL = 0,					// ????
		LOVE_CONTRACT_INFO_TYPE_NOTICE_BUY_CONTRACT,	  // ????????????????
	};

	class SCQingyuanLoveContractInfo		// 4794 ????????????
	{
	public:
		SCQingyuanLoveContractInfo();
		MessageHeader header;

		int info_type;									// ????????
		int self_love_contract_reward_flag;
		int can_receive_day_num;						// -1 ????????????
		unsigned int lover_love_contract_timestamp;		// 0 ??????????????????

		int self_notice_list_count;
		ContractNoticePack self_notice_list[MAX_NOTICE_COUNT];

		int lover_notice_list_count;
		ContractNoticePack lover_notice_list[MAX_NOTICE_COUNT];
	};

	//-------------------------------- ????????-----------------------------------------

	enum QINGYUAN_COUPLE_HALO_REQ_TYPE
	{
		QINGYUAN_COUPLE_REQ_TYPE_INFO = 0,				// ????????
		QINGYUAN_COUPLE_REQ_TYPE_USE,					// ???????? param_1????????
		QINGYUAN_COUPLE_REQ_TYPE_UP_LEVEL,				// ???????? param_1???????? param_2 ????????????
		QINGYUAN_COUPLE_REQ_TYPE_BUY_HALO,				// ???????? param_1 ????????

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

		char equiped_couple_halo_type;														// ????????????????
		char  cur_tejia_halo_type;															// ????????????????
		short couple_halo_max_type_count;													// ??????????????
		unsigned int tejie_halo_invalid_time;												// ????????????????????
		short couple_halo_level_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];						// ????????????
		short couple_halo_exp_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];							// ????????????
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

	//-------------------------------- ????????-----------------------------------------
	enum QINGYUAN_EQUIP_REQ_TYPE
	{
		QINGYUAN_EQUIP_REQ_SELF_EQUIP_INFO = 0,			// ????????????????
		QINGYUAN_EQUIP_REQ_OTHER_EQUIP_INFO,			// ????????????????
		QINGYUAN_EQUIP_REQ_ACTIVE_SUIT,					// ???????????? param_1??????????param_2 ??????, param_3????????
		QINGYUAN_EQUIP_REQ_TAKE_OFF,					// ??????????, param_1 ????????

		QINGYUAN_EQUIP_REQ_ACTIVE_SPECIAL_BABY,			// ???????????????? param_1 ???? param_2 ????

		QINGYUAN_EQUIP_REQ_EQUIP2_UPLEVEL = 10,			// ???????????????????? param_1 ????????
		QINGYUAN_EQUIP_REQ_BUY_GIFT,					// ???????? param_1 ????????

		QINGYUAN_EQUIP_REQ_TYPE_MAX,
	};

	// ???????????? 8392
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

	// ???????????? 8394
	class SCQingyuanEquipInfo
	{
	public:
		SCQingyuanEquipInfo();
		MessageHeader header;

		int is_self;    // ????1 ???? 0
		int marry_level;
		int marry_level_exp;
		unsigned char qingyuan_suit_flag[QINGYUAN_EQUIPSUIT_MAX_COUNT];		// ????????????
		char buy_gift_flag;													// ????????????????
		char reserve;
		ItemDataWrapper qingyuan_equip_list[QINGYUAN_EQUIP_TYPE_MAX];		// ????????????
		QingyuanEquip2Item qingyuan_equip2_list[QINGYUAN_EQUIP_TYPE_MAX];	// ????????????????
		QingyuanSpecialBaby special_baby_list[SPECIAL_BABY_TYPE_MAX];		// ????????????
	};

	//////////////////////////////// ???? //////////////////////////////////////////
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

	//////////////////////////////////// ????BOSS /////////////////////////////////
	class CSGetWorldBossInfo					// 10151????????boss????
	{
	public:
		CSGetWorldBossInfo();
		MessageHeader	header;

	};

	class SCWorldBossInfoToAll						// 10100 ????????boss????????
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

	class SCWorldBossInfoList						// 10101 ????????boss????
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

	class SCWorldBossBorn						// 10102????boss????
	{
	public:
		SCWorldBossBorn();
		MessageHeader	header;
	};

	class CSWorldBossPersonalHurtInfoReq				// 10152????boss????????????????
	{
	public:
		CSWorldBossPersonalHurtInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossSendPersonalHurtInfo				// 10103????????boss????????????????
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

	class CSWorldBossGuildHurtInfoReq					// 10153????boss????????????????
	{
	public:
		CSWorldBossGuildHurtInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossSendGuildHurtInfo					// 10104????????boss????????????????
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

	class CSWorldBossWeekRankInfoReq			// 10154????boss????????????????????
	{
	public:
		CSWorldBossWeekRankInfoReq();
		MessageHeader header;

	};

	class SCWorldBossWeekRankInfo				// 10105????????boss????????????????????
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
	
	class SCWorldBossCanRoll				// 10106????boss??????????????????????????
	{
	public:
		SCWorldBossCanRoll();
		MessageHeader header;

		int boss_id;
		int hudun_index;					// ??????????1????????????2
	};

	class CSWorldBossKillerInfoReq				// 10150 ????????boss????????
	{
	public:
		CSWorldBossKillerInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossKillerList						// 10109 ????boss????????
	{
	public:
		SCWorldBossKillerList();
		MessageHeader header;

		WorldBossManager::KillerInfo killer_info_list[WORLD_KILLER_LIST_MAX_COUNT];
	};

	class CSWorldBossRollReq				// 10155????????????
	{
	public:
		CSWorldBossRollReq();
		MessageHeader header;

		int boss_id;
		int hudun_index;
	};

	class SCWorldBossRollInfo				// 10107????????????????
	{
	public:
		SCWorldBossRollInfo();
		MessageHeader header;

		int roll_point;
		int hudun_index;
	};

	class SCWorldBossRollTopPointInfo     // 10108??????????????
	{
	public:
		SCWorldBossRollTopPointInfo();
		MessageHeader header;

		int boss_id;
		int hudun_index;
		int top_roll_point;
		GameName top_roll_name;
	};

	class SCWorldBossHPInfo				 // 10111????boss????????
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
	class SCActivityFindInfo				// 6005 ????????????????
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

		int activity_open_flag;				// ??????????
		int activity_reward_flag;			// ??????????
		int activity_day_enter_flag;		// ??????????
		int notify_reason;					// ????????
	};

	//////////////////////////////////////////////////////////////////////////
	class CSSupplyBuyItem				// 2772????????
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

	class CSSupplySetRecoverRangePer	// 2773??????????????
	{
	public:
		CSSupplySetRecoverRangePer();
		MessageHeader header;

		int supply_type;
		int recover_range_per;
	};

	class SCSupplyInfo					// 2726????????????
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
		int msg_identify;			// ??????????????????
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
		int msg_identify;			// ??????????????????
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
// 		short select_jinhua_img;				// ??????????????????????
// 		short jinhua;							// ??????
// 		int jinhua_bless;						// ??????????	
// 		short special_img_flag;					// ????????????
// 		char select_special_img;				// ??????????????
// 		char reserve_ch;	
// 		short grade;							// ????
// 		short use_grade;						// ????????
// 		int grade_bless_val;					// ??????????
// 
// 		int shuxingdan_list[WingParam::SHUXINGDAN_MAX_TYPE];			// ??????
// 		char special_img_grade_list[WingParam::MAX_SPECIAL_IMG_COUNT];	// ????????????
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
// 		short is_auto_buy;						// 1 ??????????0 ??
// 		short repeat_times;
// 	};
// 
// 	class CSWingUseImage		// 4758
// 	{
// 	public:
// 		CSWingUseImage();
// 		MessageHeader header;
// 
// 		short img_id;						// ????id
// 		short is_special_img;				// ????????????
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
	class CSDiscountBuyGetInfo			// 4760 ????????????????
	{
	public:
		CSDiscountBuyGetInfo();
		MessageHeader header;
	};

	class CSDiscountBuyReqBuy			// 4761 ???????? ????
	{
	public:
		CSDiscountBuyReqBuy();
		MessageHeader header;

		short seq;
		short reserve_sh;
	};

	class SCDiscountBuyInfo				// 4706 ????????????
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
	class CSZeroGiftOperate				// 4797????????????
	{
	public:
		CSZeroGiftOperate();
		MessageHeader header;

		enum ZERO_GIFT_OPERATE_TYPE
		{
			ZERO_GIFT_GET_INFO = 0,
			ZERO_GIFT_BUY,
			ZERO_GIFT_FETCH_REWARD_ITEM,     //????1 seq   ????2 ????
			ZERO_GIFT_BUY_GOD_COSTUME,					//????????????
			ZERO_GIFT_FETCH_GOD_COSTUME_REWARD_ITEM,     //??????????????????????1 seq   ????2 ????????
		};

		int operate_type;
		int param_1;
		int param_2;
	};

	class SCZeroGiftInfo				//  4796 ????????????
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
	class CSAdvanceNoitceOperate				// 4798????????????
	{
	public:
		CSAdvanceNoitceOperate();
		MessageHeader header;

		enum ADVANCE_NOTICE_OPERATE_TYPE
		{
			ADVANCE_NOTICE_GET_INFO = 0,
			ADVANCE_NOTICE_FETCH_REWARD,

			ADVANCE_NOTICE_DAY_GET_INFO = 2,		// ????????
			ADVANCE_NOTICE_DAY_FETCH_REWARD,		// ????????
		};

		int operate_type;
		int param_1;
	};

	class SCAdvanceNoticeInfo				//  4799 ????????????????
	{
	public:
		SCAdvanceNoticeInfo();
		MessageHeader header;

		enum ADVANCE_NOTICE_TYPE
		{
			ADVANCE_NOTICE_TYPE_LEVEL,				// ????
			ADVANCE_NOTICE_TYPE_DAY,				// ????
		};

		int notice_type;							// ????????
		int last_fecth_id;							// ????????????id
		unsigned char notice_day_fectch_flag_list[CommonDataParam::NOTICE_DAY_MAX_NUM]; // ????????????????
	};

	//////////////////////////////////////////////////////////////////////////
	class CSGoldTurntableOperateReq				// 8425 ????????????????
	{
	public:
		CSGoldTurntableOperateReq();
		MessageHeader header;

		enum
		{
			INFO_REQ = 0,						// ????????????
			CHOU_JIANG = 1,						// ????
			
		};

		int operate_type;
	};

	class SCGoldTurntableReward					// 8426 ???????? ????????
	{
	public:
		SCGoldTurntableReward();
		MessageHeader header;

		int index;
	};

	class SCGoldTurntableInfo					//8427 ??????????
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

	class CSTurnTableReq							// 5257????????????
	{
	public:
		CSTurnTableReq();
		MessageHeader header;

		char type;
		char is_roll;
		short reserve_sh;
	};

	class SCTurnTableInfo							// 5205????????????
	{
	public:
		SCTurnTableInfo();
		MessageHeader header;

		short lucky_point[TURNTABLE_TYPE_MAX_COUNT];
	};

	class SCTurnTableReward							// 5204????????????
	{
	public:
		SCTurnTableReward();
		MessageHeader header;

		char type;
		char rewards_index;
		short reserve_sh;
	};

	class SCTurnTableMillionaireView				// 5201??????????????
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
		int baotu_count;				// ????????
		int wabao_reward_type;			// ????????
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

		int msg_length;							//????????
		//????????????????
	};

	class CSFetchCommandRedPaper				//4728
	{
	public:
		CSFetchCommandRedPaper();
		MessageHeader header;

		int red_paper_id;						//????id
	};

	class CSCommandRedPaperCheckInfo			//4729
	{
	public:
		CSCommandRedPaperCheckInfo();
		MessageHeader header;

		int red_paper_id;						//????id
	};

	class SCCommandRedPaperSendInfo				//4730
	{
	public:
		SCCommandRedPaperSendInfo();
		MessageHeader header;

		int creater_uid;
		GameName creater_name;
		long long creater_avator_timestamp;
		int msg_len;							//????????
		char creater_sex;
		char creater_prof;
		short reserve;
		int red_paper_id;
		//????????????
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

		char shizhuang_type;	// 0-??????1-????
		char img_type;			// 0-??????????1-????????
		short img_id;			// ????ID
	};

	class CSShizhuangUpgradeReq
	{
	public:
		CSShizhuangUpgradeReq();
		MessageHeader header;
	
		short shizhuang_type;
		char is_auto_buy;								// ????????????????????
		char repeat_times;								// ????????????????????????
	};

	class CSShizhuangSpecialImgUpgradeReq				// ????????????????????  4809
	{
	public:
		CSShizhuangSpecialImgUpgradeReq();
		MessageHeader header;

		short shizhuang_type;
		char special_img_id;							// ????????ID
		char reserve_ch;
	};

	class SCShizhuangInfo
	{
	public:
		SCShizhuangInfo();
		MessageHeader header;
		
		struct ShizhuangItemMsg 
		{
			char use_img;					// ??????????????ID
			char use_special_img;			// ??????????????????ID????????????????
			char grade;						// ????
			char is_use_special_img;        // ????????????????(????????),0??????,1????
			int grade_bless;				// ??????????
			unsigned char active_flag[SHIZHUANG_MAX_ACTIVE_IMAGE_BYTE];
			unsigned char special_img_active_flag[SHIZHUANG_MAX_ACTIVE_SPECIAL_IMAGE_BYTE]; // ????????????????
			short special_img_grade_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT]; // ????????????????
			unsigned int valid_timestamp_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT];
			short shuxingdan_count;         // ??????????????(??????)
			short reserve;
			short skill_level_list[ShizhuangItem::SKILL_COUNT];              //????????
			short equip_level_list[ShizhuangItem::EQUIP_COUNT];              //????????
			int equip_skill_level;                                           //????????????
			unsigned int clear_bless_value_time;                             //????????????????
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

	class SCAllZhiBaoInfo								// ????????????
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

	class CSFriendExpBottleOP							// ?????????????????? 1790
	{
	public:
		CSFriendExpBottleOP();
		MessageHeader header;

		int type;										// 0????????,1????????
	};

	class SCFriendExpBottleAddExp						// ?????????????????????? 1791
	{
	public:
		SCFriendExpBottleAddExp();
		MessageHeader header;

		int next_broadcast_time;						// ????????????
		int get_exp_count;								// ????????????
		long long exp;									// ????????????
		int auto_add_friend_count;						// ??????????????????
	};

	class CSSkipReq
	{
	public:
		CSSkipReq();
		MessageHeader header;

		enum SKIP_TYPE
		{
			SKIP_TYPE_CHALLENGE = 0,					//????????????????
			SKIP_TYPE_SAILING,							//????????????
			SKIP_TYPE_MINE,								//????????????
			SKIP_TYPE_FISH,								//????
			SKIP_TYPE_JINGLING_ADVANTAGE,				//????????
			SKIP_TYPE_SHENZHOU_WEAPON,					//????????
			SKIP_TYPE_XINGZUOYIJI,						//????????
			SKIP_TYPE_QYSD,								//????????
			SKIP_TYPE_PRECIOUS_BOSS,					//????boss
			SKIP_TYPE_PAOHUAN_TASK,						//????????
			SKIP_TYPE_CROSS_GUIDE_BATTLE,						//????????
		};

		int type;										//????
		int param;										//????,??0????(??????????????????????)
	};

	//????????
	class SCShenYinScoreInfo // 8444
	{
	public:
		SCShenYinScoreInfo();
		MessageHeader header;

		int notify_type;
		int add_score;

		int score;				// ??????????????	
		int chouhun_score;		//????????????
	};

	struct ShenYinGridParamMsg
	{
		int param1; // type 0 ??????????  type 1 ??????
		short v_item_id;
		short item_num;
		short is_bind;
		short level;
		int grade;

		ShenYinPastureSpiritGridParam::AttrItem attr_param;
		ShenYinPastureSpiritGridParam::AttrItem new_attr_param;
	};


	class SCShenYinBagInfo			// ????????????8445
	{
	public:
		enum TYPE
		{
			BAG,			// param1  ??????????
			IMPRINT_SLOT,	// param1  ??????
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
		ROLE_TALENT_OPERATE_TYPE_INFO = 0,		// ????????????
		ROLE_TALENT_OPERATE_TYPE_UPLEVEL,		// ????????
		ROLE_TALENT_OPERATE_TYPE_RESET,			// ??????????
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

		char is_concern;	// ????????????BOSS
		char reserve_ch;
		short reserve_sh;
	};

	///////////////////////////////// ????boss ///////////////////////////////////////
	static const int SC_SHANGGU_BOSS_LAYER_MAX_COUNT = 10;	// ????BOSS - ??????????boss????
	static const int SC_SHANGGU_BOSS_MAX_LAYER = 6;			// ????BOSS - ????????
	enum SHANGGU_BOSS_OPERA_TYPE
	{
		SHANGGU_BOSS_OPERA_TYPE_ENTER = 0,				// ??????????param1 ??????param2 bossID
		SHANGGU_BOSS_OPERA_TYPE_ALL_BOSS_INFO = 1,		// ????????
		SHANGGU_BOSS_OPERA_TYPE_LAYER_BOSS_INFO = 2,	// ??????????param1 ????
		SHANGGU_BOSS_OPERA_TYPE_SET_CONCERN = 3,		// ????????, param1 ???? param2 BOSS id  param3 ??????????1??0??
	};

	class CSShangGuBossReq    // ???????? 8545
	{
	public:
		CSShangGuBossReq();
		MessageHeader header;

		short opera_type;
		short param1;
		short param2;
		short param3;
	};

	class SCShangGuBossAllInfo        // ???????? 8546
	{
	public:
		SCShangGuBossAllInfo();
		MessageHeader header;

		struct LayerBossInfo
		{
			short reserve_sh;
			short boss_count;	// ??????boss??
			SCWorldBossItemInfo boss_info_list[SC_SHANGGU_BOSS_LAYER_MAX_COUNT];
		};

		int tire_value;		// ??????
		short enter_times;	// ??????????
		short layer_count;	// ????
		LayerBossInfo layer_list[SC_SHANGGU_BOSS_MAX_LAYER];
	};

	class SCShangGuBossLayerInfo   // ???????? 8547
	{
	public:
		SCShangGuBossLayerInfo();
		MessageHeader header;

		short cur_layer;
		short boss_count;	// ??????boss??
		SCWorldBossItemInfo boss_info_list[SC_SHANGGU_BOSS_LAYER_MAX_COUNT];
	};

	class SCShangGuBossSceneInfo				// BOSS?????????? 8548
	{
	public:
		SCShangGuBossSceneInfo();
		MessageHeader	header;

		short angry_value;				// ??????
		short reserve_sh;
		unsigned int kick_out_time;		// ????????????????
	};

	class SCShangGuBossSceneOtherInfo	// ??????????????????????boss???????????????? 8549
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

		int layer_count;	// ????
		LayerInfo layer_info_list[SC_SHANGGU_BOSS_MAX_LAYER];
	};

	/////////////////////////////// BOSS???? //////////////////////////////////////
	enum BOSS_CARD_OPERA_TYPE
	{
		BOSS_CARD_OPERA_TYPE_ALL_INFO = 0,       //????????
		BOSS_CARD_OPERA_TYPE_ACTIVE,			 // ???? param1 ????seq
		BOSS_CARD_OPERA_TYPE_FETCH,              // ???????????? param1 ????type

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

		unsigned long long card_group_reward_fetch_flag;                              // ????????????????
		unsigned char card_can_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// ??????????
		unsigned char card_has_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// ??????????
	};

	class SCReliveTire				          //  ????????   5740
	{
	public:
		SCReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// ????????
		unsigned int tire_buff_end_time;		// ????????BUFF????????
		unsigned int tire_can_relive_time;		// ????????????????
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

	////////////////////////// ???????????? //////////////////////////
	enum ROLE_BIG_SMALL_GOAL_OPERA_TYPE
	{
		ROLE_BIG_SMALL_GOAL_OPERA_INFO = 0,      // ????????   param_1 ????????
		ROLE_BIG_SMALL_GOAL_OPERA_FETCH,         // ????       param_1 ????????  param_2 ????????????
	};

	class CSRoleBigSmallGoalOper            // 8821
	{
	public:
		CSRoleBigSmallGoalOper();
		MessageHeader header;

		int oper_type;                    // ????????
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
			char active_flag;                                               // ????????
			char fetch_flag;                                                // ????????
		};

		short system_type;                                                   // ????????
		char res_ch;
		char active_special_attr_flag;                                       // ????????????????
		unsigned int open_system_timestamp;                                  // ????????????????
		GoalInfoItem goal_item_info[ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX];    // 2   0 ??????  1 ??????
	};

	//-------------------------------------------------------------------------------------------

	// ????????           8766
	class SCRedEquipCollect
	{
	public:
		SCRedEquipCollect();
		MessageHeader header;

		int seq;
		ItemDataWrapper equip_slot[RED_EQUIP_COLLECT_SLOT_COUNT];
	};

	// ????????-????????        8767
	class SCRedEquipCollectOther
	{
	public:
		SCRedEquipCollectOther();
		MessageHeader header;

		UInt32 seq_active_flag;					// ??????????????????????????????
		int collect_count;						// ??????????????
		UInt32 act_reward_can_fetch_flag;		// ??????????????????
		UInt32 active_reward_fetch_flag;		// ????????????????????????????????
		RedEquipCollectInfo collect_info_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT];		// ????????????
	};

	//-------------------------------------------------------------------------------------------

	// ????????               8768
	class SCOrangeEquipCollect
	{
	public:
		SCOrangeEquipCollect();
		MessageHeader header;

		int seq;
		ItemDataWrapper equip_slot[ORANGE_EQUIP_COLLECT_SLOT_COUNT];
	};

	// ????????-????????            8769
	class SCOrangeEquipCollectOther
	{
	public:
		SCOrangeEquipCollectOther();
		MessageHeader header;

		UInt32 seq_active_flag;					// ??????????????????????????????
		int collect_count;						// ??????????????
		UInt32 act_reward_can_fetch_flag;		// ??????????????????
		UInt32 active_reward_fetch_flag;		// ????????????????????????????????
		OrangeEquipCollectInfo collect_info_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT];		// ????????????
	};

	//---------------------------??????--------------------------------------------------------
	enum XIANZUNKA_OPERA_REQ_TYPE
	{
		XIANZUNKA_OPERA_REQ_TYPE_ALL_INFO = 0,				// ????????????
		XIANZUNKA_OPERA_REQ_TYPE_BUY_CARD,					// ?????????? param_1 : ??????????
		XIANZUNKA_OPERA_REQ_TYPE_FETCH_DAILY_REWARD,		// ???????????? param_1 :??????????
		XIANZUNKA_OPERA_REQ_TYPE_ACTIVE,					// ???????????? param_1:??????????

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

		short forever_active_flag;						// ??????????????	
		char first_active_reward_flag;					// ????????????????
		char daily_reward_fetch_flag;					// ????????????????

		unsigned int temporary_valid_end_timestamp_list[XIANZUNKA_TYPE_MAX];		// ??????????????????
	};

	///////////////////// ???????? ////////////////////////////////
	enum EQUIP_BAPTIZE_OPERA_TYPE
	{
		EQUIP_BAPTIZE_OPERA_TYPE_ALL_INFO = 0,		// ????????
		EQUIP_BAPTIZE_OPERA_TYPE_OPEN_SLOT,			// ??????          param_1 ????????   param_2  ??????????
		EQUIP_BAPTIZE_OPERA_TYPE_LOCK_OR_UNLOCK,	// ???? or ????    param_1 ????????   param_2  ??????????
		EQUIP_BAPTIZE_OPERA_TYPE_BEGIN_BAPTIZE,		// ????????        param_1 ????????   param_2  ????????????   param_3  ??????????

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

	class SCMiZangBossReliveTire				          //  ????????????   8860
	{
	public:
		SCMiZangBossReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// ????????
		unsigned int tire_buff_end_time;		// ????????BUFF????????
		unsigned int tire_can_relive_time;		// ????????????????
	};

	class SCYouMingBossReliveTire				          //  ???????????? 8855
	{
	public:
		SCYouMingBossReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// ????????
		unsigned int tire_buff_end_time;		// ????????BUFF????????
		unsigned int tire_can_relive_time;		// ????????????????
	};

	class SCUpgradeCardBuyInfo										// 8878 ????????
	{
	public:
		SCUpgradeCardBuyInfo();
		MessageHeader header;

		short activity_id;
		short grade;												// ????????
		short is_already_buy;	                                    // 0 ????????  1????????
		short reserve_sh;
	};

	class CSUpgradeCardBuyReq										// 8879 ????????
	{
	public:
		CSUpgradeCardBuyReq();
		MessageHeader header;

		short activity_id;
		ItemID item_id;												// unsigned short
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	class SCHuguozhiliInfo											// 8610  ???????? - ????
	{
	public:
		SCHuguozhiliInfo();
		MessageHeader header;

		HuguozhiliParam info;
	};

	class CSHuguozhiliReq											// 8611  ???????? - ????
	{
	public:
		enum
		{
			REQ_INFO,				// ????????
			REQ_ACTIVE_HUGUOZHILI,	// ????????????????
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

		int seq;				//????????????????????
	};

	class SCTodayThemeRewardFlagInfo	//8917
	{
	public:
		SCTodayThemeRewardFlagInfo();
		MessageHeader header;

		unsigned char fetch_flag[TODAY_THEME_REWARD_MAX_BYTE];	//????????
	};

	//////////////////////////////////////////////////////////////////////////
	class SCWorldAcquisitionLog										// 8612  ????????????????
	{
	public:
		SCWorldAcquisitionLog();
		MessageHeader header;

		enum NOTICE_TYPE
		{
			NOTICE_TYPE_ADD = 0, // ????????
			NOTICE_TYPE_ALL = 1, // ????????
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

			char log_type;			// ????
			char log_str_id;		// ??????????ID
			ItemID item_id;			// ????ID
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ????????????

			unsigned int timestamp;	// ??????????
			GameName	role_name;	// ????
			long long uuid;
		};

		short notice_type;		// ??????????????NOTICE_TYPE
		short count;
		LogItem log_List[MAX_SERVER_ACQUISITION_RECORD_NUM];  // ????100??
	};

	class CSWorldAcquisitionLogReq									// 8613  ????????????????
	{
	public:
		CSWorldAcquisitionLogReq();
		MessageHeader header;

		char log_type;			// ????
		char log_str_id;		// ??????????ID
		ItemID item_id;			// ????ID
		unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ????????????
	};

	class CSCrossGoalOperaReq	 //8625
	{
	public:
		enum OPERA_TYPE
		{
			CROSS_GOAL_INFO_REQ = 0,		//????????
			FETCH_CROSS_GOAL_REWARD_REQ = 1,//????????
			FETCH_GUILD_GOAL_REWAED_REQ = 2,//????????????????
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

		char fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // ????????????
		unsigned short kill_cross_boss_num;		//????????boss
		unsigned short cross_boss_role_killer;	//????boss??????????
		unsigned short kill_baizhan_boss_num;		//????????boss??
		unsigned short finish_baizhan_task_num;		//????????boss??
		char guild_fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // ???????????????? 0 ?????? 1 ?????? 2 ??????
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

	class SCZeroGiftGodCostumeInfo				//  8922 ????????????????????
	{
	public:
		SCZeroGiftGodCostumeInfo();
		MessageHeader header;

		ZeroGiftGodCostumeInfo zero_gift_god_costume_info[ZERO_GIFT_GOD_COSTUME_MAX];
	};

	class SCSceneActivityRewardInfo				//  8925 ????????????
	{
	public:

		enum REWARD_TYPE
		{
			REWARD_TYPE_SHIZHUANG = 0,		//????
			REWARD_TYPE_WEAPON = 1,			//????
			REWARD_TYPE_MOUNT = 2,			//????
			REWARD_TYPE_TITLE = 3,		    //????
			REWARD_TYPE_WING = 4,		    //????
		};

		enum REWARD_SOURCE_ID				//????????????????????????????????
		{
			REWARD_SOURCE_ID_CROSS_ADD_CAP = 10000,					//??????????
			REWARD_SOURCE_ID_CROSS_ADD_CHARM = 10001,				//??????????
			REWARD_SOURCE_ID_CROSS_QINGYUAN_CAP = 10002,			//??????????
			REWARD_SOURCE_ID_CROSS_GUILD_KILL_BOSSP = 10003,		//??????????????
			REWARD_SOURCE_ID_CHALLENGEFIELD = 10004,				//??????????
		};

		SCSceneActivityRewardInfo();
		MessageHeader header;

		int activity_id;
		int reward_type;
		int reward_id;
		int param;
	};

	class SCEquipUplevelSuitInfo				//  8924 ????????????
	{
	public:

		SCEquipUplevelSuitInfo();
		MessageHeader header;

		int suit_level;
		int suit_active_flag;
	};

	class CSEquipUplevelSuitActive				//  8937 ????????????
	{
	public:

		CSEquipUplevelSuitActive();
		MessageHeader header;

		int active_suit_level;
	};

	/////////////////////////////////////// ???????? ///////////////////////////////////////////////////
	// ????????????????
	enum BAIZHAN_EQUIP_OPERATE_TYPE
	{
		BAIZHAN_EQUIP_OPERATE_TYPE_ALL_INFO = 0,		// ????????????????

		BAIZHAN_EQUIP_OPERATE_TYPE_TAKE_OFF,			// ???? p1: part_index

		BAIZHAN_EQUIP_OPERATE_TYPE_UP_LEVEL,			// ????????	p1: part_index p2:item_id
	};

	class CSBaizhanEquipOpera		// 8926
	{
	public:
		CSBaizhanEquipOpera();
		MessageHeader header;

		short operate;                                  // ??????????0 = ??????????1 = ????...??
		short param1;
		int param2;
	};

	class SCBaizhanEquipAllInfo			// 8927 ????????????
	{
	public:
		SCBaizhanEquipAllInfo();
		MessageHeader header;

		ItemDataWrapper baizhan_equip_list[E_INDEX_BAIZHAN_MAX];

		char part_order_list[E_INDEX_BAIZHAN_MAX];			// ????????
		short reserve_sh;
	};


	enum LITTLE_HELPER_COMPLETE_TYPE	//CSLittleHelperOpera::type
	{
		LITTLE_HELPER_COMPLETE_TYPE_ALL = -1,
		LITTLE_HELPER_COMPLETE_TYPE_EASY_BOSS = 0,					//????boss
		LITTLE_HELPER_COMPLETE_TYPE_DIFFICULT_BOSS = 1,				//????boss
		LITTLE_HELPER_COMPLETE_TYPE_BABY_BOSS = 2,					//????boss
		LITTLE_HELPER_COMPLETE_TYPE_SUIT_BOSS = 3,					//????boss
		LITTLE_HELPER_COMPLETE_TYPE_DEMON_BOSS = 4,					//????boss
		LITTLE_HELPER_COMPLETE_TYPE_PET_ADVENTURE = 5,				//????????
		LITTLE_HELPER_COMPLETE_TYPE_ESCORT_FAIRY = 6,				//????????
		LITTLE_HELPER_COMPLETE_TYPE_EXP_FB = 7,						//???????? p0: ???????? p1: ??????????????
		LITTLE_HELPER_COMPLETE_TYPE_TOWER_DEFENSE_FB = 8,			//???????? p0: ???????? p1: ????????????
		LITTLE_HELPER_COMPLETE_TYPE_CYCLE_TASK = 9,					//????????
		LITTLE_HELPER_COMPLETE_TYPE_EXP_TASK = 10,					//????????
		LITTLE_HELPER_COMPLETE_TYPE_GUILD_TASK = 11,				//????????

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

			short type;									 // ????????
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

		int count;										// ????????
		CSLittleHelperOpera::Req req_list[Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX];		// ????
	};

	class SCLittleHelperInfo
	{
	public:
		SCLittleHelperInfo();
		MessageHeader header;

		short type;									// ??????????????????????????
		short reverse_sh;
	};

	class CSGetGuildRareLog                 // 8930  ????????????
	{
	public:
		CSGetGuildRareLog();
		MessageHeader header;
	};

	class SCGuildRareLogRet                 // 8931  ????????????
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
		long long zhuanzhi_equip_fangyu;									//??????????
		unsigned int fetch_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];		// ????????????????
		unsigned int act_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];			// ????????????????
		short baizhan_equip_num;
		short reserve_sh;
	};

	enum CS_TIANSHUXZ_SEQ_TYPE
	{
		CS_TIANSHUXZ_SEQ_TYPE_INFO = 0,	//????????
		CS_TIANSHUXZ_SEQ_TYPE_FETCH = 1,//????????
	};

	class CSTianShuXZFetchReward	// 8936
	{
	public:
		CSTianShuXZFetchReward();
		MessageHeader header;

		int type;				// ????????
		short tianshu_type;		// ????????
		short seq;				// ????????????
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
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// ????????????
		};

		int count;
		RewardItem	item_list[lhns::MAX_DROP_ITEM_COUNT];
	};

}

#pragma pack(pop)

#endif

