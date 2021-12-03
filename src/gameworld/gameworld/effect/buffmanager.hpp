#ifndef __BUFFMANAGER_HPP__
#define __BUFFMANAGER_HPP__

#include "servercommon/servercommon.h"

static const int SPECIAL_BUFF_MAX_BYTE = 16;

enum EFFECT_BUFF_TYPE
{
	EBT_INVALID = 0,

	EBT_XUANYUN = 1,										// ѣ��
	EBT_DINGSHEN = 2,										// ����
	EBT_CHENMO = 3,											// ��Ĭ
	EBT_CHIHUAN = 4,										// �ٻ�

	EBT_MIANYI_XUANYUN = 5,									// ����ѣ��
	EBT_MIANYI_DINGSHEN = 6,								// ���߶���
	EBT_MIANYI_CHENMO = 7,									// ���߳�Ĭ
	EBT_MIANYI_CHIHUAN = 8,									// ���߳ٻ�

	EBT_WUDI = 9,											// �޵�
	EBT_JIASU = 10,											// ����
	EBT_INVISIBLE = 11,										// ����
	EBT_NUQI_BAOFA = 12,									// ŭ������
	EBT_FIER = 13,											// ��

	EBT_REDUCE_GONGJI = 14,									// ���͹���
	EBT_REDUCE_FANGYU = 15,									// ���ͷ���
	EBT_XIANNV_ADD_GONGJI = 16,								// ���ӹ���-Ů��������Ч
	EBT_XIANNV_ADD_FANGYU = 18,								// ���ӷ���-Ů��������Ч
	EBT_XIANNV_ADD_BAOJI = 19,								// Ů�񱩻�-Ů��������Ч
	EBT_WING_FENGDUN = 20,									// �����ܼ�����Ч
	EBT_WING_JIEJIE = 21,									// �����缼����Ч
	EBT_WING_SHOUHU = 22,									// �����ػ�������Ч
	EBT_SHENYI_JIUSHU = 23,									// �����ܾ���
	EBT_SHENYI_SHENPAN = 24,								// ����������
	EBT_SHENGGONG_ZHUMO = 25,								// �񹭼�����ħ
	EBT_REBOUNDHURT = 26,									// ����BUFF
	EBT_ADD_FANGYU = 27,									// ���ӷ���
	EBT_PINK_EQUIP_NARROW = 28,								// ��ɫװ��-��С
	EBT_ADD_GONGJI = 29,									// ���ӹ���

	EBT_SPECIAL_EFFECT_MAX = 30,							// ������⼼��buff

	EBT_SPECIAL_OTHER_BEGIN = 40,
	EBT_SUPER_MIANYI = EBT_SPECIAL_OTHER_BEGIN,				// ���߶������ԣ���Ĭ������ 
	EBT_HPSTORE = 41,										// ����
	EBT_WUDI_PROTECT = 42,									// �޵б���
	EBT_BIAN_XING = 43,										// ���� ---- �ɹ���
	EBT_BIANXING_FOOL = 44,									// ���� ---- ���ɹ���
	EBT_DEC_FANGYU = 45,									// ���ٷ���
	EBT_DEC_HURT = 46,										// �����˺�
	EBT_BE_HURT_ADD = 47,									// �ܵ��˺�����
	EBT_MOJIESTORE = 48,									// ħ�似�ܻ�����Ч
	EBT_TERRITORYWAR_TOWER_WUDI = 49,						// ����ս�������޵�
	EBT_DROP_BLOOD = 50,									// ��Ѫ
	EBT_ADD_BLOOD = 51,										// ��Ѫ
	EBT_FROZEN = 52,										// ����
	EBT_DEADLY_ATTACK = 53,									// ����һ��
	EBT_MOHUA = 54,											// ������-ħ��
	EBT_TALENT_MOUNT_SKILL = 55,							// �츳����-����
	EBT_TALENT_WING_SKILL = 56,								// �츳����-����
	EBT_ADD_MULTI_ATTR = 57,								// ��������
	EBT_FORBID_RECOVER_HP = 58,								// ��ֹ��Ѫ
	EBT_DEC_CONTROL_BUFF_TIME = 59,							// ���ٿ���debuff�Ŀ���ʱ��
	EBT_ZHUANZHI_ADD_SHANBI = 60,							// תְ���ܼ�����
	EBT_ZHUANZHI_ADD_SHANGHAI = 61,							// תְ���ܼ��˺�
	EBT_ZHUANZHI_ABSORB_HP = 62,							// תְ������Ѫ
	EBT_DROP_BLOOD_POISON = 63,								// ��Ů�����ж�ʧѪ
	EBT_XIANNV_RECOVER_HP = 64,								// ��Ů���ָܻ�Ѫ��
	EBT_MOJIE_WUDI = 65,									// ħ�似���޵�
	EBT_MOJIE_RESTORE_HP = 66,								// ħ�似�ܻ�Ѫ
	EBT_MOJIESKILL_THUNDER = 67,							// ���������׵�
	EBT_MOJIESKILL_RESTORE_HP = 68,							// ����������Ѫ
	EBT_MOJIESKILL_SLOW = 69,								// ������������
	EBT_HUGUOZHILI = 70,									// ������
	EBT_INVISIBLE_HURT_ABLE = 71,							// ����[�ɹ���]

	EBT_SPECIAL_OTHER_END = 72,

	// ���϶������ڴ�������ʾ�� bu������Ҫ��ʾ�ļ����±�

	EBT_ACTIVITY_BUFF_MIN = 160,						// �buff��Сֵ

	EBT_ACTIVITY_BUFF_MAX,								// �buff���ֵ
	
	EBT_MAX,
};

UNSTD_STATIC_CHECK(EBT_SPECIAL_EFFECT_MAX <= 34);


#endif
