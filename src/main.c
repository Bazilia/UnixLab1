#include "labHeader.h"
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static const struct option opts[] = {
{"mode", required_argument, NULL, 0},
{"amount", required_argument, NULL, 1},
{"signal", required_argument, NULL, 2},
{"pid", required_argument, NULL, 3}
};

int main(int argc, char *argv[]){
  int modeNumber = -1;
  int posixCount;
  int killSignal;
  pid_t killPid;
  int currentOpt = 0;
  int index = 0;
  char* modeNames[] = {"std","child","posix","kill","pipe"};

  printf("Pid: %i\n", getpid());
  printf("Аргументы:\n");

  currentOpt = getopt_long( argc, argv, "", opts, &index );
  while (currentOpt != -1) {
    if (index == 0) {
      int i;
      for (i=0;i<5;i++){
        //optarg - значение аргумента
        //strcmp сравнение строк
        if (!strcmp(modeNames[i], optarg)){
          modeNumber = i;
          break;
        }
      }
      printf("Режим: %s\n", modeNames[modeNumber] );
    } else if (index == 1) {
        //atoi - конверт из строки в int
        posixCount = atoi(optarg);
        printf("Количество сигналов: %d\n", posixCount );
    } else if (index == 2) {
        killSignal = atoi(optarg);
        printf("Номер сигнала: %i\n", killSignal );
    } else if (index == 3) {
        killPid = atoi(optarg);
        printf("PID: %i\n", killPid );
    }
    currentOpt = getopt_long( argc, argv, "", opts, &index);
  }
  printf("\n");

  switch (modeNumber) {
    case 0:{
      doStd();
      break;
    }
    case 1:{
      doChild();
      break;
    }
    case 2:{
      if (posixCount <= 0){
        fprintf(stderr,"НУЖНО БОЛЬШЕ СИГНАЛОВ!!! D:<\n");
      } else{
        doPosix(posixCount);
      }
      break;
    }
    case 3:{
      if (!killPid){
        fprintf(stderr,"Не умею в такой pid процесса =(\n");
      } else if (killSignal <= 0){
        fprintf(stderr,"Не знаю такого сигнала =(\n");
      } else {
        doKill(killSignal,killPid);
      }
      break;
    }
    case 4:{
      doPipe();
      break;
    }
    default:{
      fprintf(stderr,"Не знаю такого режима =(\n");
    }
  }
  return 0;
}
