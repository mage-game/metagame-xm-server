#pragma once

#include "netmsg/serialize_base.hpp"
#include <string>
#include <vector>

namespace Protocol {

    enum CS_TIAN_XIANG_REQ_TYPE
    {
        CS_TIAN_XIANG_REQ_TYPE_ALL_INFO,                 // ����������Ϣ
        CS_TIAN_XIANG_REQ_TYPE_CHANGE_BEAD_TYPE,         // ����ı�������ɫ��p1 = x , p2 = y�� p3 = Ҫ�ĵ���ɫ
        CS_TIAN_XIANG_REQ_TYPE_CHANGE_BEAD,              // ����ı�λ�ã�p1 = x , p2 = y�� p3 = Ŀ����ӵ�x, p4 = Ŀ����ӵ�y
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_UP_START,         // ӡλ���� p1 ӡλ���� p2 �Ƿ�ʹ�ñ����� p3 �Ƿ��Զ�����
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_UP_LEVEL,         // ӡλͻ��
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_EQUIT,            // װ��ӡ�� p1 ���ⱳ�������� p2 ӡλ����
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_TAKE_OFF,         // ж��ӡ�� p1 ӡλ����
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_ADD_ATTR_COUNT,   // ������������ p1 ӡλ����
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_FLUSH_ATTR_TYPE,  // ӡλϴ���������� p1 ӡλ����
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_FLUSH_ATTR_VALUE, // ӡλϴ������ֵ p1 ӡλ����
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_APLY_FLUSH,       // Ӧ��ϴ�� p1 ���� 0 �����ࣺ1 ����ֵ
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_RECYCLE,          // ӡ�ǻ��� p1 ���ⱳ�������� p2 ����
        CS_TIAN_XIANG_REQ_TYPE_IMPRINT_EXCHANGE,         // ӡ�Ƕһ� p1 �̵�����
        CS_TIAN_XIANG_REQ_TYPE_SORT,                     // ��������
        CS_TIAN_XIANG_REQ_TYPE_CHOUHUN,                  // ��ȡ p1 �Ƿ�ʹ�û���
        CS_TIAN_XIANG_REQ_TYPE_SUPER_CHOUHUN,            // �������
        CS_TIAN_XIANG_REQ_TYPE_BATCH_HUNSHOU,            // ���� p1 �Ƿ�ʹ�û���
        CS_TIAN_XIANG_REQ_TYPE_PUT_BAG,                  // ���뱳�� p1 ����id
        CS_TIAN_XIANG_REQ_TYPE_CONVERT_TO_EXP,           // һ������
        CS_TIAN_XIANG_REQ_TYPE_SINGLE_CONVERT_TO_EXP,    // ���� p1 ����id
        CS_TIAN_XIANG_REQ_TYPE_PUT_BAG_ONE_KEY,          // һ�����뱳��
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
     * ���
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
     * ����
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
     * ȫ����Ϣ�б�
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
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPGRADE,                // �������,param1 = �Ƿ��Զ�����,param2 = �ظ����״���
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_ACTIVESPECIALIMG,       // ���󼤻���������,param1 = ��������id,param2 = �Ƿ񹫲�
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UNACTIVESPECIALIMG,     // ��������������,�����ȡ������,param1 = ��������id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_USESPECIALIMG,          // ����ʹ����������,param1 = ��������id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UNUSESPECIALIMG,        // ����ж����������,����,param1 = ��������id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_FABAOSPECIALIMGUPGRADE, // �����������,param1 = ��������id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_USEIMA,                 // ʹ������,param1 = ����id
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPLEVELSKILL,           // ������������,param1 = ��������,param2 = �Ƿ��Զ��������
        CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPLEVELEQUIP,           // ��������װ��,param1 = װ������
    };

	/*
	 * ������Ϣ
	 */
	class SCSendFabaoInfo : public MsgSerializeBase
	{
	public:
		static const int MSG_TYPE = 11002;

		short grade; // �׼�
		short used_imageid; // ʹ�õ�����
		short used_special_id; // ʹ�õ���������
		std::vector<unsigned char> active_image_flag; // ���������
		std::vector<unsigned char> active_special_image_flag; // �������������
		int grade_bless_val; // ����ף��ֵ
		std::vector<char> special_img_grade_list; // �����������
		short shuxingdan_count; // ʹ�õ����Ե�����
		int clear_bless_time; // ����ף��ֵ����ʱ��
		int last_upgrade_succ_time; // ��һ�ν��׳ɹ���ʱ��
		std::vector<short> skill_level_list; // ���ܵȼ��б��±�Ϊ��������
		std::vector<short> equip_level_list; // װ���ȼ��б��±�Ϊ��������
		int equip_skill_level; // װ�����ܵȼ�
		int is_used_special_img; // �Ƿ�ʹ�ûû�����

		virtual void Serialize(StaticByteArray &collector) const;
		virtual void Unserialize(StaticByteArray &collector) {};
		virtual int GetMsgType() const { return MSG_TYPE; }
	};
	DEFINE_PROTOCOL_DESCRIBE(SCSendFabaoInfo);

    /*
     * ������ز�������
     */
    class CSFabaoOperateReq : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11003;

        short req_type; // ��������
        short param1;
        short param2;
        short param3;

        virtual void Serialize(StaticByteArray &collector) const {};
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(CSFabaoOperateReq);

    /*
     * ʱװ�������������
     */
    class CSShizhuangSkillUplevelReq : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11004;

        int skill_idx; // �������
        short auto_buy;
        short shizhuang_type;

        virtual void Serialize(StaticByteArray &collector) const {};
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(CSShizhuangSkillUplevelReq);

    /*
     * ʱװ�����װ������
     */
    class CSShizhuangUplevelEquip : public MsgSerializeBase
    {
    public:
        static const int MSG_TYPE = 11005;

        short shizhuang_type; // ��������
        short equip_idx;

        virtual void Serialize(StaticByteArray &collector) const {};
        virtual void Unserialize(StaticByteArray &collector);
        virtual int GetMsgType() const { return MSG_TYPE; }
    };
    DEFINE_PROTOCOL_DESCRIBE(CSShizhuangUplevelEquip);

}