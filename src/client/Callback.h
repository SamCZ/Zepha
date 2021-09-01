#pragma once

#include <list>
#include <iostream>
#include <functional>

#include "util/Types.h"


class CallbackRef {
public:
	CallbackRef() = default;
	explicit CallbackRef(std::function<void()> unbind): unbind(std::make_shared<std::function<void()>>(unbind)) {};
	~CallbackRef() { if (unbind && unbind.unique()) unbind->operator()(); }
	
private:
	sptr<std::function<void()>> unbind = nullptr;
};

template <typename... Args>
class CallbackManager {
public:
	typedef std::function<void(Args...)> CB_TYPE;
	
	void update() {
		while (invalidInds.size()) {
			callbacks.remove(invalidInds.front());
			invalidInds.pop_front();
		}
	};
	
	void call(const Args&... args) {
		for (const let& cb : callbacks) if (cb.second.valid) cb.second.call(args...);
	};
	
	CallbackRef bind(const CB_TYPE& cb) {
		usize cID = next++;
		callbacks.emplace(cID, InvalidatableCB(cb));
		return CallbackRef([=]() { unbind(cID); });
	};
	
	void unbind(usize ind) {
		callbacks.at(ind).valid = false;
	};
	
private:
	struct InvalidatableCB {
		InvalidatableCB(const CB_TYPE& cb): call(cb) {};
		
		CB_TYPE call;
		bool valid = true;
	};
	
	usize next = 0;
	std::list<usize> invalidInds;
	std::unordered_map<usize, InvalidatableCB> callbacks;
};

template <typename E, typename... Args>
class CallbackGroup {
public:
	typedef E CB_IDENTIFIER;
	typedef std::function<void(Args...)> CB_TYPE;
	
	void update() {
		for (usize i = 0; i < callbacks.size(); i++) callbacks[i].update();
	};
	
	void call(CB_IDENTIFIER type, const Args&... args) {
		callbacks[static_cast<usize>(type)].call(args...);
	};
	
	CallbackRef bind(CB_IDENTIFIER type, const CB_TYPE& cb) {
		return callbacks[static_cast<usize>(type)].bind(cb);
	};
	
	void unbind(CB_IDENTIFIER type, usize ind) {
		callbacks[static_cast<usize>(type)].unbind(ind);
	};
	
private:
	array<CallbackManager<Args...>, static_cast<usize>(E::_END)> callbacks;
};
