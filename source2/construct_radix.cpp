#include <string>
#include <iostream>
#include <cstdlib>
#include <bitset>
#include <sstream>

#include "radix_tree.hpp"

radix_tree<std::string, int> tree;

void insert() {

  tree["affair"]    = 3;
  tree["afford"]    = 1;
  tree["apache"]    = 0;
  tree["available"] = 2;
  tree["avenger"]   = 4;
  tree["binary"]    = 5;
  tree["bind"]      = 6;
  tree["blind"]     = 9;
  tree["brace"]     = 8;
  tree["bro"]       = 10;
  tree["brother"]   = 7;
}

void longest_match(std::string key)
{
    radix_tree<std::string, int>::iterator it;

    it = tree.longest_match(key);

    std::cout << "longest_match(\"" << key << "\"):" << std::endl;

    if (it != tree.end()) {
        std::cout << "    " << it->first << ", " << it->second << std::endl;
    } else {
        std::cout << "    failed" << std::endl;
    }
}

void prefix_match(std::string key)
{
    std::vector<radix_tree<std::string, int>::iterator> vec;
    std::vector<radix_tree<std::string, int>::iterator>::iterator it;

    tree.prefix_match(key, vec);

    std::cout << "prefix_match(\"" << key << "\"):" << std::endl;

    for (it = vec.begin(); it != vec.end(); ++it) {
        std::cout << "    " << (*it)->first << ", " << (*it)->second << std::endl;
    }
}

void greedy_match(std::string key)
{
    std::vector<radix_tree<std::string, int>::iterator> vec;
    std::vector<radix_tree<std::string, int>::iterator>::iterator it;

    tree.greedy_match(key, vec);

    std::cout << "greedy_match(\"" << key << "\"):" << std::endl;

    for (it = vec.begin(); it != vec.end(); ++it) {
        std::cout << "    " << (*it)->first << ", " << (*it)->second << std::endl;
    }
}

void traverse() {
    radix_tree<std::string, int>::iterator it;

    std::cout << "traverse:" << std::endl;
    for (it = tree.begin(); it != tree.end(); ++it) {
        std::cout << "    " << it->first << ", " << it->second << std::endl;
    }
}

int main()
{
    insert();
    std::string str = "a";

    longest_match("binder");
    longest_match("bracelet");
    longest_match("apple");



    traverse();


    return EXIT_SUCCESS;
}
