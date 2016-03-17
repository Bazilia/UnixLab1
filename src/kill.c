#include "labHeader.h"
#include <stdio.h>

void doKill(int sig, int pid){
  kill(pid,sig);
}
