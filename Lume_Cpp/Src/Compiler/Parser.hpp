#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <variant>
#include <vector>
#include "./astNodes.hpp"

using namespace std;

using MyType = std::variant<astNode, vector<astNode>>;
typedef vector<MyType> branch;

#endif