
local print = print

-- 打印普通日志
function Log(...)
	print("[" .. os.date("%X", os.time()) .. "]" .. "[Lua Info] ", ...)
end

function Warn(...)
	print("[" .. os.date("%X", os.time()) .. "]" .. "[Lua Warn!] ", ...)
end

-- 会打印栈信息
function DebugLog(...)
	print(debug.traceback() .. "\n\t[Debug]", ...)
end

-- 错误日志，会打印栈信息
function ErrorLog(...)
	print(debug.traceback() .. "\n\t[Error]", ...)
end

-- 格式化输出字符串，类似c函数printf风格
function Printf(fmt, ...)
	print(string.format(fmt, ...))
end

-- 打印一个table
function PrintTable(tbl, level)
	level = level or 1

	local indent_str = ""
	for i = 1, level do
		indent_str = indent_str.."  "
	end

	print(indent_str .. "{")
	for k,v in pairs(tbl) do
		local item_str = string.format("%s%s = %s", indent_str .. "  ", tostring(k), tostring(v))
		print(item_str)
		if type(v) == "table" then
			PrintTable(v, level + 1)
		end
	end
	print(indent_str .. "}")
end
