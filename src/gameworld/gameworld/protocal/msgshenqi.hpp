#pragma once
#ifndef __MSG_SHENQI_HPP__
#define __MSG_SHENQI_HPP__
#include "servercommon/shenqidef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push, 4)
namespace Protocol
{
	enum SHENQI_OPERA_REQ_TYPE
	{
		SHENQI_OPERA_REQ_TYPE_INFO = 0,						//����������Ϣ
		SHENQI_OPERA_REQ_TYPE_DECOMPOSE,					//�ֽ� param_1:��Ҫ�ֽ����id
		SHENQI_OPERA_REQ_TYPE_SHENBING_INLAY,				//�����Ƕ���� param_1:id  param_2:��λ param_3:Ʒ��
		SHENQI_OPERA_REQ_TYPE_SHENBING_UPLEVEL,				//����������� param_1:id  param_2:�Ƿ��Զ����� param_3:һ��������
		SHENQI_OPERA_REQ_TYPE_SHENBING_USE_IMAGE,			//�������ʹ������ param_1:ʹ������id(0ȡ��ʹ��)
		SHENQI_OPERA_REQ_TYPE_SHENBING_USE_TEXIAO,			//���������Ч���� param_1:ʹ����Чid(0ȡ��ʹ��)
		SHENQI_OPERA_REQ_TYPE_BAOJIA_INLAY,					//������Ƕ���� param_1:id  param_2:��λ param_3:Ʒ��
		SHENQI_OPERA_REQ_TYPE_BAOJIA_UPLEVEL,				//������������ param_1:id  param_2:�Ƿ��Զ����� param_3:һ��������
		SHENQI_OPERA_REQ_TYPE_BAOJIA_USE_IMAGE,				//���׸���ʹ������ param_1:ʹ������id(0ȡ��ʹ��)
		SHENQI_OPERA_REQ_TYPE_BAOJIA_USE_TEXIAO,			//���׸�����Ч���� param_1:ʹ����Чid(0ȡ��ʹ��)
		SHENQI_OPERA_REQ_TYPE_SHENBING_TEXIAO_ACTIVE,		//�����Ч���� param_1 �������
		SHENQI_OPERA_REQ_TYPE_BAOJIA_TEXIAO_ACTIVE,			//������Ч���� param_1 �����
		SHENQI_OPERA_REQ_TYPE_BAOJIA_OPEN_TEXIAO,			//��ʾ������Ч
		SHENQI_OPERA_REQ_TYPE_MAX
	};

	enum SHENQI_SC_INFO_TYPE									//�·���Ϣ����
	{
		SHENQI_SC_INFO_TYPE_SHENBING = 0,						// ���
		SHENQI_SC_INFO_TYPE_BAOJIA,								// ����

		SHENQI_SC_INFO_TYPE_MAX
	};

	struct CSShenqiOperaReq					//8586
	{
		CSShenqiOperaReq();
		MessageHeader header;

		int opera_type;
		int param_1;
		int param_2;
		int param_3;
		
	};

	struct SCShenqiAllInfo				//8587
	{
		SCShenqiAllInfo();
		MessageHeader header;

		unsigned long long shenbing_image_flag;			//������󼤻���
		unsigned long long shenbing_texiao_flag;		//�����Ч������
		unsigned long long baojia_image_flag;			//�������󼤻���
		unsigned long long baojia_texiao_flag;			//������Ч������

		char shenbing_cur_image_id;						//��ǰʹ���������id
		char shenbing_cur_texiao_id;					//��ǰʹ�������Чid
		char baojia_cur_image_id;						//��ǰʹ�ñ�������id
		char baojia_cur_texiao_id;						//��ǰʹ�ñ�����Чid

		ShenqiItemParam shenbing_list[SHENQI_SUIT_NUM_MAX];
		ShenqiItemParam baojia_list[SHENQI_SUIT_NUM_MAX];
		char baojia_texiao_open_flag;
		char reserve_ch;
		short reserve_sh;
	};

	struct SCShenqiSingleInfo		//8588
	{
		SCShenqiSingleInfo();
		MessageHeader header;

		short info_type;						//������Ϣ����
		short item_index;						//������Ϣ��Ӧ�±�

		ShenqiItemParam shenqi_item;			//����������Ϣ
	};

	struct SCShenqiImageInfo		//8589
	{
		SCShenqiImageInfo();
		MessageHeader header;

		short info_type;						// ������Ϣ����	
		char cur_use_imgage_id;					// ��ǰʹ������id
		char cur_use_texiao_id;					// ��ǰʹ����Чid

		unsigned long long image_active_flag;	// ���󼤻���
		unsigned long long texiao_active_flag;	// ��Ч������
		char texiao_open_flag;
		char reserve_ch;
		short reserve_sh;
	};

	class SCShenqiDecomposeResult	//8590	�ֽ���
	{
	public:
		SCShenqiDecomposeResult();
		MessageHeader header;

		struct ItemInfo
		{
			int item_id;
			short num;
			char is_bind;
			char reserve_ch;
		};

		int item_count;
		ItemInfo item_list[MAX_ATTACHMENT_ITEM_NUM];
	};
}

#pragma pack(pop)

#endif