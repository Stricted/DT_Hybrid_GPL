local lp = require"lp"
local char, find, gsub = string.char, string.find, string.gsub
local open = io.open
local translate = lp.translate
----------------------------------------------------------------------------
-- Translates and executes a template in a given file.
-- The translation creates a Lua function which will be executed in an
-- optionally given environment.
-- @param filename String with the name of the file containing the template.
-- @param env Table with the environment to run the resulting function.
local BOM = char(239) .. char(187) .. char(191)

lp.setoutfunc("web.print")

local function precompile(insuffix, outsuffix, inputfile)
	print(inputfile)
	local fh = assert (open (inputfile))
	local src = fh:read("*a")
	fh:close()
	if src:sub(1,3) == BOM then src = src:sub(4) end
	
	local lua = translate (src)
	local outputfile = gsub(inputfile, insuffix, outsuffix)
	local fo = assert (open (outputfile, "w"))
	fo:write(lua)
	fo:close()

end

local insuffix 	= select(1, ...)
local outsuffix = select(2, ...)

local argc = select("#", ...)

for i = 3, argc do
	local inputfile = select(i, ...)
	precompile(insuffix, outsuffix, inputfile)
end


	



