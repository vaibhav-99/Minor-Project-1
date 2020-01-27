struct process{
  int atime,btime,wtime,ctime,tat,btemp,priority,done,chosen;
  char pid[3];
  struct process* link, *qnext, *self, *next;
} *fcfs=NULL, *sjf=NULL, *priority=NULL, *srtf=NULL, *rr=NULL, *q_add=NULL, *q=NULL;
typedef struct process process;
process p[50], h[50];
int i=0, count = 0 ,hs = 0, n=0;

void gc_insertFCFS(int t, process *i)
{
  process newp;
  newp.atime = i->atime;
  newp.btime = i->btime;
  newp.ctime = t;
  newp.wtime = i->wtime;
  strcpy(newp.pid, i->pid);
  p[n++] = newp;
}

void print_gantt_chartFCFS(int n)
{
    int i, j;
    int last = p[n-1].ctime;
    // printing top bar
    printf(" ");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].btime; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    // middle position
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].btime-1; j++) printf(" ");
        printf("%s", p[i].pid);
        for(j=0; j<p[i].btime-1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].btime; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    // printing waiting time
    int minus = 0;
    for(i=0; i<n; i++) {
        if(p[i].wtime>9) printf(" ");
        printf("%d", p[i].wtime);
        if(p[i+1].wtime>9){
          minus = 1;
        }
        if(i+1 == n )  if (last>9) minus = 1;
        for(j=0; j<p[i].btime-minus; j++) printf("  ");

    }
    if(last>9) printf(" ");
    printf("%d\n", last);
}

void initialise(process *temp, process p)
{
    temp->atime = p.atime;
    temp->btime = p.btime;
    temp->btemp = p.btime;
    temp->link = NULL;
    temp->qnext = NULL;
    temp->next = NULL;
    strcpy(temp->pid, p.pid);
    temp->priority = p.priority;
    temp->done = 0;
    temp->chosen = 0;
}

void insertFCFS(process p)
{
    if (fcfs == NULL)
     {
         process* temp = (process*)malloc(sizeof(process));
         initialise(temp, p);
         temp->link = NULL;
         fcfs = temp;
         return;
     }

    process* temp = fcfs,*last=NULL;
    process* next = NULL;
    while (temp!=NULL && (temp->atime < p.atime))
    {
        if (temp->link!=NULL && temp->link->atime >= p.atime)
        {
         next = temp->link;
         last = temp;
         break;
        }
       last = temp;
       temp = temp->link;
    }
    process* temp2 = (process*)malloc(sizeof(process));
    initialise(temp2, p);
    temp2->link = next;
     if(last == NULL)
     {
      temp2->link = fcfs;
      fcfs = temp2;
     }
     else
      last->link = temp2;

}

void executeFCFS()
{
  int t=0;
  process* temp = fcfs;

  while(temp!=NULL)
  {
    if (temp->atime <= t)
    {
     t += temp->btime;
     temp->ctime = t;
     temp->tat = temp->ctime - temp->atime;
     temp->wtime = temp->tat - temp->btime;
     gc_insertFCFS(t,temp);
    }
    else
    {
     t++;
     continue;
    }

    temp = temp->link;
  }
}

void showResultsFCFS()
{
    process* temp = fcfs;
    printf("PID  AT   BT   CT   TAT   WT\n");
    while(temp!=NULL)
    {
     printf(" %s   %d    %d    %d    %d    %d\n", temp->pid, temp->atime, temp->btime, temp->ctime, temp->tat, temp->wtime);
     temp = temp->link;
    }
}

float avgwtime(process *p)
{
    int w = 0,count=0;
    process* temp = p;
    while(temp!=NULL)
    {
        w += temp->wtime;
        temp = temp->link;
        count++;
    }
    return (float)w/count;
}

float avgtat(process *p)
{
    int tat = 0,count=0;
    process* temp = p;
    while(temp!=NULL)
    {
        tat += temp->tat;
        temp = temp->link;
        count++;
    }
    return (float)tat/count;
}
