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
#include <iostream>

/*
 * Zepha, designed, developed, and created by Nicole Collings
 * This is my child, and I hope you like it.
 * Copyright 2018 - present Auri Collings, All Rights Reserved.
 */

struct TexData {
	TexData() = default;
	TexData(string p): p(p) {}
	string p;
};

int main(int argc, char* argv[]) {
//	//	return StartGame(argc, argv);

	
//	using DataOrIden = variant<string, TexData>;
//
	StringParser<TexData> s;
//
//	s.addFn<std::variant<int, string>>("variant", [](std::variant<int, string> var) {
//		const int* intV = std::get_if<int>(&var);
//		if (intV) std::cout << "int " << *intV << std::endl;
//		else std::cout << "str " << std::get<string>(var) << std::endl;
//		return Res { 1 };
//	});
//
//	s.addFn<std::optional<string>>("optional", [](std::optional<string> var) {
//		if (var) std::cout << "opt filled " << *var << std::endl;
//		else std::cout << "opt empty" << std::endl;
//		return Res { 2 };
//	});
////
////	s.parse("crop(0, 0, 8, 8, tint(0, zeus:default:grass))");
//
//	std::cout << s.parse("variant(2)") << std::endl;
//	std::cout << s.parse("variant(hello)") << std::endl;
//	std::cout << s.parse("optional(,,,)") << std::endl;
//	std::cout << s.parse("optional(hello)") << std::endl;
//	std::cout << s.parse("optional()") << std::endl;

	s.addFn<u16, u16, u16, u16, TexData>("crop", [](u16 x, u16 y, u16 w, u16 h, TexData tex) {
		return TexData { "cropped(" + tex.p + ")" };
	});

	s.addFn<u16, TexData>("tint", [](u16 ind, TexData tex) {
		return TexData { "tinted(" + tex.p + ")" };
	});

	s.addLiteralFn<string>([](string lit) {
		return TexData { lit };
	});

	std::cout << s.parse("crop(0,0,0,0,hello world)").p << std::endl;
	std::cout << s.parse("tint(1,tint(2, crop(0,0,0,0,tint(0,delta))))").p << std::endl;
	std::cout << s.parse("hello").p << std::endl;
	
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