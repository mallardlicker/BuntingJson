#include "Json.h"

#include <iomanip>
#include <stdexcept>

namespace Bunting {
	
	// ===================================================================================
	//	JsonValue
	// ===================================================================================
	
	std::ostream& operator<<(std::ostream& os, const JsonValue& j) {
		
		// use std::visit to apply actions based on std::variant's type
		std::visit([&os](const auto& val) {
			using T = std::decay_t<decltype(val)>;
			
			if constexpr (std::is_same_v<T, std::nullptr_t>) {
				os << "null";
			}
			else if constexpr (std::is_same_v<T, bool>) {
				os << (val ? "true" : "false");
			}
			else if constexpr (std::is_same_v<T, double>) {
				os << val;
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				os << std::quoted(val);
			}
			else if constexpr (std::is_same_v<T, std::vector<std::unique_ptr<JsonValue>>>) {
				
				os << "[\n";
				bool first = true;
				for (const auto& item : val) {
					
					if (!first) os << ",\n";
					os << *item;
					first = false;
				}
				os << "\n]";
			}
			else if constexpr (std::is_same_v<T, std::map<std::string, std::unique_ptr<JsonValue>>>) {
				
				os << "{\n";
				bool first = true;
				for (const auto& [key, value] : val) {
					
					if (!first) os << ",\n";
					os << std::quoted(key) << ": " << *value;
					first = false;
				}
				os << "\n}";
			}
			
		}, j.value);
		
		return os;
	}
	
	// ===================================================================================
	//	Json
	// ===================================================================================
	
	std::unique_ptr<JsonValue> Json::init() {
		
		currentToken = getNextToken();

		std::unique_ptr<JsonValue> res = parseValue();
		
		consume(TokenType::END_OF_FILE);
		
		return res;
	}
	
	std::unique_ptr<JsonValue> Json::parseValue() {
		
		switch (currentToken.type) {
			case TokenType::BEGIN_OBJECT: 	return parseObject();
			case TokenType::BEGIN_ARRAY: 	return parseArray();
			case TokenType::STRING:			return std::make_unique<JsonValue>(consume(TokenType::STRING).value);
			case TokenType::NUMBER:			return parseNumber();
			case TokenType::BOOLEAN:		return std::make_unique<JsonValue>(consume(TokenType::BOOLEAN).value == "true");
			case TokenType::NULL_VALUE:		consume(TokenType::NULL_VALUE); return std::make_unique<JsonValue>(nullptr);
			case TokenType::ERROR:			throw std::runtime_error(consume(TokenType::ERROR).value);
			case TokenType::END_OF_FILE:	throw std::runtime_error("Parser: unexpected file termination.");
			default: throw std::runtime_error("Parser: unexpected token type for current sequence.");
		}
	}
	
	std::unique_ptr<JsonValue> Json::parseObject() {
		
		consume(TokenType::BEGIN_OBJECT);
		
		std::map<std::string, std::unique_ptr<JsonValue>> obj;
		
		if (!match(TokenType::END_OBJECT)) {
			while (true) {
				
				std::string key = consume(TokenType::STRING).value; // "key"
				consume(TokenType::NAME_SEPARATOR); // :
				
				obj[key] = parseValue(); // value
				
				if (!match(TokenType::VALUE_SEPARATOR)) { // ,
					
					consume(TokenType::END_OBJECT); 
					break; 
				}
			}
		}
		
		return std::make_unique<JsonValue>(std::move(obj));
	}
	
	std::unique_ptr<JsonValue> Json::parseArray() {
		
		consume(TokenType::BEGIN_ARRAY);
		
		std::vector<std::unique_ptr<JsonValue>> arr;
		
		if (!match(TokenType::END_ARRAY)) {
			while (true) {
				
				arr.push_back(parseValue());
				
				if (!match(TokenType::VALUE_SEPARATOR)) { // ,
					
					consume(TokenType::END_ARRAY);
					break; 
				}
			}
		}
		
		return std::make_unique<JsonValue>(std::move(arr));
	}
	
	std::unique_ptr<JsonValue> Json::parseNumber() {
		
		// todo: fix numbers
		
		std::string numStr = consume(TokenType::NUMBER).value;
		// auto res;
		
		// // floating point first
		// if (numStr.find('.') != std::string::npos) {
			
		// 	long double val = std::stold(numStr);
			
		// 	res = std::stof(numStr);
		// 	if (res == val) {
				
		// 		return res;
		// 	}
			
		// 	res = std::stod(numStr);
		// 	if (res == val) {
				
		// 		return res;
		// 	}
			
		// 	return val;
		// }
		
		// long val = std::stol(numStr);
		
		// res = std::stoi(numStr);
		// if (res == val) {
			
		// 	return res;
		// }
		
		// return val;
		
		return std::make_unique<JsonValue>(std::stod(numStr));
	}
	
	Token Json::consume(TokenType expectedType) {
		
		if (currentToken.type == expectedType) {
			
			Token t = currentToken;
			currentToken = getNextToken(); // advance
			return t; // but return the consumed token
		}
		else {
			
			std::string msg = "";
			
			if (currentToken.type == TokenType::ERROR) {
				msg = currentToken.value.c_str();
			}
			else {
				msg = msg + "Parser: expected '" + tokenTypeStr(expectedType).c_str() + "', but got '" + 
						tokenTypeStr(currentToken.type).c_str() + "' with value '" + currentToken.value.c_str() + "'.";
			}
			
			throw std::runtime_error(msg);
		}
	}
	
	bool Json::match(TokenType expectedType) {
		
		if (currentToken.type == expectedType) {
			
			consume(expectedType);
			return true;
		}
		
		return false;
	}
}