#ifndef BUNTING_JSON_LEXER
#define BUNTING_JSON_LEXER

#include <string>
#include <iostream>
#include <stdexcept>

namespace Bunting {
	
	enum class TokenType {
		
		BEGIN_OBJECT, END_OBJECT, BEGIN_ARRAY, END_ARRAY,
		NAME_SEPARATOR, VALUE_SEPARATOR,
		STRING, NUMBER, BOOLEAN, NULL_VALUE,
		END_OF_FILE,
		ERROR
	};
	
	struct Token {
		TokenType type;
		std::string value;
	};
	
	class Lexer {
		public:
			// Lexer() { }
			Lexer(std::istream& input) : input(input) { nextChar(); }
			
			Token getNextToken();
			bool hasNextToken();
			
			std::string tokenTypeStr(TokenType tt);
			
		private:
			void skipWhitespace();
			Token parseString();
			Token parseNumber();
			Token parseLiteral();
			void nextChar();
			
			std::string errorText(std::string str, bool skip = true);
			
			std::istream& input;
			int currentChar;
			int currentLine = 1;
	};
}

#endif