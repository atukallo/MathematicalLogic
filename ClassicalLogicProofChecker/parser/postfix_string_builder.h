//
// Created by Aleksandr Tukallo on 14.10.16.
//

#ifndef PROOFCHECKER_POSTFIX_STRING_BUILDER_H
#define PROOFCHECKER_POSTFIX_STRING_BUILDER_H

#include "parser_exception.h"
#include <stack>
#include <map>
#include <cstdint>

enum token_types
{
    open_parenthesis,
    close_parenthesis,
    conj,
    disj,
    impl,
    neg,
    var,
};

struct postfix_string_builder
{
    std::string const& expression;
    std::string& output_string;

    postfix_string_builder(std::string const& expression, std::string& output) : expression(expression),
                                                                                 output_string(output), current_index(0)
    {}

    void build_postfix_notation_string();

private:

    // Our grammar:
    //
    //  expr:   E --> F (B F)*
    //  factor: F --> "(" E ")" | U F | v[variable]
    //  unary:  U --> "!"
    //  binary: B --> "&" | "|" | "->"

    std::stack<token_types> operand_stack;
    size_t current_index;

    void expr_postfx();
    void factor_postfx();
    void expr_inside_parenthesis_postfx();
    void binary_postfx();

    token_types get_next_token(bool move_index);
    bool is_end_of_string();
    void skip_whitespace();
    std::string get_var_name();

    bool is_preced_stack_head_bigger(token_types);
    void concat_output_with_argument(token_types);

};

#endif //PROOFCHECKER_POSTFIX_STRING_BUILDER_H
