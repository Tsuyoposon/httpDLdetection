#include "DLdetection.h"


bool func(const u_char* p, int l){
  pgen_unknown u(p,l);
  if(u.isTCP() && u.TCP.port()==57750){
    u.summary();
    u.send(w);
  }
  return true;
}
