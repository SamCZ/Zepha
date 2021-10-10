local api = hot_wheel
local hud = zepha.player:get_hud()

local health_elem = nil
if health then
    health.render_default(false)
    health_elem = health.get_element()
end

hud:append(function()
    local root = Gui.Box {
        id = 'hot_wheel_root',

        size = { '140dp', '80dp' },
        pos = { '0', '100ch - 100sh' },

        Gui.Box {
            pos = { '52dp', '19dp' },
            size = { '94dp', '18dp' },
            background = '@auri:hot_wheel:hot_wheel_line',

            Gui.Box {
                id = 'expanded_indicator',

                size = '18dp',
                pos = { '2dp', 0 },
                background = '@auri:hot_wheel:hot_wheel_selection'
            },

--             Gui.InventoryList {
--                 key = 'list_expanded',
--                 pos = { 3, 1 },
--                 gap = { 2, 2 },
--
--                 length = 6,
--                 list = 'hot_wheel_1',
--                 source = 'current_player',
--             }
        },

        Gui.Box {
            size = '80dp',
            background = '@auri:hot_wheel:hot_wheel_circle',

--             Gui.InventoryList {
--                 key = 'list_1',
--                 slot_spacing = { 2, 2 },
--                 length = 1,
--                 source = 'current_player',
--                 list = 'hot_wheel_1',
--             },
--             Gui.InventoryList {
--                 key = 'list_2',
--                 slot_spacing = { 2, 2 },
--                 length = 1,
--                 source = 'current_player',
--                 list = 'hot_wheel_2',
--             },
--             Gui.InventoryList {
--                 key = 'list_3',
--                 slot_spacing = { 2, 2 },
--                 length = 1,
--                 source = 'current_player',
--                 list = 'hot_wheel_3',
--             },
--             Gui.InventoryList {
--                 key = 'list_4',
--                 slot_spacing = { 2, 2 },
--                 length = 1,
--                 source = 'current_player',
--                 list = 'hot_wheel_4',
--             },
--             Gui.InventoryList {
--                 key = 'list_5',
--                 slot_spacing = { 2, 2 },
--                 length = 1,
--                 source = 'current_player',
--                 list = 'hot_wheel_5',
--             },
--             Gui.InventoryList {
--                 key = 'list_6',
--                 slot_spacing = { 2, 2 },
--                 length = 1,
--                 source = 'current_player',
--                 list = 'hot_wheel_6',
--             },

            Gui.Text {
                id = 'label_1',
                content = '1'
            },
            Gui.Text {
                id = 'label_2',
                content = '2'
            },
            Gui.Text {
                id = 'label_3',
                content = '3'
            },
            Gui.Text {
                id = 'label_4',
                content = '4'
            },
            Gui.Text {
                id = 'label_5',
                content = '5'
            },
            Gui.Text {
                id = 'label_6',
                content = '6'
            }
        },

        Gui.Box {
            pos = { '75dp', '40dp' },

            health_elem
        }
    }

    api.labels = {
        root:get('label_1'),
        root:get('label_2'),
        root:get('label_3'),
        root:get('label_4'),
        root:get('label_5'),
        root:get('label_6')
    }

    api.lists = {
        root:get('list_1'),
        root:get('list_2'),
        root:get('list_3'),
        root:get('list_4'),
        root:get('list_5'),
        root:get('list_6')
    }

    api.list_expanded = root:get('list_expanded')
    api.list_expanded_indicator = root:get('expanded_indicator')

    return root
end)