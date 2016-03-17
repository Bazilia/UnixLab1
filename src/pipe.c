#include "labHeader.h"
#include <stdio.h>

void pipeHandler(int sigNumber, siginfo_t *siginfo, void *context){
  gotSignal = 1;
}

void doPipe(){
  gotSignal = 0;
  struct sigaction act;
  //sa_sigaction - обработчик сигналов
  act.sa_sigaction = &pipeHandler;
  //sa_flags - параметры обработчика
  act.sa_flags = SA_SIGINFO;
  //mask - тут записываются сигналы, которые нужно перехватить
  sigset_t mask;
  //sigemptyset - делаем пустой сэт для маски
  sigemptyset(&mask);
  //sigaddset - добавляем сигналы в сэт маски
  sigaddset(&mask, SIGPIPE);
  act.sa_mask = mask;
  //sigaction - делает сигналу новый обработчик
  if (sigaction(SIGPIPE, &act, NULL) == -1) {
    fprintf(stderr, "Не могу поменять обработчик SIGPIPE");
  }

  while(!gotSignal){
    sleep(10);
  }
  int fd[2];
  if(pipe(fd)==0){
    printf("Пайпа создалась\n");
    pid_t pidChild = fork();
    if (pidChild == -1){
      fprintf(stderr, "Не могу форкнуться\n");
    } else if(pidChild == 0){
        if(close(fd[0]) == -1){
          fprintf(stderr, "Не могу закрыть читателя\n");
        }
    }else{
      int status;
      waitpid(pidChild,&status,0);
      printf("Потомок завершился\n");
    }
  }
  else{
    fprintf(stderr, "Не могу открыть пайпу\n");
  }
}
