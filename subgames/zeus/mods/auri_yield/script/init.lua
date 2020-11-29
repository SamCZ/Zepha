runfile("@auri:yield/dropped_item")

local DROP_ENTITY = true

if DROP_ENTITY then runfile("@auri:yield/mode/entity")
else runfile("@auri:yield/mode/direct") end