//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <string>
#include <sol/forward.hpp>

#include "SubgameUsertype.h"

#include "../../util/CovariantPtr.h"

class Subgame;
class ItemStack;

namespace Api::Usertype {
	class ItemStack : SubgameUsertype {
	public:
		ItemStack() = default;
		
		explicit ItemStack(sol::table tbl);
		
		ItemStack(const ::ItemStack& stack, SubgamePtr game);
		
		ItemStack(const std::string& name, unsigned short count);
		
		std::string get_name();
		
		void set_name(const std::string& newName);
		
		int get_count();
		
		void set_count(int newCount);
		
		bool is_empty();
		
		static void bind(State state, sol::state& lua, sol::table& core);
		
	private:
		std::string name;
		unsigned short count;
	};
}