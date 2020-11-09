//
// Created by aurailus on 2020-07-30.
//

#pragma once

#include "SubgameModule.h"

#include "../usertype/Dimension.h"

namespace Api::Module {
	class Dimension : public Api::Module::SubgameModule {
		public:
		using SubgameModule::SubgameModule;
		
		void bind() override;
		
		protected:
		Api::Usertype::Dimension createDimension(const std::string& identifier, sol::table data);
		
		sol::object getDimension(sol::this_state s, const std::string& identifier);
		
		sol::object getDefaultDimension(const sol::this_state s);
		
		void setDefaultDimension(const std::string& identifier);
	};
}