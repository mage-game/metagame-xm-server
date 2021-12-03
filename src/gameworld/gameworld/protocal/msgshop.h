#ifndef MSGSHOP_H
#define MSGSHOP_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/basedef.h"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/roleshopdef.hpp"
#include "servercommon/mysteriousshopdef.hpp"
#include "servercommon/struct/converrecordparam.hpp"
#include "servercommon/mysteriousshopinmalldef.hpp"

#include "gamedef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		�̳�
	*/

	class CSShopBuy
	{
	public:
		CSShopBuy();
		MessageHeader		header;

		unsigned short item_id;
		unsigned short item_num;
		char	is_bind;			//	�Ƿ���ʹ�ð�Ԫ������
		char	if_use;				//  �Ƿ�ֱ��ʹ��
		char	reserve_ch1;		//	�Ƿ�������		
		char	reserve_ch2;
	};

	/////////////////////////////////////////////     �����̵�     /////////////////////////////////////////////
	class CSChestShopRecordList						// 4863��ȡѰ����¼ 1װ��Ѱ����2װ��Ѱ��, 3װ��Ѱ��, 4����Ѱ��
	{
	public:
		CSChestShopRecordList();
		MessageHeader header;

		short shop_type;
		short reserve;
	};

	class SCChestShopRecordList						// 4864Ѱ����¼����
	{
	public:
		SCChestShopRecordList();
		MessageHeader header;

		static const int MAX_RECORD_COUNT = 30;		// Ѱ����¼�����

		struct ListItem
		{
			ListItem() : item_id(0), reserve(0)
			{
				memset(role_name, 0, sizeof(role_name));
			}

			GameName role_name;
			ItemID item_id;
			short reserve;
		};

		short count;
		short record_type;
		ListItem record_list[MAX_RECORD_COUNT];
	};

	class CSGetSelfChestShopItemList				// 4850��ȡ�Լ������̵���Ʒ�б�
	{
	public:
		CSGetSelfChestShopItemList();
		MessageHeader header;

		short shop_type;
		short reserve;
	};

	class CSBuyChestShopItem						// 4851�����̵깺��
	{
	public:
		CSBuyChestShopItem();
		MessageHeader header;

		short shop_type;
		short mode;															
		short reserve_sh1;
		short reserve_sh2;
	};

	class CSFetchChestShopItem						// 4852��ȡ�����̵���Ʒ
	{
	public:
		CSFetchChestShopItem();
		MessageHeader header;
		
		short shop_type;
		short grid_index;
		short if_fetch_all;
		short reserve;
	};

	class CSChestShopGetFreeInfo					// 4856��ȡ�����̵����ʱ��		
	{
	public:
		CSChestShopGetFreeInfo();
		MessageHeader header;
	};

	class CSChestShopAutoRecycle					// 4858�����Զ�����
	{
	public:
		CSChestShopAutoRecycle();
		MessageHeader header;

		short shop_type;
		short color;
		short is_auto;
		short gird_index;
	};

	class SCSelfChestShopItemList					// 4800�����Լ������̵���Ʒ�б�
	{
	public:
		SCSelfChestShopItemList();
		MessageHeader header;

		short storehouse_type;						// �ֿ�����
		short reserve;
		int count;
		ChestItemInfo item_list[MAX_ROLE_CHEST_GRID];
	};

	class SCChestShopItemListPerBuy					// 4801����ÿ�ο�����õ�����Ʒ
	{
	public:
		SCChestShopItemListPerBuy();
		MessageHeader header;
		
		short shop_type;
		short mode;
		int count;
		ChestItemInfo item_list[MAX_ROLE_CHEST_GRID];
	};

	class SCChestShopFreeInfo						// 4802���ͱ��������Ϣ
	{
	public:
		SCChestShopFreeInfo();
		MessageHeader	header;

		unsigned int chest_shop_next_free_time_1;
		unsigned int chest_shop_1_next_free_time_1;
		unsigned int chest_shop_2_next_free_time_1;
		unsigned int chest_shop_jl_next_free_time_1;
	};

	class CSRareChestShopReq						// 4806�����챦�齱����
	{
	public:
		CSRareChestShopReq();
		MessageHeader header;

		int times;
	};

	/////////////////////////////////////////////     ������     /////////////////////////////////////////////
	class CSMoveChessResetReq						// 5221��������
	{
	public:
		CSMoveChessResetReq();
		MessageHeader header;
	};


	class CSMoveChessShakeReq						// 5222����ҡ����
	{
	public:
		CSMoveChessShakeReq();
		MessageHeader header;

		short is_use_item;							// 1ʹ����Ʒ��0��ʹ��
		short reserve;

	};

	class SCMoveChessShakePoint						// 5224ҡ����ҡ������
	{
	public:
		SCMoveChessShakePoint();
		MessageHeader header;

		int shake_point;
	};

	class CSMoveChessFreeInfo						// 5223��ȡ��������Ϣ		
	{
	public:
		CSMoveChessFreeInfo();
		MessageHeader header;
	};

	/////////////////////////////////////////////     �һ��̵�     /////////////////////////////////////////////
	
	static const int MAX_CONVERT_SCORE_TO_ITEM_NUM = 128;

	class CSScoreToItemConvert								// 4855���Ļ��ֶһ���Ʒ����
	{
	public:
		CSScoreToItemConvert();
		MessageHeader	header;

		int					scoretoitem_type;
		short				index;
		short				num;
	};

	class CSGetConvertRecordInfo							// 4857�һ���¼��Ϣ����
	{
	public:
		CSGetConvertRecordInfo();
		MessageHeader	header;
	};

	class SCConvertRecordInfo								// 4803�һ���¼��Ϣ
	{
	public:
		SCConvertRecordInfo();
		MessageHeader	header;

		struct RecordItem
		{
			RecordItem() { this->Reset(); }

			void Reset()
			{
				reserve = 0;
				convert_type = 0;
				reserve_1 = 0;
				convert_count = 0;
				seq = 0;
				timestamp = 0;
			}

			short reserve;
			char convert_type;
			char reserve_1;
			short convert_count;
			short seq;
			unsigned int timestamp;
		};

		short record_count;
		short lifetime_record_count; 
		int limittime_record_count;
		RecordItem record_list[MAX_CONVERT_RECORD_ITEM_COUNT + MAX_CONVERT_RECORD_ITEM_COUNT + MAX_TIME_CONVERT_RECORD_ITEM_COUNT];
	};

	class CSGetSocreInfoReq									// 4860��ȡ������������
	{
	public:
		CSGetSocreInfoReq();
		MessageHeader	header;
	};

	class SCSendScoreInfo									// 4861���ͻ�����Ϣ
	{
	public:
		SCSendScoreInfo();
		MessageHeader	header;

		int chest_shop_mojing;
		int chest_shop_shengwang;
		int chest_shop_gongxun;
		int chest_shop_weiwang;
		int chest_shop_treasure_credit;
		int chest_shop_jingling_credit;
		int chest_shop_happytree_grow;
		int chest_shop_mysterious_shop_credit;
		int chest_shop_blue_lingzhi;
		int chest_shop_purple_lingzhi;
		int chest_shop_orange_lingzhi;
		int chest_shop_guanghui;
		int chest_shop_precious_boss_score;
		int chest_shop_shenzhouweapon_score;							// ������
		int chest_shop_treasure_credit1;								// �۷�Ѱ������
		int chest_shop_treasure_credit2;								// ����Ѱ������
		int chest_shop_treasure_credit3;								// ������Ѱ�����֣�������
		int chest_shop_zhuanzhi_stone_score;							// תְ��ʯ����
		int chest_shop_hunjing;											// �꾧
	};

	class SCSendScoreInfoNotice									//4862���ͻ������ڼӻ������½���ʾ
	{
	public:
		SCSendScoreInfoNotice();
		MessageHeader	header;

		int chest_shop_mojing;
		int chest_shop_shengwang;
		int chest_shop_gongxun;
		int chest_shop_weiwang;
		int chest_shop_treasure_credit;
		int chest_shop_jingling_credit;
		int chest_shop_happytree_grow;
		int chest_mysterious_shop_credit;
		int chest_shop_blue_lingzhi;
		int chest_shop_purple_lingzhi;
		int chest_shop_orange_lingzhi;
		int chest_shop_guanghui;
		int chest_shop_precious_boss_score;
		int chest_shop_shenzhouweapon_score;
		int chest_shop_treasure_credit1;								// �۷�Ѱ������
		int chest_shop_treasure_credit2;								// ����Ѱ������
		int chest_shop_treasure_credit3;								// ������Ѱ�����֣�������
		int chest_shop_zhuanzhi_stone_score;							// תְ��ʯ���� 
		int chest_shop_hunjing;											// �꾧
	};

	/////////////////////////////////////////////     ��������     /////////////////////////////////////////////
	enum MYSTERIOUSSHOP_OPERATE_TYPE
	{
		MYSTERIOUSSHOP_OPERATE_TYPE_REQINFO = 0,
		MYSTERIOUSSHOP_OPERATE_TYPE_BUY = 1,
	};

	class CSMysteriosshopOperate
	{
	public:
		CSMysteriosshopOperate();
		MessageHeader		header;

		int operate_type;
		int param1;
		int param2;
	};

	static const int MAX_MYSTERIOUS_SALEITEM_NUM = 9;

	class SCMysteriosNpcRefresh
	{
	public:
		SCMysteriosNpcRefresh();
		MessageHeader	header;

		int disappeartime;
		int nextrefreshtime;
		int npc_sceneid;
		int npc_x;
		int npc_y;
		
	};

	class SCMysteriosshopInfo
	{
	public:
		SCMysteriosshopInfo();
		MessageHeader	header;

		struct SaleItem
		{
			SaleItem() : seq(0), buynum(0) {}

			int seq;
			int buynum;
		};

		SaleItem saleitem_list[MAX_MYSTERIOUS_SALEITEM_NUM];
	};

	/////////////////////////////////////////////     �����̵�     /////////////////////////////////////////////
	enum MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE
	{
		MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE_MONEY = 0,			// �����̵깺����Ʒ p1:������Ʒ������
		MYSTERIOUSSHOP_IN_MALL_OPERATE_TYPE_REFRESH = 1,		// ˢ�������̵���Ʒ p1:0Ϊ��һ������ˢ�� 1Ϊȫ��ˢ��
		MYSTERIOUSSHOP_IN_MALL_OPERATE_INFO_REQ = 2,			// �����̵���Ʒ��Ϣ 
		MYSTERIOUSSHOP_IN_MALL_OPERATE_OPEN_VIEW = 3,           // �ͻ����������ѱ��
	};

	class CSMysteriosshopinMallOperate					// 4807
	{
	public:
		CSMysteriosshopinMallOperate();
		MessageHeader		header;

		int operate_type;								// ��������
		int param1;										// 
	};

	class SCSendMysteriosshopItemInfo									// 4808
	{
	public:
		SCSendMysteriosshopItemInfo();
		MessageHeader		header;

		unsigned int next_shop_item_refresh_time;							// �´���Ʒˢ��ʱ��
		int item_count;														// ��Ʒ����
		char client_remind_flag;                                            // �ͻ��˺���ǣ����⴦����Ҫ����˼�¼
		char today_free_count;												// ������Ѵ���
		short reserve_sh;													// 

		MysteriousShopInMallItemInfo item[MYSTERIOUSSHOP_ITEM_MAX_COUNT];	// ��Ʒ�б�
	};

	class CSDiscountShopBuy					// 8950
	{
	public:
		CSDiscountShopBuy();
		MessageHeader		header;

		int seq;										
		int num;										 
	};

	class SCSendDiscounthopItemInfo									// 8951
	{
	public:
		SCSendDiscounthopItemInfo();
		MessageHeader		header;

		struct DiscountItem
		{
			DiscountItem() : seq(0), today_buy_count(0) {}

			int seq;
			int today_buy_count;		//�����ѹ�������
		};

		int item_count;														// ��Ʒ����
		int today_refresh_level;
		DiscountItem item_list[DISCOUNTSHOP_ITEM_MAX_COUNT];
	};
}

#pragma pack(pop)

#endif


