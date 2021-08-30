#include <stack>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "client/gui/Expression.h"

#include "Gui.h"
#include "util/Util.h"

Gui::Expression::Expression(const string& exp) {
	setExpression(exp);
}

void Gui::Expression::setExpression(string exp) {
	// Avoid reparsing the same expression.
	usize newHash = std::hash<string>{}(exp);
	if (hash == newHash) return;
	hash = newHash;
	
	// Sanitize expression
	exp.erase(std::remove_if(exp.begin(), exp.end(), isspace), exp.end());
	
	// Process Infix into Postfix (RPN)
	std::queue<string> queue {};
	std::stack<char> operators {};
	
	bool nextOperatorIsUnary = true;
	
	string temp = {};
	
	while (exp.size()) {
		let& c = exp[0];
		// Binary Operator
		if (!nextOperatorIsUnary && (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')) {
			if (temp.size()) {
				queue.emplace(temp);
				temp = {};
			}
			
			while (operators.size() && operators.top() != '(' &&
				((c != '^' && PRECEDENCE.at(operators.top()) >= PRECEDENCE.at(c)) ||
					PRECEDENCE.at(operators.top()) > PRECEDENCE.at(c))) {
				
				queue.emplace(string(1, operators.top()));
				operators.pop();
			}
			
			operators.emplace(c);
			nextOperatorIsUnary = true;
		}
		// Opening Parentheses
		else if (c == '(') {
			if (temp.size()) {
				queue.emplace(temp);
				temp = {};
			}
			
			operators.push(c);
			nextOperatorIsUnary = true;
		}
		// Closing Parentheses
		else if (c == ')') {
			if (!temp.size()) throw std::logic_error("Empty or mismatched parentheses.");
			queue.emplace(temp);
			temp = {};
			
			if (!operators.size()) throw std::logic_error("Mismatched parentheses.");
			while (operators.top() != '(') {
				queue.emplace(string(1, operators.top()));
				operators.pop();
				if (!operators.size()) throw std::logic_error("Mismatched parentheses.");
			}
			if (operators.top() != '(') throw std::logic_error("Mismatched parentheses.");
			operators.pop();
			nextOperatorIsUnary = false;
		}
		// Number or Unit or Keyword
		else {
			temp += c;
			nextOperatorIsUnary = false;
		}
		
		exp.erase(0, 1);
	}
	
	if (temp.size()) {
		queue.push(temp);
		temp = {};
	}
	
	while (operators.size()) {
		if (operators.top() == '(') throw std::logic_error("Mismatched parentheses.");
		queue.emplace(string(1, operators.top()));
		operators.pop();
	}
	
	expression.clear();
	expression.reserve(queue.size());
	
	while (queue.size()) {
		expression.push_back(Token(queue.front()));
		queue.pop();
	}
}

f32 Gui::Expression::eval(const ExpressionInfo& info) {
	if (!expression.size()) return nanf("");
	
	std::stack<Token> eval {};
	
	for (usize i = 0; i < expression.size(); i++) {
		let& t = expression[i];
		
		if (!t.isOperator()) {
			eval.emplace(t);
		}
		else {
			if (eval.size() < 2) throw std::runtime_error("Eval stack has < 2 items! This is an engine error!");
			
			let b = eval.top();
			eval.pop();
			let a = eval.top();
			eval.pop();
			
			switch (t.unit) {
			default:
				throw std::logic_error("Tried to operate with a non-operator token! This is an engine error!");
			case UnitOrOperator::ADD:
				eval.emplace(a.eval(info) + b.eval(info), UnitOrOperator::RAW);
				break;
			case UnitOrOperator::SUBTRACT:
				eval.emplace(a.eval(info) - b.eval(info), UnitOrOperator::RAW);
				break;
			case UnitOrOperator::MULTIPLY:
				eval.emplace(a.eval(info) * b.eval(info), UnitOrOperator::RAW);
				break;
			case UnitOrOperator::DIVIDE:
				eval.emplace(a.eval(info) / b.eval(info), UnitOrOperator::RAW);
				break;
			case UnitOrOperator::EXPONENT:
				eval.emplace(pow(a.eval(info), b.eval(info)), UnitOrOperator::RAW);
				break;
			}
		}
	}
	
	if (!eval.size()) throw std::runtime_error("Eval stack is empty! This is an engine error!");
	return eval.top().eval(info);
}

const std::unordered_map<char, u8> Gui::Expression::PRECEDENCE {
	{ '^', 4 },
	{ '*', 3 },
	{ '/', 3 },
	{ '+', 2 },
	{ '-', 2 }
};

Gui::Expression::Token::Token(const string& str) {
	// Operator
	if (str.size() == 1) {
		switch (str[0]) {
			default: break;
			case '+': unit = UnitOrOperator::ADD; return;
			case '-': unit = UnitOrOperator::SUBTRACT; return;
			case '*': unit = UnitOrOperator::MULTIPLY; return;
			case '/': unit = UnitOrOperator::DIVIDE; return;
			case '^': unit = UnitOrOperator::EXPONENT; return;
		}
	}
	
	// Value
	usize unitInd = -1;
	val = std::stof(str, &unitInd);
	string unitStr = str.substr(unitInd);
	
	// Unit
	switch (Util::hash(unitStr.data())) {
		default: throw std::logic_error("Unknown unit '" + unitStr + "'.");
		
		case Util::hash(""):
		case Util::hash("px"): unit = UnitOrOperator::RAW; return;
		case Util::hash("dp"): unit = UnitOrOperator::DISPLAY_PIXEL; return;
		case Util::hash("cw"): unit = UnitOrOperator::CONTAINER_WIDTH; return;
		case Util::hash("ch"): unit = UnitOrOperator::CONTAINER_HEIGHT; return;
		case Util::hash("sw"): unit = UnitOrOperator::SELF_WIDTH; return;
		case Util::hash("sh"): unit = UnitOrOperator::SELF_HEIGHT; return;
		case Util::hash("deg"): unit = UnitOrOperator::DEGREE; return;
	}
}

bool Gui::Expression::Token::isOperator() {
	return static_cast<u8>(unit) >= 128;
}

f32 Gui::Expression::Token::eval(const ExpressionInfo& info) {
	switch (unit) {
		default: throw std::logic_error("Tried to eval() on an Operator token! This is an engine error!");
		
		case UnitOrOperator::DISPLAY_PIXEL: return val * Gui::PX_SCALE;
		case UnitOrOperator::RAW: return val;
		case UnitOrOperator::CONTAINER_WIDTH: {
//			std::cout << info.containerSize << ":" << val << std::endl;
			return (val / 100.f) * info.containerSize.x;
		}
		case UnitOrOperator::CONTAINER_HEIGHT: return (val / 100.f) * info.containerSize.y;
		case UnitOrOperator::SELF_WIDTH: return (val / 100.f) * info.selfSize.x;
		case UnitOrOperator::SELF_HEIGHT: return (val / 100.f) * info.selfSize.y;
		case UnitOrOperator::DEGREE: return val * M_PI / 180.f;
	}
}
