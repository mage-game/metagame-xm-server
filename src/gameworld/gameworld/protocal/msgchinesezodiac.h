#ifndef __MSG_CHINESE_ZODIAC_HPP__
#define __MSG_CHINESE_ZODIAC_HPP__

#include "servercommon/chinesezodiacdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum CS_TIAN_XIANG_TYPE_OLD
	{
		CS_TIAN_XIANG_TYPE_OLD_ALL_INFO,			// ����������Ϣ
		CS_TIAN_XIANG_TYPE_OLD_CHANGE_BEAD,			// ����ı�λ�ã�p1 = x , p2 = y�� p3 = Ŀ����ӵ�x, p4 = Ŀ����ӵ�y  , p5 = �½�
		CS_TIAN_XIANG_TYPE_OLD_XIE_BEAD,			// ж����   p1 = x , p2 = y, p3 = �½�
		CS_TIAN_XIANG_TYPE_OLD_GUNGUN_LE_REQ,		// �����ֳ齱  p1�� 0�ǳ�1�Σ�1�ǳ�10��
		CS_TIAN_XIANG_TYPE_OLD_UNLOCK,				// �����������
		CS_TIAN_XIANG_TYPE_OLD_PUT_MIJI,			// ���ؼ�  P1 = �������ͣ� p2 = �ؼ�index
		CS_TIAN_XIANG_TYPE_OLD_CALC_CAPACITY,		// �����ؼ��ɹ������¼���ս��
		CS_TIAN_XIANG_TYPE_OLD_MIJI_COMPOUND,		// �ؼ��ϳ� p1��index1
		CS_TIAN_XIANG_TYPE_OLD_PUT_BEAD,			// �ֶ������� p1:type, p2:�½�
		CS_TIAN_XIANG_TYPE_OLD_UPGRADE_XINGLING,	// ��������p1:��ͼid, p2:�Ƿ��Զ�����
		CS_TIAN_XIANG_TYPE_OLD_UPLEVEL_XINGHUN,		// �����ǻ� p1:��Ф���ͣ�p2:�Ƿ��Զ�����p3:�Ƿ�ʹ�ñ�����
		CS_TIAN_XIANG_TYPE_OLD_XINGHUN_UNLOCK,		// ��������ǻ�
	};
	
	// �ؼ������޸� 8210
	class SCMijiSingleChange
	{
	public:
		SCMijiSingleChange();
		MessageHeader header;

		int zodiac_type;
		int kong_index;
		int miji_index;
	};

	// �ؼ��ϳɳɹ� 8211
	class SCMijiCombineSucc
	{
	public:
		SCMijiCombineSucc();
		MessageHeader header;

		int miji_index;
	};
	
	/*
     * ���� 8205
     */
    class CSTianxiangReq
    {
    public:
		CSTianxiangReq();
		MessageHeader header;

        short info_type;
        short param1;
        short param2;
        short param3;
        short param4;
		short param5;
    };

	 /*
     * ���󵥸����� 8207
     */
    class SCTianXiangSignBead
    {
    public:
        SCTianXiangSignBead();
		MessageHeader header;

		int chapter;
		char x;
		char y;
		short type;
    };

	/*
     * ���������Ϣ 8208
     */
    class SCTianXiangCombind
    {
    public:
        SCTianXiangCombind();
		MessageHeader header;

		char active_list[TIAN_XIANG_ALL_CHAPTER_COMBINE];
		char chapter;
		ComPosi bead_by_combine_list[TIAN_XIANG_ALL_CHAPTER_COMBINE][TIAN_XIANG_COMBINE_MEX_BEAD_NUM];
    };

	struct TianXiangChapter
	{
		short grid_list[TIAN_XIANG_TABEL_ROW_COUNT * TIAN_XIANG_TABEL_MIDDLE_GRIDS];
		ComPosi bead_by_combine_list[TIAN_XIANG_ALL_CHAPTER_COMBINE][TIAN_XIANG_COMBINE_MEX_BEAD_NUM];
	};

    /*
     * ȫ��������Ϣ�б� 8206
     */
    class SCTianXiangAllInfo
    {
    public:
		SCTianXiangAllInfo();
		MessageHeader header;
		
		char curr_chapter;
		char active_list[TIAN_XIANG_CURR_CHAPTER_NUM][TIAN_XIANG_ALL_CHAPTER_COMBINE];

		TianXiangChapter chapter_list[TIAN_XIANG_CHAPTER_NUM];

		char is_finish_all_chapter;
		char reserve_ch;
		short reserve_sh;
    };

	/*
     * ��������Ϣ 8209
     */
    class SCGunGunLeInfo
    {
    public:
        SCGunGunLeInfo();
		MessageHeader header;
		
		int today_ggl_free_times;
		int count;
		unsigned int last_free_ggl_time;
		int combine_type_list[GGL_COMBINE_TYPE_MAX_NUM];
    };

	/*
     * ������Ϣ 8212
     */
    class SCXinglingInfo
    {
    public:
        SCXinglingInfo();
		MessageHeader header;
		
		XingLingParam xingling_lsit[TIAN_XIANG_CHAPTER_NUM];
    };

	class SCChineseZodiacAllInfo				// 8200,							// ����ϵͳ������Ϣ						
	{
	public:
		SCChineseZodiacAllInfo();
		MessageHeader header;
		
		short zodiac_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		short xinghun_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		short xinghun_level_max_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		short xinghun_baoji_value_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		EquipLevelList chinesezodiac_equip_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		MijiParam miji_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];

		int zodiac_progress;
		char upgrade_zodiac;
		char xinghun_progress;
		short reserve_sh;
	};

	class SCChineseZodiacEquipInfo				// 8201,							// ����ϵͳװ����Ϣ			
	{
	public:
		SCChineseZodiacEquipInfo();
		MessageHeader header;

		short zodiac_type;
		short equip_type;
		int equip_item;
	};

	class CSChineseZodiacPromoteXingHun			// 8202,							// ���������ǻ�ȼ�
	{
	public:
		CSChineseZodiacPromoteXingHun();
		MessageHeader header;

		int zodiac_type;
		short is_auto_buy;
		short is_use_promote_item;
	};

	class CSChineseZodiacPromoteEquip			// 8203,							// ��������װ���ȼ�
	{
	public:
		CSChineseZodiacPromoteEquip();
		MessageHeader header;

		short zodiac_type;
		short equip_slot;
	};

	class CSChineseZodiacPromote				// 8204,							// ������		
	{
	public:
		CSChineseZodiacPromote();
		MessageHeader header;

		short zodiac_type;
		short is_auto_buy;
	};

	class CSChineseZodiacTakeOffEquip			// 8213,							// ж��װ��
	{
	public:
		CSChineseZodiacTakeOffEquip();
		MessageHeader header;

		short zodiac_type;   //��Ф���� 
		short equip_type;	 //װ����id	
	};

	class CSChineseZodiacRecycle		// 8214 ��Фװ������
	{
	public:
		CSChineseZodiacRecycle();
		MessageHeader header;

		int index_count;											// ��������
		short index_list[MAX_CHINESEZODIAC_RECYCLE_NUM];		    // ���������б�
	};
}

#pragma pack(pop)

#endif 


