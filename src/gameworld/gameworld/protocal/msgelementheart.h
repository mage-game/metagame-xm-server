#ifndef __MSG_ELEMENT_HEART_HPP__
#define __MSG_ELEMENT_HEART_HPP__

#include "servercommon/elementheartdef.hpp"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenzhuangdef.hpp"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum ELEMENT_HEART_REQ_TYPE											// ��������
	{
		ELEMENT_HEART_REQ_TYPE_ACTIVE_GHOST,							// ����Ԫ��֮�� param1 Ԫ��֮��id
		ELEMENT_HEART_REQ_TYPE_CHANGE_GHOST_WUXING_TYPE,				// �ı�Ԫ��֮������ param1 Ԫ��֮��id
		ELEMENT_HEART_REQ_TYPE_FEED_ELEMENT,							// ι��Ԫ��֮��	param1 Ԫ��֮��id param2 ������Ʒid param3 ��Ʒ����
		ELEMENT_HEART_REQ_TYPE_UPGRADE_GHOST,							// Ԫ��֮�Ľ���	param1 Ԫ��֮��id param2 �Ƿ�һ��  param3 �Ƿ��Զ�����
		ELEMENT_HEART_REQ_TYPE_GET_PRODUCT,								// Ԫ��֮�Ĳɼ�	param1 Ԫ��֮��id 
		ELEMENT_HEART_REQ_TYPE_PRODUCT_UP_SEED,							// Ԫ��֮�Ĳ�������	param1 Ԫ��֮��id 
		ELEMENT_HEART_REQ_TYPE_UPGRADE_CHARM,							// Ԫ��֮������ para1 ����Ԫ��֮���±� param2 ���ĸ����±�
		ELEMENT_HEART_REQ_TYPE_ALL_INFO,								// ����������Ϣ
		ELEMENT_HEART_REQ_TYPE_CHOUJIANG,								// Ԫ��֮�ĳ齱 param1 ����
		ELEMENT_HEART_REQ_TYPE_FEED_GHOST_ONE_KEY,						// һ��ι��Ԫ��֮��	param1 id
		ELEMENT_HEART_REQ_TYPE_SET_GHOST_WUXING_TYPE,					// ����Ԫ��֮������	param1 id
		ELEMENT_HEART_REQ_TYPE_SHOP_REFRSH,								// �̵�ˢ�� param 1�Ƿ�ʹ�û���ˢ��
		ELEMENT_HEART_REQ_TYPE_SHOP_BUY,								// �̳ǹ��� param 1 ��Ʒseq
		ELEMENT_HEART_REQ_TYPE_XILIAN,									// ϴ�� param1 Ԫ��id�� param2 ��ϴ��־ param3ϴ����ɫ�� param4 �Ƿ��Զ�����
		ELEMENT_HEART_REQ_TYPE_PUTON_EQUIP,								// ��װ�� param1Ԫ��id param2װ������
		ELEMENT_HEART_REQ_TYPE_UPGRADE_EQUIP,							// װ������ Parma1 Ԫ��id param2 �Ƿ�һ������
		ELEMENT_HEART_REQ_TYPE_EQUIP_RECYCLE,							// װ���ֽ� param1 �������� param 2 ��������
	};

	class CSElementHeartReq						// 8454 ��������
	{
	public:
		CSElementHeartReq();
		MessageHeader header;

		int info_type;						// ��Ӧ ELEMENT_HEART_REQ_TYPE ����
		int param1;
		int param2;
		int param3;
		int param4;
	};

	class SCElementHeartInfo					// Ԫ��֮����Ϣ	8455											
	{
	public:
		enum INFO_TYPE
		{
			INFO_TYPE_SINGLE_ELEMENT,			// ������Ϣ
			INFO_TYPE_ALL_ELEMENT,				// ȫ����Ϣ
			INFO_TYPE_WUXING_CHANGE,			// ����ת����Ϣ
		};
	
		SCElementHeartInfo();
		MessageHeader header;

		int pasture_score;
		char info_type;	
		char free_chou_times;
		short count;
		ElementSingle element_heart_list[ELEMENT_HEART_MAX_COUNT]; // Ԫ��֮���б�
	};
		
	class SCElementTextureInfo						// Ԫ��֮���б���Ϣ 8456
	{
	public:
		SCElementTextureInfo();
		MessageHeader header;
		
		ElementTextureParam charm_list[SHENZHUANG_MAX_PART];
	};
	
	class SCElemnetHeartSingleCharmInfo				// ����Ԫ��֮����Ϣ 8457
	{
	public:
		SCElemnetHeartSingleCharmInfo();
		MessageHeader header;

		int e_index;								// Ԫ��֮������װ���±�
		ElementTextureParam charm_info;				// Ԫ��֮����Ϣ
	};

	struct ElementHeartChouRewardParam
	{
		short v_item_id;
		short num;
		char is_bind;
		char reserve_ch;
		short reserve_sh;
	};

	struct ElementChouRewardItem
	{
		ItemID item_id;
		char num;
		char is_bind;
	};

	class SCElementHeartChouRewardListInfo			// �齱��Ʒ	8458										
	{
	public:
		SCElementHeartChouRewardListInfo();
		MessageHeader header;

		short free_chou_times;
		short count;
		ElementChouRewardItem reward_list[ELEMENT_HEART_MAX_CHOU_TIMES_MAX];
	};

	class SCElementProductListInfo				// �����б�	8459								
	{
	public:
		const static int MAX_PRODUCT_LIST_COUNT = 10;
		UNSTD_STATIC_CHECK(MAX_PRODUCT_LIST_COUNT % 2 == 0)

		SCElementProductListInfo();
		MessageHeader header;

		short info_type;
		short count;
		unsigned short product_list[MAX_PRODUCT_LIST_COUNT];
	};

	class SCElementShopInfo															// �̵���Ϣ  8460
	{
	public:
		SCElementShopInfo();
		MessageHeader header;

		unsigned int next_refresh_timestamp;
		short reserve_sh;
		short today_shop_flush_times;												// �����̵�ˢ�´���
		ElementHeartShopItem shop_item_list[ELEMENT_SHOP_ITEM_COUNT];				// �̵굱ǰˢ�³�������Ʒ�б�
	};

	class SCElementXiLianSingleInfo			// 8461
	{
	public:
		SCElementXiLianSingleInfo();
		MessageHeader header;

		int element_id;
		ElementHeartXiLianParam xilian_info;
	};

	class SCElementXiLianAllInfo			// 8462
	{
	public:
		SCElementXiLianAllInfo();
		MessageHeader header;

		ElementHeartXiLianParam xilian_list_info[ELEMENT_HEART_MAX_COUNT];
	};
}

#pragma pack(pop)

#endif	//__MSG_ELEMENT_HEART_HPP__
