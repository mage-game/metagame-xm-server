#include "performancecatch.hpp"
#include "servercommon.h"

PerformanceCatch::PerformanceCatch()
	: m_working(false), m_output_interval(0)
{
 	#ifdef _DEBUG
		// m_working = true;
 	#endif 
}

PerformanceCatch::~PerformanceCatch()
{

}

PerformanceCatch & PerformanceCatch::Instance()
{
	static PerformanceCatch performance_catch;
	return performance_catch;
}

void PerformanceCatch::CalcPerformanceCatchBegin(int catch_type)
{
	if (m_working)
	{
		if (m_catch_stack.size() >= 32)
		{
			m_working = false;
			printf("PerformanceCatch::PerformanceCatchBegin catch_type[%d] too much stack size >= 32 [stop working] \n", catch_type);
		}
		else 
		{
			StackItem stack_item;
			stack_item.catch_type = catch_type;

#ifdef _WIN32
			QueryPerformanceFrequency(&stack_item.nFreq);
			QueryPerformanceCounter(&stack_item.start_time);
#else 
			gettimeofday(&stack_item.tv_start, NULL);
#endif 

			m_catch_stack.push(stack_item);
		}
	}
}

void PerformanceCatch::CalcPerformanceCatchEnd(int catch_type)
{
	if (m_working)
	{
		StackItem &stack_item = m_catch_stack.top();

		if (stack_item.catch_type != catch_type)
		{
			m_working = false;
			printf("PerformanceCatch::PerformanceCatchEnd catch_type[%d] dismatch [stop working] \n", catch_type);
		}
		else 
		{
			long long elapse_us = 0;

#ifdef _WIN32
			QueryPerformanceCounter(&stack_item.end_time);
			if (stack_item.nFreq.QuadPart > 0)
			{
				elapse_us = (long long)(stack_item.end_time.QuadPart - stack_item.start_time.QuadPart) * 1000000 / stack_item.nFreq.QuadPart;
			}
#else 
			gettimeofday(&stack_item.tv_end, NULL);

			if (stack_item.tv_end.tv_sec == stack_item.tv_start.tv_sec)
			{
				elapse_us = (long long)(stack_item.tv_end.tv_usec - stack_item.tv_start.tv_usec);
			}
			else 
			{
				elapse_us = (long long)((stack_item.tv_end.tv_sec - stack_item.tv_start.tv_sec) * 1000000 + (stack_item.tv_end.tv_usec - stack_item.tv_start.tv_usec));
			}
#endif 
			PerformanceItemMapIt it = m_performance_map.find(catch_type);
			if (it != m_performance_map.end())
			{
				++it->second.call_count;
				it->second.total_us += elapse_us;
			}
			else 
			{
				m_performance_map[catch_type] = PerformanceItem(1, elapse_us);
			}

			m_catch_stack.pop();
		}
	}
}

void PerformanceCatch::Update(unsigned long interval)
{
	if (m_working)
	{
		m_output_interval += interval;
		if (m_output_interval >= 10000)
		{
			printf("\n===================================== PerformanceCatch Begin ====================================== \n");

			for (PerformanceItemMapIt it = m_performance_map.begin(), end = m_performance_map.end(); it != end; ++ it)
			{
				if (it->second.call_count > 0)
				{
					printf("[catch_type:%d] [call_count:%lld total_us:%lld avg_us:%lld] \n", it->first, it->second.call_count, it->second.total_us, it->second.total_us / it->second.call_count);
				}
			}

			printf("======================================== PerformanceCatch End =================================== \n\n");

			m_output_interval = 0;
		}
	}
}

void PerformanceCatch::SimplePerformanceCatchBegin(int catch_type)
{
#ifdef _WIN32
	QueryPerformanceFrequency(&m_simple_catch.nFreq);
	QueryPerformanceCounter(&m_simple_catch.start_time);
#else 
	gettimeofday(&m_simple_catch.tv_start, NULL);
#endif 

	printf("\nSimplePerformanceCatchBegin please waiting...... \n"); fflush(stdout);
}

void PerformanceCatch::SimplePerformanceCatchEnd(int catch_type)
{
	long long elapse_us = 0;

#ifdef _WIN32
	QueryPerformanceCounter(&m_simple_catch.end_time);
	if (m_simple_catch.nFreq.QuadPart > 0)
	{
		elapse_us = (long long)(m_simple_catch.end_time.QuadPart - m_simple_catch.start_time.QuadPart) * 1000000 / m_simple_catch.nFreq.QuadPart;
	}
#else 
	gettimeofday(&m_simple_catch.tv_end, NULL);

	if (m_simple_catch.tv_end.tv_sec == m_simple_catch.tv_start.tv_sec)
	{
		elapse_us = (long long)(m_simple_catch.tv_end.tv_usec - m_simple_catch.tv_start.tv_usec);
	}
	else 
	{
		elapse_us = (long long)((m_simple_catch.tv_end.tv_sec - m_simple_catch.tv_start.tv_sec) * 1000000 + (m_simple_catch.tv_end.tv_usec - m_simple_catch.tv_start.tv_usec));
	}
#endif

	printf("SimplePerformanceCatchEnd :%lld \n\n", elapse_us); fflush(stdout);
}



