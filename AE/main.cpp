#include <iostream>
#include <random>

#include <tao/pegtl.hpp>

#include "AE/Grammar.hpp"
#include "AE/Action.hpp"

void help() {
	std::cout << "AE is a language with expression as given below:\n";
    std::cout << "e ::= n\n";
    std::cout << "    | {+ e e}\n";
    std::cout << "    | {- e e}\n";
    std::cout << "where\n'n' is a 32-bit integer and\n'e' is an expression.\n\n";
    std::cout << "One can get a result of an expression by typing in an expression in a single line.\n";
    std::cout << "Also, by typing in \"debug\" in the very first of the line,\n";
    std::cout << "the program will show the full progress using separated operational semantics.\n\n";
	std::cout << "The \"sample\" command will give out an arbitrary expression.\nYou may add \"debug\" at the front.\n";
    std::cout << "For more information, visit https://github.com/qawbecrdtey/LanguageExecutor \n\n";	
}

void sample( std::random_device &rd, std::mt19937_64 &mt, std::uniform_int_distribution<> &dist, std::string &in, std::size_t depth) {
	if(dist(mt) < -20 || depth > 5) {
		in = std::to_string(dist(mt));
		return;
	}
	in = "{";
	in.append(dist(mt) < 0 ? "- " : "+ ");
	std::string first, second;
	sample(rd, mt, dist, first, depth + 1); sample(rd, mt, dist, second, depth + 1);
	in.append(first + " " + second + "}");
}

int main() {
	using value_stack = AE::value_stack;

	std::cout << "This is the implementation of AE in class CS320 at KAIST.\nType in \"help\" for more info!\n\n";

	std::random_device rd;
	std::mt19937_64 mt(rd());

	std::uniform_int_distribution<> dist(-100, 100);

	while(true) {
		try {
			value_stack vs;
			std::string input;
			
			std::cout << "> ";
			std::getline(std::cin, input);

			if(input == "help") {
				help();
				continue;
			}

			tao::pegtl::memory_input check_debug_in(input, "");
			std::string tmp;
			bool flag = false;
			tao::pegtl::parse<AE::check_debug_grammar, AE::check_debug_action>(check_debug_in, tmp, flag);
			std::cout << "debug mode : " << (flag ? "true" : "false") << std::endl;

			if(tmp == "sample") {
				sample(rd, mt, dist, tmp, 0);
			}

			std::cout << "Input is : " << tmp << std::endl;
			tao::pegtl::memory_input in(tmp, "");
			tao::pegtl::parse<AE::grammar, AE::main_action>(in, vs, flag);

			if(vs.size() != 1) {
				throw tao::pegtl::parse_error("Unknown error", in);
			}
			std::cout << "Result = " << vs.top() << std::endl;
		}
		catch(tao::pegtl::parse_error &e) {
			for(auto pos : e.positions) {
				std::cout << "Error at position " << pos.byte_in_line << ": The input does not match the grammar!" << std::endl;
			}
		}
		catch(std::exception &) {
			std::cout << "Unknown error! Please give your input to the developer." << std::endl;
		}
	}
}