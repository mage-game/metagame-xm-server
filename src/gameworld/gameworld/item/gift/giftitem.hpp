#ifndef __GIFT_ITEM_H__
#define __GIFT_ITEM_H__

#include "item/itembase.h"
#include "item/itemextern.hpp"

class GiftItem : public ItemBase
{
public:
	static const int GIFT_ITEM_MAX_NUM = 40;

	enum GIFT_TYPE
	{
		GIFT_TYPE_INVALID,

		GIFT_TYPE_DEF,							// �̶����
		GIFT_TYPE_RAND,							// ������
		GIFT_TYPE_SELECT,						// ѡ�����

		GIFT_TYPE_MAX,
	};

	enum IS_BOX_TYPE
	{
		IS_BOX_TYPE_INVALID = -1,

		IS_BOX_TYPE_SAME_AS_THE_GIFT,			//��������Ʒ�����������������Ӱ��,���Ҳ�֪ͨ�ͻ���
		IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG,	//��������Ʒ�����Բ������������Ӱ��,��֪ͨ�ͻ���

		IS_BOX_TYPE_MAX,
	};

	GiftItem(short sub_type) : ItemBase(I_TYPE_GIFT), m_sub_type(sub_type), m_coin_bind(0), m_gold_bind(0), m_gold(0), m_is_check_prof(false), m_item_num(0), m_day_usetimes(0), is_box(0) {}
	virtual ~GiftItem() {}
	short GetSubType(){ return m_sub_type; }

protected:
	short m_sub_type;
	int m_coin_bind;							// ��Ǯ��
	int m_gold_bind;							// ��Ԫ��
	int m_gold;									// Ԫ��
	bool m_is_check_prof;						// �Ƿ�����ְҵ
	short m_item_num;							// ��������
	short m_day_usetimes;
	int is_box;									// �Ƿ�Ϊ����. 1��������Ʒ�����Բ������������Ӱ�죬 0��������Ʒ�����������������Ӱ��
	ItemConfigData m_gift_item_list[GIFT_ITEM_MAX_NUM];
};

#endif //__GIFT_ITEM_H__
