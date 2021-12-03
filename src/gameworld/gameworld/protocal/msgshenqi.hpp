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
		SHENQI_OPERA_REQ_TYPE_INFO = 0,						//请求所有信息
		SHENQI_OPERA_REQ_TYPE_DECOMPOSE,					//分解 param_1:需要分解材料id
		SHENQI_OPERA_REQ_TYPE_SHENBING_INLAY,				//神兵镶嵌请求 param_1:id  param_2:部位 param_3:品质
		SHENQI_OPERA_REQ_TYPE_SHENBING_UPLEVEL,				//神兵升级请求 param_1:id  param_2:是否自动升级 param_3:一键发包数
		SHENQI_OPERA_REQ_TYPE_SHENBING_USE_IMAGE,			//神兵更换使用形象 param_1:使用形象id(0取消使用)
		SHENQI_OPERA_REQ_TYPE_SHENBING_USE_TEXIAO,			//神兵更换特效形象 param_1:使用特效id(0取消使用)
		SHENQI_OPERA_REQ_TYPE_BAOJIA_INLAY,					//宝甲镶嵌请求 param_1:id  param_2:部位 param_3:品质
		SHENQI_OPERA_REQ_TYPE_BAOJIA_UPLEVEL,				//宝甲升级请求 param_1:id  param_2:是否自动升级 param_3:一键发包数
		SHENQI_OPERA_REQ_TYPE_BAOJIA_USE_IMAGE,				//宝甲更换使用形象 param_1:使用形象id(0取消使用)
		SHENQI_OPERA_REQ_TYPE_BAOJIA_USE_TEXIAO,			//宝甲更换特效形象 param_1:使用特效id(0取消使用)
		SHENQI_OPERA_REQ_TYPE_SHENBING_TEXIAO_ACTIVE,		//神兵特效激活 param_1 激活神兵
		SHENQI_OPERA_REQ_TYPE_BAOJIA_TEXIAO_ACTIVE,			//宝甲特效激活 param_1 激活宝甲
		SHENQI_OPERA_REQ_TYPE_BAOJIA_OPEN_TEXIAO,			//显示宝甲特效
		SHENQI_OPERA_REQ_TYPE_MAX
	};

	enum SHENQI_SC_INFO_TYPE									//下发信息类型
	{
		SHENQI_SC_INFO_TYPE_SHENBING = 0,						// 神兵
		SHENQI_SC_INFO_TYPE_BAOJIA,								// 宝甲

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

		unsigned long long shenbing_image_flag;			//神兵形象激活标记
		unsigned long long shenbing_texiao_flag;		//神兵特效激活标记
		unsigned long long baojia_image_flag;			//宝甲形象激活标记
		unsigned long long baojia_texiao_flag;			//宝甲特效激活标记

		char shenbing_cur_image_id;						//当前使用神兵形象id
		char shenbing_cur_texiao_id;					//当前使用神兵特效id
		char baojia_cur_image_id;						//当前使用宝甲形象id
		char baojia_cur_texiao_id;						//当前使用宝甲特效id

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

		short info_type;						//神器信息类型
		short item_index;						//神器信息对应下标

		ShenqiItemParam shenqi_item;			//神器单个信息
	};

	struct SCShenqiImageInfo		//8589
	{
		SCShenqiImageInfo();
		MessageHeader header;

		short info_type;						// 神器信息类型	
		char cur_use_imgage_id;					// 当前使用形象id
		char cur_use_texiao_id;					// 当前使用特效id

		unsigned long long image_active_flag;	// 形象激活标记
		unsigned long long texiao_active_flag;	// 特效激活标记
		char texiao_open_flag;
		char reserve_ch;
		short reserve_sh;
	};

	class SCShenqiDecomposeResult	//8590	分解结果
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