minetest.__util.identifier = {}

function minetest.__util.identifier.encode(identifier)
	local split = identifier:find(':')
	if not split then return identifier end
	return identifier:sub(split + 1, #identifier)
end

function minetest.__util.identifier.decode(identifier)
	local split = identifier:find(':')
	if not split then return identifier end
	
	if identifier == '' then return '' end
	return 'minetest:' .. identifier
end
