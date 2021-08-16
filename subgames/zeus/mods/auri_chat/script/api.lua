-- The global chat object.
_G['chat'] = {
    channels = {},
    channel_order = {},
}

if zepha.server then runfile(_PATH .. 'api_server')
else runfile(_PATH .. 'api_client') end