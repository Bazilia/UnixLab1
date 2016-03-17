#include "labHeader.h"
#include <stdio.h>

siginfo_t *sigInfos;
int sigInfoCount = 0;

void posixHandler(int sigNumber, siginfo_t *siginfo, void *context){
  sigInfos[sigInfoCount] = *siginfo;
  sigInfoCount++;
}

void doPosix(int posixCount){
  //malloc - выделяем память под массив
  sigInfos = (siginfo_t*)malloc(posixCount*sizeof(siginfo_t));
  int i;
  struct sigaction act;
  //sa_sigaction - обработчик сигналов
  act.sa_sigaction = &posixHandler;
  //sa_flags - параметры обработчика
  act.sa_flags = SA_SIGINFO;
  //mask - тут записываются сигналы, которые нужно перехватить
  sigset_t mask;
  //sigfillset - добавляем все сигналы в сэт маски
  sigfillset(&mask);
  act.sa_mask = mask;
  //sigaction - делает сигналу новый обработчик
  for (i=SIGRTMIN; i<SIGRTMAX + 1; i++){
    if (sigaction(i, &act, NULL) == -1) {
      fprintf(stderr, "Не могу поменять обработчик %d",i);
    }
  }
  //sigprocmask - блокируем сигналы
  sigprocmask(SIG_BLOCK, &mask, NULL);

  pid_t pidChild = fork();
  if (pidChild == -1){
    fprintf(stderr, "Не могу форкнуться\n");
  } else if(pidChild == 0){
    printf("N | pid | ppid | signum | val\n");
    for (i=0;i<posixCount;i++){
      int sigRndNumber = rand()%(SIGRTMAX-SIGRTMIN + 1) + SIGRTMIN;
      //union - принимает нужный в данный момент тип
      union sigval value;
      value.sival_int = rand();
      sigqueue(getppid(), sigRndNumber, value);

      printf("%d | %d | %d | %d | %d\n",i,getppid(),getpid(),sigRndNumber,value);
    }
  }else{
    int status;
    waitpid(pidChild,&status,0);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    printf("Потомок завершился\n");

    printf("От потомка пришло:\n");
    for (i=0;i<sigInfoCount;i++){
      printf("%d | %d | %d | %d | %d\n",i,getpid(),sigInfos[i].si_pid,sigInfos[i].si_signo,sigInfos[i].si_value);
    }
  }
}
