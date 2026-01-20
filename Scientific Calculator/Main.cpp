// Including libraries //
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stack>
#include <cmath>

// Creating character types so we can correctly define each character when the user inputs a equation //
enum tokenType
{
	NUMBER,
	OPERATOR,
	FUNCTION,
	PARENTHESIS
};

// This is a template for each character inside the tokenList which is later defined in the token class //
struct token
{
	/* This is very important so we can track each character from the equation and then
	   give it a type (eg number, operator, function or parenthesis, Note the function
	   is for scientific equations like sin, cos tan or log) once we are able to define each
	   character we can then use this to do each calculation in order for example doing
	   a multiplcation first then a addition etc. */

	tokenType type;
	std::string value;
};

// This class will handle all the logic required for the calculations and sorting out the equations //
class Calculator
{
private:
	/* We are creating a vector list so we can later add each character from the users
	   equation and then fill it with the corrected type so we can calculate at the end. */
	std::vector<token> tokenList;

	/* Here we are returning a priority number depending on what the operator is, this will be used to construct BIDMAS.
       sin, cos, tan and log have the second highest priority meaning they will be done first after the parenthesis which we havnt
       included here but they will be done first, then we do multiplication and division next after that we do addition and
       subtraction last as thats the BIDMAS order. */
	const int precedence(const std::string& op) const
	{
		if (op == "+" || op == "-") { return 1; }
		if (op == "*" || op == "/") { return 2; }
		if (op == "sin" || op == "cos" || op == "tan" || op == "log") { return 3; }

		return 0;
	}

	// We collect both numbers and do the equation based on the operator then return the total //
	const double apply_operation_order(double a, double b, const std::string& op) const
	{
		if (op == "+") { return a + b; }
		if (op == "-") { return a - b; }
		if (op == "*") { return a * b; }
		if (op == "/") { return a / b; }

		return 0;
	}

	// If the equation contains sin, cos, tan or log we then use this function to do the equation and return the result //
	const double applyFunc(double value, const std::string& func)
	{
		if (func == "sin") { return std::sin(value); }
		if (func == "cos") { return std::cos(value); }
		if (func == "tan") { return std::tan(value); }
		if (func == "log") { return std::log(value); }

		return value;
	}

	/* In this function we first recieve 2 references through the parameters, first one is the values
   inside our defined stack in the calculate function and this basically contains all the numbers
   from the equation which we have extracted from the users equation inside the calculate function
   secondly we take operators, this is all the operators from the users equation which also has
   been extracted from the calculate function, the reason we do this is because in order to calculate
   in BIDMAS (giving multiplication and division priority over addition and subtracting) we must
   first create 2 stacks which are like a list that will include the priority values and operatos at
   the top then we go over both stacks in this function and do the equation from top to bottom using
   the stacks. This ultimatly makes BIDMAS equations possible and we can also add specific functions
   like sin, cos and tan. */

   /* Overall the goal of this function is to take both stacks and then peform a if statement which will
	  apply sin, cos, tan or log if the operator on the oprerators stack at the top is sin, cos, tan or log
	  however if its not then we assume its a normal operator like (/, +, *, -, (), ) Note that parentheses
	  are handled as specific tokens to control the flow and override standard operator precedence. finally
	  we then call the function that will handle the equation and then we push that return value to the values
	  stack which contains all the results from the equations. (Extra tip) we keep calculating all the numbers
	  and equations until nothing is left and then we end up with the total. */

	void processMath(std::stack<double>& values, std::stack<std::string>& operators)
	{
		std::string op = operators.top(); operators.pop();

		if (op == "sin" || op == "cos" || op == "tan" || op == "log")
		{
			double val = values.top(); values.pop();
			values.push(applyFunc(val, op));
		}
		else {
			double val2 = values.top(); values.pop();
			double val1 = values.top(); values.pop();
			values.push(apply_operation_order(val1, val2, op));
		}
	}

	/* The purpose of this function is to fill our tokenList vector with all the characters from the users equation
   and individually give each character a type meaning a number will get the type NUMBER and a operator like / or *
   will get the type OPERATOR, all the types are defined above named tokenType (they are the enums).*/

	void tokenise(const std::string& userInput)
	{
		tokenList.clear();

		for (int i = 0; i < userInput.length(); i++)
		{
			char c = userInput[i];
			if (isspace(c)) { continue; }

			if (isdigit(c) || c == '.')
			{
				std::string num;
				while (i < userInput.length() && (isdigit(userInput[i]) || userInput[i] == '.'))
				{
					num += userInput[i];
					i++;
				}
				i--;
				tokenList.push_back({ NUMBER, num });
			}
			else if (isalpha(c))
			{
				std::string func;
				while (i < userInput.length() && isalpha(userInput[i]))
				{
					func += userInput[i];
					i++;
				}
				i--;
				tokenList.push_back({ FUNCTION, func });
			}
			else if (c == '+' || c == '-' || c == '*' || c == '/')
			{
				tokenList.push_back({ OPERATOR, std::string(1, c) });
			}
			else if (c == '(' || c == ')')
			{
				tokenList.push_back({ PARENTHESIS, std::string(1, c) });
			}
		}
	}
public:
	/* This is where the main logic for calculating the users equation */
	void Calculate(std::string& userInput)
	{
		// First we tokenise the users equation and call this function to fill the tokenList with the all the equations characters and correct type //
		tokenise(userInput);


		/* We create 2 stacks that will contain both the numbers / values and the operators. Its important this is a stack and not an array or vector as with using a stack
		   it will force us to from top to bottom therefore not modifying any numbers in between and messing up the BIDMAS logic aswell as anything else that requires
		   a specific order for the equation to calculate correctly. */
		std::stack<double> values;
		std::stack<std::string> operators;

		/* Once we have filled our tokenList we then run a for loop that will iriterate over the whole tokenList list and do the calculations in order from the top of the stack
		   to the bottom of the stack until there are no more operators and we have a total result. */
		for (int i = 0; i < tokenList.size(); i++)
		{
			if (tokenList[i].type == NUMBER)
			{
				values.push(std::stod(tokenList[i].value));
			}
			else if (tokenList[i].type == PARENTHESIS && tokenList[i].value == "(")
			{
				operators.push("(");
			}
			else if (tokenList[i].type == PARENTHESIS && tokenList[i].value == ")")
			{
				while (!operators.empty() && operators.top() != "(")
				{
					processMath(values, operators);
				}
				if (!operators.empty()) { operators.pop(); }
			}
			else if (tokenList[i].type == OPERATOR)
			{
				while (!operators.empty() && precedence(operators.top()) >= precedence(tokenList[i].value))
				{
					processMath(values, operators);
				}

				operators.push(tokenList[i].value);
			}
			else if (tokenList[i].type == FUNCTION)
			{
				operators.push(tokenList[i].value);
			}
		}

		// We check if there are any operators left after the for loop and process any leftover equations using the process math function. //
		while (!operators.empty())
		{
			processMath(values, operators);
		}

		// Once we have completed calculating all the numbers with the operators we then print out the result to the user. //
		if (!values.empty())
		{
			std::cout << "Result: " << values.top() << "\n";
			std::cout << "[Exit] = Exit Program" << "\n\n";
		}
		else
		{
			std::cout << "Values not found please try again" << '\n';
		}
	}
};

/* When we have the users input we use this function to validate if the string is a valid
   equation and not some random letters. Note this is not a fully working safety guard for
   the equation because if the users input contains any of these letters then have invalid
   strings the program will fail when trying to grab the operator or value however it does
   stop the users from type random letters and most of invalid input but not perfect. */
bool validateString(const std::string& userInput)
{
	// These are all the characters that are valid //
	std::string whitelistedChars = "1234567890.sincostanlog()*/+- ";

	// Looping through the users equation and validating if it has any invalid characters //
	for (const char& c : userInput)
	{
		// If we dont find the characters position inside the string we will return false to say that the string / equation is invalid //
		if (whitelistedChars.find(c) == std::string::npos)
			return false;
	}

	return true;
}

// Program main entry point and where we handle all the logic and functions / class methods //
int main()
{
	std::string calculation;
	Calculator calculator;

	std::cout << "**** Scientific Calculator ****" << '\n';
	std::cout << "-->Press Enter to begin . . ." << "\n";
	std::cin.get();


	// Creating a while loop so we keep the program running the user exits manually when prompted //
	while (true)
	{
		std::cout << "\nInput: ";
		std::getline(std::cin, calculation);

		// We lower the users string so that we dont have to worry about capital letters and can validate the string properly //
		for (char& c : calculation) { c = tolower(c); }

		// If the user has inputted exit we will then exit the program //
		if (calculation.find("exit") != std::string::npos)
			break;

		/* If the users input is valid we then pass it to our class calculator member function named Calculate which
		   will start the process of looping through all the characters inside the string validating what type they
		   are and adding the value to our vector list after that we create 2 stacks and add them in order so we can
		   do the calculation accordingly to bidmas*/
		if (validateString(calculation))
		{
			calculator.Calculate(calculation);
		}
		else
		{
			std::cout << "Invalid Equation!" << '\n';
		}
	}

	// Optional //
	return 0;
}
