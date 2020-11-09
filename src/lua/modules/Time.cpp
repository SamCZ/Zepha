//
// Created by aurailus on 2020-07-26.
//

#include "Time.h"

#include "../Lua.h"

void Api::Module::Time::bind() {
	auto time = lua.create_table();
	core["time"] = time;
	
	time.set_function("ns", Util::bind_this(this, &Time::ns));
	time.set_function("ms", Util::bind_this(this, &Time::ms));
	time.set_function("s", Util::bind_this(this, &Time::s));
}

float Api::Module::Time::ns() {
	return timer.elapsedNs();
}

float Api::Module::Time::ms() {
	return timer.elapsedNs() / 1000000.f;
}

float Api::Module::Time::s() {
	return timer.elapsedNs() / 1000000.f / 1000.f;
}
