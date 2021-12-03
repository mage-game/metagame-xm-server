-- 配置检查策略
CHKPOLICY = 
{
	GTR = 1,		-- 大于
	LESS = 2,		-- 小于
	EQU = 3,		-- 等于
	NEQ = 4,		-- 不等于
	GTQ = 5,		-- 大于或等于
	LEQ = 6, 		-- 小于或等于
	
	INC = 7,		-- 递增
	INC_1 = 8,		-- 递增1
	FIRST_VAL = 9,	-- 第一个值必须是某个值
	LAST_VAL = 10,	-- 最后一个值必须是某个值

	ITEM = 20,		-- 是否是物品
	ITEM_T = 21,	-- 是否是物品表结构
	EQUIP = 22, 	-- 是否是装备
	MONSTER = 23, 	-- 是否是怪物
	SCENE = 24, 	-- 是否是场景
}

CheckPolicyMap = {}

function ClearPolicy()
	CheckPolicyMap = {}
end

function AddPolicy(depth, field, policy1, policy2, policy3)
	CheckPolicyMap[field] = {p_list = {policy1, policy2, policy3}, checked = false, construct_depth = depth}
end

function CheckPolicy(cfg_list)
	if nil == cfg_list then
		return false, "not found cfg_list"
	end

	local max_construce_depth = 0
	for k, v in pairs(CheckPolicyMap) do
		if v.construct_depth > max_construce_depth then
			max_construce_depth = v.construct_depth
		end
	end

	local last_cfg_item_list = {}

	for item_index, cfg_item in ipairs(cfg_list) do

		local cur_change_depth = max_construce_depth

		-- 找出当前有变化的最低深度
		for field_name, field_val in pairs(cfg_item) do
			local policy_t = CheckPolicyMap[field_name]
			if nil ~= policy_t and policy_t.construct_depth < max_construce_depth then

				local last_cfg_item = last_cfg_item_list[policy_t.construct_depth]
				
				if nil ~= last_cfg_item and last_cfg_item[field_name] ~= cfg_item[field_name] then
					if policy_t.construct_depth < cur_change_depth then
						cur_change_depth = policy_t.construct_depth
					end 
				end
			end
		end

		-- 对最低变化深度上面所有层的记录进行合法性检查，然后删除记录
		local ret, log = CheckAndRemoveUpDepthCfgItem(last_cfg_item_list, cur_change_depth)
		if not ret then
			return ret, log
		end

		local temp_cfg_item_list = {}

		for field_name, field_val in pairs(cfg_item) do				-- 检查每个项下的各个字段
			local policy_t = CheckPolicyMap[field_name] 			-- 获得各个字段的检查策略表
			if nil == policy_t then
				return false, string.format("%s is not in CheckPolicyMap", field_name)
			end

			local last_cfg_item = last_cfg_item_list[policy_t.construct_depth]

			for _, p_item in ipairs(policy_t.p_list) do
				local p_type = p_item[1]
				local p_param1 = p_item[2] or 0

				-- 大于
				if CHKPOLICY.GTR == p_type and not (field_val > p_param1) then
					return false, string.format("CHKPOLICY.GTR %s %d %d", field_name, p_param1, field_val)
				end

				-- 小于
				if CHKPOLICY.LESS == p_type and not (field_val < p_param1) then	 
					return false, string.format("CHKPOLICY.LESS %s %d %d", field_name, p_param1, field_val)
				end

				-- 等于
				if CHKPOLICY.EQU == p_type and not (field_val == p_param1) then	 
					return false, string.format("CHKPOLICY.EQU %s %d %d", field_name, p_param1, field_val)
				end

				-- 不等于
				if CHKPOLICY.NEQ == p_type and not (field_val ~= p_param1) then	 
					return false, string.format("CHKPOLICY.NEQ %s %d %d", field_name, p_param1, field_val)
				end

				-- 大于或等于
				if CHKPOLICY.GTQ == p_type and not (field_val >= p_param1) then	 
					return false, string.format("CHKPOLICY.GTQ %s %d %d", field_name, p_param1, field_val)
				end

				-- 小于或等于
				if CHKPOLICY.LEQ == p_type and not (field_val <= p_param1) then	 
					return false, string.format("CHKPOLICY.LEQ %s %d %d", field_name, p_param1, field_val)
				end

				-- 递增
				if CHKPOLICY.INC == p_type and nil ~= last_cfg_item then
					if not (field_val >= last_cfg_item[field_name]) then
						return false, string.format("CHKPOLICY.INC %s %d %d", field_name, last_cfg_item[field_name], field_val)
					end
				end

				-- 递增1
				if CHKPOLICY.INC_1 == p_type and nil ~= last_cfg_item then
					if not (1 == field_val - last_cfg_item[field_name]) then
						return false, string.format("CHKPOLICY.INC_1 %s %d %d", field_name, last_cfg_item[field_name], field_val)
					end
				end

				-- 第一个值必须是X
				if CHKPOLICY.FIRST_VAL == p_type and nil == last_cfg_item then
					if not (field_val == p_param1) then
						return false, string.format("CHKPOLICY.FIRST_VAL %s %d %d", field_name, p_param1, field_val)
					end
				end

				-- 是否是物品
				if not IS_PRE_DEBUG_CHECK_ING and CHKPOLICY.ITEM == p_type and not ItemManager.Instance:IsItemBase(field_val) then
					return false, string.format("CHKPOLICY.ITEM %s", field_name)
				end

				-- 是否是物品表结构
				if not IS_PRE_DEBUG_CHECK_ING and CHKPOLICY.ITEM_T == p_type then
					if not ("table" == type(field_val)) then
						return false, string.format("CHKPOLICY.ITEM_T not table, %s", field_name)
					end
					
					if not (field_val.item_id and ItemManager.Instance:IsItemBase(field_val.item_id)) then
						return false, string.format("CHKPOLICY.ITEM_T item_id, %s", field_name)
					end

					if not (field_val.num and field_val.num > 0) then
						return false, string.format("CHKPOLICY.ITEM_T num, %s", field_name)
					end

					if not (field_val.is_bind and field_val.is_bind >= 0 and field_val.is_bind <= 1) then
						return false, string.format("CHKPOLICY.ITEM_T is_bind, %s", field_name)
					end
				end

				-- 是否是装备
				if not IS_PRE_DEBUG_CHECK_ING and CHKPOLICY.EQUIP == p_type and not ItemManager.Instance:IsEquipment(field_val) then
					return false, string.format("CHKPOLICY.EQUIP %s", field_name)
				end

				-- 是否是怪物
				if not IS_PRE_DEBUG_CHECK_ING and CHKPOLICY.MONSTER == p_type and not MonsterManager.Instance:IsMonster(field_val) then
					return false, string.format("CHKPOLICY.MONSTER %s", field_name)
				end

				-- 是否是场景（加入c++里的场景检查列表）
				if not IS_PRE_DEBUG_CHECK_ING and CHKPOLICY.SCENE == p_type then
					C_LuaToCFun:AddSceneCheck(field_val)
				end
			end

			policy_t.checked = true
			temp_cfg_item_list[policy_t.construct_depth] = cfg_item
		end

		last_cfg_item_list = temp_cfg_item_list
	end

	-- 对最后记录列表进行合法性检查，然后删除记录
	local ret, log = CheckAndRemoveUpDepthCfgItem(last_cfg_item_list, 0)
	if not ret then
		return ret, log
	end

	-- 检查策略列表里的所有字段都是否检查过
	for k, v in pairs(CheckPolicyMap) do
		if not v.checked then
			return false, string.format("%s not checked", k)
		end
	end

	return true, "success"
end

function CheckAndRemoveUpDepthCfgItem(last_cfg_item_list, cur_change_depth)
	for i = #last_cfg_item_list, 1, -1 do
		if i > cur_change_depth then
			local last_cfg_item = last_cfg_item_list[i]
			for field_name, field_val in pairs(last_cfg_item) do				-- 检查每个项下的各个字段
				local policy_t = CheckPolicyMap[field_name]
				if nil == policy_t then
					return false, string.format("%s is not in CheckPolicyMap", field_name)
				end

				for _, p_item in ipairs(policy_t.p_list) do
					local p_type = p_item[1]
					local p_param1 = p_item[2] or 0

					-- 最后一个值必须是X
					if CHKPOLICY.LAST_VAL == p_type and policy_t.construct_depth > cur_change_depth then
						if not (last_cfg_item[field_name] == p_param1) then
					 		return false, string.format("CHKPOLICY.LAST_VAL %s %d %d", field_name, last_cfg_item[field_name], p_param1)
				 		end
					end
				end
			end

			table.remove(last_cfg_item_list, i)
		end
	end

	return true, "success"
end