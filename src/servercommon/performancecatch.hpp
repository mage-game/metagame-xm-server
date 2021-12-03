#ifndef __PERFORMANCE_CATCH_HPP__
#define __PERFORMANCE_CATCH_HPP__

#include "servercommon/platfromhelper.hpp"

#include <map>
#include <stack>

enum PERFORMANCE_CATCH_TYPE
{
	PERFORMANCE_CATCH_TYPE_INVALID = 0,
	PERFORMANCE_CATCH_TYPE_RELOAD_CONFIG,						// ÷ÿ∂¡≈‰÷√
	PERFORMANCE_TEST_SINGLE_CHAT,								// ÀΩ¡ƒ
	PERFORMANCE_TEST_CHANEL_CHAT,								// ∆µµ¿¡ƒÃÏ
	PERFORMANCE_CATCH_TYPE_MAX,
};

class PerformanceCatch
{
public:
	static PerformanceCatch & Instance();

	void CalcPerformanceCatchBegin(int catch_type);
	void CalcPerformanceCatchEnd(int catch_type);

	void Update(unsigned long interval);

	void SimplePerformanceCatchBegin(int catch_type);
	void SimplePerformanceCatchEnd(int catch_type);

private:
	PerformanceCatch();
	~PerformanceCatch();

	struct StackItem
	{
		StackItem() : catch_type(PERFORMANCE_CATCH_TYPE_INVALID) {}

		int catch_type;

#ifdef _WIN32
		LARGE_INTEGER nFreq;
		LARGE_INTEGER start_time;
		LARGE_INTEGER end_time;
#else 
		timeval tv_start;
		timeval tv_end;
#endif 
	};

	bool m_working;
	time_t m_output_interval;

	std::stack<StackItem> m_catch_stack;

	struct PerformanceItem
	{
		PerformanceItem() : call_count(0), total_us(0) {}
		PerformanceItem(long long _call_count, long long _total_us) : call_count(_call_count), total_us(_total_us) {}

		long long call_count;
		long long total_us;
	};

	typedef std::map<int, PerformanceItem> PerformanceItemMap;
	typedef std::map<int, PerformanceItem>::iterator PerformanceItemMapIt;
	PerformanceItemMap m_performance_map;

	StackItem m_simple_catch;
};

#endif // __PERFORMANCE_CATCH_HPP__

