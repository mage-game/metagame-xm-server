WorldDB = WorldDB or BaseClass(BaseDB)
function WorldDB:__init()

end

function WorldDB:__delete()
end

function WorldDB:GetDBPageNum()
	return WORLD_DB_PAGE_NUM
end

function WorldDB:GetFieldSizeList()
	return WORLD_DB_FIELD_SIZE_LIST
end

-- 重写实现
function WorldDB:Serialize(c_db, fieldid)
	if nil == c_db then
		return
	end

	if WORLD_DB_FIELD.RAND_ACTIVITY == fieldid then
	
	end
	
	if WORLD_DB_FIELD.COMMONDATA == fieldid then
		WordCommonData.Instance:Serialize(c_db)
	end
end

-- 重写实现
function WorldDB:UnSerialize(c_db, fieldid)
	if nil == c_db then
		return
	end

	if WORLD_DB_FIELD.RAND_ACTIVITY == fieldid then
		
	end

	if WORLD_DB_FIELD.COMMONDATA == fieldid then
		WordCommonData.Instance:UnSerialize(c_db)
	end
end