
#ifndef MEMMONITOR_H
#define MEMMONITOR_H


namespace memmonitor
{
	long long GetMemUse(int *block_num);
	long long GetMemAlloc(int *block_num);
	int PrintDyMemInfo(char *buff);
}



#endif

