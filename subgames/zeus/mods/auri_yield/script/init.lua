require("@auri:yield/dropped_item")

local DROP_ENTITY = true

if DROP_ENTITY then require("@auri:yield/mode/entity")
else require("@auri:yield/mode/direct") end