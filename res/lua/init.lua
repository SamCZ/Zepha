-- Dump function
function dump(tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
  	if type(k) == "number" then
  		k = "[" .. k .. "]"
  	end
  	local indentString = string.rep("  ", indent)
    local formatting = indentString .. k .. " = "
    if type(v) == "table" then
      print(formatting .. "{")
      dump(v, indent+1)
      print(indentString .. "}")
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))      
    else
      print(formatting .. v)
    end
  end
end

-- Create cube model
zeus.register_blockmodel("default:block", {
    {
        face = "left",
        tex = 3,
        points = {
            0, 0, 0, 0, 1,
            0, 0, 1, 1, 1,
            0, 1, 1, 1, 0,
            0, 1, 0, 0, 0
        }
    }, {
        face = "right",
        tex = 4,
        points = {
            1, 1, 1, 1, 0,
            1, 0, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 1, 0, 0, 0
        }
    }, {
        face = "top",
        tex = 1,
        points = {
            0, 1, 0, 0, 0,
            0, 1, 1, 0, 1,
            1, 1, 1, 1, 1,
            1, 1, 0, 1, 0
        }
    }, {
        face = "bottom",
        tex = 2,
        points = {
            0, 0, 0, 0, 0,
            1, 0, 0, 1, 0,
            1, 0, 1, 1, 1,
            0, 0, 1, 0, 1
        }
    }, {
        face = "front",
        tex = 5,
        points = {
            0, 0, 1, 0, 1,
            1, 0, 1, 1, 1,
            1, 1, 1, 1, 0,
            0, 1, 1, 0, 0
        }
    }, {
        face = "back",
        tex = 6,
        points = {
            0, 0, 0, 0, 1,
            0, 1, 0, 0, 0,
            1, 1, 0, 1, 0,
            1, 0, 0, 1, 1
        }
    }
})

-- Create plantlike model
zeus.register_blockmodel("default:plantlike", {
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

-- Create leaves model
zeus.register_blockmodel("default:block_poof", {
    {
        face = "left",
        tex = 1,
        points = {
            0, 0, 0, 0, 1,
            0, 0, 1, 1, 1,
            0, 1, 1, 1, 0,
            0, 1, 0, 0, 0
        }
    }, {
        face = "right",
        tex = 1,
        points = {
            1, 1, 1, 1, 0,
            1, 0, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 1, 0, 0, 0
        }
    }, {
        face = "top",
        tex = 1,
        points = {
            0, 1, 0, 0, 0,
            0, 1, 1, 0, 1,
            1, 1, 1, 1, 1,
            1, 1, 0, 1, 0
        }
    }, {
        face = "bottom",
        tex = 1,
        points = {
            0, 0, 0, 0, 0,
            1, 0, 0, 1, 0,
            1, 0, 1, 1, 1,
            0, 0, 1, 0, 1
        }
    }, {
        face = "front",
        tex = 1,
        points = {
            0, 0, 1, 0, 1,
            1, 0, 1, 1, 1,
            1, 1, 1, 1, 0,
            0, 1, 1, 0, 0
        }
    }, {
        face = "back",
        tex = 1,
        points = {
            0, 0, 0, 0, 1,
            0, 1, 0, 0, 0,
            1, 1, 0, 1, 0,
            1, 0, 0, 1, 1
        }
    }, {
        face = "nocull",
        tex = 2,
        points = {
            -0.31, 1.30, -0.3, 0, 0,
            -0.31, -0.30, -0.31, 0, 1,
            1.3, -0.30, 1.3, 1, 1,
            1.3, 1.30, 1.29, 1, 0,

            1.3, -0.30, 1.3, 1, 1,
            -0.31, -0.30, -0.31, 0, 1,
            -0.31, 1.30, -0.3, 0, 0,
            1.3, 1.30, 1.29, 1, 0,

            -0.31, 1.30, 1.29, 0, 0,
            -0.31, -0.30, 1.3, 0, 1,
            1.3, -0.30, -0.31, 1, 1,
            1.3, 1.30, -0.3, 1, 0,

            1.3, -0.30, -0.31, 1, 1,
            -0.31, -0.30, 1.3, 0, 1,
            -0.31, 1.30, 1.29, 0, 0,
            1.3, 1.30, -0.3, 1, 0
        }
    }
})

zeus.register_blockmodel("default:block_side_foliage", {
    {
        face = "left",
        tex = 3,
        points = {
            0, 0, 0, 0, 1,
            0, 0, 1, 1, 1,
            0, 1, 1, 1, 0,
            0, 1, 0, 0, 0
        }
    }, {
        face = "right",
        tex = 4,
        points = {
            1, 1, 1, 1, 0,
            1, 0, 1, 1, 1,
            1, 0, 0, 0, 1,
            1, 1, 0, 0, 0
        }
    }, {
        face = "top",
        tex = 1,
        points = {
            0, 1, 0, 0, 0,
            0, 1, 1, 0, 1,
            1, 1, 1, 1, 1,
            1, 1, 0, 1, 0
        }
    }, {
        face = "bottom",
        tex = 2,
        points = {
            0, 0, 0, 0, 0,
            1, 0, 0, 1, 0,
            1, 0, 1, 1, 1,
            0, 0, 1, 0, 1
        }
    }, {
        face = "front",
        tex = 5,
        points = {
            0, 0, 1, 0, 1,
            1, 0, 1, 1, 1,
            1, 1, 1, 1, 0,
            0, 1, 1, 0, 0
        }
    }, {
        face = "back",
        tex = 6,
        points = {
            0, 0, 0, 0, 1,
            0, 1, 0, 0, 0,
            1, 1, 0, 1, 0,
            1, 0, 0, 1, 1
        }
    }, {
        --Floats begin here
        face = "front",
        tex = 7,
        points = {
            0, 1, 1, 0, 0,
            0, 0.2, 1.2, 0, 1,
            1, 0.2, 1.2, 1, 1,
            1, 1, 1, 1, 0
        }
    }, {
        face = "back",
        tex = 7,
        points = {
            1, 0.2, -0.2, 1, 1,
            0, 0.2, -0.2, 0, 1,
            0, 1, 0, 0, 0,
            1, 1, 0, 1, 0
        }
    }, {
        face = "right",
        tex = 7,
        points = {
            1.2, 0.2, 1, 1, 1,
            1.2, 0.2, 0, 0, 1,
            1, 1, 0, 0, 0,
            1, 1, 1, 1, 0
        }
    }, {
        face = "left",
        tex = 7,
        points = {
            0, 1, 0, 0, 0,
            -0.2, 0.2, 0, 0, 1,
            -0.2, 0.2, 1, 1, 1,
            0, 1, 1, 1, 0
        }
    }
})

--[[

**Block Creation**

The function zeus.register_block creates a block in the scene.
The first argument is the name of the block, which should be prefixed by the mod name and a colon.
The next argument is a table of properties. These are the following accepted properties:

name: The display name of the block

model: The blockModel of the block, will default to "default:cube"

textures: The list of textures for the blockmodel. Which textures go where are defined by the blockmodel.

--]]

-- Ignore this, it is temporary
-- Air [ID 0]
zeus.register_block("_:air", {
    visible = false,
    culls = false,
    solid = false,
	name = "Air",
	model = "default:block",
	textures = {"_missing"},
})

-- Grass [ID 1]
zeus.register_block("default:grass", {
	name = "Grass",
	model = "default:block_side_foliage",
	textures = {
		"default_grass_top",
		"default_dirt",
        "default_grass_side",
        "default_grass_side",
        "default_grass_side",
        "default_grass_side",
        "default_grass_float",
	},
    lowdef_textures = {
        "default_grass_top",
        "default_dirt",
        "default_grass_side"
    },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
})

-- Dirt [ID 2]
zeus.register_block('default:dirt', {
	name = "Dirt",
	model = "default:block",
	textures = {"default_dirt"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
})

-- Stone [ID 3]
zeus.register_block('default:stone', {
	name = "Stone",
	model = "default:block",
	textures = {"default_stone"},
    toughness = {
        hand = 14,
        pick = 3
    },
})

-- Leaves [ID 4]
zeus.register_block('default:leaves', {
    visible = true,
    culls = false,
    name = "Log",
    model = "default:block_poof",
    textures = {
        "default_leaves",
        "default_leaves_puff"
    },
    lowdef_textures = {
        "default_leaves_opaque",
    },
    toughness = {
        hand = 1,
        axe = 0.2,
    },
})

-- Wood [ID 5]
zeus.register_block('default:wood', {
    name = "Log",
    model = "default:block",
    textures = {
        "default_log_top",
        "default_log_top",
        "default_log_side"
    },
    toughness = {
        hand = 5,
        axe = 3,
    },
})

-- TallGrass [ID 6..10]
for i=1,5,1 do
    zeus.register_block("default:tallgrass_" .. i, {
        culls = false,
        solid = false,
        name = "Tall Grass",
        model = "default:plantlike",
        textures = {
            "default_tallgrass_" .. i,
        },
        lowdef_render = false,
        selection_box = {
            {1/16, 0, 1/16, 15/16, 0.30 + i * 0.1, 15/16}
        },
        toughness = {
            hand = 0
        }
    })
end

-- Flowers [ID 11..18]
local flowers = {
    "rose",
    "tulip",
    "viola",
    "geranium",
    "mushroom_red",
    "mushroom_brown",
    "dandelion_white",
    "dandelion_yellow"
}

for _,flower in ipairs(flowers) do

    local function tchelper(first, rest)
        return first:upper()..rest:lower()
    end

    local name = flower:gsub("_", " "):gsub("(%a)([%w_']*)", tchelper)

    zeus.register_block("default:flower_" .. flower, {
        culls = false,
        solid = false,
        name = name,
        model = "default:plantlike",
        textures = {
            "flowers_" .. flower
        },
        lowdef_render = false,
        selection_box = {
            {4/16, 0, 4/16, 12/16, 14/16, 12/16}
        },
        toughness = {
            hand = 0
        }
    })
end

-- Sand [ID 19]
zeus.register_block('default:sand', {
    name = "Sand",
    model = "default:block",
    textures = {"default_sand"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
})

-- Sandstone [ID 20]
zeus.register_block('default:sandstone', {
    name = "Sandstone",
    model = "default:block",
    textures = {"default_sandstone"},
    toughness = {
        hand = 14,
        pick = 3
    },
})