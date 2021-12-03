LUA_SCRIPT_ROOT_DIR = ""					-- lua脚本所处的相对路径（loadfile方法要用）
IS_SERVER_STARTING = false					-- 服务器是否在启动过程中

HotUpdateImpl = nil
HotUpdateCacheDataSetCount = 0
HotUpdateCacheDataSetList = {}
HotUpdateCacheDynamicCreateDataCount = 0
HotUpdateCacheDynamicCreateDataList = {}
LuaCodesBak = nil

function __G__TRACKBACK__(msg)
	local log = debug.traceback() .. "\n[LUA-ERROR] " .. msg
	print(log)

	if nil ~= Strong then
		Strong:OnLuaError(log)
	end

	return msg
end

function __start(script_dir)
	LUA_SCRIPT_ROOT_DIR = script_dir
	return start()
end

function start()
	print("lua start")

	IS_SERVER_STARTING = true
	math.randomseed(os.time())
	
	LuaCodesBak = {}
	local update_list = __loadfile("hotupdatelist", LuaCodesBak)()

	for i, v in ipairs(update_list) do
		__loadfile(v, LuaCodesBak)()
	end

	HotUpdateImpl = HotUpdate.New()

	return Strong:CheckOnStart()
end

function __loadfile(path, codes_bak)
	local long_path = string.format("%s/%s.lua", LUA_SCRIPT_ROOT_DIR, path)
	local code = assert(loadfile(long_path))
	codes_bak[path] = code

	return code
end

function __getbakcode(codes_bak, path)
	return codes_bak[path]
end

function __pushcachedataset(data)
	HotUpdateCacheDataSetCount = HotUpdateCacheDataSetCount + 1
	table.insert(HotUpdateCacheDataSetList, data)
end

function __popcachedataset()
	HotUpdateCacheDataSetCount = HotUpdateCacheDataSetCount - 1

	return table.remove(HotUpdateCacheDataSetList, 1)
end

function __pushcachedynamiccreatedata(data)
	HotUpdateCacheDynamicCreateDataCount = HotUpdateCacheDynamicCreateDataCount + 1
	table.insert(HotUpdateCacheDynamicCreateDataList, data)
end

function __popcachedynamiccreatedata()
	HotUpdateCacheDynamicCreateDataCount = HotUpdateCacheDynamicCreateDataCount - 1

	return table.remove(HotUpdateCacheDynamicCreateDataList, 1)
end

function __hotupdate()
	Strong:PrintInfo("start hotupdate")
	
	if nil == LuaCodesBak then
		Strong:PrintInfo("start fail:bak is not exist")
		return
	end

	-- 先清理全局表数据
	HotUpdateCacheDataSetCount = 0
	HotUpdateCacheDynamicCreateDataCount = 0
	HotUpdateCacheDataSetList = {}
	HotUpdateCacheDynamicCreateDataList = {}

	-- 缓存数据到全局表
	HotUpdateImpl:CacheDataset()
	HotUpdateImpl:CacheCreateData()

	-- 备份缓存到的数据，用于热更时失败后恢复数据用
	local HotUpdateCacheDataSetCount_BAK = HotUpdateCacheDataSetCount
	local HotUpdateCacheDynamicCreateDataCount_BAK = HotUpdateCacheDynamicCreateDataCount
	local HotUpdateCacheDataSetList_BAK = TableCopy(HotUpdateCacheDataSetList)
	local HotUpdateCacheDynamicCreateDataList_BAK = TableCopy(HotUpdateCacheDynamicCreateDataList)

	local new_codes_bak = {}

	function restart()
		HotUpdateImpl:DeleteObjects()
		HotUpdateImpl:DeleteMe()
		
		local update_list = __loadfile("hotupdatelist", new_codes_bak)()
		for i, v in ipairs(update_list) do
			__loadfile(v, new_codes_bak)()
		end
	
		HotUpdateImpl = HotUpdate.New()
		HotUpdateImpl:ResumeDataset()
		HotUpdateImpl:OnHotUpdateFinish()

		if HotUpdateCacheDataSetCount > 0 or HotUpdateCacheDynamicCreateDataCount > 0 then
			return -1
		end
	 	
		if not LogicConfigManager.Instance:LoadFromRecreate(new_codes_bak) then
			return -2
		end

		return 0
	end

	function revert_on_restart_fail()
		HotUpdateCacheDataSetCount = HotUpdateCacheDataSetCount_BAK
		HotUpdateCacheDynamicCreateDataCount = HotUpdateCacheDynamicCreateDataCount_BAK
		HotUpdateCacheDataSetList = HotUpdateCacheDataSetList_BAK
		HotUpdateCacheDynamicCreateDataList = HotUpdateCacheDynamicCreateDataList_BAK

		HotUpdateImpl:DeleteObjects()
		HotUpdateImpl:DeleteMe()

		local update_list = __getbakcode(LuaCodesBak, "hotupdatelist")()
		for i, v in ipairs(update_list) do
			 __getbakcode(LuaCodesBak, v)()
		end

		HotUpdateImpl = HotUpdate.New()
		HotUpdateImpl:ResumeDataset()
		LogicConfigManager.Instance:LoadFromRevert()
	end

	-- 开始加载新代码并重启
	local call_ret, return_val = xpcall(restart, __G__TRACKBACK__)

	if not call_ret or return_val < 0 then	 -- 热更失败后需回滚
		Strong:PrintInfo("hotupdate fail, so start revert")
		revert_on_restart_fail()
		Strong:PrintInfo("revert success, but hotupdate fail")
	else									 -- 热更成功
		LuaCodesBak = new_codes_bak
		Strong:PrintInfo("hotupdate success")
	end

	new_codes_bak = nil
	HotUpdateCacheDataSetList = {}
	HotUpdateCacheDynamicCreateDataList = {}
end
