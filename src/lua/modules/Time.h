#pragma once

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