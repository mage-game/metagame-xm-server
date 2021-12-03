#ifndef __CROSS_PERSON_RANK_SHADOW_HPP__
#define __CROSS_PERSON_RANK_SHADOW_HPP__

#include "servercommon/rankdef.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/struct/roleglobalmoduleinfo.hpp"

class CrossPersonRankShadow
{
public:
	static CrossPersonRankShadow &Instance();

	void OnSyncCrossTitleInfo(crossgameprotocal::CrossGameSyncCrossPersonRankTitleInfo *title_info);
	void GetTopRankInfo(int uid, CrossPersonRankInfo *personrank_info);
	long long GetTopUuid(int top_type, int rank_index);

protected:
	CrossPersonRankShadow();
	~CrossPersonRankShadow();

	CrossPersonRankTitleUuidInfo m_rank_title_uuid_info_list[CROSS_RANK_TOP_TYPE_MAX]; 
};

#endif


