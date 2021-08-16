-- Zepha Vector Library
-- Version 1.0

_G["vector"] = {}

-- create_vector
-- Create a new vector, and give it a metatable.
-- x, y, and z must all be numbers.
local function create_vector(x, y, z)
    local v = {x, y, z}
    setmetatable(v, vector)
    return v
end

-- vector.equal
-- Return a boolean indicating if v1 == v2
function vector.equal(v1, v2)
    assert(vector.is_vector(v1) and vector.is_vector(v2))
    return (rawget(v1, 1) == rawget(v2, 1) and rawget(v1, 2) == rawget(v2, 2) and rawget(v1, 3) == rawget(v2, 3))
end

vector.__eq = vector.equal

-- vector.copy
-- Copies v and returns a new vector
function vector.copy(v)
    assert(vector.is_vector(v))
    return create_vector(rawget(v, 1), rawget(v, 2), rawget(v, 3))
end

-- vector.add
-- Add two vectors or a vector and number
function vector.add(v1, v2)
    assert(vector.is_vector(v1))
    if vector.is_vector(v2) then return create_vector(rawget(v1, 1) + rawget(v2, 1), rawget(v1, 2) + rawget(v2, 2), rawget(v1, 3) + rawget(v2, 3))
    elseif type(v2) == "number" then return create_vector(rawget(v1, 1) + v2, rawget(v1, 2) + v2, rawget(v1, 3) + v2) end
end

vector.__add = vector.add

-- vector.negative
-- Flips a vector's content's signs
function vector.negative(v)
    assert(vector.is_vector(v))
    return create_vector(-rawget(v, 1), -rawget(v, 2), -rawget(v, 3))
end

vector.__unm = vector.negative

-- vector.subtract
-- Subtract v2 from v1
function vector.subtract(v1, v2)
    return vector.add(v1, -v2)
end

vector.__sub = vector.subtract

-- vector.multiply
-- Multiply v1 by a vector or number
function vector.multiply(v1, m)
    assert(vector.is_vector(v1))
    if vector.is_vector(m) then return create_vector(rawget(v1, 1) * rawget(m, 1), rawget(v1, 2) * rawget(m, 2), rawget(v1, 3) * rawget(m, 3))
    elseif type(m) == "number" then return create_vector(rawget(v1, 1) * m, rawget(v1, 2) * m, rawget(v1, 3) * m) end
end

vector.__mul = vector.multiply

-- vector.divide
-- Divice v1 by a vector or number
function vector.divide(v1, m)
    assert(vector.is_vector(v1))
    if vector.is_vector(m) then
        return create_vector(rawget(v1, 1) / rawget(m, 1), rawget(v1, 2) / rawget(m, 2), rawget(v1, 3) / rawget(m, 3))
    elseif type(m) == "number" then
        return create_vector(rawget(v1, 1) / m, rawget(v1, 2) / m, rawget(v1, 3) / m)
    end
end

vector.__div = vector.divide

-- vector.pow
-- Return v to the power of p
function vector.pow(v, m)
    assert(vector.is_vector(v))
   local res = create_vector(rawget(v, 1), rawget(v, 2), rawget(v, 3))
   for i = 1, power - 1 do res = res * v end
   return res
end

vector.__pow = vector.pow

-- vector.abs
-- Return the absolute value of v
function vector.abs(v)
    assert(vector.is_vector(v))
    return create_vector(math.abs(rawget(v, 1)), math.abs(rawget(v, 2)), math.abs(rawget(v, 3)))
end

-- vector.round
-- Round each vector value to the nearest integer
function vector.round(v)
    assert(vector.is_vector(v))
    return create_vector(math.round(rawget(v, 1)), math.round(rawget(v, 2)), math.round(rawget(v, 3)))
end

-- vector.floor
-- Floor each vector value to the lowest integer
function vector.floor(v)
    assert(vector.is_vector(v))
    return create_vector(math.floor(rawget(v, 1)), math.floor(rawget(v, 2)), math.floor(rawget(v, 3)))
end

-- vector.ceil
-- Ceil each vector value to the highest integer
function vector.ceil(v)
    assert(vector.is_vector(v))
    return create_vector(math.ceil(rawget(v, 1)), math.ceil(rawget(v, 2)), math.ceil(rawget(v, 3)))
end

-- vector.length2
-- Get the square of the length of a vector
function vector.length2(v)
    assert(vector.is_vector(v))
    return rawget(v, 1) ^ 2 + rawget(v, 2) ^ 2 + rawget(v, 3) ^ 2
end

-- vector.length
-- Get the length of a vector
function vector.length(v)
    assert(vector.is_vector(v))
    return math.sqrt(vector.length2(v))
end

-- vector.distance2
-- Get the square of the distance between two vectors
function vector.distance2(v1, v2)
    assert(vector.is_vector(v1) and vector.is_vector(v2))
    return vector.length2(vector.abs(vector.subtract(v1, v2)))
end

-- vector.distance
-- Get the distance between two vectors
function vector.distance(v1, v2)
    assert(vector.is_vector(v1) and vector.is_vector(v2))
    return math.sqrt(vector.distance2(v1, v2))
end

-- vector.normalize
-- Normalizes a vecotr
function vector.normalize(v)
    assert(vector.is_vector(v))
    return v / v:length()
end

-- vector.unit
-- An alias to vector.normalize
vector.unit = vector.normalize

-- Alias x, y, z to 1, 2, 3

function vector:__index(key)
    if key == "x" then return rawget(self, 1) end
    if key == "y" then return rawget(self, 2) end
    if key == "z" then return rawget(self, 3) end
    return getmetatable(self)[key]
end

function vector:__newindex(key, val)
    if     key == "x" then rawset(self, 1, val)
    elseif key == "y" then rawset(self, 2, val)
    elseif key == "z" then rawset(self, 3, val) end
end

function vector:__tostring()
    return table.concat({
      "{",
      tostring(rawget(self, 1)), ", ",
      tostring(rawget(self, 2)), ", ",
      tostring(rawget(self, 3)), "}"
    })
end

function vector.__concat(a, b)
	return tostring(a) .. tostring(b)
end

-- vector.is_vector
-- Checks if a value is a vector.
function vector.is_vector(v)
    return getmetatable(v) == vector
end

-- vector.new
-- Constructor function for creating vectors from a table or 2-3 values.
-- Table constructor works with 1-3 values
vector.new = function(x, y, z)
    -- Blank new constructor, return empty vector
    if x == nil or (type(x) == "table" and x[1] == nil) then return create_vector(0, 0, 0)
    -- Invalid type passed to function, return nil
    elseif type(x) ~= "number" and type(x) ~= "table" then return nil
    -- Passed a table as x with at least x and y parameters, z will be set to table value or 0
    elseif type(x) == "table" and (vector.is_vector(x) or (type(x[1]) == "number" and type(x[2]) == "number")) then return create_vector(x[1], x[2], x[3] or 0)
    -- Only one number was passed, give a vector with all three values set to it
    elseif y == nil then return create_vector(x, x, x)
    -- Invalid type passed to function, return nil
    elseif (type(y) ~= "number" and type(y) ~= "table") or type(z) ~= "number" then return end
    -- Create a vector with the values x, y, and z
    return create_vector(x, y, z)
end

-- vector.new shorthand
_G["V"] = vector.new