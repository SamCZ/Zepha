-- The global chat object.
_G['chat'] = {
    channels = {},
    channel_order = {},
}

if zepha.server then require(_PATH .. 'api_server')
else require(_PATH .. 'api_client') end