#include <string>
#include <dirent.h>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <chrono>
#include <sys/stat.h>

#include "header/radix_tree.hpp"
#include "header/DLdetection.h"
using namespace std;

void insert_file_data(std::string file_path);
bool func(const u_char* p, int l);
bool func2(const u_char* p, int l);
void traverse();
void delete_file(char *dir);

int SIGCOUNT = 5;

radix_tree<std::string, int> tree;
pgen_t* w, hit_packs;
std::vector<std::string> pack_bufs(5);//10件のパケットデータ(末尾64Byte)を保持
const u_char* pack_bufs_p[5];
int pack_bufs_length[5];
std::vector<std::bitset<512>> pack_bufs_binary(5);

int sig_count = 0;
int hit_sig_count = 0;
int total_hit_count = 0;
//計測結果用
int insert_file_count = 0;//登録シグネチャの件数
int sig_first_hit = 0;//シグネチャ法でのヒット件数
int sig_second_hit = 0;//詳しく調べてのヒット件数
int get_size;
long long int packfile_count = 0;
std::string newfile;

std::bitset<512> bit_buf(0);
std::bitset<512> char_buf;

std::bitset<512> signeture(0);
std::vector<std::bitset<512>> read_file_binary(1000);
std::vector<radix_tree<std::string, int>::iterator> vec;
std::vector<radix_tree<std::string, int>::iterator>::iterator it;

int main(){
  //時間計測の処理（スタート）
  auto start = std::chrono::system_clock::now();

  //検知したいデータファイルからデータを入れる
  insert_file_data("./source3/test_data_files/");
  //トライ木の確認関数
  // traverse();

  pgen_t* handle = pgen_open_offline("long_rapidgor.pcap", PCAP_READ);
  // w = pgen_open_offline("out.pcap", PCAP_WRITE);
  if(handle==NULL){
    pgen_perror("oops");
    return -1;
  }
  sniff(handle, func);
  pgen_close(handle);
  // pgen_close(w);

  //時間計測の処理（終わり）
  auto end = std::chrono::system_clock::now();
  auto dur = end - start;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  double total_hit = static_cast<double>(total_hit_count)/static_cast<double>(packfile_count * 1000);
  double first_hit = static_cast<double>(sig_first_hit)/static_cast<double>(packfile_count);
  double second_hit = static_cast<double>(sig_second_hit)/static_cast<double>(sig_first_hit);
  std::cout << "registration_file:" << insert_file_count << std::endl;
  std::cout << "search_time:" << msec << std::endl;
  std::cout << "total_hit_count:(" << total_hit_count << "/" << packfile_count * 1000 <<")" <<
  total_hit*100 << "%"<< std::endl;
  std::cout << "first_hit:(" << sig_first_hit << "/" << packfile_count << ")" <<
  first_hit*100 << "%"<< std::endl;
  std::cout << "second_hit:(" << sig_second_hit << "/" << sig_first_hit << ")" <<
  second_hit*100 << "%"<< std::endl;
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
      // std::cout << path << entry->d_name << std::endl;
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
        read_file_binary[insert_file_count] = bs;
        fs.close();
        insert_file_count++;
        // std::cout << "file_name:" << entry->d_name << std::endl;
      }
    }
  } while (entry != NULL);

  // for(int i=0;i<insert_file_count;i++){
  //   std::cout << "insert_file:(" << i << ")" << read_file_binary[i] << std::endl;
  // }
}

bool func(const u_char* p, int l){
  bit_buf.reset();
  // pgen_unknown u(p,l);
  pack_bufs_p[sig_count] = p;
  pack_bufs_length[sig_count] = l;
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
  pack_bufs_binary[sig_count] = bit_buf;
  //out.pcapに送る
  // u.send(w);

  sig_count++;
  //パケットがある程度溜まったらの処理
  packfile_count++;
  if (SIGCOUNT<=sig_count){
    sig_count=0;

    //ファイルの移動のため一度閉じる
    // pgen_close(w);

    // std::cout << "packfile_count:" << packfile_count << std::endl;
    for(int k=0;k<insert_file_count;k++){
      bit_buf = signeture & read_file_binary[k];
      // std::cout << "bit_buf:" << bit_buf << std::endl;
      // std::cout << "signeture:" << signeture << std::endl;
      // std::cout << "read_file_binary[k]:" << read_file_binary[k] << std::endl;

      if(bit_buf == read_file_binary[k]){ // 内包していた場合
        // 細かく計算
        sig_first_hit++;
        // std::cout << "sig_first_hit:" << sig_first_hit << std::endl;
        for(int i=0;i<SIGCOUNT;i++){
          for(int n=0;n<insert_file_count;n++){
            if(read_file_binary[n] == pack_bufs_binary[i]){
              sig_second_hit++;
              pgen_unknown u(pack_bufs_p[i],pack_bufs_length[i]);
              u.hex();
            }
          }
        }
      }
    }
    signeture.reset();
    // std::cout << "reset:" << std::endl;
    // w = pgen_open_offline("out.pcap", PCAP_WRITE);
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
