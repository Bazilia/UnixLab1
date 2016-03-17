#include "labHeader.h"
#include <stdio.h>


void stdHandler(int sigNumber, siginfo_t *siginfo, void *context){
  gotSignal = 1;
  info = *siginfo;
}

void doStd(){
  gotSignal = 0;
  struct sigaction act;
  //sa_sigaction - обработчик сигналов
  act.sa_sigaction = &stdHandler;
  //sa_flags - параметры обработчика
  act.sa_flags = SA_SIGINFO;
  //mask - тут записываются сигналы, которые нужно перехватить
  sigset_t mask;
  //sigemptyset - делаем пустой сэт для маски
  sigemptyset(&mask);
  //sigaddset - добавляем сигналы в сэт маски
  sigaddset(&mask, SIGHUP);
  sigaddset(&mask, SIGUSR1);
  sigaddset(&mask, SIGUSR2);
  act.sa_mask = mask;
  //sigaction - делает сигналу новый обработчик
  if (sigaction(SIGHUP, &act, NULL) == -1) {
    fprintf(stderr, "Не могу поменять обработчик SIGHUP");
  }
  if (sigaction(SIGUSR1, &act, NULL) == -1) {
    fprintf(stderr, "Не могу поменять обработчик SIGUSR1");
  }
  if (sigaction(SIGUSR2, &act, NULL) == -1) {
    fprintf(stderr, "Не могу поменять обработчик SIGUSR2");
  }

  while(1){
    sleep(10);
    if(gotSignal != 0){
      printf("Получен сигнал: %d\n",info.si_signo);
      printf("От процесса с PID: %d\n", info.si_pid);
      printf("из группы GID: %d\n",getpgid(info.si_pid));

      gotSignal = 0;
    }
  }
}
