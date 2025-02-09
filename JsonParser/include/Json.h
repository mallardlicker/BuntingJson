#ifndef BUNTING_JSON
#define BUNTING_JSON

#include "Lexer.h"
#include "Lexer.h"

#include <map>
#include <string>
#include <variant>
#include <iostream>

namespace Bunting {
	
	class Json : private Lexer {
		public:
			// todo: constructor, destructor
			// Json();
			Json(std::istream& input) : input(input) { init(); }
			// constructor will generate entire datastructure recursively with Lexer
			
			// bool contains(std::string key); // determine whether current json object has a certain key
			
			// auto& operator[](std::string key); // returns Json objects or value types
			// Json& operator[](int index); // will only return Json objects from arrays
			
		private:
			void init();
			std::variant<Json, int, double, long, bool> parseValue();
			Json parseObject();
			Json parseArray();
			
			Token consume(TokenType expectedType); // consumes current token (advances it) and returns previous token
			
			Token currentToken;
			std::map<
				std::string, 
				std::variant<Json, int, double, long, bool> // can be another Json object or any of the underlying datatypes
					> storage;
	};
}

#endif