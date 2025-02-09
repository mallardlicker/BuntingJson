#include "Json.h"

#include <stdexcept>

namespace Bunting {
	
	Json::init() {
		
		advanceToken();
		
		if (currentToken.type == TokenType::BEGIN_OBJECT) {
			
			parseObject();
		}
		else {
			
			throw std::runtime_error("Json init: failed to begin parsing. Expected '{' and recieved '" + t.value.c_str() + "'");
		}
		
		advanceToken();
		if (currentToken.type == TokenType::END_OBJECT) {
			
			advanceToken();
			if (currentToken.type != TokenType::END_OF_FILE) {
				
				throw std::runtime_error("Json init: failed to terminate file");
			}
		}
		else {
			
			throw std::runtime_error("Json init: failed to end parsing. Expected '}' and recieved '" + t.value.c_str() + "'");
		}
	}
	
	std::variant<Json, int, double, long, bool> Json::parseValue() {
		
		switch (currentToken.type) {
			case TokenType::BEGIN_OBJECT: 	return parseObject();
			case TokenType::BEGIN_ARRAY: 	return parseArray();
			case TokenType::STRING: return 0; // 
		}
	}
	
	Json Json::parseObject() {
		
		consume(TokenType::BEGIN_OBJECT);
		
		// use this current json object only for outer braces, otherwise create
		//  searchable object (which already has a key)
		Json obj;
		if (storage.empty()) {
			
		 	obj = this;
		}
		
		while (currentToken.type != TokenType::END_OBJECT) {
			
			std::string key = currentToken.value; // "key"
			consume(TokenType::NAME_SEPARATOR); // :
			
			this.storage[key] = parseValue(); // value
			
			if (currentToken.type != TokenType::END_OBJECT) {
				
				consume(TokenType::VALUE_SEPARATOR); // ,
			}
		}
		
		consume(TokenType::END_OBJECT);
		
		return obj;
	}
	
	Token Json::consume(TokenType expectedType) {
		
		if (currentToken.type == expectedType) {
			
			Token t = currentToken;
			currentToken = getNextToken(); // advance
			return t; // but return the consumed token
		}
		else {
			
			throw std::runtime_error("Expected '" + tokenTypeString(expectedType).c_str() + "', but got '" + 
					tokenTypeString(currentToken.type).c_str() + "' with value '" + currentToken.value + "'.");
		}
	}
}