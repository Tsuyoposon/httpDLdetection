// cp -rf pgen /usr/local/bin/pgen

#include <pgen.h>

pgen_t* w;
bool func(const u_char* p, int l){
  pgen_unknown u(p,l);
  if(u.isTCP() && u.TCP.port()==57750){
    u.summary();
    u.send(w);
  }
  return true;
}

int main(){
  pgen_t* handle = pgen_open_offline("in.pcapng", PCAPNG_READ);
  w = pgen_open_offline("out.pcapng", PCAPNG_WRITE);
  if(handle==NULL || w==NULL){
    pgen_perror("oops");
    return -1;
  }
  sniff(handle, func);
  pgen_close(handle);
  pgen_close(w);
}
