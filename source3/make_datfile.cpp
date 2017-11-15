#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

void create_datafile();
void create_test_data();
std::string digitString(int num, int digit);

int main() {

    //create_datafile();
    create_test_data();

    // ifstream fs;
    //
    // fs.open("test_data.txt", ios::in | ios::binary);
    // if(! fs.is_open()) {
    //   return 0;
    // }
    //
    // std::bitset<512> bs;
    // fs.seekg(-64, ios_base::end);
    // fs.read((char*)&bs, sizeof bs);
    //
    // cout << bs << endl;
    //
    // fs.close();

    return 0;
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

  for(int i=1;i <= 1000; i++){

  fstream fs;
  fs.open("source3/test_data_files/" + std::to_string(i)+".bin", ios::out | ios::binary);
  if(! fs.is_open()) {
      return;
  }
  std::string damy_data("aaaaaaaaaabbbbbbbbbbcccccccccc");
  fs.write((const char*)&damy_data, sizeof damy_data);
  std::string set_value("8901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
  set_value = digitString(i, 3)+ set_value;
  cout << set_value << endl;
  fs.write(set_value.c_str(), 256);
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
