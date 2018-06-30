-- this is a Lua 5.3 script
-- you also need to have installed GraphViz and optionally ImageMagick

local source_directory = "~/github/image-test/src/"
local ignored_prefixes = {"Sk", "tinyutf8", "glm", "include/glm", "selene"}
local ignored_suffixes = {".vert", ".frag"}
local merge_headers    = true -- whether to show .h and .cpp files separately or not
local ignore_system_libraries = true

local echo = io.popen("echo ~", "r")
local home_directory = echo:read()
echo:close()

local inclusions = {}

local dir = io.popen("ls " .. source_directory)

for entry in dir:lines() do
	local suffix = string.lower(string.sub(entry,-2))
	
	local is_source_file = false
	local extensionless_filename
	if suffix == ".h" or suffix == ".c" then
		extensionless_filename = string.sub(entry, 1, -3)
		is_source_file = true
	else
		suffix = string.lower(string.sub(entry, -4))
		if suffix == ".cpp" or suffix == ".hpp" or suffix == ".cxx" then
			is_source_file = true
			extensionless_filename = string.sub(entry, 1, -5)
		end
	end

	if is_source_file then
		local src_dir = home_directory .. string.sub(source_directory, 2)
		local includes
		if merge_headers then
			if not inclusions[extensionless_filename] then
				inclusions[extensionless_filename] = {}
			end
			includes = inclusions[extensionless_filename]
		else
			inclusions[entry] = {}
			includes = inclusions[entry]
		end

		local file = io.open(src_dir .. entry)
		print("Scanning: " .. src_dir .. "\x1b[1m" .. entry .. "\x1b[0m")
--		print(file)
		for line in file:lines() do

			if not ignore_system_libraries then
				local sys_lib_match = string.match(line, "%s*#include%s*<%s*(%w*)")
				if sys_lib_match then
					includes[#includes + 1] = sys_lib_match
				end
			end

			local user_inclusion_match = string.match(line, "^%s*#include%s*\"%s*([%w/_.]*)")

			if user_inclusion_match then
				local ignore_it = false
				for _,v in pairs(ignored_prefixes) do
					if string.match(user_inclusion_match, "^"..v) then
						ignore_it = true
					end
				end
				for _,v in pairs(ignored_suffixes) do
					if string.match(user_inclusion_match, v.."$") then
						ignore_it = true
					end
				end
				if not ignore_it then
					local dependency = user_inclusion_match

					if merge_headers and string.lower(string.sub(user_inclusion_match, -2)) == ".h" then
						dependency = string.sub(user_inclusion_match, 1, -3)
					end
					if merge_headers then
						if extensionless_filename ~= dependency then
							includes[#includes + 1] = dependency
						end
					else
						includes[#includes + 1] = dependency
					end
				end
			end
		end
		file:close()

	end
end
dir:close()

local out = io.open("deps.graphviz", "w")

out:write([[digraph G {
	edge [color = "#888888", penwidth = "3"]
	node [fontcolor="#eeeeee",
			color = "#222222",
			fontname="sans",
			style = filled, fillcolor = "#333333"]
	graph [bgcolor = "#000000",style=filled,fillcolor="#000000"] 

	selene [label=<selene<br /><sub><i>(Lua interface lib)</i></sub>>]
	]])
--	globals [label=<globals<br /><sub><i>debugging macros, constants, and<br />containers for user-configurable settings</i></sub>>]



for file, includes in pairs(inclusions) do
	for _, included in pairs(includes) do
		out:write('\t"' .. file .. "\" -> \"" .. included .. '"\n')
	end
end

out:write("\n}")
out:close()

-- use GraphViz to draw the diagram
os.execute("dot -Tpng deps.graphviz -o graph_of_dependencies.png")

-- use ImageMagick to remove one pixel wide transparent margin which looks weird in my image viewer
os.execute("convert graph_of_dependencies.png -crop -1-1 graph_of_dependencies.png")
