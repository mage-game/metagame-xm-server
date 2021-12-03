/******************************************************************************************
FileName: wantbuyparam.hpp
Author: dreamwang
Description: ���������ݿ��еĶ���  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __WANTBUY_PARAM_HPP__
#define __WANTBUY_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"

class WantBuyParam
{
public:
	WantBuyParam() : count(0) {}
	~WantBuyParam() {}

	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct WantBuyItem
	{
		int buyer_db_index;
		int buyer_role_id;
		int buy_index;

		GameName buyer_name;

		int item_id;					// ��ƷID
		int item_num;					// ���� 
		int color;						// ��ɫ
		int level;						// �ȼ�
		int prof;						// ְҵ
		int gold_per;					// ���۵��� gold

		unsigned int wantbuy_time;		// ��ʼ����ʱ��
		unsigned int due_time;			// ����ʱ��
	};

	struct WantBuyAttr
	{
		char change_state;
		long long id_wantbuy;
		WantBuyItem wantbuy_item;
	};

	static const int MAX_WANT_BUY_COUNT = 1000;

	int count;
	WantBuyAttr wantbuy_list[MAX_WANT_BUY_COUNT];
};

#endif  // __PUBLIC_WANTBUY_PARAM_HPP__


