#include <string>
#include <dirent.h>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <dirent.h>

#include "header/radix_tree.hpp"
#include "header/DLdetection.h"
using namespace std;

void insert_file_data(std::string file_path);

radix_tree<std::string, int> tree;
pgen_t* w;

int main(){
  pgen_t* handle = pgen_open_offline("11.pcap", PCAP_READ);
  w = pgen_open_offline("out.pcap", PCAP_WRITE);
  if(handle==NULL || w==NULL){
    pgen_perror("oops");
    return -1;
  }
  sniff(handle, func);
  pgen_close(handle);
  pgen_close(w);
}


void insert_file_data(std::string file_path){
  const char* path = file_path.c_str();
  DIR *dp;       // ディレクトリへのポインタ
  dirent* entry; // readdir() で返されるエントリーポイント
  int i=0;
  std::string d_buf;

  dp = opendir(path);
  if (dp==NULL) exit(1);
  do {
    entry = readdir(dp);
    if (entry != NULL){
      std::cout << path << entry->d_name << std::endl;
      d_buf = std::string(path) + std::string(entry->d_name);
      i++;
      if (2<i){
        ifstream fs;
        fs.open(d_buf, ios::in | ios::binary);
        if(! fs.is_open()) {
          return;
        }
        std::bitset<512> bs;
        fs.seekg(-64, ios_base::end);
        fs.read((char*)&bs, sizeof bs);
        tree.insert(std::pair<std::string, int>(bs.to_string(), i));
        fs.close();
      }
    }
  } while (entry != NULL);
}
