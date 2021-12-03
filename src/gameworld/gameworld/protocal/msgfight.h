
#ifndef MSGFIGHT_H
#define MSGFIGHT_H

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"

#include "skill/skilldef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		战斗相关
	*/
	
	// 释放技能
	class CSPerformSkillReq
	{
	public:
		CSPerformSkillReq();
		MessageHeader	header;

		int				skill_index;				// 在技能槽中的位置
		Axis			pos_x;						// AOE技能 X
		Axis			pos_y;						// AOE技能 Y
		ObjID			target_id;					// 目标对象id

		short			is_specialskill;			// 是否特殊技能
		short			client_pos_x;
		short			client_pos_y;
		int				skill_data;
	};

	// 释放QTE技能
	class CSPerformQTESkillReq
	{
	public:
		CSPerformQTESkillReq();
		MessageHeader	header;

		int				right;
	};

	// 开始技能CD
	class CSSkillCDReq
	{
	public:
		CSSkillCDReq();
		MessageHeader	header;

		short			skill_index;
		short			reserve;
	};

	// 播放普通技能
	class SCPerformSkill
	{
	public:
		SCPerformSkill();
		MessageHeader	header;

		ObjID			character;
		ObjID			target;
		UInt16			skill;
		short			reserved;
		int				skill_data;
	};

	// 播放AOE技能
	class SCPerformAOESkill
	{
	public:
		SCPerformAOESkill();
		MessageHeader	header;

		ObjID			character;
		UInt16			skill;
		Axis			pos_x;
		Axis			pos_y;
		short			aoe_reason;
		ObjID			target;
		int				skill_data;
	};

	// 播放技能施法阶段
	class SCSkillPhase
	{
	public:
		SCSkillPhase();
		MessageHeader	header;

		ObjID			character;
		UInt16			skill;
		int				phase;
	};

	// 播放技能提示
	class SCSkillTip
	{
	public:
		SCSkillTip();
		MessageHeader	header;

		ObjID			character;
		UInt16			skill;
	};

	//  战斗特殊飘字（如有防护罩被打时飘吸收的伤害）
	class SCFightSpecialFloat				// 1318
	{
	public:
		SCFightSpecialFloat();
		MessageHeader	header;

		enum FLOAT_VALUE_TYPE
		{
			EFFECT_HPSTORE = 0,					// EffectHpStore抵挡的伤害值
			EFFECT_UP_GRADE_SKILL,				// 进阶系统技能伤害
			EFFECT_REBOUNDHURT,					// 反弹伤害
			EFFECT_RESTORE_HP,					// 回血飘字
			EFFECT_NORMAL_HURT,					// 通用伤害飘字

			EFFECT_JUST_SPECIAL_EFFECT,			// 仅仅播放特效，不需要飘字
		};

		enum ATTATCH_SKILL_SPECIAL_EFFECT		// 飘血附加技能特效
		{
			SPECIAL_EFFECT_NON = 0,
			SPECIAL_EFFECT_MOUNT,				// 坐骑-雷电
			SPECIAL_EFFECT_WING,				// 羽翼-陨石
			SPECIAL_EFFECT_HALO,				// 光环-火龙卷
			SPECIAL_EFFECT_SHENGONG,			// 神弓-雷神锤
			SPECIAL_EFFECT_WATER_SHENYI,		// 神翼-水龙卷
			SPECIAL_EFFECT_FIGHT_MOUNT,			// 战斗坐骑-剑
			SPECIAL_EFFECT_FOOTPRINT,			// 足迹-
			SPECIAL_EFFECT_FABAO,			    // 法宝-装备技能

			SPECIAL_EFFECT_XIANNV_SHENGWU_RESTORE_HP = 20,	// 仙女圣物技能 - 回血
			SPECIAL_EFFECT_XIANNV_SHENGWU_HURT,				// 仙女圣物技能 - 直接伤害技能

			SPECIAL_EFFECT_JINGLING_REBOUNDHURT = 30,		// 精灵反弹伤害

			SPECIAL_EFFECT_REBOUNDHURT = 40,				// 反弹伤害
			
			SPECIAL_EFFECT_IMG_FULING_MOHUA = 50,			// 形象赋灵-魔化

			SPECIAL_EFFECT_MAX,
		};
		
		ObjID obj_id;
		short float_type;
		Attribute float_value;
		
		short skill_special_effect;
		ObjID deliver_obj_id;
	};

	class SCUpGradeSkillInfo
	{
	public:
		SCUpGradeSkillInfo();
		MessageHeader	header;

		int upgrade_next_skill;
		int upgrade_cur_calc_num;
	};

	// 播放技能结果（包括技能效果和加减血等）
	class SCObjChangeBlood				// 1300
	{
	public:
		SCObjChangeBlood();
		MessageHeader	header;
		ObjID			obj_id;
		ObjID			deliverer;
		UInt16			skill;
		unsigned char	fighttype;		// 使用FIGHT_TYPE表示
		unsigned char	product_method;	// 使用PRODUCT_METHOD表示
		Attribute		real_blood;		// 实际的伤害
		Attribute		blood;			// 显示用的伤害
		int				passive_flag;	// 被动技能标记
	};

	class SCSpecialShieldChangeBlood   // 1319  广播世界BOSS护盾次数变化
	{
	public:
		SCSpecialShieldChangeBlood();
		MessageHeader	header;

		ObjID			obj_id;
		short			reserve_sh;
		long long		real_hurt;		    // 实际的伤害
		int				left_times;			// 护盾剩余次数
		int				max_times;			// 护盾总次数
	};

	// 通知技能效果（仅用于播放特效）
	class SCSkillEffect
	{
	public:
		SCSkillEffect();
		MessageHeader	header;

		static const int MAX_RECEIVER_COUNT = 4;

		ObjID			deliverer;
		UInt16			skill;

		ObjID			receiver_obj_list[MAX_RECEIVER_COUNT];
	};

	// 技能目标位置修正
	class SCSkillTargetPos
	{
	public:
		SCSkillTargetPos();
		MessageHeader	header;

		ObjID			target_obj_id;
		short			reserve;
		int				pos_x;
		int				pos_y;
	};

	// 广播魔法变动消息
	class SCObjChangeMP				// 1304
	{
	public:
		SCObjChangeMP();
		MessageHeader	header;
		ObjID			obj_id;
		short			reserved;
		Attribute		mp;
	};

	class CSRoleReAliveReq
	{
	public:
		enum REALIVE_TYPE
		{
			REALIVE_TYPE_BACK_HOME = 0,		// 回城复活
			REALIVE_TYPE_HERE_GOLD,			// 使用元宝原地复活
			REALIVE_TYPE_HERE_ICON,			// 使用铜钱原地复活	
		};

		CSRoleReAliveReq();
		MessageHeader	header;

		char			realive_type;
		char			is_timeout_req;
		short			item_index;
	};

	class SCRoleReAlive
	{
	public:
		SCRoleReAlive();
		MessageHeader	header;
		short			reserved;
		ObjID			obj_id;
		Axis			pos_x;
		Axis			pos_y;
	};

	class SCBuffMark
	{
	public:
		SCBuffMark();
		MessageHeader	header;
		ObjID			obj_id;
		short			reserved;
		long long		buff_mark;
	};

	class SCBuffAdd
	{
	public:
		SCBuffAdd();
		MessageHeader	header;
		ObjID			obj_id;
		short			buff_type;
	};

	class SCBuffRemove
	{
	public:
		SCBuffRemove();
		MessageHeader	header;
		ObjID			obj_id;
		short			buff_type;
	};

	class SCEffectList					// 1307
	{
	public:
		SCEffectList();
		MessageHeader	header;

		static const int EFFECT_ITEM_MAX = 64;

		ObjID			obj_id;
		short			count;
		EffectInfo		effect_info_list[EFFECT_ITEM_MAX];
	};

	class SCBianShenView								// 1320变身effect形象广播
	{
	public:
		SCBianShenView();
		MessageHeader header;

		ObjID obj_id;
		short bianshen_image;
		int obj_id_des;
		int model_size;
	};

	// Buff 效果信息
	class SCEffectInfo						// 1308
	{
	public:
		SCEffectInfo();
		MessageHeader	header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_ADD,
			NOTIFY_REASON_CHANGE,
		};

		short			notify_reason;
		ObjID			obj_id;
		EffectInfo		effect_info;
	};

	class SCEffectRemove
	{
	public:
		SCEffectRemove();
		MessageHeader	header;

		unsigned int	effect_key;
	};
	
	class SCItemEffectPackageCD
	{
	public:
		SCItemEffectPackageCD();
		MessageHeader	header;

		int				buff_type;
		int				cooldown_time;
	};

	class CSGetEffectListReq
	{
	public:
		CSGetEffectListReq();
		MessageHeader	header;

		ObjID			target_obj_id;
		short			reserved;
	};

	class CSGetEffectInfoReq
	{
	public:
		CSGetEffectInfoReq();
		MessageHeader	header;

		ObjID			target_obj_id;
		char			effect_type;
		char			reserve;
		unsigned int	effect_key;
	};

	class SCExplode
	{
	public:
		SCExplode();
		MessageHeader	header;

		enum EXPLODE_REASON
		{
			EXPLODE_REASON_ZHUOSHAO = 0,				// 灼烧
			EXPLODE_REASON_JUHUAPIG,					// 菊花猪
		};

		short			explode_reason;
		short			reserve;
		Axis			pos_x;
		Axis			pos_y;
	};

	class SCReAliveWaiting
	{
	public:
		SCReAliveWaiting();
		MessageHeader		header;

		ObjID				objid;
		short				reserved;
		int					waiting_time_ms;
	};

	class SCQTEReady
	{
	public:
		SCQTEReady();
		MessageHeader		header;

		ObjID				objid;
		short				suit_id;
	};

	class SCContinueKillInfo
	{
	public:
		SCContinueKillInfo();
		MessageHeader		header;

		int					kill_count;
		unsigned int        trigger_continue_kill_invalid_timestamp;
	};

	class SCInvisibleView		// 1323 广播隐身信息
	{
	public:
		SCInvisibleView();
		MessageHeader header;

		ObjID obj_id;
		short is_invisible;
	};
}

#pragma pack(pop)

#endif
