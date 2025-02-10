#include "Lexer.h"

#include <sstream>

namespace Bunting {
	
	// std::istream& getInput() {
		
	// 	return input;
	// }
	
	// void setInput(std::istream& i) {
		
	// 	input = i;
	// }
	
	Token Lexer::getNextToken() {
		skipWhitespace();
		if (input.eof()) return {TokenType::END_OF_FILE, ""};
		
		switch (currentChar) {
			
			case '{': nextChar(); return {TokenType::BEGIN_OBJECT, "{"};
			case '}': nextChar(); return {TokenType::END_OBJECT, "}"};
			case '[': nextChar(); return {TokenType::BEGIN_ARRAY, "["};
			case ']': nextChar(); return {TokenType::END_ARRAY, "]"};
			case ':': nextChar(); return {TokenType::NAME_SEPARATOR, ":"};
			case ',': nextChar(); return {TokenType::VALUE_SEPARATOR, ","};
			case '"': return parseString();
			case 't': case 'f': case 'n': return parseLiteral();
			case '-': case '0': case '1': case '2': case '3': case '4': 
			case '5': case '6': case '7': case '8': case '9': return parseNumber();
			default:  return {TokenType::ERROR, errorText("unexpected character")};
		}
	}
	
	void Lexer::skipWhitespace() {
		
		while (!input.eof() && isspace(currentChar)) {
			
			if (currentChar == '\n') {
				currentLine++;
			}
			
			nextChar();
		}
	}
	
	Token Lexer::parseString() {
		
		std::string value; 
		nextChar(); // Skip opening '"'
		while (!input.eof() && currentChar != '"') {
			
			// parse escaped characters
			if (currentChar == '\\') {
				
				nextChar();
				switch (currentChar) {
					case '\\': value += '\\'; break;
					case '/': value += '/'; break;
					case '"': value += '"'; break;
					case 'n': value += '\n'; break;
					case 't': value += '\t'; break;
					case 'r': value += '\r'; break;
					case 'b': value += '\b'; break;
					case 'f': value += '\f'; break;
					case 'u': 
						
						// std::string unicodeVal = "";
						// for (int i = 0; i < 4; i++) {
						// 	nextChar();
						// 	if (!input.eof() && isxdigit(currentChar)) {
								
						// 		unicodeVal += currentChar;
						// 	}
						// 	else {
								
						// 		throw std::runtime_error("Invalid unicode character");
						// 	}
						// }
						
						// value += static_cast<wchar_t>(static_cast<int>(unicodeVal));
						
						break;
					default:
						return {TokenType::ERROR, errorText("invalid escape sequence")};
				}
			}
			else {
				
				value += currentChar;
			}
			
			nextChar();
		}
		
		if (currentChar == '"') {
			nextChar();
			return {TokenType::STRING, value};
		}
		else {
			
			return {TokenType::ERROR, errorText("unterminated string")};
		}
	}
	
	Token Lexer::parseNumber() {
		
		std::string value;
		while (!input.eof() && (isdigit(currentChar) || currentChar == '.' || currentChar == '-')) {
			value += currentChar;
			nextChar();
		}
		return {TokenType::NUMBER, value};
	}
	
	Token Lexer::parseLiteral() {
		
		std::string value;
		std::string terms[] = {"false", "true", "null"};
		int index = currentChar == 'f' ? 0 :
					currentChar == 't' ? 1 :
					currentChar == 'n' ? 2 : 3;
		
		for (int i = 0; i < terms[index].length(); i++) {
			
			if (input.eof() || currentChar != terms[index][i]) {
				
				return {TokenType::ERROR, errorText("failed to parse literal")};
			}
			else {
				
				nextChar();
			}
		}
		
		if (index == 2) {
			return {TokenType::NULL_VALUE, terms[index]};
		}
		
		return {TokenType::BOOLEAN, terms[index]};
	}
	
	void Lexer::nextChar() {
		
		currentChar = input.get();
	}
	
	// char Lexer::peekChar() {
		
	// 	return input.peek();
	// }
	
	std::string Lexer::tokenTypeStr(TokenType tt) {
		
		switch (tt) {
			case TokenType::BEGIN_OBJECT:		return "BEGIN_OBJECT";
			case TokenType::END_OBJECT:			return "END_OBJECT";
			case TokenType::BEGIN_ARRAY:		return "BEGIN_ARRAY";
			case TokenType::END_ARRAY:			return "END_ARRAY";
			case TokenType::NAME_SEPARATOR:		return "NAME_SEPARATOR";
			case TokenType::VALUE_SEPARATOR:	return "VALUE_SEPARATOR";
			case TokenType::STRING:				return "STRING";
			case TokenType::NUMBER:				return "NUMBER";
			case TokenType::BOOLEAN:			return "BOOLEAN";
			case TokenType::NULL_VALUE:			return "NULL_VALUE";
			case TokenType::END_OF_FILE:		return "END_OF_FILE";
			case TokenType::ERROR:				return "ERROR";
		}
	}
	
	std::string Lexer::errorText(std::string str, bool skip) {
		
		std::stringstream errText; 
		errText << "Lexer: " << str << " (line: " << currentLine << ", '" <<
			static_cast<char>(currentChar) << "')";
		
		if (skip) {
			
			nextChar();
		}
		
		return errText.str();
	}
}