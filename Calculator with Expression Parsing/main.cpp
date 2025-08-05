#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <cctype>
#include <map>
#include <stdexcept>
#include <iomanip>
using namespace std;

// Helper functions for operator precedence and associativity
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

vector<string> infixToPostfix(const string& expr) {
    vector<string> output;
    stack<char> ops;
    string num;
    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        if (isdigit(c) || c == '.') {
            num += c;
        } else {
            if (!num.empty()) {
                output.push_back(num);
                num.clear();
            }
            if (isspace(c)) continue;
            if (c == '(') {
                ops.push(c);
            } else if (c == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    output.push_back(string(1, ops.top()));
                    ops.pop();
                }
                if (!ops.empty() && ops.top() == '(') ops.pop();
            } else if (isOperator(c)) {
                while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                    output.push_back(string(1, ops.top()));
                    ops.pop();
                }
                ops.push(c);
            }
        }
    }
    if (!num.empty()) output.push_back(num);
    while (!ops.empty()) {
        output.push_back(string(1, ops.top()));
        ops.pop();
    }
    return output;
}

double evalPostfix(const vector<string>& postfix) {
    stack<double> stk;
    for (const string& token : postfix) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stk.size() < 2) throw runtime_error("Invalid Expression");
            double b = stk.top(); stk.pop();
            double a = stk.top(); stk.pop();
            if (token == "+") stk.push(a + b);
            else if (token == "-") stk.push(a - b);
            else if (token == "*") stk.push(a * b);
            else if (token == "/") {
                if (b == 0) throw runtime_error("Division by zero");
                stk.push(a / b);
            }
        } else {
            stk.push(stod(token));
        }
    }
    if (stk.size() != 1) throw runtime_error("Invalid Expression");
    return stk.top();
}

int main() {
    cout << "--- Calculator with Expression Parsing ---\n";
    cout << "Type an arithmetic expression (supports +, -, *, /, parentheses).\n";
    cout << "Type 'exit' to quit.\n";
    string expr;
    while (true) {
        cout << "\nEnter expression: ";
        getline(cin, expr);
        if (expr == "exit") break;
        try {
            auto postfix = infixToPostfix(expr);
            double result = evalPostfix(postfix);
            cout << "Result: " << fixed << setprecision(8) << result << endl;
        } catch (const exception& ex) {
            cout << "Error: " << ex.what() << endl;
        }
    }
    cout << "Goodbye!\n";
    return 0;
}
