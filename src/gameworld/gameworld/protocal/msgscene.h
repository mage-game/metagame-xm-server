#ifndef MSGSCENE_H
#define MSGSCENE_H

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"
#include "obj/character/role.h"

#include "effect/sceneeffect/sceneskillbase.hpp"
#include "scene/speciallogic/speciallogicdef.hpp"
#include "other/impguard/impguard.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
	进入退出场景
	*/

	class SCEnterScene				// 1100
	{
	public:
		SCEnterScene();
		MessageHeader	header;
		int				sceneid;
		ObjID			obj_id;
		short			open_line;
		short			pos_x;
		short			pos_y;
		Attribute		hp;
		Attribute		max_hp;
		int				scene_key;
	};

	class SCLeaveScene
	{
	public:
		SCLeaveScene();
		MessageHeader	header;
		int				sceneid;
	};

	class SCVisibleObjEnterRole		// 1102
	{
	public:
		SCVisibleObjEnterRole();
		MessageHeader	header;

		Dir				dir;
		char			move_mode_param;
		char			role_status;
		ObjID			obj_id;

		long long 		uuserverid;
		long long 		role_uuid;
		int				role_id;
		GameName		role_name;

		Attribute		hp;
		Attribute		max_hp;

		Attribute		mp;
		Attribute		max_mp;

		short			level;
		char			camp;
		char			prof;

		char			sex;
		char			vip_level;
		ObjID			rest_partner_obj_id;
		
		Attribute		speed;
		short			pos_x;
		short			pos_y;
		Scalar			distance;

		char			attack_mode;
		char			name_color;
		char			move_mode;
		char			authority_type;

		char			husong_color;
		char			guild_post;
		UInt16			mount_appeid;
		
		RoleAppearance	appearance;								// 角色外观

		unsigned short	husong_task_id;							// 护送id
		ItemID			used_jingling_id;						// 使用的精灵id
		char			jingling_use_imageid;					// 精灵使用形象
		char			used_xiannv_quality;					// 仙女品质等级
		short			chengjiu_title_level;					// 成就称号等级
		GameName		jingling_name;							// 精灵名字

		long long		avatar_timestamp;

		short			flyup_use_image;						// 坐骑飞升形象
		short			use_xiannv_id;							// 出战仙女id
		short			use_jingling_level;						// 精灵等级
		short			xianjie_level;							// 仙阶等级
		int				pata_pass_level;						// 爬塔通关层数
		short			jingling_phantom_img;					// 精灵幻象
		char			top_dps_flag;							// 最高dps标记
		char			first_hurt_flag;						// boss第一刀标记

		int				pet_id;									// 宠物ID
		short			pet_level;								// 宠物等级							
		short			pet_grade;								// 宠物阶级						
		GameName		pet_name;								// 宠物名字
		short			user_pet_special_img;                   // 宠物特殊形象
		short			jingling_halo_img;						// 精灵光环

		int				multi_mount_res_id;						// 双人坐骑资源id
		int				multi_mount_is_owner;					// 是否当前双人坐骑的主人
		int				multi_mount_partner_uid;				// 一起骑乘的玩家objID

		GameName		little_pet_name;						// 小宠物名字
		short			little_pet_using_id;					// 小宠物形象id
		short			fight_mount_appeid;						// 战斗坐骑外观
		GameName		xiannv_name;							// 仙女名字
		short			xiannv_huanhua_id;						// 仙女幻化id
		short			combine_server_equip_active_special;	// 屠龙-传世装备是否激活特效,0不是1是
		int				wuqi_color;								// 武器颜色
		int				total_capability;						// 总战斗力
		int				total_jingjie;							// 总境界等级

		char			is_in_xunyou;							// 是否在巡游中
		char            is_fightback_obj;						// 是否是反击对象
		char			shenbing_use_image_id;					// 神兵使用id
		char			baojia_use_image_id;					// 穿戴宝甲套装
		short			imp_appe_id[ImpGuard::IMP_GUARD_GRID_INDEX_MAX];							// 小鬼守护形象

		//////////////////////////////////////////////////////////////////////////

		enum ATTACH_TYPE
		{
			ATTACH_TYPE_GUILD = 0,											// 军团数据
			ATTACH_TYPE_USED_TITLE = 1,										// 头顶称号数据
			ATTACH_TYPE_MILLIONARE = 2,										// 大富豪活动是否转到大富豪							
			ATTACH_TYPE_BUFF_MARK = 3,										// 战斗特殊效果
			ATTACH_TYPE_SPECIAL_PARAM = 4,									// 特殊状态
			ATTACH_TYPE_HEIGHT = 5,											// 跳跃高度
			ATTACH_TYPE_SPECIAL_APPEARANCE = 6,								// 特殊外观
			ATTACH_TYPE_BIANSHEN_APPEARANCE = 7,							// 变身外观
			ATTACH_TYPE_SHENBING_APPE = 8,									// 神兵外观
			ATTACH_TYPE_MARRY_INFO = 9,										// 结婚信息
			ATTACH_TYPE_JILIAN_TYPE = 10,									// 祭炼类型
			ATTACH_TYPE_JINGHUA_HUSONG = 11,								// 精华护送
			ATTACH_TYPE_JINGLING_TITLE = 12,								// 精灵称号
			ATTACH_TYPE_COUPLE_HALO = 13,									// 夫妻光环类型
			ATTACH_TYPE_COUPLE_UID = 14,									// 夫妻光环对象UID
//			ATTACH_TYPE_FISHING_STATION = 15,								// 钓鱼状态
			ATTACH_TYPE_TASK_APPEARANCE_TYPE = 15,							// 接受任务特殊外观类型
			ATTACH_TYPE_TASK_APPEARANCE_PARAM = 16,							// 接受任务特殊外观参数
			ATTACH_TYPE_GATHER_ID = 17,										// 采集id
			ATTACH_TYPE_FLY_HEIGHT = 18,									// 飞行高度

			ATTACH_TYPE_MAX_NUM,
		};

		const static int ATTACH_DATA_LENGTH = 2048;

		int				attach_mask;
		char			attach_data[ATTACH_DATA_LENGTH];

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct AttachGuild
		{
			GuildID guild_id;
			GuildName guild_name;
			unsigned long long guild_avatar;
		};
	};

	class SCVisibleObjEnterMonster				// 1106
	{
	public:
		SCVisibleObjEnterMonster();
		MessageHeader	header;

		Dir				dir;
		short			status_type;
		ObjID			obj_id;
		Attribute		hp;
		Attribute		max_hp;
		Attribute		level;
		unsigned short	monster_id;
		short			speed;
		short			pos_x;
		short			pos_y;
		Scalar			distance;
		//unsigned long long buff_mark;
		unsigned char	buff_mark[SPECIAL_BUFF_MAX_BYTE];
		int				special_param;
		int				monster_key;
	};

	class SCVisibleObjEnterPet				// 1107
	{
	public:
		SCVisibleObjEnterPet();
		MessageHeader	header;

		Dir				dir;
		short			pet_type;
		ObjID			obj_id;
		GameName		obj_name;
		short			pet_id;
		short			image_id;
		Attribute		hp;
		Attribute		max_hp;
		Attribute		speed;
		short			pos_x;
		short			pos_y;
		Scalar			distance;
		short			grow;
		ObjID			owner_objid;
		GameName		owner_name;
		short			pet_level;
		short			pet_zizhi;
	};

	// 掉落物
	class SCVisibleObjEnterFalling   // 1105
	{
	public:
		SCVisibleObjEnterFalling();
		MessageHeader	header;

		ObjID			obj_id;
		UInt16			item_id;
		short			obj_pos_x; // 掉落的怪物的位置（为了给客户端显示抛出特效）
		short			obj_pos_y;
		short			pos_x;  // 掉落物的位置
		short			pos_y;
		int				owner_role_id;
		int				coin_or_gold;
		UInt16			monster_id;
		short			item_num;
		unsigned int    drop_time;
		short			create_interval;
		short			is_create;
		short			is_buff_falling;
		short			reserve;
		int				buff_appearan;
	};

	class SCVisibleObjEnterGather
	{
	public:
		SCVisibleObjEnterGather();
		MessageHeader	header;

		ObjID			obj_id;
		short			special_gather_type;
		int				gather_id;
		short			pos_x;
		short			pos_y;
		int				param_0;
		int				param_1;
		GuildName       param_2;
		long long       param_3;
	};

	class SCGatherChange				// 1148采集物信息变化
	{
	public:
		SCGatherChange();
		MessageHeader	header;

		ObjID			obj_id;
		short			special_gather_type;
		int				gather_id;
		short			pos_x;
		short			pos_y;
		int				param_0;
		int				param_1;
		GuildName       param_2;
	};


	class SCVisibleObjEnterWorldEventObj			// 1146
	{
	public:
		SCVisibleObjEnterWorldEventObj();
		MessageHeader	header;

		ObjID			obj_id;
		short			reserve;
		int				world_event_id;
		short			pos_x;
		short			pos_y;
		Attribute		hp;
		Attribute		max_hp;
	};

	class SCVisibleObjEnterEffect
	{
	public:
		SCVisibleObjEnterEffect();
		MessageHeader	header;

		ObjID			obj_id;
		short			product_method;
		int				product_id;
		short			pos_x;
		short			pos_y;
		unsigned int	birth_time;
		unsigned int	disappear_time;

		int				param1;
		int				param2;
		short			src_pos_x;
		short			src_pos_y;
	};

	class SCVisibleObjEnterBigCoin		
	{
	public:
		SCVisibleObjEnterBigCoin();
		MessageHeader	header;

		ObjID			obj_id;
		short			type;
		long long		value;
		short			pos_x;
		short			pos_y;
	};

	class SCVisibleObjEnterBattleFieldShenShi			// 1120
	{
	public:
		SCVisibleObjEnterBattleFieldShenShi();
		MessageHeader	header;

		int				has_owner;
		Attribute		hp;
		Attribute		max_hp;
		short			pos_x;
		short			pos_y;
		ObjID			obj_id;
		ObjID			owner_obj_id;
		int				owner_uid;
	};

	class SCVisibleObjEnterTrigger
	{
	public:
		SCVisibleObjEnterTrigger();
		MessageHeader	header;

		ObjID			obj_id;
		short			action_type;
		int				param0;
		int				param1;						// 目前两个参数就够了
		short			pos_x;
		short			pos_y;
		int				affiliation;
		GameName		trigger_name;
	};

	class SCVisibleObjEnterMonsterHalo
	{
	public:
		SCVisibleObjEnterMonsterHalo();
		MessageHeader	header;

		ObjID			obj_id;
		short			pos_x;
		short			pos_y;
		ObjID			owner_obj_id;
		short			effect_type;
		short			reserve;
	};

	class SCVisibleObjEnterMarryObj		
	{
	public:
		SCVisibleObjEnterMarryObj();
		MessageHeader	header;

		ObjID			obj_id;
		short			marry_seq;
		short			pos_x;
		short			pos_y;
		Dir				dir;
		Scalar			distance;
		int				speed;
	};

	class SCVisibleObjEnterRoleShadow				// 1144
	{
	public:
		SCVisibleObjEnterRoleShadow();
		MessageHeader	header;

		ObjID			obj_id;
		short			reserved;

		int				role_id;
		GameName		role_name;
		int				level;
		char			avatar;
		char			prof;
		char			sex;
		char			camp;

		Attribute		hp;
		Attribute		max_hp;

		Attribute		speed;
		short			pos_x;
		short			pos_y;
		Dir				dir;
		Scalar			distance;

		int				plat_type;

		RoleAppearance	appearance;

		char			vip_level;
		char			guild_post;
		short			reserve_sh;						

		GuildID			guild_id;
		GuildName		guild_name;
		long long		avatar_timestamp;

		int				shadow_type;
		int				shadow_param;
	};

	class SCVisibleObjLeave
	{
	public:
		SCVisibleObjLeave();
		MessageHeader	header;
		ObjID			obj_id;
		unsigned short	reserved;
	};

	class SCSceneMonsterDie
	{
	public:
		SCSceneMonsterDie();
		MessageHeader	header;

		ObjID			obj_id;
		unsigned short	reserved;
		int				monster_id; 
		int				monster_key;
	};

	/*
	移动
	*/
	class CSObjMove
	{
	public:
		CSObjMove();
		MessageHeader	header;
		Dir				dir;
		short			pos_x;
		short			pos_y;
		Scalar			distance;
		short			height;
		short			is_press_onward;
	};

	class SCObjMove
	{
	public:
		SCObjMove();
		MessageHeader	header;
		Dir				dir;
		short			press_onward;
		ObjID			obj_id;
		short			pos_x;
		short			pos_y;
		Scalar			distance;
		char			move_mode;
		char			reserve2;
		short			height;
	};

	class SCSkillResetPos
	{
	public:
		SCSkillResetPos();
		MessageHeader header;

		Dir				dir;
		short			reset_pos_type;
		ObjID			obj_id;
		short			pos_x;
		short			pos_y;
		int				skill_id;
		ObjID			deliver_obj_id;
		short			reserve;
	};

	// 强设主角位置
	class SCResetPost
	{
	public:
		enum
		{
			RESET_TYPE_DEFAULT,
			RESET_TYPE_SLOW,
		};

		SCResetPost();
		MessageHeader	header;
		short			x;
		short			y;
		short			reset_type;
		short			reserved;
	};

	// 位置纠偏通知
	class SCFixPos
	{
	public:
		SCFixPos();
		MessageHeader	header;

		short			x;
		short			y;
	};

	// 动作拒绝通知
	class SCActionReject
	{
	public:
		SCActionReject();
		MessageHeader	header;

		enum
		{
			ACTION_REJECT_REASON_INVALID = 0,
			ACTION_REJECT_REASON_DIE,			// 已死亡
			ACTION_REJECT_REASON_MAX
		};

		int				action_reject_reason;
	};

	/*
		采集相关
	*/
	// 请求开始采集
	class CSStartGatherReq
	{
	public:
		CSStartGatherReq();
		MessageHeader	header;
		ObjID			gather_obj_id;
		short			gather_count;
	};

	// 请求停止采集
	class CSStopGatherReq
	{
	public:
		CSStopGatherReq();
		MessageHeader	header;
	};

	// 开始采集动作(广播)
	class SCStartGather
	{
	public:
		SCStartGather();
		MessageHeader	header;
		ObjID			role_obj_id;
		ObjID			gather_obj_id;
	};

	// 停止采集动作(广播)
	class SCStopGather
	{
	public:
		SCStopGather();
		MessageHeader	header;
		ObjID			role_obj_id;
		short			reset_reason;
	};

	// 开始采集计时(给个人)
	class SCStartGatherTimer
	{
	public:
		SCStartGatherTimer();
		MessageHeader	header;
		unsigned int	gather_time;
	};

	// 请求开始或结束挂机
	class CSHookReq
	{
	public:
		CSHookReq();
		MessageHeader	header;
	};

	// 开始或结束挂机修炼
	class SCHookStatus
	{
	public:
		SCHookStatus();
		MessageHeader	header;
		ObjID			obj_id;
		short			start_or_stop;	// 1 开始， 0结束
	};

	// 请求开始或结束打坐休息
	class CSRestReq
	{
	public:
		CSRestReq();
		MessageHeader	header;
	};

	// 邀请某个玩家双修
	class CSInviteRestReq
	{
	public:
		CSInviteRestReq();
		MessageHeader	header;

		ObjID			obj_id;
		short			reserved;
	};

	// 询问玩家是否接受双修请求
	class SCRestInviteNotify
	{
	public:
		SCRestInviteNotify();
		MessageHeader	header;

		GameName		role_name;
		ObjID			obj_id;
		short			reserved;
	};

	// 邀请双修的结果
	class SCRestInviteResult
	{
	public:
		SCRestInviteResult();
		MessageHeader	header;

		short			is_accept;
		ObjID			beinviter_obj_id;
		GameName		role_name;
	};

	// 应答某个玩家的双修邀请
	class CSInviteRestAck
	{
	public:
		CSInviteRestAck();
		MessageHeader	header;

		ObjID			obj_id;
		short			is_accept;
	};

	// 确认开始双修
	class CSRestStartAffirm
	{
	public:
		CSRestStartAffirm();
		MessageHeader	header;
		
		ObjID			partner_obj_id;
		short			reserved;
	};

	// 召集队员
	class CSTeamCallTogether
	{
	public:
		CSTeamCallTogether();
		MessageHeader	header;
	};

	// 队长召集队员通知
	class SCTeamCallTogether
	{
	public:
		SCTeamCallTogether();
		MessageHeader	header;
		
		int				scene_id;
		short			pos_x;
		short			pos_y;
	};

	// 招募队员
	class CSTeamCallIn
	{
	public:
		CSTeamCallIn();
		MessageHeader	header;

		int				fb_id;
	};

	// 招募队员广播
	class SCTeamCallIn
	{
	public:
		SCTeamCallIn();
		MessageHeader	header;
		
		int				team_index;
		int				role_id;
		GameName		role_name;
		short			camp;
		short			level;
		int				capability;
		int				limit_capability;
		int				limit_level;
		int				fb_id;
	};

	// 开始或结束打坐休息
	class SCRestStatus
	{
	public:
		SCRestStatus();
		MessageHeader	header;

		ObjID			obj_id;
		short			start_or_stop;	// 1 开始， 0结束

		ObjID			partner_obj_id;	// 双修伙伴的obj_id
		short			reserved;
	};

	// 播放特效
	class SCObjEffectShow
	{
	public:
		SCObjEffectShow();
		MessageHeader	header;
		ObjID			obj_id;
		unsigned short	effect_id;
	};

	class SCSceneFbTimeOut	
	{
	public:
		SCSceneFbTimeOut();
		MessageHeader	header;

		unsigned int time_out;
	};

	struct SCTeamMemberPosItem
	{
		int				role_id;
		ObjID			obj_id;
		char			reserved;
		char			is_leave_scene;
		short			pos_x;
		short			pos_y;
		Dir				dir;
		Scalar			distance;
		int				move_speed;
	};

	class SCTeamMemberPos
	{
	public:
		SCTeamMemberPos();
		MessageHeader	header;

		SCTeamMemberPosItem member_pos;
	};

	class CSBonfireUseItem
	{
	public:
		CSBonfireUseItem();
		MessageHeader	header;

		short			item_type;
		short			item_index;
	};

	class SCBonfireWineResult
	{
	public:
		SCBonfireWineResult();
		MessageHeader	header;

		unsigned short	item_id;
		short			item_num;
	};

	class CSSetInactiveState
	{
	public:
		CSSetInactiveState();
		MessageHeader	header;

		short			is_inactive;
	};


	class SCCharaterChangeMoveSpeed
	{
	public:
		SCCharaterChangeMoveSpeed();
		MessageHeader	header;

		ObjID	obj_id;
		short	reserve;
		int		movespeed;
	};


	class SCRoleVisibleChange
	{
	public:
		SCRoleVisibleChange();
		MessageHeader	header;

		ObjID	obj_id;
		short	reserved;

		RoleAppearance appearance;
	};

	class SCSetInactiveState
	{
	public:
		SCSetInactiveState();
		MessageHeader	header;

		ObjID	obj_id;
		short	is_inactive;
	};

	class SCBonfireEffect
	{
	public:
		SCBonfireEffect();
		MessageHeader	header;

		int		fire_level;
		int		add_exp;
	};

	class CSFloatReq
	{
	public:
		CSFloatReq();
		MessageHeader header;

		int start_or_stop;
	};

	class SCFloatStatus
	{
	public:
		SCFloatStatus();
		MessageHeader header;

		int obj_id;
		int start_or_stop;
	};

	class CSEnterFB
	{
	public:
		CSEnterFB();
		MessageHeader header;

		short fb_type;
		short reserve;
		int param_1;
		int param_2;
		int param_3;
	};

	class CSAutoFB
	{
	public:
		CSAutoFB();
		MessageHeader header;

		short fb_type;
		short reserve;

		int param_1;
		int param_2;
		int param_3;
		int param_4;
	};

	class CSLeaveFB
	{
	public:
		CSLeaveFB();
		MessageHeader header;
	};

	class CSCancelMonsterStaticState		// 1154
	{
	public:
		CSCancelMonsterStaticState();
		MessageHeader header;
	};

	class SCUserSceneSkillStatus
	{
	public:
		SCUserSceneSkillStatus();
		MessageHeader		header;
	};

	class SCPerformSceneSkill
	{
	public:
		SCPerformSceneSkill();
		MessageHeader		header;
		
		ObjID				obj_id;
		UInt16				scene_skill_id;
		short				target_x;
		short				target_y;
	};

	class CSGetAllObjMoveInfoReq
	{
	public:
		CSGetAllObjMoveInfoReq();
		MessageHeader header;
	};

	static const int SC_ALL_OBJ_MOVE_INFO_MAX = 1024;

	class SCAllObjMoveInfo
	{
	public:
		SCAllObjMoveInfo();
		MessageHeader header;

		struct ObjMoveInfo
		{
			ObjID obj_id;
			short obj_type;
			int type_special_id;
			Dir dir;
			Scalar distance;
			short pos_x;
			short pos_y;
			int move_speed;
			int monster_key;
		};

		int count;
		ObjMoveInfo all_obj_moveinfo_list[SC_ALL_OBJ_MOVE_INFO_MAX];
	};

	class SCObjMoveMode
	{
	public:
		SCObjMoveMode();
		MessageHeader	header;

		ObjID			obj_id;
		char			move_mode;
		char			move_mode_param;
	};

	class SCRoleAccetpTaskAppearn
	{
	public:
		SCRoleAccetpTaskAppearn();
		MessageHeader	header;

		ObjID			obj_id;
		char			task_appearn;
		char			reserve;
		int				task_appearn_param_1;
	};

	class SCFBFollowNPCInfo
	{
	public:
		SCFBFollowNPCInfo();
		MessageHeader	header;

		ObjID			obj_id;
		UInt16			fb_follow_npc_id;
	};

	class SCTriggerNotify
	{
	public:
		SCTriggerNotify();
		MessageHeader	header;

		UInt16			trigger_id;
		ObjID			trigger_role_objid;
	};

	class CSPickBigCoin
	{
	public:
		CSPickBigCoin();
		MessageHeader		header;

		ObjID				obj_id;
		short				reserved;
	};

	class SCPickBigCoinSucc
	{
	public:
		SCPickBigCoinSucc();
		MessageHeader		header;

		ObjID				obj_id;
		short				reserved;
	};

	class SCChangeSceneNotify
	{
	public:
		SCChangeSceneNotify();
		MessageHeader	header;

		int				change_scene_reason;
		int				target_scene_id;
	};

	class SCYanhuaExplode
	{
	public:
		SCYanhuaExplode();
		MessageHeader	header;

		ObjID			obj_id;
		short			reserved;
	};

	class CSReqTeamMemberPos  // 1164
	{
	public:
		CSReqTeamMemberPos();
		MessageHeader		header;
	};

	class SCTeamMemberPosList // 1147
	{
	public:
		SCTeamMemberPosList();
		MessageHeader		header;

		int member_count;
		SCTeamMemberPosItem member_list[MAX_TEAM_MEMBER_NUM];
	};

	class SCBossDpsFlag
	{
	public:
		SCBossDpsFlag();
		MessageHeader		header;

		ObjID role_obj_id;        //归属者objid
		ObjID monster_obj_id;
		GameName owner_user_name;			// 归属玩家名字
		GuildName guild_name;				// 仙盟名字
	};

	class CSChangeSceneLineReq
	{
	public:
		CSChangeSceneLineReq();
		MessageHeader		header;

		int scene_key;
	};

	class SCGatherBeGather			// 采集物被采集信息广播 1167
	{
	public:
		SCGatherBeGather();
		MessageHeader header;

		ObjID gather_role_obj_id;
		ObjID gather_obj_id;

		int gather_id;
		short left_gather_times;			// 剩余采集次数
		short reserve_sh;
	};

	class CSObjFlyHeightChange          // 1171 改变飞行高度请求
	{
	public:
		CSObjFlyHeightChange();
		MessageHeader  header;

		short      fly_height;
		short      reserve_sh;
	};

	class SCObjFlyHeightChange          // 1172 广播飞行高度改变
	{
	public:
		SCObjFlyHeightChange();
		MessageHeader  header;

		ObjID      obj_id;
		short      fly_height;
	};

	struct SCBossFirstHurtInfo	// 1173
	{
		SCBossFirstHurtInfo();
		MessageHeader header;

		ObjID obj_id;
		short first_hurt_flag;
	};

	class SCMonsterFirstHitInfo			//1174
	{
	public:
		SCMonsterFirstHitInfo();
		MessageHeader header;

		ObjID obj_id;						//怪物对象id
		short is_show;						//是否显示第一刀玩家名字
		ObjID first_hit_user_obj_id;		//第一刀玩家对象id
		short reserve_sh;					//保留
		GameName first_hit_user_name;		// 第一刀玩家名字
		GuildName guild_name;				// 仙盟名字
	};

	struct SCBossDpsTitle // 1175
	{
		SCBossDpsTitle();
		MessageHeader header;

		ObjID obj_id;
		short first_hurt_flag;
	};

	//////////////////////////////////////////////////////////////////////////
	
	struct CSRoleReturnReAlivePosi			// 1176 请求设置主角回复活点（客户端脱离卡死）
	{
		CSRoleReturnReAlivePosi();
		MessageHeader header;
	};
}

#pragma pack(pop)

#endif

