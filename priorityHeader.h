    void gc_insertPriority(int t, process *i)
    {
      process newp;
      newp.atime = i->atime;
      newp.btime = i->btime;
      newp.ctime = t;
      newp.wtime = i->wtime;
      strcpy(newp.pid, i->pid);
      p[n++] = newp;
    }

    void print_gantt_chartPriority(int n)
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

void minHeapifyPriority(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < hs && h[l].priority < h[i].priority)
        smallest = l;
    if (r < hs && h[r].priority < h[smallest].priority)
        smallest = r;
    if (smallest != i)
    {
        swap(&h[i], &h[smallest]);
        minHeapifyPriority(smallest);
    }
}

process extractMinPriority()
{
    if(hs == 1)
    {
        hs--;
        return h[0];
    }
    process r = h[0];
    h[0] = h[hs-1];
    hs--;
    minHeapifyPriority(0);

    return r;
}

void insertPriority(process p)
{
    if (priority == NULL)
     {
         process* temp = (process*)malloc(sizeof(process));
         initialise(temp, p);
         temp->link = NULL;
         priority = temp;
         return;
     }

    process* temp = priority,*last=NULL;
    process* next = NULL;
    while (temp!=NULL && (temp->atime <  p.atime))
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
      temp2->link = priority;
      priority = temp2;
     }
     else
      last->link = temp2;

}

void insertKeyPriority(process p)
{
    hs++;
    int i = hs - 1;
    h[i] = p;

    while (i != 0 && h[parent(i)].priority > h[i].priority)
    {
       swap(&h[i], &h[parent(i)]);
       i = parent(i);
    }
}
 void insertArrivedProcessesPriority(int t)
{
    process *temp = priority;
    int flag = 0;
    while(temp != NULL && temp->atime <= t)
    {
        if(temp->done == 1)
        {
         temp = temp->link;
         continue;
        }
        process p;
        p.atime = temp->atime;
        p.btime = temp->btime;
        p.self = temp;
        p.priority = temp->priority;
        insertKeyPriority(p);
        temp = temp->link;
    }
}

void executePriority(int n)
{
  int t=0;
  while(count != n)
  {
   insertArrivedProcessesPriority(t);
   if(!hs)        // if we have not added any processes
   {
    t++;
    continue;
   }

   process exec = extractMinPriority();

   exec.self->ctime = t + exec.self->btime;
   exec.self->tat = exec.self->ctime - exec.self->atime;
   exec.self->wtime = exec.self->tat - exec.self->btime;
   exec.self->done = 1;

   t += exec.self->btime;
   count++;
   appendToQueue(exec.self);
   gc_insertPriority(t,exec.self);
  }
}

void showResultsPriority()
{
    process* temp = q_add;
    printf("PID  Priority   AT   BT   CT   TAT   WT\n");
    while(temp!=NULL)
    {
     printf("%s     %d        %d     %d    %d    %d    %d\n",temp->pid,temp->priority, temp->atime, temp->btime, temp->ctime, temp->tat, temp->wtime);
     temp = temp->qnext;
    }
}

