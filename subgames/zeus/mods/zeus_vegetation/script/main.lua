require './models/clover'

local selection_box_flower = {
    { 4/16, 0, 4/16, 12/16, 14/16, 12/16 }
}

local selection_box_grass = {
    { 2/16, 0, 2/16, 14/16, 7/16, 14/16 }
}

local function title_case(str)
    local tf = function(first, rest) return first:upper()..rest:lower() end
    return str:gsub('_', ' '):gsub("(%a)([%w_']*)", tf)
end

local function register_vegetation(identifier, override)
    zepha.register_block(':' .. identifier, table.merge({
        culls = false,
        solid = false,
        lowdef_render = false,
        light_propagates = true,
        toughness = { hand = 0 },
        name = title_case(identifier),
        model = 'zepha:base:cross_plant',
        selection_box = selection_box_flower,
        textures = { 'zeus:vegetation:' .. identifier }
    }, override or {}))
end

register_vegetation('rose')
register_vegetation('tulip')
register_vegetation('viola')
register_vegetation('purple')
register_vegetation('geranium')
register_vegetation('dandelion_white', { name = 'White Dandelion' })
register_vegetation('dandelion_yellow', { name = 'Yellow Dandelion' })
register_vegetation('tall_blue', { model = 'zepha:base:cross_plant_tall' })
register_vegetation('mushroom_red', { name = 'Red Mushroom', model = 'zepha:base:cross_large' })
register_vegetation('mushroom_glow', { name = 'Glow Mushroom', model = 'zepha:base:cross_large' })
register_vegetation('mushroom_brown', { name = 'Brown Mushroom', model = 'zepha:base:cross_large' })

register_vegetation('clover', {
    model = 'zeus:vegetation:clover',
    textures = {
        '(canvas(16),tint(0, crop(0, 0, 16, 16, zeus:vegetation:clover)))',
        '(canvas(16),tint(0, crop(16, 0, 16, 16, zeus:vegetation:clover)))'
    },
    selection_box = {
        { 1/16, 0, 1/16, 15/16, 4/16, 15/16 }
    }
})

local grass_def = {
    name = 'Tall Grass',
    model = 'zepha:base:cross_plant',
    selection_box = selection_box_grass,
    tool_props = {
        health = 5,
        multipliers = {
            snap = 1.8,
            grab = 1.2,
            _other = 0.35
        }
    },
    yields = function(pos)
        if math.random() > 0.8 then return 'zeus:materials:plant_fibre' end
    end
}

register_vegetation('tall_grass_1', table.merge(grass_def, { textures = { 'tint(0, zeus:vegetation:tall_grass_1)' }}))
register_vegetation('tall_grass_2', table.merge(grass_def, { textures = { 'tint(0, zeus:vegetation:tall_grass_2)' }}))
register_vegetation('tall_grass_3', table.merge(grass_def, { textures = { 'tint(0, zeus:vegetation:tall_grass_3)' }}))
register_vegetation('tall_grass_4', table.merge(grass_def, { textures = { 'tint(0, zeus:vegetation:tall_grass_4)' }}))
register_vegetation('tall_grass_5', table.merge(grass_def, { textures = { 'tint(0, zeus:vegetation:tall_grass_5)' }}))