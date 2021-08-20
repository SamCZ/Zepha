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
		struct String {
			String() = default;
			explicit String(const string& v): v(v) {}
		
			string v {};
		
			bool isOperator();
		
			f32 eval();
		};
		
		struct Token {
			Token() = default;
			explicit Token(f32 val, UnitOrOperator unit): val(val), unit(unit);
			explicit Token(const string& str);
			
			bool isOperator();
			f32 evalValue();
			
			f32 val;
			UnitOrOperator unit;
		};
	public:
		Expression() = default;
		Expression(const string& exp);
	
		void setExpression(string exp);
	
		f32 eval();
	private:
		usize hash = 0;
		std::queue<String> infix {};
	
		const static std::unordered_map<char, u8> PRECEDENCE;
	};
}