#ifndef __MSG_SHENGE_SYSTEM_HPP__
#define __MSG_SHENGE_SYSTEM_HPP__

#include "servercommon/shengesystemdef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push, 4)

namespace Protocol
{

	enum SHENGE_SYSTEM_REQ_TYPE
	{
		SHENGE_SYSTEM_REQ_TYPE_ALL_INFO,					// ����������Ϣ
		SHENGE_SYSTEM_REQ_TYPE_DECOMPOSE,					// �ֽ�			p1 ���ⱳ������
		SHENGE_SYSTEM_REQ_TYPE_COMPOSE,						// �ϳ�			p1 ��Ʒ1�����ⱳ������	p2 ��Ʒ2�����ⱳ������	p3 ��Ʒ1�����ⱳ��������
		SHENGE_SYSTEM_REQ_TYPE_SET_RUAN,					// װ�����		p1 ���ⱳ������			p2 ���ҳ				p3 ����������
		SHENGE_SYSTEM_REQ_TYPE_CHANGE_RUNA_PAGE,			// �л����ҳ	p1 ���ҳ
		SHENGE_SYSTEM_REQ_TYPE_CHOUJIANG,					// �齱			p1 ������
		SHENGE_SYSTEM_REQ_TYPE_UPLEVEL,						// ���� ���	p1 0 ���� 1 ���ҳ		p2 ��������/ ���ҳ		p3 ��������
		SHENGE_SYSTEM_REQ_TYPE_SORT_BAG,					// ������
		SHENGE_SYSTEM_REQ_TYPE_UNLOAD_SHENGE,				// ������		p1 ���ҳ				p2 ��������
		SHENGE_STYTEM_REQ_TYPE_CLEAR_PAGE,					// ������ҳ	p1 ���ҳ

		SHENGE_STYTEM_REQ_TYPE_UPLEVEL_ZHANGKONG,			// �����ƿ�		p1 0������1�Σ�1������10��
		SHENGE_STYTEM_REQ_TYPE_RECLAC_ATTR,					// �����ƿغ�����ս����

		SHENGE_SYSTEM_REQ_TYPE_XILIAN,						// �������ϴ�� p1 ����id				p2 ϴ����			p3 �Ƿ��Զ�����
	};

	class CSShengeSystemReq  // 8421
	{
	public:
		CSShengeSystemReq();
		MessageHeader header;

		short info_type;
		short param1;
		short param2;
		short param3;
		unsigned int param_4;

		int index_count;
		int virtual_index_list[SHENGE_SYSTEM_BAG_MAX_GRIDS];
	};

	enum SHENGE_SYSTEM_INFO_TYPE
	{
		SHENGE_SYSTEM_INFO_TYPE_SIGLE_CHANGE = 0,			// �������������Ϣ											p2 count����
		SHENGE_SYSTEM_INFO_TYPE_ALL_BAG_INFO,				// ����ȫ����Ϣ				p1��ǰʹ�����ҳ				p2 count����	
		SHENGE_SYSTEM_INFO_TYPE_SHENGE_INFO,				// ���ҳ�����������Ϣ		p1���ҳ 						p2 count����			
		SHENGE_SYSTEM_INFO_TYPE_ALL_SHENGE_INFO,			// ���ҳ��ȫ�������Ϣ		p1���ҳ						p2 count����			
		SHENGE_SYSTEM_INFO_TYPE_ALL_CHOUJIANG_INFO,			// ��Ʒ�б�					p1 ʣ����Ѵ���					p2 count����		p3 ��ѳ齱ʣ��ʱ��
		SHENGE_SYSTEM_INFO_TYPE_ALL_MARROW_SCORE_INFO,		// ������Ϣ																		p3 ��������
		SHENGE_SYSTEM_INFO_TYPE_USING_PAGE_INDEX,			// ���ҳ					p1 ��ǰʹ�����ҳ
		SHENGE_SYSTEM_INFO_TYPE_COMPOSE_SHENGE_INFO,		// �ϳ���Ϣ
		SHENGE_SYSTEM_INFO_TYPE_ACTIVE_COMBINE_INFO,		// ������������
		SHENGE_SYSTEM_INFO_TYPE_CHOUJIANG_INFO,				// �齱��Ϣ					p1 ������ѳ齱����									p3 �´γ齱cd
	};

	struct ShengeSystemBag
	{
		char quanlity;
		char type;
		unsigned char level;
		unsigned char index;
	};

	class SCShengeSystemBagInfo  // 8422
	{
	public:
		SCShengeSystemBagInfo();
		MessageHeader header;

		char info_type;
		char param1;
		short count;
		unsigned int param3;
		ShengeSystemBag bag_list[SHENGE_SYSTEM_BAG_MAX_GRIDS];
	};
	
	// �ƿ�����Ϣ
	class SCShengeZhangkongInfo  // 8423
	{
	public:
		SCShengeZhangkongInfo();
		MessageHeader header;

		ZhangkongParam zhangkong_list[SHENGE_SYSTEM_QUALITY_MAX_VALUE];
	};

	// �����ƿ��޸�
	class SCZhangkongSingleChange	// 8424
	{
	public:
		SCZhangkongSingleChange();
		MessageHeader header;
		
		int grid;
		int level;
		int exp;
		int add_exp;
	};

	// ���������Ϣ
	class SCShengeShenquAllInfo		// 8440
	{
	public:
		SCShengeShenquAllInfo();
		MessageHeader header;

		SingleShengeShenqu shenqu_list[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
		int shenqu_history_max_cap[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
	};

	// �������������Ϣ
	class SCShengeShenquInfo		// 8441
	{
	public:
		SCShengeShenquInfo();
		MessageHeader header;

		int shenqu_id;
		ShengeShenquAttrInfo shenqu_attr_list[SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM];
		int shenqu_history_max_cap;
	};

	// �ǻ���װ��Ϣ
	class SCShengeSuitInfo		// 8849
	{
	public:
		SCShengeSuitInfo();
		MessageHeader header;

		int shenge_quality_info[SHENGE_QUALITY_COUNT];		//ÿ��Ʒ���������ͳ��
	};
}

#pragma pack(pop)
#endif
