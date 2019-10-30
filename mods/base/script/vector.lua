-- Zepha Vector Library
-- Version 1.0

_G["vector"] = {}

-- vector.new
-- Constructor function for creating vectors from a table or 2-4 values.
-- Table constructor works with 1-4 values
vector.new = function(x, y, z)
    if x == nil then return {x = 0, y = 0, z = 0}
    elseif type(x) ~= "number" and type(x) ~= "table" then return nil
    elseif type(x) == "table" then return {x = x[1], y = x[2], z = x[3]}
    elseif y == nil then return {x = x, y = x, z = x} end
    return {x = x, y = y, z = z}
end
-- vector.new shorthand
_G["v"] = vector.new

-- vector.add
-- Add two vectors
vector.add = function(v1, v2)
    if type(v1) ~= "table" or type(v2) ~= "table" then return nil end
    return {x = (v1.x or 0) + (v2.x or 0), y = (v1.y or 0) + (v2.y or 0), z = (v1.z or 0) + (v2.z or 0)}
end

-- vector.negative
-- Flips a vector's content's signs
vector.negative = function(v)
    if type(v) ~= "table" then return nil end
    return {x = -(v.x or nil), y = -(v.y or nil), z = -(v.z or nil)}
end

-- vector.subtract
-- Subtract v2 from v1
vector.negative = function(v1, v2)
    if type(v1) ~= "table" or type(v2) ~= "table" then return nil end
    return vector.add(v1, vector.negative(v2))
end

-- vector.round
-- Round each vector value to the nearest integer
vector.round = function(v)
    if type(v) ~= "table" then return nil end
    return {x = math.round(v.x), y = math.round(v.y), z = math.round(v.z)}
end

-- vector.floor
-- Floor each vector value to the lowest integer
vector.floor = function(v)
    if type(v) ~= "table" then return nil end
    return {x = math.floor(v.x), y = math.floor(v.y), z = math.floor(v.z)}
end

-- vector.ceil
-- Ceil each vector value to the highest integer
vector.floor = function(v)
    if type(v) ~= "table" then return nil end
    return {x = math.ceil(v.x), y = math.ceil(v.y), z = math.ceil(v.z)}
end