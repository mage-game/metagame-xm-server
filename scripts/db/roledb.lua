RoleDB = RoleDB or BaseClass(BaseDB)
function RoleDB:__init()
	
end

function RoleDB:__delete()
end

function RoleDB:GetDBPageNum()
	return ROLE_DB_PAGE_NUM
end

function RoleDB:GetFieldSizeList()
	return ROLE_DB_FIELD_SIZE_LIST
end

-- 重写实现
function RoleDB:Serialize(c_db, fieldid, c_role)
	if nil == c_db or nil == c_role then
		return
	end

	if ROLE_DB_FIELD.COMMONDATA == fieldid then
		c_role:GetCommonData():Serialize(c_db)
	end

	if ROLE_DB_FIELD.LINGYU == fieldid then
		c_role:GetLingyu():Serialize(c_db)
	end

	if ROLE_DB_FIELD.CHALLENGE == fieldid then
		c_role:GetChallengeFb():Serialize(c_db)
	end
end

-- 重写实现
function RoleDB:UnSerialize(c_db, fieldid, c_role)
	if nil == c_db or nil == c_role then
		return
	end

	if ROLE_DB_FIELD.COMMONDATA == fieldid then
		c_role:GetCommonData():UnSerialize(c_db)
	end

	if ROLE_DB_FIELD.LINGYU == fieldid then
		c_role:GetLingyu():UnSerialize(c_db)
	end

	if ROLE_DB_FIELD.CHALLENGE == fieldid then
		c_role:GetChallengeFb():UnSerialize(c_db)
	end
end