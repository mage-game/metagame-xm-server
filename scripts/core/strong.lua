Strong = Strong or {}

IS_PRE_DEBUG_CHECK_ING = false	 	-- 是否在预先调试检查配置中
IS_ERROR_ACCEPT_ING = false			-- 是否在容错处理中
	

Strong.hitstory_error_dic = {}
Strong.error_id = os.time()

-- lua程序一启动就检查
function Strong:CheckOnStart()
	IS_PRE_DEBUG_CHECK_ING = true
	
	-- 预检查配置
	local ret, log = LogicConfigManager.Instance:LoadFromPreCheck()
	if not ret then
		ErrorLog("CheckOnStart config error", log)
		return false
	end

	-- 检查所有db
	ret, log = self:CheckAllDB()
	if not ret then
		ErrorLog("CheckOnStart db error", log)
		return false
	end

	IS_PRE_DEBUG_CHECK_ING = false

	return true
end

-- 检查所有数据库合法性
function Strong:CheckAllDB()
	local ret = self:CheckRoleDB()
	if ret < 0 then
		return false, "[Strong] CheckRoleDB Fail:" .. ret
	end

	ret = self:CheckWorldDB()
	if ret < 0 then
		return false, "[Strong] CheckWorldDB Fail:" .. ret
	end

	return true, "success"
end

-- 检查角色数据库的存取合法性
function Strong:CheckRoleDB()
	local c_db = C_LuaToCFun:GetCheckRoleDB()
	local c_param = C_LuaToCFun:GetCheckRoleDBParam()
	local roledb = RoleDB.New()
	local role = Role.New(nil)
	local field_t = roledb:GetFieldList()
	local max_size_t = roledb:GetFieldSizeList()
	local memblocks = roledb:GetAllMemblocks()
	
	if not roledb:CalcFieldsProfile(memblocks) then
		return -1
	end

	roledb:WriteMemBlock(c_param, c_db)
	for _, field in ipairs(field_t) do
		local write_size = 0
		local read_size = 0

		-- 检查写，并记下写的长度
		if nil == field.memblocks[1] or not c_db:OpenWriteStream(c_param, field.memblocks[1].pos) then
			return -2
		end

		roledb:Serialize(c_db, field.fieldid, role)
		write_size = c_db:GetTotalWriteSize()
		c_db:CloseStream()

		-- 检查读，并记下读的长度
		if nil == field.memblocks[1] or not c_db:OpenReadStream(c_param, field.memblocks[1].pos) then
			return -3
		end

		roledb:UnSerialize(c_db, field.fieldid, role)
		read_size = c_db:GetTotalReadSize()
		c_db:CloseStream()

		-- 两者必须相等
		if write_size ~= read_size then
			Warn("size must equal", field_t.fieldid, write_size, read_size)
			return -4
		end

		-- 不能超出最大值
		local max_size = max_size_t[field.fieldid]
		if write_size > max_size or read_size > max_size then
			Warn("size too big", field_t.fieldid, write_size, max_size)
			return - 5
		end
	end

	return 0
end

-- 检查世界数据库的存取合法性
function Strong:CheckWorldDB()
	local c_db = C_LuaToCFun:GetCheckWorldDB()
	local c_param = C_LuaToCFun:GetCheckWorldDBParam()
	local worlddb = WorldDB.New()
	local field_t = worlddb:GetFieldList()
	local max_size_t = worlddb:GetFieldSizeList()
	local memblocks = worlddb:GetAllMemblocks()
	
	if not worlddb:CalcFieldsProfile(memblocks) then
		return -1
	end
	
	worlddb:WriteMemBlock(c_param, c_db)
	for _, field in ipairs(field_t) do
		local write_size = 0
		local read_size = 0

		-- 检查写，并记下写的长度
		if nil == field.memblocks[1] or not c_db:OpenWriteStream(c_param, field.memblocks[1].pos) then
			return -2
		end

		worlddb:Serialize(c_db, field.fieldid)
		write_size = c_db:GetTotalWriteSize()
		c_db:CloseStream()

		-- 检查读，并记下读的长度
		if nil == field.memblocks[1] or not c_db:OpenReadStream(c_param, field.memblocks[1].pos) then
			return -3
		end

		worlddb:UnSerialize(c_db, field.fieldid)
		read_size = c_db:GetTotalReadSize()
		c_db:CloseStream()

		-- 两者必须相等
		if write_size ~= read_size then
			Warn("size must equal", write_size, read_size)
			return -4
		end

		-- 不能超出最大值
		local max_size = max_size_t[field.fieldid]
		if write_size > max_size or read_size > max_size then
			Warn("size too big", write_size, max_size)
			return - 5
		end
	end

	return 0
end

-- 当lua错误发生时的处理
function Strong:OnLuaError(error_msg)
	if IS_ERROR_ACCEPT_ING then
		return
	end

	IS_ERROR_ACCEPT_ING = true

	self:WriteErrorLog(error_msg)
	C_LuaToCFun:CloseAllDB()		-- 关闭正在运行的db

	if IS_SERVER_STARTING then		-- 服务器在启动中时lua出错停掉服器
		C_LuaToCFun:StopServer()
	end

	IS_ERROR_ACCEPT_ING = false
end

-- 输出lua错误日志到本地
function Strong:WriteErrorLog(error_msg)
	local msg = string.format("[Error]  time%s, msg:%s \n", os.date(), error_msg)
	C_LuaToCFun:WriteErrorLog(msg)

	-- local error_t = self.hitstory_error_dic[error_msg]
	-- if nil ~= error_t then
	-- 	if os.time() - error_t.time > 3 and error_t.num < 20 then	-- 内容输出做限制
	-- 		error_t.num = error_t.num + 1
	-- 		error_t.time = os.time()
	-- 		 --重复的错误内容只作统计,详细内容已在第一次错误时打出
	-- 		error_msg = string.format("[Error] time%s, id:%d, num:%d", os.date(), error_t.error_id, error_t.num) 
			
	-- 		C_LuaToCFun:WriteErrorLog(error_msg)
	-- 	end 
		
	-- 	return
	-- end

	-- error_t = {}
	-- error_t.error_id = self.error_id
	-- error_t.num = 0
	-- error_t.time = os.time()
	-- self.hitstory_error_dic[error_msg] = error_t
	-- self.error_id = self.error_id + 1

	-- error_msg = string.format("[Error]  time%s, id:%d, msg:%s \n", os.date(), error_t.error_id, error_msg)
	-- C_LuaToCFun:WriteErrorLog(error_msg)
end

-- 打印lua程序重要信息到本地
function Strong:PrintInfo(info, ...)
	Log(info, ...)
	C_LuaToCFun:WriteProgramLog(info)
end