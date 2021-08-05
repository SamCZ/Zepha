_G['minetest'] = {}

-- local global_meta = {
-- 	__newindex = function(tbl, key, val)
-- 		print("HI!")
-- 		dump(key)
-- 		dump(val)
-- 	end,
-- 	__index = getmetatable(_G)
-- }

-- setmetatable(_G, global_meta)

minetest.__usertype = {}

runfile(_PATH .. "usertype/itemstack")
runfile(_PATH .. "usertype/object")
runfile(_PATH .. "usertype/player")

minetest.__util = {}

runfile(_PATH .. "util/identifier")

runfile(_PATH .. "api/base")
runfile(_PATH .. "api/register")
