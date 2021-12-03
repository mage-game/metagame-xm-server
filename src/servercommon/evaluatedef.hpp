#ifndef __EVALUATEDEF_HPP__
#define __EVALUATEDEF_HPP__

#include "servercommon/servercommon.h"

static const int MAX_EVALUSTE_NUM = 10;			// ������۴���
static const int EVALUSTE_NEED_LEVEL = 30;		// ��������ȼ�

struct EvaluateParam
{
public:
	EvaluateParam() { this->Reset(); }

	void Reset()
	{
		memset(uid_list, 0, sizeof(uid_list));
	}

	int uid_list[MAX_EVALUSTE_NUM];
};

typedef char EvaluateParamHex[sizeof(EvaluateParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(EvaluateParamHex) < 128);

#endif

