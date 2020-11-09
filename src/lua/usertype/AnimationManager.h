//
// Created by aurailus on 2020-01-14.
//

#pragma once

#include "../Lua.h"

#include "../../util/CovariantPtr.h"
#include "SubgameUsertype.h"

namespace Api::Usertype {
	class AnimationManager : public SubgameUsertype {
		public:
		AnimationManager(EntityPtr entity);
		
		EntityPtr entity;
		
		virtual AnimationManager define(sol::table anims);
		
		virtual AnimationManager set_anim(sol::object anim, sol::optional<bool> loop);
		
		virtual AnimationManager play(sol::optional<unsigned int> offset);
		
		virtual AnimationManager pause(sol::optional<unsigned int> offset);
	};
	
	class LocalAnimationManager : public AnimationManager {
		public:
		using AnimationManager::AnimationManager;
		
		static void bind(State state, sol::state& lua, sol::table& core);
	};
	
	class ServerAnimationManager : public AnimationManager {
		public:
		using AnimationManager::AnimationManager;
		
		virtual AnimationManager set_anim(sol::object anim, sol::optional<bool> loop) override;
		
		virtual AnimationManager play(sol::optional<unsigned int> offset) override;
		
		virtual AnimationManager pause(sol::optional<unsigned int> offset) override;
		
		static void bind(State state, sol::state& lua, sol::table& core);
	};
}