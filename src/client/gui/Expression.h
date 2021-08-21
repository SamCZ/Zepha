#pragma once

#include <queue>
#include <unordered_map>

#include "util/Types.h"

namespace Gui {
	enum class UnitOrOperator: u8 {
		DISPLAY_PIXEL,
		REAL_PIXEL,
		DEGREE,
		
		ADD = 128,
		SUBTRACT,
		MULTIPLY,
		DIVIDE,
		EXPONENT
	};
	
	class Expression {
		struct Token {
			Token() = default;
			explicit Token(const string& str);
			Token(f32 val, UnitOrOperator unit): val(val), unit(unit) {};
			
			bool isOperator();
			
			f32 evalValue();
			
			f32 val = 0;
			UnitOrOperator unit;
		};
		
	public:
		Expression() = default;
		Expression(const string& exp);
	
		void setExpression(string exp);
	
		f32 eval();
		
	private:
		usize hash = 0;
		
		std::vector<Token> expression;
	
		const static std::unordered_map<char, u8> PRECEDENCE;
	};
}