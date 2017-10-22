#include <string>
#include <iostream>
#include <cstdlib>
#include <bitset>
#include <sstream>

#include "radix_tree.hpp"

radix_tree<std::string, int> tree;

void insert() {

  std::bitset<8> bs;

  bs = 0b10000011;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 1));
  bs = 0b01110101;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 2));
  bs = 0b10101110;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 3));
  bs = 0b01110111;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 4));
  bs = 0b10110101;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 5));
  bs = 0b01011010;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 6));
  bs = 0b01001011;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 7));
  bs = 0b01101001;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 8));
  bs = 0b10101000;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 9));
  bs = 0b11010111;
  tree.insert(std::pair<std::string, int>(bs.to_string(), 10));


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

void indicate_radix()
{
  tree.indicate_radix();
}

void signature_match(std::string key)
{
  std::vector<radix_tree<std::string, int>::iterator> vec;
  std::vector<radix_tree<std::string, int>::iterator>::iterator it;

  tree.signature_match(key, vec);

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
    traverse();
    // indicate_radix();

    signature_match("11111110");

    return EXIT_SUCCESS;
}
