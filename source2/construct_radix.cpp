#include <string>
#include <iostream>
#include <cstdlib>
#include <bitset>
#include <sstream>
#include <fstream>
using namespace std;

#include "radix_tree.hpp"

radix_tree<std::string, int> tree;

//サンプルデータの入力
void insert() {

  std::bitset<16> bs;
  //std::string bs2;

  bs = 0b00000000010000011;
  //bs2 = "000000010000011";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 1));
  bs = 0b00000000001110101;
  //bs2 = "000000001110101";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 2));
  bs = 0b00000000010101110;
  //bs2 = "000000010101110";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 3));
  bs = 0b00000000001110111;
  //bs2 = "000000001110111";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 4));
  bs = 0b00000000010110101;
  //bs2 = "000000010110101";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 5));
  bs = 0b00000000001011010;
  //bs2 = "000000001011010";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 6));
  bs = 0b00000000001001011;
  //bs2 = "000000001001011";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 7));
  bs = 0b00000000001101001;
  //bs2 = "000000001101001";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 8));
  bs = 0b00000000010101000;
  //bs2 = "000000010101000";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 9));
  bs = 0b00000000011010111;
  //bs2 = "000000011010111";
  tree.insert(std::pair<std::string, int>(bs.to_string(), 10));

}

void insert_file_data(){

  for(int i=1;i <= 100; i++){

    ifstream fs;

    fs.open("source3/fix_data/" + std::to_string(i)+ ".bin", ios::in | ios::binary);
    if(! fs.is_open()) {
      return;
    }

    std::bitset<512> bs;
    fs.seekg(64, ios_base::beg);
    fs.read((char*)&bs, sizeof bs);
    tree.insert(std::pair<std::string, int>(bs.to_string(), i));

    fs.close();
  }



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
    //insert();
    insert_file_data();
    traverse();
    //indicate_radix();
    //prefix_match("0000000001");
    //               1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
    signature_match("00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000100100010");

    return EXIT_SUCCESS;
}
