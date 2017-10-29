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
bool func(const u_char* p, int l);
bool func2(const u_char* p, int l);
void traverse();

radix_tree<std::string, int> tree;
pgen_t* w, hit_packs;
int sig_count = 0;
int hit_sig_count = 0;
long long int packfile_count = 0;
bitset<512> signeture(0);
std::vector<radix_tree<std::string, int>::iterator> vec;
std::vector<radix_tree<std::string, int>::iterator>::iterator it;

int main(){

  insert_file_data("./test/");
  traverse();

  pgen_t* handle = pgen_open_offline("long_rapidgor.pcap", PCAP_READ);
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

bool func(const u_char* p, int l){

  bitset<512> bit_buf(0);
  bitset<512> char_buf, buf;
  pgen_unknown u(p,l);

  //末尾64Byte分を取得
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
  //論理和で計算してシグネチャを生成
  signeture = signeture | bit_buf;
  //out.pcapに送る
  u.send(w);


  sig_count++;
  //パケットがある程度溜まったらの処理
  if (5<=sig_count){
    sig_count=0;
    hit_sig_count = 0;
    //ファイルの移動のため一度閉じる
    pgen_close(w);
    packfile_count++;
    int get_size = tree.signature_match(signeture.to_string(), vec);
    for (it = vec.begin(); it != vec.end(); ++it) {
        std::cout << "    " << (*it)->first << ", " << (*it)->second << std::endl;
    }
    std::cout << "get_size:" << get_size << std::endl;
    std::cout << "signeture:" << signeture << std::endl;
    if(0<get_size){
      //内包パケットがあった場合
      //細かく検索
      pgen_t* hit_pack = pgen_open_offline("out.pcap", PCAP_READ);
      if(hit_pack==NULL || w==NULL){
        pgen_perror("oops");
        return -1;
      }
      sniff(hit_pack, func2);
      pgen_close(hit_pack);
      //ファイルの移動処理
      std::cout << "get:" << packfile_count << std::endl;
      std::string newfile("./hit_packs/"+std::to_string(packfile_count)+".pcap");
      rename( "out.pcap", newfile.c_str());
    } else{
      //内包パケットがなかった場合
      remove("out.pcap");
    }
    signeture.reset();
    std::cout << "signeture(r):" << signeture << std::endl;
    w = pgen_open_offline("out.pcap", PCAP_WRITE);
  }

  return true;
}

bool func2(const u_char* p, int l){
  bitset<512> bit_buf(0);
  bitset<512> char_buf, buf;
  pgen_unknown u(p,l);

  //末尾64Byte分を取得
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
  //末尾64Byteがヒット文字列と同じならパケットを保存
  for (it = vec.begin(); it != vec.end(); ++it) {
    bitset<512> hit_bit((*it)->first);
    if(bit_buf == hit_bit) {
      //なんとか.pcapに送る
      hit_sig_count++;
      std::string hit_pack_file("./hit_pack/"+std::to_string(packfile_count)+
      "-"+std::to_string(hit_sig_count)+".pcap");
      pgen_t* hit_pack = pgen_open_offline(hit_pack_file.c_str(), PCAP_WRITE);
      if(hit_pack==NULL || w==NULL){
        pgen_perror("oops");
        return -1;
      }
      u.send(hit_pack);
      pgen_close(hit_pack);
    }
  }
  return true;
}

void traverse() {
    radix_tree<std::string, int>::iterator it;

    std::cout << "traverse:" << std::endl;
    for (it = tree.begin(); it != tree.end(); ++it) {
        std::cout << "    " << it->first << ", " << it->second << std::endl;
    }
}
