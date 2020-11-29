#pragma once

#include "SubgameModule.h"

namespace Api::Module {
	class Structure : public Api::Module::SubgameModule {
	public:
		using SubgameModule::SubgameModule;
		
		void bind() override;
	
	protected:
		sol::object create_structure(sol::table data);
	};
}