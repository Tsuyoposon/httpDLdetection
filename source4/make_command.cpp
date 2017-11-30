#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <dirent.h>
using namespace std;

std::string create_rand_str();
void create_test_data();
void read_file_command(std::string file_path);
void read_file_command2(std::string file_path);
std::string digitString(int num, int digit);

int main() {

  create_test_data();

  read_file_command("./source3/test_data_files/");

  // for(int i=1;i <= 1000; i++){
  //
  //   fstream fs;
  //   fs.open("source3/test_data_files/" + std::to_string(i)+".bin", ios::in | ios::binary);
  //   if(! fs.is_open()) {
  //     return 0;
  //   }
  //   std::bitset<2048> bs;
  //   fs.seekg(-256, ios_base::end);
  //   fs.read((char*)&bs, sizeof bs);
  //
  //   std::cout << "rand_data:(" << i << ")" << bs << std::endl;
  //   fs.close();
  // }

    return 0;
  }
// 乱数ビットデータ生成
std::string create_rand_str(){
  std::string str(256, '1');
  std::bitset<8> bs_char;
  for(int i=0;i<256;i++){
    int ran = rand() % 220 + 35;
      if (ran == 92 || ran == 36){
        ran = 90;
      }
    str[i] = ran;

    // bs_char(static_cast<u_int>(str[i]))
    // std::cout << "rand_char:" << static_cast<int>(str[i]) << std::endl;
  }
  // std::cout << "rand_data:" << str << std::endl;

  return str;
}

// 固定ビットデータ生成
void read_file_command(std::string file_path){
  const char* path = file_path.c_str();
  DIR *dp;       // ディレクトリへのポインタ
  dirent* entry; // readdir() で返されるエントリーポイント
  int i=0;
  std::string d_buf;
  std::string all_footers("frame contains \"");

  dp = opendir(path);
  if (dp==NULL) exit(1);
  do {
    entry = readdir(dp);
    if (entry != NULL){
      // std::cout << path << entry->d_name << std::endl;
      d_buf = std::string(path) + std::string(entry->d_name);
      i++;
      if (2<i){
        ifstream fs;
        fs.open(d_buf, ios::in);
        if(! fs.is_open()) {
          return;
        }
        fs.seekg(-16, ios_base::end);
        std::istreambuf_iterator<char> it(fs);
        std::istreambuf_iterator<char> last;
        std::string read_str(it, last);
        std::cout << "file_name:" << d_buf << std::endl;
        std::cout << "rad_str:" << read_str << std::endl;
        all_footers = all_footers + read_str + "\" or frame contains \"";
        fs.close();
      }
    }
  } while (entry != NULL);

  fstream write_wire;
  write_wire.open("source4/command_wireshark2.txt", ios::out);
  if(! write_wire.is_open()) {
      return;
  }

  write_wire.write((const char*)&all_footers[0], all_footers.size());;
  write_wire.close();
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

void read_file_command2(std::string file_path){
  const char* path = file_path.c_str();
  DIR *dp;       // ディレクトリへのポインタ
  dirent* entry; // readdir() で返されるエントリーポイント
  int i=0;
  std::string d_buf;
  std::string all_footers("frame contains \"");

  dp = opendir(path);
  if (dp==NULL) exit(1);
  do {
    entry = readdir(dp);
    if (entry != NULL){
      // std::cout << path << entry->d_name << std::endl;
      d_buf = std::string(path) + std::string(entry->d_name);
      i++;
      if (2<i){
        ifstream fs;
        fs.open(d_buf, ios::in);
        if(! fs.is_open()) {
          return;
        }
        fs.seekg(-16, ios_base::end);
        std::istreambuf_iterator<char> it(fs);
        std::istreambuf_iterator<char> last;
        std::string read_str(it, last);
        std::cout << "file_name:" << d_buf << std::endl;
        std::cout << "rad_str:" << read_str << std::endl;
        all_footers = all_footers + read_str + "\" or frame contains \"";
        fs.close();
      }
    }
  } while (entry != NULL);

  fstream write_wire;
  write_wire.open("source4/command_wireshark2.txt", ios::out);
  if(! write_wire.is_open()) {
      return;
  }

  write_wire.write((const char*)&all_footers[0], all_footers.size());;
  write_wire.close();
}
