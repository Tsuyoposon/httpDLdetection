#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
using namespace std;

std::string create_rand_str();
void create_datafile();
void create_test_data();
std::string digitString(int num, int digit);

int main() {

    create_datafile();
    create_test_data();

    for(int i=1;i <= 1000; i++){

      fstream fs;
      fs.open("source3/test_data_files/" + std::to_string(i)+".bin", ios::in | ios::binary);
      if(! fs.is_open()) {
        return 0;
      }
      std::bitset<2048> bs;
      fs.seekg(-256, ios_base::end);
      fs.read((char*)&bs, sizeof bs);

      std::cout << "rand_data:(" << i << ")" << bs << std::endl;
      fs.close();
    }

    return 0;
  }
// 乱数ビットデータ生成
std::string create_rand_str(){
  std::string str(256, '1');
  std::bitset<8> bs_char;
  for(int i=0;i<256;i++){
    str[i] = rand() % 255 + 1;
    // bs_char(static_cast<u_int>(str[i]))
    // std::cout << "rand_char:" << static_cast<int>(str[i]) << std::endl;
  }
  // std::cout << "rand_data:" << str << std::endl;

  return str;
}

// 固定ビットデータ生成
void create_datafile(){

  for(int i=1;i <= 100; i++){

  fstream fs;
  fs.open("source3/fix_data/" + std::to_string(i)+".bin", ios::out | ios::binary);
  if(! fs.is_open()) {
      return;
  }

  std::string damy_data("aaaaaaaaaabbbbbbbbbbcccccccccc");
  fs.write((const char*)&damy_data, sizeof damy_data);

  std::bitset<512> bs;
  bs.set(i-1);
  fs.write((const char*)&bs, sizeof bs);


  fs.close();

  }
}

void create_test_data(){

  for(int i=1;i <= 999; i++){

  fstream fs;
  fs.open("source3/test_data_files/" + std::to_string(i)+".bin", ios::out);
  if(! fs.is_open()) {
      return;
  }
  std::string buf_str = create_rand_str();
  fs.write((const char*)&buf_str[0], 256);;
  fs.close();

  }
}

std::string digitString(int num, int digit) {
  char c[32];
  sprintf(c, "%d", num);
  std::string s(c);

  if(s.length() > digit) {
    s = s.substr(s.length()-digit);
  } else if(s.length() < digit){
    while (s.length() < digit) {
      s = "0" + s;
    }
  }
  return s;
}
