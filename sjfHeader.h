void gc_insertSJF(int t, process *i)
{
  process newp;
  newp.atime = i->atime;
  newp.btime = i->btime;
  newp.ctime = t;
  newp.wtime = i->wtime;
  strcpy(newp.pid, i->pid);
  p[n++] = newp;
}

void print_gantt_chartSJF(int n)
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

void swap(process *p1, process *p2)
{
    process temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int left(int i)
{
    return 2*i + 1;
}
int right(int i)
{
    return 2*i + 2;
}
int parent(int i)
{
    return (i-1)/2;
}

void insertKeySJF(process p)
{
    hs++;
    int i = hs - 1;
    h[i] = p;

    while (i != 0 && h[parent(i)].btime > h[i].btime)
    {
       swap(&h[i], &h[parent(i)]);
       i = parent(i);
    }
}

void minHeapifySJF(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < hs && h[l].btime < h[i].btime)
        smallest = l;
    if (r < hs && h[r].btime < h[smallest].btime)
        smallest = r;
    if (smallest != i)
    {
        swap(&h[i], &h[smallest]);
        minHeapifySJF(smallest);
    }
}

process extractMinSJF()
{
    if(hs == 1)
    {
        hs--;
        return h[0];
    }
    process r = h[0];
    h[0] = h[hs-1];
    hs--;
    minHeapifySJF(0);

    return r;
}

void appendToQueue(process *p)
{
    if(q_add == NULL)
    {
     q_add = p;
     return;
    }
    process *temp = q_add;
    while(temp->qnext != NULL)
     temp = temp->qnext;

    temp->qnext = p;
}

void insertSJF(process p)
{
    if (sjf == NULL)
     {
         process* temp = (process*)malloc(sizeof(process));
         initialise(temp, p);
         temp->link = NULL;
         sjf = temp;
         return;
     }

    process* temp = sjf,*last=NULL;
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
      temp2->link = sjf;
      sjf = temp2;
     }
     else
      last->link = temp2;

}

 void insertArrivedProcessesSJF(int t)
{
    process *temp = sjf;
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
        insertKeySJF(p);
        temp = temp->link;
    }
}

void executeSJF(int n)
{
  int t=0;
  while(count != n)
  {
   insertArrivedProcessesSJF(t);
   if(!hs)        // if we have not added any processes
   {
    t++;
    continue;
   }

   process exec = extractMinSJF();

   exec.self->ctime = t + exec.self->btime;
   exec.self->tat = exec.self->ctime - exec.self->atime;
   exec.self->wtime = exec.self->tat - exec.self->btime;
   exec.self->done = 1;

   t += exec.self->btime;
   count++;

   appendToQueue(exec.self);
   gc_insertSJF(t,exec.self);
  }
}

void showResultsSJF()
{
    process* temp = q_add;
    printf("PID  AT   BT   CT   TAT   WT\n");
    while(temp!=NULL)
    {
     printf(" %s   %d    %d    %d    %d    %d\n", temp->pid, temp->atime, temp->btime, temp->ctime, temp->tat, temp->wtime);
     temp = temp->qnext;
    }
}

