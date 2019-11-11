#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>

using namespace std;



bool containsValidSymbols(string infix)
{
	//returns false if the string contains symbols that are not lowercase letters or valid symbols
	for (int i = 0; i < infix.size(); i++)
	{
		if (isalpha(infix[i]))
		{
			if (infix[i] != tolower(infix[i]))		// uppercase letter
				return false;
		}
		else
		{
			switch (infix[i])
			{
			case '(':
			case ')':
			case '&':										// if it's not one of these valid symbols, return false
			case '|':
			case '!':
			case ' ':
				break;
			default:
				return false;
			}
		}
	}
	return true;
}

bool toPostFix(string infix, string& postfix)
{
	if (!containsValidSymbols(infix) || infix.size() == 0)
		return false;					
	// last character can't be operator or open parentheses
	stack<char> operatorStack;
	string post = "";
	char prev = infix[0];


	if (prev != ' ')					// if space do nothing
	{
		if (isalpha(infix[0]))
			post += infix[0];
		else if (infix[0] == '(' || infix[0] == '!')
			operatorStack.push(infix[0]);
		else
			return false;							// start only with !, (, or operand
	}
	for (int i = 1; i < infix.size(); i++)																// we have the previous, check everything else
	{
		if (isalpha(infix[i]))
		{
			if (isalpha(prev) || prev == ')')				// previous cannot be another operand or closing parentheses
				return false;
			post += infix[i];
		}
		else
			switch (infix[i])
			{
			case ' ':							// skip over spaces
				continue;
			case '(':
				if (isalpha(prev) || prev == ')')			// previous cannot be another operand or closing parentheses
					return false;
				operatorStack.push(infix[i]);
				break;
			case ')':
				if (!isalpha(prev) && prev != ')')							//prev can't be open parentheses, operator, !, must be operator or another close parentheses
					return false;
				while (operatorStack.top() != '(')			
				{
					post += operatorStack.top();
					operatorStack.pop();
					if (operatorStack.empty())						// quit if no '(' before our ')'
						return false;
				}
				operatorStack.pop();
				break;
			case '!':
				if (isalpha(prev) || prev == ')')
					return false;
				operatorStack.push(infix[i]);
				break;
			case '&':
			case '|':																// ascii order: ! < & < |
				if (!isalpha(prev) && prev != ')')									// can't be operator or open parentheses, must be letter or close parentheses
					return false;
				while (!operatorStack.empty() && operatorStack.top() != '(' && infix[i] >= operatorStack.top())
				{
					post += operatorStack.top();
					operatorStack.pop();
				}
				operatorStack.push(infix[i]);
				break;
			}
		prev = infix[i];
	}

	if (!isalpha(prev) && prev != ')')				// last char processed must be operand or closing parentheses
		return false;

	while (!operatorStack.empty()) // get the rest of the operators
	{
		if (operatorStack.top() == '(') // found a open parentheses w/o a closing one, syntax invalid
			return false;
		post += operatorStack.top();
		operatorStack.pop();
	}
	postfix = post;
	return true;
}

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
	if (!toPostFix(infix, postfix))
		return 1;
	for (int i = 0; i < postfix.size(); i++)			// check letters are in truevalues and false values
		if (isalpha(postfix[i]))
		{
			if (trueValues.contains(postfix[i]) && falseValues.contains(postfix[i]))				//in both
				return 3;
			else if (!trueValues.contains(postfix[i]) && !falseValues.contains(postfix[i]))			// in neither
				return 2;
		}

	stack<bool> operandStack;
	bool operand1;
	bool operand2;
	for (int i = 0; i < postfix.size(); i++)
	{
		if (isalpha(postfix[i]))
		{
			if (trueValues.contains(postfix[i]))
				operandStack.push(true);
			else
				operandStack.push(false);
		}
		else if (postfix[i] == '!') // char is unary operator
		{
			operand1 = operandStack.top();
			operandStack.pop();
			operandStack.push(!operand1);
		}
		else                      //char is binary operator
		
		{
			operand2 = operandStack.top();
			operandStack.pop();
			operand1 = operandStack.top();
			operandStack.pop();
			if (postfix[i] == '&')
				operandStack.push(operand1 && operand2);
			else
				operandStack.push(operand1 || operand2);

		}
	}
	result = operandStack.top();
	return 0;
}

/*Valid Syntax:
	before operand: open parentheses, !, operator
	before open parentheses: !, operator, open parentheses
	before closed parentheses: operand, must be after an open parentheses but not directly after
	before operator: operand, close parentheses
	before !: operator, open parentheses, !*/

// Evaluate a boolean expression
//   If infix is a syntactically valid infix boolean expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values sets), then postfix is set to the postfix
//   form of the expression.  If not, postfix might or might not be
//   changed, result is unchanged, and the function returns 1.
//
//   If infix is a syntactically valid infix boolean expression whose
//   only operands are single lower case letters:
//
//      If every operand letter in the expression appears in either
//      trueValues or falseValues but not both, then result is set to the
//      result of evaluating the expression (using for each letter in the
//      expression the value true if that letter appears in trueValues or
//      false if that letter appears in false values) and the function
//      returns 0.
//
//      Otherwise, result is unchanged and the value the function returns
//      depends on these two conditions:
//        at least one letter in the expression is in neither the
//            trueValues nor the falseValues sets; and
//        at least one letter in the expression is in both the
//            trueValues and the falseValues set.
//      If only the first condition holds, the function returns 2; if
//      only the second holds, the function returns 3.  If both hold
//      the function returns either 2 or 3 (and the function is not
//      required to return the same one if called another time with the
//      same arguments).
