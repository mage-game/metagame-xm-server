
#pragma once

#ifndef LOGAGENT_H
#define LOGAGENT_H

#include "common/estring.h"

#ifdef __GNUC__
#define ATTRIBUTE_FORMAT_FUNC(m, n) __attribute__ ((format(printf, m, n)))
#else
#define ATTRIBUTE_FORMAT_FUNC(m, n)
#endif

class ILogModule;

class LogAgent
{
public:
	LogAgent();
	~LogAgent();

	void printf(int log_level, int log_catagory, const char *fmt, ...) ATTRIBUTE_FORMAT_FUNC(4, 5);
	void printf(int log_level, const char *fmt, ...) ATTRIBUTE_FORMAT_FUNC(3, 4);
	void print(int log_level, int log_catagory, const char *str);
	void print(int log_level, const char *str);
	void SetLogModule(ILogModule* module);
	void SetCatagory(const char *catagory, const char *output = 0, int new_file_interval=0);

private:
	ILogModule* m_logmodule;
	int m_catagory;
	estring m_cata_name;
	estring m_output;
	int m_new_file_interval;

public:
	void buff_printf(const char *fmt, ...) ATTRIBUTE_FORMAT_FUNC(2, 3);
	void buff_print(const char *str);
	void commit_buff(int log_level);
	void clear_buff();
private:
	char *m_log_buff;
	int m_cur_buff_length;
	int m_max_buff_length;
	void resize_buff();
};

#endif
