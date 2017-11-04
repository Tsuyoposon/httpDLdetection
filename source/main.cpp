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

radix_tree<std::string, int> tree;
pgen_t* w, hit_packs;
int sig_count = 0;
int hit_sig_count = 0;
//計測結果用
int sig_first_hit = 0;//シグネチャ法でのヒット件数
int sig_second_hit = 0;//詳しく調べてのヒット件数
int get_size;
long long int packfile_count = 0;
std::string newfile;

std::bitset<2048> bit_buf(0);
std::bitset<2048> char_buf;

std::bitset<2048> signeture(0);
std::vector<radix_tree<std::string, int>::iterator> vec;
std::vector<radix_tree<std::string, int>::iterator>::iterator it;

int main(){
  //パケットデータファイルのデータを消しておく
  std::string str = "hit_packs/";
  char* cstr = new char[str.size() + 1];
  std::char_traits<char>::copy(cstr, str.c_str(), str.size() + 1);
  delete_file(cstr);
  std::string str2 = "hit_pack/";
  char* cstr2 = new char[str2.size() + 1];
  std::char_traits<char>::copy(cstr2, str2.c_str(), str2.size() + 1);
  delete_file(cstr2);

  //時間計測の処理（スタート）
  auto start = std::chrono::system_clock::now();

  //検知したいデータファイルからデータを入れる
  insert_file_data("./source3/test_data_files/");
  //トライ木の確認関数
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

  //時間計測の処理（終わり）
  auto end = std::chrono::system_clock::now();
  auto dur = end - start;
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  double first_hit = static_cast<double>(sig_first_hit)/static_cast<double>(packfile_count);
  double second_hit = static_cast<double>(sig_second_hit)/static_cast<double>(sig_first_hit);
  std::cout << "search_time:" << msec << std::endl;
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
      std::cout << path << entry->d_name << std::endl;
      d_buf = std::string(path) + std::string(entry->d_name);
      i++;
      if (2<i){
        ifstream fs;
        fs.open(d_buf, ios::in | ios::binary);
        if(! fs.is_open()) {
          return;
        }
        std::bitset<2048> bs;
        fs.seekg(-256, ios_base::end);
        fs.read((char*)&bs, sizeof bs);
        tree.insert(std::pair<std::string, int>(bs.to_string(), i));
        fs.close();
      }
    }
  } while (entry != NULL);
}

bool func(const u_char* p, int l){
  bit_buf.reset();

  pgen_unknown u(p,l);

  //末尾64Byte分を取得
  for(int i=1;i<=256;i++){
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
  if (10<=sig_count){
    sig_count=0;
    hit_sig_count = 0;
    //ファイルの移動のため一度閉じる
    pgen_close(w);
    packfile_count++;
    get_size = tree.signature_match(signeture.to_string(), vec);
    // for (it = vec.begin(); it != vec.end(); ++it) {
    //     std::cout << "    " << (*it)->first << ", " << (*it)->second << std::endl;
    // }
    // std::cout << "packfile_count:" << packfile_count << std::endl;
    // std::cout << "get_size:" << get_size << std::endl;
    // std::cout << "signeture:" << signeture << std::endl;
    if(0<get_size){
      //内包パケットがあった場合
      sig_first_hit++;
      //細かく検索
      pgen_t* hit_pack = pgen_open_offline("out.pcap", PCAP_READ);
      if(hit_pack==NULL || w==NULL){
        pgen_perror("oops");
        return -1;
      }
      sniff(hit_pack, func2);
      pgen_close(hit_pack);
      //ファイルの移動処理
      // std::cout << "get:" << packfile_count << std::endl;
      newfile = "./hit_packs/"+std::to_string(packfile_count)+".pcap";
      rename( "out.pcap", newfile.c_str());
    } else{
      //内包パケットがなかった場合
      remove("out.pcap");
    }
    signeture.reset();
    w = pgen_open_offline("out.pcap", PCAP_WRITE);
  }

  return true;
}

bool func2(const u_char* p, int l){
  bit_buf.reset();
  pgen_unknown u(p,l);

  //末尾64Byte分を取得
  for(int i=1;i<=256;i++){
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
    std::bitset<2048> hit_bit((*it)->first);
    if(bit_buf == hit_bit) {
      sig_second_hit++;
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

void delete_file(char *dir)
{
    DIR *dp;
    struct dirent *ent;
    struct stat statbuf;

    if ((dp = opendir(dir)) == NULL) {
        perror(dir);
        exit(EXIT_FAILURE);
    }
    chdir(dir);
    while ((ent = readdir(dp)) != NULL) {
        lstat(ent->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(".", ent->d_name) == 0 ||
                strcmp("..", ent->d_name) == 0)
                continue;
            delete_file(ent->d_name);
        }
        else {
            unlink(ent->d_name);
        }
    }
    chdir("..");
    closedir(dp);
}
