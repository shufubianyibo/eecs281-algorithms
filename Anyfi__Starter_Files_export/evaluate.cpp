// Project identifier: 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768

#include "evaluate.hpp"

#include <cstdint>
#include <string>
#include <stack>

std::int64_t evaluate(std::string const& expression) {
    // Your implementation goes here
    std::stack<std::int64_t> values;
    std::stack<char> ops;
    for (char c : expression) {
        if (std::isdigit(c)) {
            values.push(c - '0'); 
        } else {
            ops.push(c); 
        }
        while (values.size() >= 2 && !ops.empty()) {
            std::int64_t right = values.top();
            values.pop();
            std::int64_t left = values.top();
            values.pop();
            char op = ops.top();
            ops.pop();

            std::int64_t result;
            if (op == '+') result = left + right;
            else if (op == '-') result = left - right;
            else if (op == '*') result = left * right;
            else result = left / right; 

            values.push(result); 
        }
    }
    return values.top();
}
