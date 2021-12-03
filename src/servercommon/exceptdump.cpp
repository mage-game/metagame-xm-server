#ifdef _WIN32

#define _CRT_SECURE_NO_DEPRECATE 1
#include <windows.h>
#include <dbghelp.h>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <algorithm>

#include "exceptdump.h"

char g_exe_file_path[MAX_PATH];
char g_dump_file_name[MAX_PATH];

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP) (
	HANDLE hProcess,
	DWORD ProcessId,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	const PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, 
	const PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, 
	const PMINIDUMP_CALLBACK_INFORMATION CallbackParam 
);	

static LPTOP_LEVEL_EXCEPTION_FILTER last_filter = 0;
static MINIDUMP_TYPE g_dump_type = DEFAULT_DUMP_TYPE;

const char *ErrorReportFile = "errorrpt.exe";

static void create_default_info(char *temp, PEXCEPTION_RECORD pRec)
{
	::_snprintf(temp, sizeof(temp),
		"Exception Code: %X, at address %p",
		pRec->ExceptionCode, pRec->ExceptionAddress);
}

static void log_callback(const char *str, std::string *logs)
{
	logs->append(str).append("\n");
}

LONG WINAPI _except_filter(EXCEPTION_POINTERS *pExcp)
{
	char temp[4096];
	const char* help_dll = "dbghelp.dll";

	::strncpy(temp, g_exe_file_path, strlen(g_exe_file_path) + 1);
	::strncat(temp, help_dll, strlen(help_dll) + 1);

	std::string dbgpath = temp;
	
	MINIDUMP_EXCEPTION_INFORMATION excp_info = {
		GetCurrentThreadId(), pExcp, FALSE};
	
	PEXCEPTION_RECORD pRec = pExcp->ExceptionRecord;
	
	HMODULE hDll = NULL;
	hDll = ::LoadLibrary(dbgpath.c_str());
	char filename[MAX_PATH];

	if (hDll)
	{	
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)
			::GetProcAddress(hDll, "MiniDumpWriteDump");
		if (pDump)
		{
			time_t rawtime;
			time(&rawtime);
			struct tm *timeinfo = localtime(&rawtime);

			::strftime(filename, MAX_PATH, "-%Y%m%d-%H%M%S-0.dmp", timeinfo);
			::strcat(g_dump_file_name, filename);

			for (int i = 0; i < 10; ++i)
			{
				g_dump_file_name[::strlen(g_dump_file_name) - 5] = i + 48;

				struct _stat sb;
				if (_stat(g_dump_file_name, &sb) != 0)
				{
					break;
				}
			}
				
			HANDLE hFile = ::CreateFile(g_dump_file_name, GENERIC_READ | GENERIC_WRITE, 
				0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile)
			{
				DWORD buf[256];
				DWORD *p = buf;

				MINIDUMP_USER_STREAM user_stream[1];
				user_stream[0].Type = 0x10001;
				user_stream[0].BufferSize = (DWORD)(p - buf) * sizeof(DWORD);
				user_stream[0].Buffer = buf;

				MINIDUMP_USER_STREAM_INFORMATION user_info;
				user_info.UserStreamCount = 1;
				user_info.UserStreamArray = user_stream;

				pDump(::GetCurrentProcess(), ::GetCurrentProcessId(),
					hFile, g_dump_type, &excp_info, &user_info, NULL);

				::CloseHandle(hFile);

				::_snprintf(temp, sizeof(temp), 
					"�ǳ��ź���֪ͨ���������ڲ������˲�����صĴ���\n"
					"���˵�����������Ѿ�����¼��\n\t%s%s\n�ļ��\n"
					"�ǳ�ϣ�����ܽ����ļ�����ͬ���µĴ�����룬\n"
					"Email ��������Ա�������ǿ������������������⣬���ֽ����\n\n"
					"Exception Code: %X, at address %p", 
					g_exe_file_path, g_dump_file_name, pRec->ExceptionCode, pRec->ExceptionAddress);
			} 
			else
			{
				create_default_info(temp, pRec);
			} 
		}
		else
		{
			create_default_info(temp, pRec);
		}
	}
	else
	{
		create_default_info(temp, pRec);
	}

	::MessageBox(0, temp, "�쳣����", MB_OK | MB_ICONSTOP);

	if (last_filter)
	{
		last_filter(pExcp);
	}

	ShellExecute(0, "open", ErrorReportFile, filename, 0, SW_SHOW);

	return EXCEPTION_CONTINUE_SEARCH;
}

void start_except_catch()
{
	GetModuleFileName(GetModuleHandle(NULL), g_exe_file_path, MAX_PATH);
	std::string str = g_exe_file_path;
	size_t pos = str.find_last_of('\\');
	size_t pos2 = str.find_last_of('.');

	g_exe_file_path[pos2] = 0;
	strcpy(g_dump_file_name, g_exe_file_path+pos+1);

	g_exe_file_path[pos+1] = 0;

	if (! last_filter)
	{		
		::SetErrorMode(SEM_NOGPFAULTERRORBOX);
		last_filter = ::SetUnhandledExceptionFilter(_except_filter);
	}
}

void set_dump_type(int dump_type)
{
	g_dump_type = (MINIDUMP_TYPE)dump_type;
}

void stop_except_catch()
{
	::SetErrorMode(0);
	::SetUnhandledExceptionFilter(last_filter);
}

#endif

