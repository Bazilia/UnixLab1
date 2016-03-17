#include "labHeader.h"
#include <stdio.h>

void childHandler(int sigNumber, siginfo_t *siginfo, void *context){
  gotSignal = 1;
  info = *siginfo;
}

void doChild(){
  gotSignal = 0;
  struct sigaction act;
  //sa_sigaction - обработчик сигналов
  act.sa_sigaction = &childHandler;
  //sa_flags - параметры обработчика
  act.sa_flags = SA_SIGINFO;
  //mask - тут записываются сигналы, которые нужно перехватить
  sigset_t mask;
  //sigemptyset - делаем пустой сэт для маски
  sigemptyset(&mask);
  //sigaddset - добавляем сигналы в сэт маски
  sigaddset(&mask, SIGCHLD);
  act.sa_mask = mask;
  //sigaction - делает сигналу новый обработчик
  if (sigaction(SIGCHLD, &act, NULL) == -1) {
    fprintf(stderr, "Не могу поменять обработчик SIGCHILD");
  }

  while(!gotSignal){
    sleep(10);
  }
  pid_t pidChild = fork();
  if (pidChild == -1){
    fprintf(stderr, "Не могу форкнуться\n");
  } else if(pidChild == 0){
    int sleepTime = rand()%10;
    printf("Потомок спит: %dcек.\n", sleepTime);
    sleep(sleepTime);
    printf("PID потомка: %i\n", info.si_pid);
    printf("Процесс от пользователя: %i\n", info.si_uid);
    printf("Статус: %i\n", info.si_status);
    printf("Код: %i\n", info.si_code);
  }else{
    int status;
    waitpid(pidChild,&status,0);
    printf("Потомок завершился\n");
  }
}
