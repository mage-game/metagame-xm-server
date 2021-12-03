#pragma once

#include "netmsg/serialize_base.hpp"
#include <string>
#include <vector>

namespace Protocol {

    enum CS_TIAN_XIANG_REQ_TYPE
    {
        CS_TIAN_XIANG_REQ_TYPE_ALL_INFO,                 // 请求所有信息
        CS_TIAN_XIANG_REQ_TYPE_CHANGE_BEAD_TYPE,         // 请求改变珠子颜色，p1 = x , p2 = y， p3 = 要改的颜色
        CS_TIAN_XIANG_REQ_TYPE_CHANGE_BEAD,              // 请求改变位置，p1 = x , p2 = y， p3 = 目标格子的x, p4 = 目标格子的y
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_UP_START,         // 印位升星 p1 印位类型 p2 是否使用保护符 p3 是否自动购买
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_UP_LEVEL,         // 印位突破
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_EQUIT,            // 装备印记 p1 虚拟背包索引， p2 印位类型
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_TAKE_OFF,         // 卸下印记 p1 印位类型
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_ADD_ATTR_COUNT,   // 增加属性条数 p1 印位类型
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_FLUSH_ATTR_TYPE,  // 印位洗练属性类型 p1 印位类型
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_FLUSH_ATTR_VALUE, // 印位洗练属性值 p1 印位类型
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_APLY_FLUSH,       // 应用洗练 p1 类型 0 属性类：1 属性值
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_RECYCLE,          // 印记回收 p1 虚拟背包索引， p2 数量
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_EXCHANGE,         // 印记兑换 p1 商店索引
        CS_TIAN_XIANG_REQ_TYPE_SORT,                     // 背包整理
        CS_TIAN_XIANG_REQ_TYPE_CHOUHUN,                  // 抽取 p1 是否使用积分
        CS_TIAN_XIANG_REQ_TYPE_SUPER_CHOUHUN,            // 逆天改运
        CS_TIAN_XIANG_REQ_TYPE_BATCH_HUNSHOU,            // 连抽 p1 是否使用积分
        CS_TIAN_XIANG_REQ_TYPE_PUT_BAG,                  // 放入背包 p1 格子id
        CS_TIAN_XIANG_REQ_TYPE_CONVERT_TO_EXP,           // 一键出售
        CS_TIAN_XIANG_REQ_TYPE_SINGLE_CONVERT_TO_EXP,    // 出售 p1 格子id
        CS_TIAN_XIANG_REQ_TYPE_PUT_BAG_ONE_KEY,          // 一键放入背包
    };

    /*
     * 
     */
    class CSTianxiangOperaReq : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11000;

        short info_type;
        short param1;
        short param2;
        short param3;
        short param4;

        virtual void Serialize(StaticByteArray &collector) const {};
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(CSTianxiangOperaReq);

    /*
     * 组合
     */
    class TianXiangCombineParam : public MsgSerializeBase
    {
    public:
        char x;
        char y;
        char seq;

        virtual void Serialize(StaticByteArray &collector) const;
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return 0; }
    };
    /*
     * 珠子
     */
    class TianXiangBeadParam : public MsgSerializeBase
    {
    public:
        char x;
        char y;
        char type;

        virtual void Serialize(StaticByteArray &collector) const;
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return 0; }
    };
    /*
     * 全部信息列表
     */
    class SCSendTianXiangAllInfo : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11001;

        std::vector<TianXiangBeadParam> grid_list;
        std::vector<TianXiangCombineParam> combine_list;

        virtual void Serialize(StaticByteArray &collector) const;
        virtual void Unserialize(StaticByteArray &collector) {};
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(SCSendTianXiangAllInfo);

    enum CS_FABAO_REQ_TYPE
    {
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPGRADE,                // 请求进阶,param1 = 是否自动购买,param2 = 重复升阶次数
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_ACTIVESPECIALIMG,       // 请求激活特殊形象,param1 = 特殊形象id,param2 = 是否公布
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UNACTIVESPECIALIMG,     // 请求撤消特殊形象,激活后取消激活,param1 = 特殊形象id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_USESPECIALIMG,          // 请求使用特殊形象,param1 = 特殊形象id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UNUSESPECIALIMG,        // 请求卸下特殊形象,脱下,param1 = 特殊形象id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_FABAOSPECIALIMGUPGRADE, // 特殊形象进阶,param1 = 特殊形象id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_USEIMA,                 // 使用形象,param1 = 形象id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPLEVELSKILL,           // 请求升级技能,param1 = 技能索引,param2 = 是否自动购买材料
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPLEVELEQUIP,           // 请求升级装备,param1 = 装备索引
    };

	/*
	 * 法宝信息
	 */
	class SCSendFabaoInfo : public MsgSerializeBase
	{
	public:
		static const int MSG_TYPE = 11002;

		short grade; // 阶级
		short used_imageid; // 使用的形象
		short used_special_id; // 使用的特殊形象
		std::vector<unsigned char> active_image_flag; // 激活的形象
		std::vector<unsigned char> active_special_image_flag; // 激活的特殊形象
		int grade_bless_val; // 进阶祝福值
		std::vector<char> special_img_grade_list; // 特殊形象阶数
		short shuxingdan_count; // 使用的属性丹数量
		int clear_bless_time; // 进阶祝福值清零时间
		int last_upgrade_succ_time; // 上一次进阶成功的时间
		std::vector<short> skill_level_list; // 技能等级列表，下标为技能索引
		std::vector<short> equip_level_list; // 装备等级列表，下标为技能索引
		int equip_skill_level; // 装备技能等级
		int is_used_special_img; // 是否使用幻化形象

		virtual void Serialize(StaticByteArray &collector) const;
		virtual void Unserialize(StaticByteArray &collector) {};
		virtual int GetMsgType() const { return MSG_TYPE; }
	};
	DEFINE_PROTOCOL_DESCRIBE(SCSendFabaoInfo);

    /*
     * 法宝相关操作请求
     */
    class CSFabaoOperateReq : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11003;

        short req_type; // 请求类型
        short param1;
        short param2;
        short param3;

        virtual void Serialize(StaticByteArray &collector) const {};
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(CSFabaoOperateReq);

    /*
     * 时装和神兵技能升级
     */
    class CSShizhuangSkillUplevelReq : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11004;

        int skill_idx; // 请求序号
        short auto_buy;
        short shizhuang_type;

        virtual void Serialize(StaticByteArray &collector) const {};
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(CSShizhuangSkillUplevelReq);

    /*
     * 时装和神兵装备进阶
     */
    class CSShizhuangUplevelEquip : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11005;

        short shizhuang_type; // 请求类型
        short equip_idx;

        virtual void Serialize(StaticByteArray &collector) const {};
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(CSShizhuangUplevelEquip);

}