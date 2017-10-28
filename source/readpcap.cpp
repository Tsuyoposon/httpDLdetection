#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "header/DLdetection.h"

bool func(const u_char* p, int l){
  // std::string bit_buf("");
  // char char_buf, buf;
  bitset<512> bit_buf(0);
  bitset<512> char_buf, buf;
  pgen_unknown u(p,l);
  u.summary();

  for(int i=1;i<=64;i++){
    if(i<=l){
      char_buf = p[l-i];
      //buf
      bit_buf <<= 8;
      bit_buf = bit_buf | char_buf;
    } else {
      char_buf = 0x00;
      bit_buf <<= 8;
      bit_buf = bit_buf | char_buf;
    }
  }
  std::cout << "  value:" << bit_buf << std::endl;
  //bitset<512> bs(bit_buf);
  //std::cout << "  value2:" << bs << std::endl;
  u.send(w);
  return true;
}
