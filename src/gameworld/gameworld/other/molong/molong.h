#ifndef __MOLONG_H__
#define __MOLONG_H__

#include "obj/character/attribute.hpp"

class Role;

class Molong
{
public:
	Molong();
	~Molong();

	void SetRole(Role *role) { m_role = role; }
	void Init(int curr_loop, int accumulate_consume_gold, int today_consume_gold, short total_move_step, short today_move_step,int rank_grade, int m_rank_cumulate_gold);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void GetInitParam(int *curr_loop, int *accumulate_consume_gold, int *today_consume_gold, short *total_move_step, short *today_move_step,int *rank_grade,int *rank_cumulate_gold);
	void OnDayChange(int old_day, int new_day);
	void OnConsumeGold(int gold);
	void OnRankUpgrade();
	int  GetRankGrade() { return m_rank_grade; }
	int GetRankValuePercent();
	bool OnJudgeRankGrade(Role *m_target);
	void PrintInfo(const char *reason, int param_1 = 0, long long param_2 = 0);
	void SendInfo();

private:
	Role *m_role;

	CharIntAttrs m_attrs;

	int m_curr_loop;
	int m_accumulate_consume_gold;
	int m_today_consume_gold;
	short m_total_move_step;
	short m_today_move_step;

	int m_rank_grade;
	int m_rank_cumulate_gold;
};

#endif
