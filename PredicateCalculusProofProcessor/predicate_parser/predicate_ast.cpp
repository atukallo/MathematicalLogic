//
// Created by Aleksandr Tukallo on 17.02.17.
//

#include <assert.h>
#include "predicate_ast.h"

std::map<token_types, uint8_t> precedence{
        {EQUALITY,       0},

        {FOR_ALL,        1},
        {EXISTS,         1},

        {ZERO,           3},
        {STROKE,         4},
        {MULTIPLICATION, 5},
        {SUM,            6},

        {NEGATION,       8},
        {CONJUNCTION,    9},
        {DISJUNCTION,    10},
        {IMPLICATION,    11},
        {ARGUMENTS,      12},
};

std::map<token_types, std::string> string_representation{
        {EQUALITY,       "="},

        {FOR_ALL,        "@"},
        {EXISTS,         "?"},

        {ZERO,           "0"},
        {STROKE,         "'"},
        {MULTIPLICATION, "*"},
        {SUM,            "+"},

        {NEGATION,       "!"},
        {CONJUNCTION,    "&"},
        {DISJUNCTION,    "|"},
        {IMPLICATION,    "->"},
        {ARGUMENTS,      "args"},
};

predicate_ast::node::node(node_ptr l, node_ptr r, token_types tt) : left(l), right(r), token_type(tt)
{
    this->str = string_representation.at(tt);
}

bool predicate_ast::is_var_free(std::string const& var)
{
    return is_var_free_rec(var, this->root);
}

//todo mb test this function on expressions file
bool predicate_ast::is_var_free_rec(std::string const& var, std::shared_ptr<node> const& cur_node)
{
    bool free_in_left = false;
    bool free_in_right = false;

    //if this is quantifier and it blocks needed var
    if ((cur_node->token_type == FOR_ALL || cur_node->token_type == EXISTS) && cur_node->left->str == var)
    {
        return false;
    }

    //variables never have children, but function can!!
    if (cur_node->token_type == VARIABLE /*|| cur_node->token_type == FUNCTION*/)
    {
//        assert (cur_node->left == NULL && cur_node->right == NULL); //functions can have children
        return cur_node->str == var;
    }

    if (cur_node->left != NULL)
    {
        free_in_left = is_var_free_rec(var, cur_node->left);
    }
    if (cur_node->right != NULL)
    {
        free_in_right = is_var_free_rec(var, cur_node->right);
    }

    return free_in_left || free_in_right;
}

//function walks the tree and returns vector with the names of all free vars in ast
std::vector<std::string> predicate_ast::get_all_free_vars()
{
    std::set<std::string> set;
    tree_walk(set, this->root);

    std::vector<std::string> ret;
    auto it = set.begin();
    for (; it != set.end(); it++)
    {
        if (this->is_var_free(*it))
        {
            ret.push_back(*it);
        }
    }

    return ret;
}

//functions adds all the variable to the set
void predicate_ast::tree_walk(std::set<std::string>& set, std::shared_ptr<node> const& cur_node)
{
    if (cur_node->token_type == VARIABLE)
    {
        set.insert(cur_node->str);
    }

    if (cur_node->left != NULL)
    {
        tree_walk(set, cur_node->left);
    }

    if (cur_node->right != NULL)
    {
        tree_walk(set, cur_node->right);
    }
}