//
// Created by aurailus on 22/08/19.
//

#pragma once

#include <iostream>
#include <execinfo.h>
#include <unordered_map>

#include "util/Log.h"
#include "util/Util.h"
#include "util/Types.h"
#include "client/Client.h"
#include "server/Server.h"
#include "util/net/Address.h"

enum class Mode {
	CLIENT, SERVER
};


/**
 * Parses the arg list provided by the operating system into a map of key-value strings.
 * @throws invalid_argument if it encounters a duplicate or improperly formatted argument.
 * @param argc - Argument array length
 * @param argv - Argument array
 * @returns - A map of parsed arguments.
 */

std::unordered_map<string, string> ParseArgs(i32 argc, char* argv[]) {
	std::unordered_map<string, string> args;
	
	for (usize i = 1; i < argc; i++) {
		string arg(argv[i]);
		usize equals = arg.find('=');
		string first = (equals == -1) ? arg : arg.substr(0, equals);
		
		if (args.count(first)) throw std::invalid_argument("Duplicate argument " + first + ".");
		
		if (equals == -1) args.emplace(first, "");
		else if (equals == arg.length() - 1) throw std::invalid_argument("Empty argument " + first + ".");
		else args.emplace(first, arg.substr(equals + 1, arg.length()));
	}
	
	return args;
}

/**
 * Instantiates a Client or Server instance, depending on the arguments provided.
 * @param argc - Argument array length
 * @param argv - Argument array
 * @returns - A numerical value indicating exit status.
 */

int StartGame(int argc, char* argv[]) {
	Mode mode = Mode::CLIENT;
	
//	try
	{
		u16 port = Address::DEFAULT_PORT;
		std::string subgame = "";
		bool ascii = true;
		
		/**
		 * Handle command line arguments.
		 * @arg mode ("client" | "server") - Whether to initialize a client instance, or a server. Defaults to "client".
		 * @arg port (unsigned short) - The port that the server should listen on. Defaults to Address::DEFAULT_PORT.
		 * @arg subgame (std::string) - The subgame that the server should load.
		 * @arg noascii - Switch to disable ASCII from the console output.
		 */
		
		for (auto arg : ParseArgs(argc, argv)) {
			switch (Util::hash(arg.first.c_str())) {
			default: throw std::runtime_error("Invalid argument '" + arg.first + "'.");
			
			case Util::hash("--mode"):
				if (arg.second == "client") mode = Mode::CLIENT;
				else if (arg.second == "server") mode = Mode::SERVER;
				else throw std::runtime_error("Invalid mode specified.");
				break;

//            case Util::hash("--address"):
//                addr.host = arg.second;
//                break;
			
			case Util::hash("--port"):
				port = static_cast<u16>(stoi(arg.second));
				break;
			
			case Util::hash("--subgame"):
				subgame = arg.second;
				break;
			
			case Util::hash("--noascii"):
				ascii = false;
				break;
			}
		}
		
		if (ascii) {
			Log::clear();
			std::cout <<
				"\n"
				"\t\t ____  ____  ____  _  _   __  \n"
				"\t\t(__  )(  __)(  _ \\/ )( \\ / _\\ \n"
				"\t\t / _/  ) _)  ) __/) __ (/    \\\n"
				"\t\t(____)(____)(__)  \\_)(_/\\_/\\_/\n" << std::endl;
		}
		
		if (mode == Mode::CLIENT) Client({ 1366, 768 });
		else Server(port, subgame);
		
		return 0;
	}
//	catch (const std::exception& e) {
//		void *array[10];
//		size_t size;
//		size = backtrace(array, 10);
//		std::cout << Log::err << "Zepha crashed unexpectedly.\n" << e.what() << std::endl;
//		backtrace_symbols_fd(array, size, STDERR_FILENO);
//		std::cout << Log::endl;
//		return 1;
//	}
}
