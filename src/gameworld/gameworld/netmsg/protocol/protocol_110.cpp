#include "netmsg/static_byte_array.hpp"
#include "protocol_110.hpp"

namespace Protocol {

    CREATE_PROTOCOL_DESCRIBE(CSTianxiangOperaReq);
    CREATE_PROTOCOL_DESCRIBE(SCSendTianXiangAllInfo);
    CREATE_PROTOCOL_DESCRIBE(SCSendFabaoInfo);
    CREATE_PROTOCOL_DESCRIBE(CSFabaoOperateReq);
    CREATE_PROTOCOL_DESCRIBE(CSShizhuangSkillUplevelReq);
    CREATE_PROTOCOL_DESCRIBE(CSShizhuangUplevelEquip);

    void CSTianxiangOperaReq::Unserialize(StaticByteArray &collector)
    {
		info_type = collector.ReadInt16();
		param1 = collector.ReadInt16();
		param2 = collector.ReadInt16();
		param3 = collector.ReadInt16();
		param4 = collector.ReadInt16();
    }

    void TianXiangCombineParam::Serialize(StaticByteArray &collector) const
    {
		collector.WriteInt8(x);
		collector.WriteInt8(y);
		collector.WriteInt8(seq);
    }

    void TianXiangCombineParam::Unserialize(StaticByteArray &collector)
    {
		x = collector.ReadInt8();
		y = collector.ReadInt8();
		seq = collector.ReadInt8();
    }

    void TianXiangBeadParam::Serialize(StaticByteArray &collector) const
    {
		collector.WriteInt8(x);
		collector.WriteInt8(y);
		collector.WriteInt8(type);
    }

    void TianXiangBeadParam::Unserialize(StaticByteArray &collector)
    {
		x = collector.ReadInt8();
		y = collector.ReadInt8();
		type = collector.ReadInt8();
    }

    void SCSendTianXiangAllInfo::Serialize(StaticByteArray &collector) const
    {
		collector.WriteUint16(static_cast<unsigned short>(grid_list.size()));
		for (std::vector<TianXiangBeadParam>::const_iterator it = grid_list.begin(); it != grid_list.end(); ++it)
		{
			it->Serialize(collector);
		}
		collector.WriteUint16(static_cast<unsigned short>(combine_list.size()));
		for (std::vector<TianXiangCombineParam>::const_iterator it = combine_list.begin(); it != combine_list.end(); ++it)
		{
			it->Serialize(collector);
		}
    }

	void SCSendFabaoInfo::Serialize(StaticByteArray &collector) const
	{
		collector.WriteInt16(grade);
		collector.WriteInt16(used_imageid);
		collector.WriteInt16(used_special_id);
		collector.WriteUint16(static_cast<unsigned short>(active_image_flag.size()));
		for (std::vector<unsigned char>::const_iterator it = active_image_flag.begin(); it != active_image_flag.end(); ++it)
		{
			collector.WriteUint8(*it);
		}
		collector.WriteUint16(static_cast<unsigned short>(active_special_image_flag.size()));
		for (std::vector<unsigned char>::const_iterator it = active_special_image_flag.begin(); it != active_special_image_flag.end(); ++it)
		{
			collector.WriteUint8(*it);
		}
		collector.WriteInt32(grade_bless_val);
		collector.WriteUint16(static_cast<unsigned short>(special_img_grade_list.size()));
		for (std::vector<char>::const_iterator it = special_img_grade_list.begin(); it != special_img_grade_list.end(); ++it)
		{
			collector.WriteInt8(*it);
		}
		collector.WriteInt16(shuxingdan_count);
		collector.WriteInt32(clear_bless_time);
		collector.WriteInt32(last_upgrade_succ_time);
		collector.WriteUint16(static_cast<unsigned short>(skill_level_list.size()));
		for (std::vector<short>::const_iterator it = skill_level_list.begin(); it != skill_level_list.end(); ++it)
		{
			collector.WriteInt16(*it);
		}
		collector.WriteUint16(static_cast<unsigned short>(equip_level_list.size()));
		for (std::vector<short>::const_iterator it = equip_level_list.begin(); it != equip_level_list.end(); ++it)
		{
			collector.WriteInt16(*it);
		}
		collector.WriteInt32(equip_skill_level);
		collector.WriteInt32(is_used_special_img);
	}

    void CSFabaoOperateReq::Unserialize(StaticByteArray &collector)
    {
		req_type = collector.ReadInt16();
		param1 = collector.ReadInt16();
		param2 = collector.ReadInt16();
		param3 = collector.ReadInt16();
    }

    void CSShizhuangSkillUplevelReq::Unserialize(StaticByteArray &collector)
    {
		skill_idx = collector.ReadInt32();
		auto_buy = collector.ReadInt16();
		shizhuang_type = collector.ReadInt16();
    }

    void CSShizhuangUplevelEquip::Unserialize(StaticByteArray &collector)
    {
		shizhuang_type = collector.ReadInt16();
		equip_idx = collector.ReadInt16();
    }

}