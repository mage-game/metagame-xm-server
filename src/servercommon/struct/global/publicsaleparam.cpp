#include "publicsaleparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool PublicSaleParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_PUBLIC_SALE_COUNT; ++i)
	{
		ret = outstream.Push(sale_list[i].id_pulicsale)
			&& outstream.Push(sale_list[i].change_state)
			&& outstream.Push(sale_list[i].seller_uid)
			&& outstream.Push(sale_list[i].item_param.sale_index);
		if (!ret) return false;

		if (sale_list[i].change_state == structcommon::CS_DELETE) continue;

		const char * seller_name = sale_list[i].seller_name;
		ret = outstream.Push(seller_name)
			&& outstream.Push(sale_list[i].item_param.sale_type)
			&& outstream.Push(sale_list[i].item_param.level)
			&& outstream.Push(sale_list[i].item_param.prof)
			&& outstream.Push(sale_list[i].item_param.color)
			&& outstream.Push(sale_list[i].item_param.order)
			&& outstream.Push(sale_list[i].item_param.price)
			&& outstream.Push(sale_list[i].item_param.sale_value)
			&& outstream.Push(sale_list[i].item_param.price_type)
			&& outstream.Push(sale_list[i].item_param.sale_item_type)
			&& outstream.Push(sale_list[i].item_param.sale_time)
			&& outstream.Push(sale_list[i].item_param.due_time);
		if (!ret) return false;

		if (!sale_list[i].item_param.item_wrapper.Serialize(outstream)) return false;
	}

	return true;
}

bool PublicSaleParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_PUBLIC_SALE_COUNT; ++i)
	{
		ret = instream.Pop(&sale_list[i].id_pulicsale)
			&& instream.Pop(&sale_list[i].change_state)
			&& instream.Pop(&sale_list[i].seller_uid)
			&& instream.Pop(&sale_list[i].item_param.sale_index);
		if (!ret) return false;

		if (sale_list[i].change_state == structcommon::CS_DELETE) continue;

		const char * seller_name = 0;

		ret = instream.Pop(&seller_name)
			&& instream.Pop(&sale_list[i].item_param.sale_type)
			&& instream.Pop(&sale_list[i].item_param.level)
			&& instream.Pop(&sale_list[i].item_param.prof)
			&& instream.Pop(&sale_list[i].item_param.color)
			&& instream.Pop(&sale_list[i].item_param.order)
			&& instream.Pop(&sale_list[i].item_param.price)
			&& instream.Pop(&sale_list[i].item_param.sale_value)
			&& instream.Pop(&sale_list[i].item_param.price_type)
			&& instream.Pop(&sale_list[i].item_param.sale_item_type)
			&& instream.Pop(&sale_list[i].item_param.sale_time)
			&& instream.Pop(&sale_list[i].item_param.due_time);
		if (!ret) return false;

		if (!sale_list[i].item_param.item_wrapper.Unserialize(instream)) return false;

		STRNCPY(sale_list[i].seller_name, seller_name, sizeof(sale_list[0].seller_name));
	}

	return true;
}

