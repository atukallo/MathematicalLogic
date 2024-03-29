//
// Created by Aleksandr Tukallo on 03.10.16.
//

#include "parser.h"

//public:

std::shared_ptr<parser::node> parser::build_AST_from_postfix()
{
    //here postfix string using struct postfix_string_builder is built, then AST using postfix string is built.
    //  As a result, string is being read twice, though it can be read once.

    std::string postfix_notation;

    {
        postfix_string_builder builder(this->expression, postfix_notation);
        builder.build_postfix_notation_string();
    }

    size_t cur_ind = 0;
    std::stack<std::shared_ptr<node>> var_nodes_stack;

    while (cur_ind < postfix_notation.size())
    {
        token_types token = get_next_token(postfix_notation, cur_ind);
        if (token == var)
        {
            var_nodes_stack.push(std::make_shared<node>(get_var_name(postfix_notation, cur_ind)));
        } else if (token == neg)
        {
            std::shared_ptr<node> new_node = std::make_shared<node>(token, var_nodes_stack.top(), (std::shared_ptr<node>)NULL); //cast is made, because sh_ptr is not primitive ptr
            var_nodes_stack.pop();
            var_nodes_stack.push(new_node);
        } else
        {
            std::shared_ptr<node> right_son;
            right_son = var_nodes_stack.top();
            var_nodes_stack.pop();

            std::shared_ptr<node> new_node = std::make_shared<node>(token, var_nodes_stack.top(), right_son);
            var_nodes_stack.pop();
            var_nodes_stack.push(new_node);
        }
    }
    return var_nodes_stack.top();
}

//private:

token_types parser::get_next_token(std::string& str, size_t& ind)
{
    skip_whitespace(str, ind);

    if (is_end_of_string(str, ind))
        throw parser_exception("expected token, but not found");

    if (str[ind] == '&')
    {
        ind++;
        return conj;
    }
    if (str[ind] == '|')
    {
        ind++;
        return disj;
    }
    if (str[ind] == '!')
    {
        ind++;
        return neg;
    }
    if (ind + 1 < str.size() && str[ind] == '-' && str[ind + 1] == '>')
    {
        ind += 2;
        return impl;
    }
    if (str[ind] >= 'A' && str[ind] <= 'Z')
    {
        return var;
    }

    throw parser_exception("unexpected token");
}

void parser::skip_whitespace(std::string& str, size_t& ind)
{
    while (str[ind] == ' ')
        ind++;
    return;
}

bool parser::is_end_of_string(std::string& str, size_t& ind)
{
    skip_whitespace(str, ind);
    return str.size() == ind;
}

//pre: function is called, when str[ind] = 'A'...'Z'
std::string parser::get_var_name(std::string& str, size_t& ind)
{
    std::string name;
    name.push_back(str[ind]);
    ind++;

    while ((str[ind] >= '0' && str[ind] <= '9')
           || (str[ind] >= 'A' && str[ind] <= 'Z'))
    {
        name.push_back(str[ind]);
        ind++;
    }
    return name;
}