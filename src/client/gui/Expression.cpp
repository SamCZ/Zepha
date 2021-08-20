#include <stack>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "client/gui/Expression.h"

#include "util/Util.h"
#include "Gui.h"

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
	infix = {};
	std::stack<char> operators {};
	
	bool nextOperatorIsUnary = true;
	
	String temp = {};
	
	while (exp.size()) {
		let& c = exp[0];
		// Number or Unit or Keyword
		if ((c >= '0' && c <= '9') || c == '.' || (c >= 97 && c <= 122) ||
		(nextOperatorIsUnary && (c == '+' || c == '-'))) {
			temp.v += c;
			nextOperatorIsUnary = false;
		}
		// Binary Operator
		else if (!nextOperatorIsUnary && (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')) {
			if (temp.v.size()) {
				infix.emplace(temp);
				temp = {};
			}
			
			while (operators.size() && operators.top() != '(' &&
				((c != '^' && PRECEDENCE.at(operators.top()) >= PRECEDENCE.at(c)) ||
					PRECEDENCE.at(operators.top()) > PRECEDENCE.at(c))) {
				
				infix.emplace(string(1, operators.top()));
				operators.pop();
			}
			
			operators.emplace(c);
			nextOperatorIsUnary = true;
		}
		// Opening Parentheses
		else if (c == '(') {
			if (temp.v.size()) {
				infix.emplace(temp);
				temp = {};
			}
			
			operators.push(c);
			nextOperatorIsUnary = true;
		}
		// Closing Parentheses
		else if (c == ')') {
			if (!temp.v.size()) throw std::logic_error("Empty or mismatched parentheses.");
			infix.emplace(temp);
			temp = {};
			
			if (!operators.size()) throw std::logic_error("Mismatched parentheses.");
			while (operators.top() != '(') {
				infix.emplace(string(1, operators.top()));
				operators.pop();
				if (!operators.size()) throw std::logic_error("Mismatched parentheses.");
			}
			if (operators.top() != '(') throw std::logic_error("Mismatched parentheses.");
			operators.pop();
			nextOperatorIsUnary = false;
		}
		
		exp.erase(0, 1);
	}
	
	if (temp.v.size()) {
		infix.push(temp);
		temp = {};
	}
	
	while (operators.size()) {
		if (operators.top() == '(') throw std::logic_error("Mismatched parentheses.");
		infix.emplace(string(1, operators.top()));
		operators.pop();
	}
}

f32 Gui::Expression::eval() {
	let infix = this->infix;
	std::stack<String> eval {};
	
	while (infix.size()) {
		let& t = infix.front();
		infix.pop();
		
		if (!t.isOperator()) {
			eval.emplace(t);
		}
		else {
			if (eval.size() < 2) throw std::runtime_error("Eval stack has < 2 items! This is an engine error!");
			
			String b = eval.top();
			eval.pop();
			String a = eval.top();
			eval.pop();
			
			switch (t.v[0]) {
			case '+':
				eval.emplace(std::to_string(a.eval() + b.eval()));
				break;
			case '-':
				eval.emplace(std::to_string(a.eval() - b.eval()));
				break;
			case '*':
				eval.emplace(std::to_string(a.eval() * b.eval()));
				break;
			case '/':
				eval.emplace(std::to_string(a.eval() / b.eval()));
				break;
			case '^':
				eval.emplace(std::to_string(pow(a.eval(), b.eval())));
				break;
			}
		}
	}
	
	if (!eval.size()) throw std::runtime_error("Eval stack is empty! This is an engine error!");
	return eval.top().eval();
}

const std::unordered_map<char, u8> Gui::Expression::PRECEDENCE {
	{ '^', 4 },
	{ '*', 3 },
	{ '/', 3 },
	{ '+', 2 },
	{ '-', 2 }
};

bool Gui::Expression::String::isOperator() {
	return v.size() == 1 && (v[0] == '+' || v[0] == '-' || v[0] == '*' || v[0] == '/' || v[0] == '^');
}

f32 Gui::Expression::String::eval() {
	usize unitInd = -1;
	
	f32 value = std::stof(v, &unitInd);
	string unit = v.substr(unitInd);
	
	switch (Util::hash(unit.data())) {
	default:
		throw std::logic_error("Unknown unit '" + unit + "'.");
	
	case Util::hash("dp"):
		return value * Gui::PX_SCALE;
		
	case Util::hash("deg"):
		return value * M_PI / 180.f;
	
	case Util::hash(""):
	case Util::hash("px"):
		return value;
	}
}
