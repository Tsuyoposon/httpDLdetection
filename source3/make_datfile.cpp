#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

void create_datafile();

int main() {

    //create_datafile();


    ifstream fs;

    fs.open("test_data.txt", ios::in | ios::binary);
    if(! fs.is_open()) {
      return 0;
    }

    std::bitset<512> bs;
    fs.seekg(-64, ios_base::end);
    fs.read((char*)&bs, sizeof bs);

    cout << bs << endl;

    fs.close();

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
