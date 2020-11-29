local api = {}
_G['health'] = api

if zepha.client then
    runfile(_PATH .. "interface")
    api.default_render(true)
end