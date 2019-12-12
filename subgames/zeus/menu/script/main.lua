zepha.set_gui([[
    body
        background: asset(zeus_background)

        text[name]
            position: 4px 4px
            content: "Zeus Subgame v0.0.1"

        rect[inv]
            position: 50% 50%
            position_anchor: 50% 50%
            size: 150px 44px
            padding: 12px 12px 12px 12px
            background: #000

            text[intro]
                content: "This is a Lua defined title screen!\nIt's not really... done... yet!~"
                color: #ff7
            end
        end
    end
]])