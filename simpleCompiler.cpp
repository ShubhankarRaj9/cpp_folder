#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

enum TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    int value; 
};

class Lexer {
private:
    std::string input;
    size_t pos;
public:
    Lexer(const std::string& str) : input(str), pos(0) {}

    Token getNextToken() {
        while (pos < input.length() && std::isspace(input[pos])) pos++;

        if (pos >= input.length()) return {END, 0};

        char current = input[pos];
        if (std::isdigit(current)) {
            int num = 0;
            while (pos < input.length() && std::isdigit(input[pos])) {
                num = num * 10 + (input[pos] - '0');
                pos++;
            }
            return {NUMBER, num};
        } else if (current == '+') {
            pos++;
            return {PLUS, 0};
        } else if (current == '-') {
            pos++;
            return {MINUS, 0};
        } else if (current == '*') {
            pos++;
            return {MULTIPLY, 0};
        } else if (current == '/') {
            pos++;
            return {DIVIDE, 0};
        } else if (current == '(') {
            pos++;
            return {LPAREN, 0};
        } else if (current == ')') {
            pos++;
            return {RPAREN, 0};
        } else {
            throw std::runtime_error("Invalid character: " + std::string(1, current));
        }
    }
};

class Parser {
private:
    Lexer& lexer;
    Token currentToken;

    void eat(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        } else {
            throw std::runtime_error("Syntax error: expected " + std::to_string(type));
        }
    }

    int factor() {
        Token token = currentToken;
        if (token.type == NUMBER) {
            eat(NUMBER);
            return token.value;
        } else if (token.type == LPAREN) {
            eat(LPAREN);
            int result = expr();
            eat(RPAREN);
            return result;
        } else {
            throw std::runtime_error("Syntax error: invalid factor");
        }
    }

    int term() {
        int result = factor();
        while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
            Token token = currentToken;
            if (token.type == MULTIPLY) {
                eat(MULTIPLY);
                result *= factor();
            } else if (token.type == DIVIDE) {
                eat(DIVIDE);
                int divisor = factor();
                if (divisor == 0) throw std::runtime_error("Division by zero");
                result /= divisor;
            }
        }
        return result;
    }

    int expr() {
        int result = term();
        while (currentToken.type == PLUS || currentToken.type == MINUS) {
            Token token = currentToken;
            if (token.type == PLUS) {
                eat(PLUS);
                result += term();
            } else if (token.type == MINUS) {
                eat(MINUS);
                result -= term();
            }
        }
        return result;
    }

public:
    Parser(Lexer& lex) : lexer(lex), currentToken(lexer.getNextToken()) {}

    int parse() {
        int result = expr();
        if (currentToken.type != END) {
            throw std::runtime_error("Syntax error: unexpected token");
        }
        return result;
    }
};

int main() {
    std::string input;
    std::cout << "Enter an arithmetic expression: ";
    std::getline(std::cin, input);

    try {
        Lexer lexer(input);
        Parser parser(lexer);
        int result = parser.parse();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
