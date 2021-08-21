#pragma once

#include <queue>
#include <unordered_map>

#include "util/Types.h"

namespace Gui {
	struct ExpressionInfo {
		ExpressionInfo() = default;
		ExpressionInfo(ivec2 containerSize, ivec2 selfSize): containerSize(containerSize), selfSize(selfSize) {}
		
		ivec2 containerSize;
		ivec2 selfSize;
	};
	
	class Expression {
		enum class UnitOrOperator: u8 {
			RAW,
			DISPLAY_PIXEL,
			CONTAINER_WIDTH,
			CONTAINER_HEIGHT,
			SELF_WIDTH,
			SELF_HEIGHT,
			DEGREE,
		
			ADD = 128,
			SUBTRACT,
			MULTIPLY,
			DIVIDE,
			EXPONENT
		};
		
		struct Token {
			Token() = default;
			explicit Token(const string& str);
			Token(f32 val, UnitOrOperator unit): val(val), unit(unit) {};
			
			bool isOperator();
			
			f32 eval(const ExpressionInfo& info);
			
			f32 val = 0;
			UnitOrOperator unit;
		};
		
	public:
		Expression() = default;
		Expression(const string& exp);
	
		void setExpression(string exp);
	
		f32 eval(const ExpressionInfo& info);
		
	private:
		usize hash = 0;
		
		vec<Token> expression;
	
		const static std::unordered_map<char, u8> PRECEDENCE;
	};
}