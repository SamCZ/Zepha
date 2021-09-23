#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define STB_IMAGE_IMPLEMENTATION
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <cute_files.h>
#include <util/StringParser.h>

#pragma clang diagnostic pop

//#include "StartGame.h"

/*
 * Zepha, designed, developed, and created by Nicole Collings
 * This is my child, and I hope you like it.
 * Copyright 2018 - present Auri Collings, All Rights Reserved.
 */

//constexpr char c[] = "ha";
int main(int argc, char* argv[]) {
//	//	return StartGame(argc, argv);
	
	StringParser<string> s {{
		{ "crop", Fn::create<int, int, int, int, string>([](int x, int y, int w, int h, string tex) {
			std::cout << "cropping " << x << "/" << y << ", " << w << "/" << h << " : " << tex << std::endl;
		})},
		{ "tint", Fn::create<int, string>([](int ind, string tex) {
			std::cout << "tinting " << tex << " with ind " << ind << std::endl;
		})},
		{ "_", Fn::create<string>([](string tex) {
			std::cout << "UNARY _ DEFAULT BITCH: " << tex << std::endl;
		})}
	}};
	
	s.parse("crop(0,0,0,0,tint(0,hello_world))");
	s.parse("crop(0,8,16,24,tint(0,hello_world))");
	s.parse("tint(0,hello_world)");
	s.parse("hello_world");

	//	s.parse("aaa");
//	return 0;
	
//	Fn i = Fn::create<int, float>([](int a, float b) -> void {
//		std::cout << a << " : " << b << std::endl;
//	});
//
//	i({ "5", "52.3123" });

//	TexParser::Fn<int> f("aaa", [&](int a) {
//		return "";
//	});

}