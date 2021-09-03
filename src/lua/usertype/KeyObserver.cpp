#include <GLFW/glfw3.h>

#include "KeyObserver.h"

#include "client/Input.h"
#include "lua/LocalLuaParser.h"

usize Api::Usertype::KeyObserver::ID_NEXT = 0;

void Api::Usertype::KeyObserver::start() {
	if (nativeCBs.size()) return;
	parser.addKBObserver(id);
	nativeCBs.emplace_back(input.events.bind(Input::CBType::KEY, [&](i32 key, u32 state) {
		if (state == GLFW_PRESS && on_press) on_press(key);
		else if (state == GLFW_RELEASE && on_release) on_release(key);
	}));
}

void Api::Usertype::KeyObserver::stop() {
	parser.removeKBObserver(id);
	nativeCBs.clear();
}

Api::Usertype::KeyObserver::~KeyObserver() {
	stop();
}

void Api::Usertype::KeyObserver::bind(sol::state& lua, sol::table& core, Input& input, LocalLuaParser& parser) {
	lua.new_usertype<KeyObserver>("KeyObserver",
		sol::meta_function::construct, sol::factories([&]() {
			return make_shared<KeyObserver>(input, parser);
		}),
		
		"start", &KeyObserver::start,
		"stop", &KeyObserver::stop,
		
		"on_press", &KeyObserver::on_press,
		"on_release", &KeyObserver::on_release,
		"on_change", &KeyObserver::on_change
	);
}
