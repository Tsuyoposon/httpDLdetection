
#include "DLdetection.h"

pgen_t* w;

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
