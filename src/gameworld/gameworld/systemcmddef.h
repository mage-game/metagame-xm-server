#ifndef __SYSTEM_CMD_DEF_HPP__
#define __SYSTEM_CMD_DEF_HPP__

namespace systemcmd
{
	enum config_type				// ���ñ����͵Ķ���
	{
		CONF_NONE = 0,
		CONF_TASK = 1,				// ��������
		CONF_ITEM_GIFT = 2,			// ��Ʒ�������
		CONF_COMMONACTIVITY = 3,	// ͨ�û����
	};

	enum EffectType					// �Խ�ɫ���������Ͷ���
	{
		EFFECT_AUTHORITY = 0,		// �ı����Ȩ��
		EFFECT_USE_GOLD,			// ����Ԫ��
		EFFECT_USE_GOLD_BIND,		// ���İ�Ԫ��
		EFFECT_USE_COIN_BIND,		// ����ͭ��
		EFFECT_ADD_HISTORY_CHONGZHI,// �����ۼƳ�ֵ��
		EFFECT_SET_CHARM,			// ��������ֵ
		EFEECT_ADD_UNIQUE_TITLE,	// ����Ψһ�ƺ�
		EFFECT_COMPLETE_TASK,		// �������
		EFFECT_SET_LEVEL,			// ���õȼ�
		EFFECT_CONSUME_ITEM,		// ������Ʒ
	};

	enum RoleCountType
	{
		RCT_INVALID = 0,
		RCT_TUMO_TASK,				// ��ħ����
		RCT_HUSONG_TASK,			// ��������

		RCT_MAX,
	};
}

#endif

