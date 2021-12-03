BindTool = BindTool or {}

function BindTool.UnPack(param, count, i, ...)
	if i >= count then
		if i == count then
			return param[i], ...
		end
		return ...
	end
	return param[i], BindTool.UnPack(param, count, i + 1, ...)
end

function BindTool.Bind(func, ...)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind error!")
		return function() end
	end

	local count = select('#', ...)
	local param = {...}

	if 0 == count then
		return function(...) return func(...) end
	elseif 1 == count then
		return function(...) return func(param[1], ...) end
	elseif 2 == count then
		return function(...) return func(param[1], param[2], ...) end
	end

	return function(...) return func(BindTool.UnPack(param, count, 1, ...)) end
end

function BindTool.Bind1(func, param1)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind1 error!")
		return function() end
	end
	return function(...)
		return func(param1, ...)
	end
end

function BindTool.Bind2(func, param1, param2)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind2 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, ...)
	end
end

function BindTool.Bind3(func, param1, param2, param3)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind3 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3,...)
	end
end

function BindTool.Bind4(func, param1, param2, param3, param4)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind4 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3, param4, ...)
	end
end

function BindTool.Bind5(func, param1, param2, param3, param4, param5)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind5 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3, param4, param5, ...)
	end
end

function BindTool.Bind6(func, param1, param2, param3, param4, param5, param6)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind6 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3, param4, param5, param6, ...)
	end
end

function BindTool.Bind7(func, param1, param2, param3, param4, param5, param6, param7)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind7 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3, param4, param5, param6, param7, ...)
	end
end

function BindTool.Bind8(func, param1, param2, param3, param4, param5, param6, param7, param8)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind8 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3, param4, param5, param6, param7, param8, ...)
	end
end

function BindTool.Bind9(func, param1, param2, param3, param4, param5, param6, param7, param8, param9)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind9 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3, param4, param5, param6, param7, param8, param9, ...)
	end
end

function BindTool.Bind10(func, param1, param2, param3, param4, param5, param6, param7, param8, param9, param10)
	if type(func) ~= "function" then
		ErrorLog("BindTool.Bind10 error!")
		return function() end
	end
	return function(...)
		return func(param1, param2, param3, param4, param5, param6, param7, param8, param9, param10, ...)
	end
end
