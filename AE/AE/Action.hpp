#ifndef AE_Action_hpp
#define AE_Action_hpp

#include <iostream>
#include <stack>

#include <tao/pegtl.hpp>

#include "Grammar.hpp"

namespace AE {
    using value_stack = std::stack<int>;

    template<typename Rule>
    struct main_action : tao::pegtl::nothing<Rule> {};

    template<>
    struct main_action<integer> {
        template<typename Input>
        static void apply(Input const &in, value_stack &vs, bool debug) {
            vs.push(std::stoi(in.string()));
        }
    };

    template<>
    struct main_action<add> {
        template<typename Input>
        static void apply(Input const &in, value_stack &vs, bool debug) {
            auto const val2 = vs.top(); vs.pop();
            auto const val1 = vs.top(); vs.pop();
            vs.push(val1 + val2);
        }
    };

    template<>
    struct main_action<subtract> {
        template<typename Input>
        static void apply(Input const &in, value_stack &vs, bool debug) {
            auto const val2 = vs.top(); vs.pop();
            auto const val1 = vs.top(); vs.pop();
            vs.push(val1 - val2);
        }
    };

    template<>
    struct main_action<expression> {
        template<typename Input>
        static void apply(Input const &in, value_stack &vs, bool debug) {
            if(debug) {
                std::cout << "|- " << in.string() << " => " << vs.top() << std::endl;
            }
        }
    };


    template<typename Rule>
    struct check_debug_action : tao::pegtl::nothing<Rule> {};

    template<>
    struct check_debug_action<debug_key> {
        static void apply0(std::string &s, bool &debug) {
            debug = true;
        }
    };

    template<>
    struct check_debug_action<tao::pegtl::star<tao::pegtl::any>> {
        template<typename Input>
        static void apply(Input const &in, std::string &s, bool &debug) {
            s = in.string();
        }
    };
}

#endif