#ifndef BUNTING_JSON
#define BUNTING_JSON

#include "Lexer.h"
#include "Lexer.h"

#include <map>
#include <string>
#include <variant>
#include <vector>
#include <iostream>
#include <memory>

namespace Bunting {
	
	struct JsonValue;
	
	using JsonValueType = std::variant<
		std::nullptr_t, 
		bool, 
		double, 
		std::string, 
		std::vector<std::unique_ptr<JsonValue>>, 
		std::map<std::string, std::unique_ptr<JsonValue>>
	>;
	
	struct JsonValue {
		JsonValueType value;
		
		JsonValue() : value(nullptr) { }
		explicit JsonValue(JsonValueType v) : value(std::move(v)) { }
		
		friend std::ostream& operator<<(std::ostream& os, const JsonValue& j);
		// friend const auto& get(const JsonValue& j);
	};
	
	class Json : protected Lexer {
		public:
			// todo: {} constructor, destructor?
			Json(std::istream& input) : Lexer(input) { jsonObject = init(); }
			
			int size();
			bool empty();
			bool contains(const std::string& key);
			
			// Json& operator=(const Json& other);
			
			// auto& operator[](std::string key);
			// auto& operator[](int index);
			
			std::unique_ptr<JsonValue>& getData() { return jsonObject; }
			
		private:
			std::unique_ptr<JsonValue> init();
			std::unique_ptr<JsonValue> parseValue();
			std::unique_ptr<JsonValue> parseObject();
			std::unique_ptr<JsonValue> parseArray();
			std::unique_ptr<JsonValue> parseNumber();
			
			Token consume(TokenType expectedType); // consumes current token (advances it) and returns previous token
			bool match(TokenType expectedType); // consumes token and returns true if type is expected
			Token currentToken;
			
			std::unique_ptr<JsonValue> jsonObject;
	};
}

#endif