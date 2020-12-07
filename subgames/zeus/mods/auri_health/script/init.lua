local api = {}
_G['health'] = api

runfile(_PATH .. 'api')
runfile(_PATH .. 'core')

if zepha.client then
    runfile(_PATH .. 'interface')
    api.default_render(true)
end