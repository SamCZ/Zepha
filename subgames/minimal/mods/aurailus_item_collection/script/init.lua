runfile("@aurailus:item_collection/dropped_item")

local DROP_ENTITY = true

if DROP_ENTITY then runfile("@aurailus:item_collection/mode/entity")
else runfile("@aurailus:item_collection/mode/direct") end