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
			
			// std::istream& getInput();
			// void setInput(std::istream& i);
			
			std::string tokenTypeStr(TokenType tt);
			
		private:
			void skipWhitespace();
			Token parseString();
			Token parseNumber();
			Token parseLiteral();
			void nextChar();
			// char peekChar();
			
			// format -> Lexer: str (line: 0, 'a')
			std::string errorText(std::string str, bool skip = true);
			
			std::istream& input;
			int currentChar;
			int currentLine = 1;
	};
}

#endif


// any json key can be searched for as a dict (eg j.contains("taco") or j["taco"])
// any json key -> value pair is a single json "object"
// if the value is an array, then number indexing is allowed (eg .size() and j["taco"][4] <- if "taco"'s value is an array')
// if the value is another json object, then that key can be bypassed/succeded by (jInner = j["taco"])
// any json key -> value pair should be able to be initialized using braces (eg {"key", value})

// [] allows number indexing or key indexing
// {} only allows key indexing