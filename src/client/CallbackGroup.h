#pragma once

#include <list>
#include <functional>

#include "util/Types.h"

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
	
	void call(Args&&... args) {
		for (const let& cb : callbacks) if (cb.second.valid) cb.second.call(std::forward<Args>(args)...);
	};
	
	usize bind(const CB_TYPE& cb) {
		callbacks.emplace(next, InvalidatableCB(cb));
		return next++;
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
	
	void call(CB_IDENTIFIER type, Args&&... args) {
		callbacks[static_cast<usize>(type)].call(std::forward<Args>(args)...);
	};
	
	usize bind(CB_IDENTIFIER type, const CB_TYPE& cb) {
		return callbacks[static_cast<usize>(type)].bind(cb);
	};
	
	void unbind(CB_IDENTIFIER type, usize ind) {
		callbacks[static_cast<usize>(type)].unbind(ind);
	};
	
private:
	array<CallbackManager<Args...>, static_cast<usize>(E::_END)> callbacks;
};
