DB_MEM_BLOCK_HEAD_SIZE = 8 				-- 内存区块头大小8个字节（不能改）
DB_MEMBLOCK_SIZE = 256					-- DB一个内存区块的大小(包括内存区块头)
DB_MAX_PAGE_SIZE = 4096   				-- DB页的最大大小，不能更改。若要增加存储，增加页数即可

-- 页数不够用时，在真实数据库里增加字段，改c++,
ROLE_DB_PAGE_NUM = 5 					-- 角色DB页的数量，若要增加需要C++数据库role表增加页数字段
WORLD_DB_PAGE_NUM = 2 					-- 世界DB页的数量，若要增加需要C++数据库worldstatus表增加页数字段		

-- 角色数据库表字段定义，必须从1开始,id不能删改
ROLE_DB_FIELD = {
	COMMONDATA = 1,		-- 通用数据
	LINGYU = 2,			-- 灵玉
	CHALLENGE = 3,		-- 挑战本
}

-- 世界数据库表字段定义，必须从1开始,id不能删改
WORLD_DB_FIELD = {
	COMMONDATA = 1,				-- 通用数据
}

-- 角色数据字段大小，页数足够的情况下，可任意调整增大
ROLE_DB_FIELD_SIZE_LIST = {}
ROLE_DB_FIELD_SIZE_LIST[ROLE_DB_FIELD.COMMONDATA] = 512
ROLE_DB_FIELD_SIZE_LIST[ROLE_DB_FIELD.LINGYU] = 256
ROLE_DB_FIELD_SIZE_LIST[ROLE_DB_FIELD.CHALLENGE] = 320

-- 世界数据字段大小，页数足够的情况下，可任意调整增大
WORLD_DB_FIELD_SIZE_LIST = {}
WORLD_DB_FIELD_SIZE_LIST[WORLD_DB_FIELD.COMMONDATA] = 512
