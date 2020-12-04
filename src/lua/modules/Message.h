#pragma once

#include <string>

#include "SubgameModule.h"

namespace Api::Module {
	class Message : public Api::Module::SubgameModule {
	public:
		using SubgameModule::SubgameModule;
		
		void bind() override;
	
	protected:
		void send_message(const std::string& channel, const std::string& message);
	};
}


