#include <iostream>

#include <CppUtils/CppUtils.h>
using namespace CppUtils;

#include "Lexer.h"
using namespace Bunting;

std::string GetTypeStr(TokenType t);

int main() {
	
	system("clear");
	
	// std::cout << "\tHello JsonParser!" << std::endl;
	
	for (std::filesystem::path& file : File::getFilesInDir("tests/step4")) {
		
		std::cout << "File: " << file.string() << std::endl;
		
		std::ifstream input(file);
		Lexer lex(input);
		
		Token result = {TokenType::NULL_VALUE, ""};
		int i = 0;
		while (result.type != TokenType::END_OF_FILE) {
			
			// try {
				
				result = lex.getNextToken();
			// }
			// catch (const std::runtime_error& e) {
				
			// 	std::cout << "Encountered error: " << e.what() << std::endl;
			// 	break;
			// }
			
			std::cout << "(" << std::setw(2) << i++ << ") t: " << std::setw(15) << std::left << GetTypeStr(result.type) << " v: " << result.value << std::right;
			
			std::cout << std::endl;
		}
		
		std::cout << std::endl;
	}
	
	return 0;
}

std::string GetTypeStr(TokenType t) {
	
	switch (t) {
		case (TokenType::BEGIN_OBJECT): return "begin object";
		case (TokenType::END_OBJECT): return "end object";
		case (TokenType::BEGIN_ARRAY): return "begin array";
		case (TokenType::END_ARRAY): return "end array";
		case (TokenType::NAME_SEPARATOR): return "name separator";
		case (TokenType::VALUE_SEPARATOR): return "value separator";
		case (TokenType::STRING): return "string";
		case (TokenType::NUMBER): return "number";
		case (TokenType::BOOLEAN): return "boolean";
		case (TokenType::NULL_VALUE): return "null value";
		case (TokenType::END_OF_FILE): return "end of file";
		case (TokenType::ERROR): return "error";
		default: return "NO TYPE";
	}
}