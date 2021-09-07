require './dropped_item'

local DROP_ENTITY = true

if DROP_ENTITY then
    require './mode/entity'
else
    require './mode/direct'
end