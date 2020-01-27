void gc_insertSRTF(int t, process *i)
{
  process newp;
  newp.atime = i->atime;
  newp.btime = 1;
  newp.ctime = t;
  strcpy(newp.pid, i->pid);
  p[n++] = newp;
}

void print_gantt_chartSRTF()
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

    // printing completion time
    int minus = 0;
    printf("%d", p[0].atime);

    if(p[0].ctime>9) printf(" ");
        if(n != 1 && p[1].ctime>9){
          minus = 1;
        }
        for(j=0; j<p[0].btime-minus; j++) printf("  ");
        printf("%d", p[0].ctime);

    printf("  ");
    minus = 0;

   // printf("  ");
    for(i=1; i<n; i++) {
        if(p[i].ctime>9) printf(" ");
        printf("%d", p[i].ctime);
        if(p[i+1].ctime>9){
          minus = 1;
        }
        if(i+1 == n )  if (last>9) minus = 1;
        for(j=0; j<p[i].btime-minus; j++) printf("  ");

    }
 //   if(last>9) printf(" ");
   // printf("%d\n", last);
   printf("\n");
}

void insertSRTF(process p)
{
    if (srtf == NULL)
     {
         process* temp = (process*)malloc(sizeof(process));
         initialise(temp, p);
         temp->link = NULL;
         srtf = temp;
         return;
     }

    process* temp = srtf,*last=NULL;
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
      temp2->link = srtf;
      srtf = temp2;
     }
     else
      last->link = temp2;

}

void insertArrivedProcessesSRTF(int t)
{
    process *temp = srtf;
    int flag = 0;
    while(temp != NULL && temp->atime <= t)
    {
        if(temp->done == 1 || temp->chosen == 1)
        {
         temp = temp->link;
         continue;
        }
        process p;
        p.atime = temp->atime;
        p.btime = temp->btime;
        p.self = temp;
        insertKeySJF(p);
        flag = 1;
        temp = temp->link;
    }
}

void executeSRTF(int n)
{
  int t=0;
  while(count != n)
  {
   insertArrivedProcessesSRTF(t);
   if(!hs)
   {
    t++;                            // if we have not added any processes
    continue;
   }

   process exec = extractMinSJF();

   exec.self->chosen = 1;
   exec.self->btemp--;

   if(exec.self->btemp == 0)
   {
    exec.self->ctime = t+1;
    exec.self->tat = exec.self->ctime - exec.self->atime;
    exec.self->wtime = exec.self->tat - exec.self->btime;
    exec.self->done = 1;
    t++;
    count++;
    appendToQueue(exec.self);
    gc_insertSRTF(t,exec.self);
   }
   else
   {
       t++;
       process p;
       process *temp = exec.self;
       p.atime = temp->atime;
       p.btime = temp->btemp;
       p.self = temp;
       insertKeySJF(p);
   }

  }
}

void showResultsSRTF()
{
    process* temp = q_add;
    printf("PID  AT   BT   CT   TAT   WT\n");
    while(temp!=NULL)
    {
     printf(" %s   %d    %d    %d    %d    %d\n", temp->pid, temp->atime, temp->btime, temp->ctime, temp->tat, temp->wtime);
     temp = temp->qnext;
    }
}
