local assert, error, loadstring, char = assert, error, loadstring, string.char
local open = io.open

module (...)

-- support utf-8
local BOM = char(239) .. char(187) .. char(191)

function Sendfile (filename)
	-- read the whole contents of the file
	local fh = assert (open (filename))
	local src = fh:read("*a")
	fh:close()
	if src:sub(1,3) == BOM then src = src:sub(4) end
	
	local prog, err = loadstring(src, filename)
	if not prog then error (err, 3) end
	prog ()
end

function Includefile (filename)
	-- read the whole contents of the file
	local fh = assert (open (filename))
	local src = fh:read("*a")
	fh:close()
	if src:sub(1,3) == BOM then src = src:sub(4) end
	
	local prog, err = loadstring(src, filename)
	if not prog then error (err, 3) end
	prog ()
end