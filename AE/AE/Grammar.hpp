#ifndef AE_Grammar_hpp
#define AE_Grammar_hpp

#include <tao/pegtl.hpp>

namespace AE {
    namespace {
    /// misc
        struct digit : tao::pegtl::digit {};
        struct digits : tao::pegtl::plus<digit> {};
        struct space : tao::pegtl::space {};
        struct spaces : tao::pegtl::plus<space> {};
        struct space_s : tao::pegtl::star<space> {};

    /// brackets
        struct open_curly : tao::pegtl::one<'{'> {};
        struct close_curly : tao::pegtl::one<'}'> {};

    /// symbol
        struct plus_sym : tao::pegtl::one<'+'> {};
        struct minus_sym : tao::pegtl::one<'-'> {};

    /// keyword
        struct debug_key : tao::pegtl::string<'d', 'e', 'b', 'u', 'g'> {};

    /// sign
        struct sign : tao::pegtl::one<'+', '-'> {};

    /// integer
        struct integer : tao::pegtl::seq<
            tao::pegtl::opt<sign>,
            digits
        > {};

    /// grammar
        struct expression;

        /// binary operator
        template<typename sym>
        struct binary_operator : tao::pegtl::seq<
            open_curly,
                space_s,
                sym,
                space_s,
                expression,
                space_s,
                expression,
                space_s,
            close_curly
        > {};

        struct add : binary_operator<plus_sym> {};
        struct subtract : binary_operator<minus_sym> {};

        struct expression : tao::pegtl::must<
            tao::pegtl::sor<
                integer,
                add,
                subtract
            >
        > {};
    }
    struct grammar : tao::pegtl::must<space_s, expression, space_s, tao::pegtl::eolf> {};

    struct check_debug_grammar : tao::pegtl::must<
        tao::pegtl::opt<
            tao::pegtl::star<space>,
            debug_key
        >,
        tao::pegtl::star<space>,
        tao::pegtl::star<tao::pegtl::any>,
        tao::pegtl::eolf
    > {};
}

#endif