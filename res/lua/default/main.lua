print("Hello from default, Zeus " .. (zeus.is_server() and "Server" or "Client") .. "!")

local path = zeus.get_path("default")

dofile(path .. "models.lua")
dofile(path .. "blocks.lua")

-- API DOCUMENTATION --

-- # Register BlockModel
-- `zeus.register_blockmodel(string identifier, table definition)`
--
-- Definition is stored as `zeus.registered_blockmodels[identifier] = definition`.
-- There are no requirements for the contents of the definition table in this function, but for it to be well
-- formed and usable in `register_block`, it should be an array-table of face tables, which contain the following
-- information:
--
-- face:    One of "left", "right", "front", "back", "top", "bottom", or "nocull". Used when determining which parts
--          of the model to cull.
--
-- tex:     The texture index which this face uses. What textures these refer to are defined in the `register_block`
--          `textures` table. The index must be greater than or equal to 1.
--
-- points:  An array of integers that define one or more quads. Each quad is defined by 20 numbers or 4 vertices,
--          in the format of x, y, z, tex_coord_x, tex_coord_y. The vertices are indexed in the order
--          0, 1, 2, 2, 3, 0.
--
-- ## Example
-- {
--     face = "left"
--     tex = 3,
--     points = {
--         0, 0, 0, 0, 1,
--         0, 0, 1, 1, 1,
--         0, 1, 1, 1, 0,
--         0, 1, 0, 0, 0
--     }
-- }