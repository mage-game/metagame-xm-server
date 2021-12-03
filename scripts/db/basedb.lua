BaseDB = BaseDB or BaseClass(BaseHotUpdate)
function BaseDB:__init()
	self:InitDataset()

	self:CreatePages()
	self:CreateFields()
end

function BaseDB:__delete()
end

function BaseDB:InitDataset()
	self.dataset.profile_iscalced = false
	self.dataset.page_t = {}
	self.dataset.field_t = {}
end

function BaseDB:GetFieldList()
	return self.dataset.field_t
end

function BaseDB:SerializeProfile(c_datastream)
	if nil == c_datastream then
		return
	end

	local memblocks = self:GetAllMemblocks()
	c_datastream:WriteInt(#memblocks)

	for i, v in ipairs(memblocks) do
		c_datastream:WriteUShort(v.fieldid)
		c_datastream:WriteUShort(v.size)
		c_datastream:WriteUShort(v.pos)
		c_datastream:WriteUShort(v.nextpos)
	end
end

function BaseDB:UnSerializeProfile(c_datastream)
	if nil == c_datastream then
		return -100
	end

	local memblocks = {}
	local memblock_num = c_datastream:ReadInt()

	for i = 1, memblock_num do
		local memblock = self:CreateOneMemblock()
		memblock.fieldid = c_datastream:ReadUShort()
		memblock.size = c_datastream:ReadUShort()
		memblock.pos = c_datastream:ReadUShort()
		memblock.nextpos = c_datastream:ReadUShort()
		table.insert(memblocks, memblock)
	end

	local ret = self:CalcFieldsProfile(memblocks)

	return ret
end

function BaseDB:GetMemblockLogicPos(pageid, pos)
	return (pageid - 1) * DB_MAX_PAGE_SIZE + pos
end

function BaseDB:GetPageId(pos)
	return math.floor(pos / DB_MAX_PAGE_SIZE) + 1
end

function BaseDB:CreateOnePage()
	local page = {}
	page.pageid = 0
	page.size = 0
	return page
end

function BaseDB:CreateOneMemblock()
	local memblock = {}
	memblock.fieldid = 0
	memblock.size = DB_MEMBLOCK_SIZE
	memblock.pos = 0
	memblock.nextpos = 0
	return memblock
end

function BaseDB:CreateOneField()
	local field = {}
	field.fieldid = 0
	field.size = 0
	field.memblocks = {}
	return field
end

function BaseDB:CreatePages()
	self.dataset.page_t = {}

	local page_num = self:GetDBPageNum()
	for i = 1, page_num do
		local page = self:CreateOnePage()
		page.pageid = i
		page.size = 0
		table.insert(self.dataset.page_t, page)
	end
end

function BaseDB:CreateFields()
	self.dataset.field_t = {}
	
	local field_size_list = self:GetFieldSizeList()
	for i = 1, #field_size_list do
		local field = self:CreateOneField()
		field.fieldid = i
		field.size = field_size_list[i]
		table.insert(self.dataset.field_t, field)
	end
end

function BaseDB:GetAllMemblocks()
	local memblocks = {}

	for i, v in ipairs(self.dataset.field_t) do
		for _, memblock in ipairs(v.memblocks) do
			table.insert(memblocks, memblock)
		end
	end

	return memblocks
end

function BaseDB:CalcFieldsProfile(memblocks)
	local field_old_size_t = {}
	local ret = 0

	for _, field in ipairs(self.dataset.field_t) do
		field.memblocks = {}
	end

	for _, memblock in ipairs(memblocks) do
		local field = self.dataset.field_t[memblock.fieldid]
		if nil ~= field then
			table.insert(field.memblocks, memblock)
			field_old_size_t[memblock.fieldid] = (field_old_size_t[memblock.fieldid] or 0) + memblock.size
		end
	end

	self:CalcPageSize(memblocks)

	for _, field in ipairs(self.dataset.field_t) do
		local old_size = field_old_size_t[field.fieldid] or 0
		if field.size - old_size > 0 then
			ret = self:AllocMoreMemForField(field.size - old_size, field)
			if ret < 0 then
				break
			end
		end
	end

	self:CalcPageSize(self:GetAllMemblocks())

	if ret >= 0 then
		self.dataset.profile_iscalced = true
	end

	return ret
end

function BaseDB:CalcPageSize(memblocks)
	for i, v in ipairs(self.dataset.page_t) do
		v.size = 0
	end

	for i, v in ipairs(memblocks) do
		local pageid = self:GetPageId(v.pos)
		local page = self.dataset.page_t[pageid]
		if nil ~= page then
			page.size = page.size + v.size
		end
	end
end

function BaseDB:AllocMoreMemForField(alloc_size, field)
	if nil == field then
		return -1
	end

	while(alloc_size > 0) do
		local alloc_size_once = math.min(alloc_size, DB_MEMBLOCK_SIZE)
		local memblock = self:AllockFreeMemBlock(alloc_size_once)
		
		if nil == memblock then
			return -2
		end

		if #field.memblocks > 0 then
			field.memblocks[#field.memblocks].nextpos = memblock.pos
		end
		memblock.fieldid = field.fieldid
		table.insert(field.memblocks, memblock)

		alloc_size = alloc_size - alloc_size_once
	end

	return 0
end

function BaseDB:AllockFreeMemBlock(size)
	for _, page in ipairs(self.dataset.page_t) do
		if page.size + size <= DB_MAX_PAGE_SIZE then

			local memblock = self:CreateOneMemblock()
			memblock.pos = self:GetMemblockLogicPos(page.pageid, page.size)
			memblock.size = size
			
			page.size = page.size + size  -- 调整页的大小

			return memblock
		end
	end

	return nil
end

function BaseDB:OnInitParam(c_param, c_db, param1)
	if not self.dataset.profile_iscalced or nil == c_param or nil == c_db then
		return
	end

	for _, field in ipairs(self.dataset.field_t) do
		if nil ~= field.memblocks[1] and c_db:OpenReadStream(c_param, field.memblocks[1].pos) then
			self:UnSerialize(c_db, field.fieldid, param1)
			c_db:CloseStream()
		end
	end
end

function BaseDB:OnGetInitParam(c_param, c_db, param1)
	if not self.dataset.profile_iscalced or nil == c_param or nil == c_db then
		return
	end

	self:WriteMemBlock(c_param, c_db)
	self:ResizePageSize(c_param, c_db)

	for _, field in ipairs(self.dataset.field_t) do
		if nil ~= field.memblocks[1] and c_db:OpenWriteStream(c_param, field.memblocks[1].pos) then
			self:Serialize(c_db, field.fieldid, param1)
			c_db:CloseStream()
		end
	end
end

function BaseDB:WriteMemBlock(c_param, c_db)
	if nil == c_param or nil == c_db then
		return
	end

	for _, field in ipairs(self.dataset.field_t) do
		for _,memblock in ipairs(field.memblocks) do
			c_db:WriteMemBlock(c_param, memblock.size, memblock.pos, memblock.nextpos)
		end
	end

	for _, page in ipairs(self.dataset.page_t) do
		c_db:ResizePageSize(c_param, page.pageid, page.size)	
	end
end

function BaseDB:ResizePageSize(c_param)
	if nil == c_param or nil == c_db then
		return
	end

	for i, v in ipairs(self.dataset.page_t) do
		c_db:ResizePageSize(c_param, v.pageid, v.size)	
	end
end

-- 重写实现
function BaseDB:Serialize(c_db, fieldid, param1)

end

-- 重写实现
function BaseDB:UnSerialize(c_db, fieldid, param1)

end

function BaseDB:GetDBPageNum()
	return 0
end

function BaseDB:GetFieldSizeList()
	return {}
end

function BaseDB:OnHotUpdateFinish()
	local memblocks = self:GetAllMemblocks()
	local ret = self:CalcFieldsProfile(memblocks)
	if ret < 0 then
		ErrorLog("[db updatefinish] CalcFieldsProfile Fail", ret)
	end

	BaseHotUpdate.OnHotUpdateFinish(self)
end

function BaseHotUpdate:PrintDBInfo()
	PrintTable(self.dataset.field_t)
end