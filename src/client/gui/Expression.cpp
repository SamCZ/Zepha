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
		// Number or Unit or Keyword
		if ((c >= '0' && c <= '9') || c == '.' || (c >= 97 && c <= 122) ||
		(nextOperatorIsUnary && (c == '+' || c == '-'))) {
			temp += c;
			nextOperatorIsUnary = false;
		}
		// Binary Operator
		else if (!nextOperatorIsUnary && (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')) {
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

f32 Gui::Expression::eval() {
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
				throw std::logic_error("Tried to operate with a non-operator token.");
			case UnitOrOperator::ADD:
				eval.emplace(a.evalValue() + b.evalValue(), UnitOrOperator::REAL_PIXEL);
				break;
			case UnitOrOperator::SUBTRACT:
				eval.emplace(a.evalValue() - b.evalValue(), UnitOrOperator::REAL_PIXEL);
				break;
			case UnitOrOperator::MULTIPLY:
				eval.emplace(a.evalValue() * b.evalValue(), UnitOrOperator::REAL_PIXEL);
				break;
			case UnitOrOperator::DIVIDE:
				eval.emplace(a.evalValue() / b.evalValue(), UnitOrOperator::REAL_PIXEL);
				break;
			case UnitOrOperator::EXPONENT:
				eval.emplace(pow(a.evalValue(), b.evalValue()), UnitOrOperator::REAL_PIXEL);
				break;
			}
		}
	}
	
	if (!eval.size()) throw std::runtime_error("Eval stack is empty! This is an engine error!");
	return eval.top().evalValue();
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
		
		case Util::hash("dp"): unit = UnitOrOperator::DISPLAY_PIXEL; return;
		case Util::hash(""):
		case Util::hash("px"): unit = UnitOrOperator::REAL_PIXEL; return;
		case Util::hash("deg"): unit = UnitOrOperator::DEGREE; return;
	}
}

bool Gui::Expression::Token::isOperator() {
	return static_cast<u8>(unit) >= 128;
}

f32 Gui::Expression::Token::evalValue() {
	switch (unit) {
		default: throw std::logic_error("Tried to evalValue() on an Operator token.");
		
		case UnitOrOperator::DISPLAY_PIXEL: return val * Gui::PX_SCALE;
		case UnitOrOperator::REAL_PIXEL: return val;
		case UnitOrOperator::DEGREE: return val * M_PI / 180.f;
	}
}
