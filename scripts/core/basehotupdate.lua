BaseHotUpdate = BaseHotUpdate or BaseClass()

function BaseHotUpdate:__init()
	self.dynamic_creatdata_queue = {}

	self.object_queue = {}
	self.object_t = {}

	self.dataset = {}

	local create_data_queue = __popcachedynamiccreatedata()

	self:CreateObjects()
	
	if nil ~= create_data_queue then
		for _, v in ipairs(create_data_queue) do
			self:CreateDynamicObject(v.name, v.class, v.param1, v.param2)
		end
	end
end

function BaseHotUpdate:__delete()
end

function BaseHotUpdate:DeleteObjects()
	for _, v in ipairs(self.object_queue) do
		v:DeleteObjects()
		v:DeleteMe()
	end

	self.dynamic_creatdata_queue = {}
	self.object_queue = {}
	self.object_t = {}
end

function BaseHotUpdate:CreateObjects()
end

function BaseHotUpdate:CacheDataset()
	__pushcachedataset(self.dataset or {})

	for _, v in ipairs(self.object_queue) do
		v:CacheDataset()
	end
end

function BaseHotUpdate:ResumeDataset()
	self.dataset = __popcachedataset()

	for _, v in ipairs(self.object_queue) do
		v:ResumeDataset()
	end
end

function BaseHotUpdate:CacheCreateData()
	__pushcachedynamiccreatedata(self.dynamic_creatdata_queue or {})

	for _, v in ipairs(self.object_queue) do
		v:CacheCreateData()
	end
end

function BaseHotUpdate:CreateDynamicObject(name, class, param1, param2)
	local object = class.New(param1, param2)
	self:AddObject(name, object)

	table.insert(self.dynamic_creatdata_queue, {name = name, class = class, param1 = param1, param2 = param2})
	return object
end

function BaseHotUpdate:AddObject(name, object)
	if nil == self:GetObject(name) and nil ~= object then
		self.object_t[name] = object
		table.insert(self.object_queue, object)
	end
end

function BaseHotUpdate:DeleteObject(name)
	if nil == name then
		return
	end

	local object = self:GetObject(name)

	if nil ~= object then
		object:DeleteObjects()
		object:DeleteMe()

		for i, v in ipairs(self.object_queue) do
			if v == object then
				table.remove(self.object_queue, i)
				break
			end
		end
		
		for i, v in ipairs(self.dynamic_creatdata_queue) do
			if v.name == name then
				table.remove(self.dynamic_creatdata_queue, i)
				break
			end
		end
	end

	self.object_t[name] = nil
end

function BaseHotUpdate:GetObject(name)
	return self.object_t[name]
end

function BaseHotUpdate:OnHotUpdateFinish()
	for _, v in ipairs(self.object_queue) do
		v:OnHotUpdateFinish()
	end
end
