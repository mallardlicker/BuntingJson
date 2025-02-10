#include <iostream>

#include <CppUtils/CppUtils.h>
using namespace CppUtils;

#include "Lexer.h"
#include "Json.h"
using namespace Bunting;

int main() {
	
	system("clear");
	
	std::cout << "\tHello JsonParser!\n" << std::endl;
	
	for (std::filesystem::path& file : File::getFilesInDir("tests/step4")) {
		
		std::cout << "File: " << file.string() << std::endl;
		
		std::ifstream input(file);
		
		if (true) {// test Lexer
			// Lexer tester
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
				
				std::cout << "(" << std::setw(2) << i++ << ") t: " << std::setw(15) << std::left << lex.tokenTypeStr(result.type) << " v: " << result.value << std::right;
				
				std::cout << std::endl;
			}
			
			std::cout << std::endl;
			input.clear();
			input.seekg(0, std::ios::beg);
		}
		
		if (true) { // test Parser
			try {
				Json j(input);
				
				std::cout << "Valid json:\n" << *(j.getData()) << std::endl;
			}
			catch (const std::runtime_error& e) {
				
				std::cout << "Encountered error: " << e.what() << std::endl << std::endl;
			}
		}
		
		std::cout << std::endl;
	}
	
	return 0;
}