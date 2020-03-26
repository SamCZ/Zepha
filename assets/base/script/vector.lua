-- Zepha Vector Library
-- Version 1.0

-- Vector metatable
-- A metatable to be assigned to vectors which applies mathematic operators
local vector_mt = {
    __is_vector = true,

    -- Value manipulation functions
    __index = function(tbl, key)
        if key == "x" then return rawget(tbl, 1) end
        if key == "y" then return rawget(tbl, 2) end
        if key == "z" then return rawget(tbl, 3) end

        local val = rawget(tbl, key)
        if val == nil then val = rawget(getmetatable(tbl), key) end
        return val
    end,
    __newindex = function(tbl, key, val)
        if     key == "x" or key == 0 then rawset(tbl, 1, val)
        elseif key == "y" or key == 1 then rawset(tbl, 2, val)
        elseif key == "z" or key == 2 then rawset(tbl, 3, val) end
    end,
    __tostring = function(tbl)
        return table.concat({
          "{ ",
          tostring(tbl[1]), ", ",
          tostring(tbl[2]), ", ",
          tostring(tbl[3]), " }"
        })
    end,
    __eq = function(tbl, o)
        return vector.equal(tbl, o)
    end,

    -- Arithmetic functions
    __unm = function(tbl)
        return vector.negative(tbl)
    end,
    __add = function(tbl, o)
        return vector.add(tbl, o)
    end,
    __sub = function(tbl, o)
        return vector.subtract(tbl, o)
    end,
    __mul = function(tbl, o)
        return vector.multiply(tbl, o)
    end,
    __pow = function(tbl, power)
        return vector.pow(tbl, o)
    end,

    -- Higher level methods
    abs = function(tbl)
        return vector.abs(tbl)
    end,
    round = function(tbl)
        return vector.round(tbl)
    end,
    floor = function(tbl)
        return vector.floor(tbl)
    end,
    ceil = function(tbl)
        return vector.ceil(tbl)
    end,
    distance_squared = function(tbl, o)
        return vector.distance_squared(tbl, o)
    end,
    distance = function(tbl, o)
        return vector.distance(tbl, o)
    end
}

-- create_vector
-- Create a new vector, and give it a metatable.
-- x, y, and z must all be numbers.
local function create_vector(x, y, z)
    local vector = {x, y, z}
    setmetatable(vector, vector_mt)
    return vector
end

-- check_vector
-- Checks if a value is a vector.
local function check_vector(v)
    if type(v) ~= "table" or not v.__is_vector then return false end
    return true
end

_G["vector"] = {}

-- vector.new
-- Constructor function for creating vectors from a table or 2-3 values.
-- Table constructor works with 1-3 values
vector.new = function(x, y, z)
    -- Blank new constructor, return empty vector
    if x == nil then return create_vector(0, 0, 0)
    -- Invalid type passed to function, return nil
    elseif type(x) ~= "number" and type(x) ~= "table" then return nil
    -- Passed a table as x with at least x and y parameters, z will be set to table value or 0
    elseif type(x) == "table" and (x.__is_vector or (type(x[1]) == "number" and type(x[2]) == "number")) then return create_vector(x[1], x[2], x[3] or 0)
    -- Only one number was passed, give a vector with all three values set to it
    elseif y == nil then return create_vector(x, x, x)
    -- Invalid type passed to function, return nil
    elseif (type(y) ~= "number" and type(y) ~= "table") or type(z) ~= "number" then return end
    -- Create a vector with the values x, y, and z
    return create_vector(x, y, z)
end

-- vector.new shorthand
_G["V"] = vector.new

-- vector.add
-- Add two vectors or a vector and number
vector.add = function(v1, v2)
    if not check_vector(v1) then return end
    if check_vector(v2) then
        return create_vector(rawget(v1, 1) + rawget(v2, 1), rawget(v1, 2) + rawget(v2, 2), rawget(v1, 3) + rawget(v2, 3))
    elseif type(v2) == "number" then
        return create_vector(rawget(v1, 1) + v2, rawget(v1, 2) + v2, rawget(v1, 3) + v2)
    end
end

-- vector.negative
-- Flips a vector's content's signs
vector.negative = function(v)
    if not check_vector(v) then return end
    return create_vector(-rawget(v, 1), -rawget(v, 2), -rawget(v, 3))
end

-- vector.subtract
-- Subtract v2 from v1
vector.subtract = function(v1, v2)
    return vector.add(v1, vector.negative(v2))
end

-- vector.multiply
-- Multiply v1 by a vector or number
vector.multiply = function(v1, m)
    if not check_vector(v1) then return end
    if check_vector(m) then
        return create_vector(rawget(v1, 1) * rawget(m, 1), rawget(v1, 2) * rawget(m, 2), rawget(v1, 3) * rawget(m, 3))
    elseif type(m) == "number" then
        return create_vector(rawget(v1, 1) * m, rawget(v1, 2) * m, rawget(v1, 3) * m)
    end
end

-- vector.pow
-- Return v to the power of p
vector.pow = function(v, m)
   if not check_vector(v) then return end
   local res = create_vector(rawget(v, 1), rawget(v, 2), rawget(v, 3))
   for i = 1, power - 1 do res = res * v end
   return res
end

-- vector.equal
-- Return a boolean indicating if v1 == v2
vector.equal = function(v1, v2)
    if not check_vector(v1) or not check_vector(v2) then return end
    return (rawget(v1, 1) == rawget(v2, 1) and rawget(v1, 2) == rawget(v2, 2) and rawget(v1, 3) == rawget(v2, 3))
end

-- vector.abs
-- Return the absolute value of v
vector.abs = function(v)
    if not check_vector(v) then return end
    return create_vector(math.abs(rawget(v, 1)), math.abs(rawget(v, 2)), math.abs(rawget(v, 3)))
end

-- vector.round
-- Round each vector value to the nearest integer
vector.round = function(v)
    if not check_vector(v) then return end
    return create_vector(math.round(rawget(v, 1)), math.round(rawget(v, 2)), math.round(rawget(v, 3)))
end

-- vector.floor
-- Floor each vector value to the lowest integer
vector.floor = function(v)
    if not check_vector(v) then return end
    return create_vector(math.floor(rawget(v, 1)), math.floor(rawget(v, 2)), math.floor(rawget(v, 3)))
end

-- vector.ceil
-- Ceil each vector value to the highest integer
vector.ceil = function(v)
    if not check_vector(v) then return end
    return create_vector(math.ceil(rawget(v, 1)), math.ceil(rawget(v, 2)), math.ceil(rawget(v, 3)))
end

-- vector.distance_squared
-- Get the square of the distance between two vectors
-- This function is faster if you only need to compare two distances
vector.distance_squared = function(v1, v2)
    if not check_vector(v1) or not check_vector(v2) then return end
    local diff = vector.abs(vector.subtract(v1, v2))
    return diff.x ^ 2 + diff.y ^ 2 + diff.z ^ 2
end

-- vector.distance
-- Get the distance between two vectors
vector.distance = function(v1, v2)
    if not check_vector(v1) or not check_vector(v2) then return end
    return math.sqrt(vector.distance_squared(v1, v2))
end