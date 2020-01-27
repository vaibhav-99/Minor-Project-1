#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "fcfsHeader.h"
#include "sjfHeader.h"
#include "priorityHeader.h"
#include "srtfHeader.h"
#include "roundRobinHeader.h"

process take_processes_input(int n, int i)
{
      process p;
	  printf("Enter Process id of Process %d : ", i+1);
	  scanf("%s",p.pid);
	  printf("Enter Arrival time of Process %d : ", i+1);
	  scanf("%d",&p.atime);
	  printf("Enter Burst time of Process %d : ", i+1);
	  scanf("%d",&p.btime);
	  return p;
}

int main()
{
   int ch;
   printf("1) First Come - First Serve (FCFS)\n");
   printf("2) Shortest Job First (SJF)\n");
   printf("3) Priority\n");
   printf("4) Shortest Remaining Time First (SRTF)\n");
   printf("5) Round-Robin\n");
   printf("Enter Your Choice : ");
   scanf("%d", &ch);

   int n, i;
   printf("Enter No. of Processes : ");
   scanf("%d", &n);
   switch(ch)
   {
       case 1:
           for(i=0; i<n; i++)
           {
             process p;
             p = take_processes_input(n, i);
             insertFCFS(p);
           }

           executeFCFS(n);
	       showResultsFCFS();
           print_gantt_chartFCFS(n);
           printf("Avg. Waiting Time : %.2fs\n", avgwtime(fcfs));
	       printf("Avg TAT : %.2fs\n",avgtat(fcfs));
           break;

       case 2:
           for(i=0; i<n; i++)
           {
             process p;
             p = take_processes_input(n, i);
             insertSJF(p);
           }
           executeSJF(n);
	       showResultsSJF();
           print_gantt_chartSJF(n);
           printf("Avg. Waiting Time : %.2fs\n", avgwtime(sjf));
	       printf("Avg TAT : %.2fs\n",avgtat(sjf));
           break;

       case 3:
           for(i=0; i<n; i++)
           {
             process p;
             p = take_processes_input(n, i);
             printf("Enter Priority of Process %d : ", i+1);
             scanf("%d", &p.priority);
             insertPriority(p);
           }

           executePriority(n);
	       showResultsPriority();
	       print_gantt_chartPriority(n);
	       printf("Avg. Waiting Time : %.2fs\n", avgwtime(priority));
	       printf("Avg TAT : %.2fs\n",avgtat(priority));
           break;

       case 4:
           for(i=0; i<n; i++)
           {
             process p;
             p = take_processes_input(n, i);
             insertSRTF(p);
           }
           executeSRTF(n);
           showResultsSRTF();
           print_gantt_chartSRTF(n);
           printf("Avg. Waiting Time : %.2fs\n", avgwtime(srtf));
	       printf("Avg TAT : %.2fs\n",avgtat(srtf));
           break;

        case 5:
           for(i=0; i<n; i++)
           {
             process p;
             p = take_processes_input(n, i);
             insertRR(p);
           }
           int ts;
           printf("Enter Time Slice : ");
           scanf("%d", &ts);
           executeRR(n, ts);
	       showResultsRR();
           print_gantt_chartRR(n);
           printf("Avg. Waiting Time : %.2fs\n", avgwtime(rr));
	       printf("Avg TAT : %.2fs\n",avgtat(rr));
           break;

         default : printf("Invalid Choice!\n");
   }
   return 0;
}
