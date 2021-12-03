
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
		ս�����
	*/
	
	// �ͷż���
	class CSPerformSkillReq
	{
	public:
		CSPerformSkillReq();
		MessageHeader	header;

		int				skill_index;				// �ڼ��ܲ��е�λ��
		Axis			pos_x;						// AOE���� X
		Axis			pos_y;						// AOE���� Y
		ObjID			target_id;					// Ŀ�����id

		short			is_specialskill;			// �Ƿ����⼼��
		short			client_pos_x;
		short			client_pos_y;
		int				skill_data;
	};

	// �ͷ�QTE����
	class CSPerformQTESkillReq
	{
	public:
		CSPerformQTESkillReq();
		MessageHeader	header;

		int				right;
	};

	// ��ʼ����CD
	class CSSkillCDReq
	{
	public:
		CSSkillCDReq();
		MessageHeader	header;

		short			skill_index;
		short			reserve;
	};

	// ������ͨ����
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

	// ����AOE����
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

	// ���ż���ʩ���׶�
	class SCSkillPhase
	{
	public:
		SCSkillPhase();
		MessageHeader	header;

		ObjID			character;
		UInt16			skill;
		int				phase;
	};

	// ���ż�����ʾ
	class SCSkillTip
	{
	public:
		SCSkillTip();
		MessageHeader	header;

		ObjID			character;
		UInt16			skill;
	};

	//  ս������Ʈ�֣����з����ֱ���ʱƮ���յ��˺���
	class SCFightSpecialFloat				// 1318
	{
	public:
		SCFightSpecialFloat();
		MessageHeader	header;

		enum FLOAT_VALUE_TYPE
		{
			EFFECT_HPSTORE = 0,					// EffectHpStore�ֵ����˺�ֵ
			EFFECT_UP_GRADE_SKILL,				// ����ϵͳ�����˺�
			EFFECT_REBOUNDHURT,					// �����˺�
			EFFECT_RESTORE_HP,					// ��ѪƮ��
			EFFECT_NORMAL_HURT,					// ͨ���˺�Ʈ��

			EFFECT_JUST_SPECIAL_EFFECT,			// ����������Ч������ҪƮ��
		};

		enum ATTATCH_SKILL_SPECIAL_EFFECT		// ƮѪ���Ӽ�����Ч
		{
			SPECIAL_EFFECT_NON = 0,
			SPECIAL_EFFECT_MOUNT,				// ����-�׵�
			SPECIAL_EFFECT_WING,				// ����-��ʯ
			SPECIAL_EFFECT_HALO,				// �⻷-������
			SPECIAL_EFFECT_SHENGONG,			// ��-����
			SPECIAL_EFFECT_WATER_SHENYI,		// ����-ˮ����
			SPECIAL_EFFECT_FIGHT_MOUNT,			// ս������-��
			SPECIAL_EFFECT_FOOTPRINT,			// �㼣-
			SPECIAL_EFFECT_FABAO,			    // ����-װ������

			SPECIAL_EFFECT_XIANNV_SHENGWU_RESTORE_HP = 20,	// ��Ůʥ�＼�� - ��Ѫ
			SPECIAL_EFFECT_XIANNV_SHENGWU_HURT,				// ��Ůʥ�＼�� - ֱ���˺�����

			SPECIAL_EFFECT_JINGLING_REBOUNDHURT = 30,		// ���鷴���˺�

			SPECIAL_EFFECT_REBOUNDHURT = 40,				// �����˺�
			
			SPECIAL_EFFECT_IMG_FULING_MOHUA = 50,			// ������-ħ��

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

	// ���ż��ܽ������������Ч���ͼӼ�Ѫ�ȣ�
	class SCObjChangeBlood				// 1300
	{
	public:
		SCObjChangeBlood();
		MessageHeader	header;
		ObjID			obj_id;
		ObjID			deliverer;
		UInt16			skill;
		unsigned char	fighttype;		// ʹ��FIGHT_TYPE��ʾ
		unsigned char	product_method;	// ʹ��PRODUCT_METHOD��ʾ
		Attribute		real_blood;		// ʵ�ʵ��˺�
		Attribute		blood;			// ��ʾ�õ��˺�
		int				passive_flag;	// �������ܱ��
	};

	class SCSpecialShieldChangeBlood   // 1319  �㲥����BOSS���ܴ����仯
	{
	public:
		SCSpecialShieldChangeBlood();
		MessageHeader	header;

		ObjID			obj_id;
		short			reserve_sh;
		long long		real_hurt;		    // ʵ�ʵ��˺�
		int				left_times;			// ����ʣ�����
		int				max_times;			// �����ܴ���
	};

	// ֪ͨ����Ч���������ڲ�����Ч��
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

	// ����Ŀ��λ������
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

	// �㲥ħ���䶯��Ϣ
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
			REALIVE_TYPE_BACK_HOME = 0,		// �سǸ���
			REALIVE_TYPE_HERE_GOLD,			// ʹ��Ԫ��ԭ�ظ���
			REALIVE_TYPE_HERE_ICON,			// ʹ��ͭǮԭ�ظ���	
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

	class SCBianShenView								// 1320����effect����㲥
	{
	public:
		SCBianShenView();
		MessageHeader header;

		ObjID obj_id;
		short bianshen_image;
		int obj_id_des;
		int model_size;
	};

	// Buff Ч����Ϣ
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
			EXPLODE_REASON_ZHUOSHAO = 0,				// ����
			EXPLODE_REASON_JUHUAPIG,					// �ջ���
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

	class SCInvisibleView		// 1323 �㲥������Ϣ
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
