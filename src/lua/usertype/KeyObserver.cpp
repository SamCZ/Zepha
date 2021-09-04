#include <GLFW/glfw3.h>

#include "KeyObserver.h"

#include "client/Input.h"
#include "lua/LocalLuaParser.h"

// http://www.zedwood.com/article/cpp-utf8-char-to-codepoint
string utf8chr(i32 cp) {
	char c[5]={ 0x00,0x00,0x00,0x00,0x00 };
	if     (cp<=0x7F) { c[0] = cp;  }
	else if(cp<=0x7FF) { c[0] = (cp>>6)+192; c[1] = (cp&63)+128; }
	else if(0xd800<=cp && cp<=0xdfff) {} //invalid block of utf8
	else if(cp<=0xFFFF) { c[0] = (cp>>12)+224; c[1]= ((cp>>6)&63)+128; c[2]=(cp&63)+128; }
	else if(cp<=0x10FFFF) { c[0] = (cp>>18)+240; c[1] = ((cp>>12)&63)+128; c[2] = ((cp>>6)&63)+128; c[3]=(cp&63)+128; }
	return string(c);
}

usize Api::Usertype::KeyObserver::ID_NEXT = 0;

void Api::Usertype::KeyObserver::start() {
	if (nativeCBs.size()) return;
	parser.addKBObserver(id);
	
	nativeCBs.emplace_back(input.events.bind(Input::CBType::KEY, [&](i32 key, u32 state) {
		if (state == GLFW_PRESS && on_press) on_press(key);
		else if (state == GLFW_RELEASE && on_release) on_release(key);
		
		if (key == GLFW_KEY_BACKSPACE && state != GLFW_RELEASE && buffer.size()) {
			buffer.pop_back();
			if (on_change) on_change(buffer);
		}
	}));
	
	nativeCBs.emplace_back(input.events.bind(Input::CBType::CHAR, [&](i32 codepoint, u32) {
		buffer += utf8chr(codepoint);
		if (on_change) on_change(buffer);
	}));
}

void Api::Usertype::KeyObserver::stop() {
	parser.removeKBObserver(id);
	nativeCBs.clear();
}

const string& Api::Usertype::KeyObserver::getBuffer() {
	std::cout << "getting buffre :)" << std::endl;
	return buffer;
}

void Api::Usertype::KeyObserver::setBuffer(const string& buffer) {
	this->buffer = buffer;
	if (on_change) on_change(this->buffer);
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
		
		"buffer", sol::property(&KeyObserver::getBuffer, &KeyObserver::setBuffer),
		
		"on_press", &KeyObserver::on_press,
		"on_release", &KeyObserver::on_release,
		"on_change", &KeyObserver::on_change
	);
}