#include "Message.h"

#include "lua/Lua.h"
#include "world/World.h"

void Api::Module::Message::bind() {
	core.set_function("send_message", Util::bind_this(this, &Message::send_message));
}

void Api::Module::Message::send_message(const std::string& channel, sol::object message) {
	world.sendMessage(channel, core["serialize"](message));
}
