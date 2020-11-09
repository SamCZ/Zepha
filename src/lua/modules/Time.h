//
// Created by aurailus on 2020-07-26.
//

#pragma once

#include <glm/vec3.hpp>

#include "SubgameModule.h"
#include "../../util/Timer.h"

namespace Api::Module {
	class Time : public Api::Module::BaseModule {
		public:
		using BaseModule::BaseModule;
		
		void bind() override;
		
		protected:
		float ns();
		
		float ms();
		
		float s();
		
		Timer timer{};
	};
}