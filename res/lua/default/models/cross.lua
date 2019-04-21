--
-- Diagonal cross-shaped model. Has 2 vertical faces that make an X pattern when looked at from above.
-- Useful for representing plants, grass, etc.
-- Only takes one texture, which is displayed on all faces.
--

zeus.register_blockmodel("default:cross", {
    {
        face = "nocull",
        tex = 1,
        points = {
            0.1, 0, 0.1, 0, 1,
            0.9, 0, 0.9, 1, 1,
            0.9, 0.9, 0.9, 1, 0,
            0.1, 0.9, 0.1, 0, 0
        }
    }, {
        face = "nocull",
        tex = 1,
        points = {
            0.9, 0.9, 0.9, 1, 0,
            0.9, 0, 0.9, 1, 1,
            0.1, 0, 0.1, 0, 1,
            0.1, 0.9, 0.1, 0, 0
        }
    }, {
        face = "nocull",
        tex = 1,
        points = {
            0.9, 0.9, 0.1, 1, 0,
            0.9, 0, 0.1, 1, 1,
            0.1, 0, 0.9, 0, 1,
            0.1, 0.9, 0.9, 0, 0
        }
    }, {
        face = "nocull",
        tex = 1,
        points = {
            0.1, 0, 0.9, 0, 1,
            0.9, 0, 0.1, 1, 1,
            0.9, 0.9, 0.1, 1, 0,
            0.1, 0.9, 0.9, 0, 0
        }
    }
})