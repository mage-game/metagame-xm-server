#ifndef __DROP_CONFIG_HPP__
#define __DROP_CONFIG_HPP__

#include <string>

#include "servercommon/basedef.h"
#include "item/itemextern.hpp"

struct DropItemCfg
{
	DropItemCfg() : prob(0), broadcast(0) {}

	ItemConfigData item_config_data;										// ��Ʒ��Ϣ
	int prob;																// ��Ʒ����
	short broadcast;														// ��������
};

class DropConfig
{
public:
	static const int DROP_ITEM_PROB_MAX = 64;								// ������Ʒ���ø���
	static const int DROP_CERTAIN_PROB = -1;								// �ص���Ʒ�ĸ��� ��-1����ص���

	DropConfig();
	~DropConfig();

	bool LoadConfig(const char *path, std::string *err);
	
	UInt16 GetDropID() { return m_drop_id; }

	int RandDropItem(char prof, DropItemCfg item_list[DROP_ITEM_PROB_MAX]);
	int GetFixDropItem(int rand_count, DropItemCfg item_list[DROP_ITEM_PROB_MAX]);


	//������ID����ִ��N�ε��� �Ż��㷨  ֱ�Ӹ������ ���� ����1ֱ�Ӹ�һ��,������ִ����Ӻ�ĸ���
	int RandDropItemN(int n, DropItemCfg item_list[DROP_ITEM_PROB_MAX]);



private:
	UInt16 m_drop_id;														// ����ID

	int m_drop_item_total_count;											// ȫ����Ʒ���ø���
	int m_drop_item_prob_count;												// �и��ʵ���Ʒ���ø���
	DropItemCfg m_drop_item_prob_list[DROP_ITEM_PROB_MAX];					// �����б�
};

#endif // __DROP_CONFIG_HPP__

