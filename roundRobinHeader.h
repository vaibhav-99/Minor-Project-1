void gc_insertRR(int t, process *i, int ts)
{
  process newp;
  newp.atime = i->atime;
  newp.btime = ts;
  newp.ctime = t;
  newp.wtime = ts;
  strcpy(newp.pid, i->pid);
  p[n++] = newp;
}

void print_gantt_chartRR()
{
    int i, j;
    int last = p[n-1].btime + ( n == 1 ? 0 : p[n-1].ctime);
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



void insertRR(process p)
{
    if (rr == NULL)
     {
         process* temp = (process*)malloc(sizeof(process));
         initialise(temp, p);
         temp->link = NULL;
         rr = temp;
         return;
     }

    process* temp = rr,*prev=NULL;
    process* next = NULL;
    while (temp!=NULL && (temp->atime < p.atime))
    {
        if (temp->link!=NULL && temp->link->atime >= p.atime)
        {
         next = temp->link;
         prev= temp;
         break;
        }
       prev = temp;
       temp = temp->link;
    }
    process* temp2 = (process*)malloc(sizeof(process));
    initialise(temp2, p);
    temp2->link = next;
     if(prev == NULL)
     {
      temp2->link = rr;
      rr = temp2;
     }
     else
      prev->link = temp2;
}

void append(process *p)
{
    if(q == NULL)
    {
     q = p;
     q->next = NULL;
     return;
    }
    process *temp = q;
    while(temp->next != NULL)
     temp = temp->next;

    temp->next = p;
}

void fun(int t)
{
  	process* temp=rr;

	while(temp!=NULL && temp->atime <= t)
	{
		if(temp->chosen != 1)
		{
		 temp->chosen = 1;
		 append(temp);
		}
		temp = temp->link;
	}
}

void roundRobin(int *t, int ts)
{
	 if(q == NULL)    // if round robin queue is empty
	 {
	  (*t)++;
	  return;
	 }
	 if(q->btime - ts >= 0)
	 {
	  q->btime -= ts;
	  *t += ts;
      gc_insertRR(*t, q, ts);
	 }
	 else
	 {
	  *t += q->btime;
	   q->btime = 0;
	   gc_insertRR(*t, q, q->btime);
	 }
     fun(*t);       // inserting arrived processes again
	 if(q->btime == 0)
	 {
	    q->ctime = *t;
	    q->tat = q->ctime - q->atime;
	    q->wtime = q->tat - q->btemp;
	    q->done = 1;
	    count++;
	    process *forward = q->next;
	    q->next = NULL;
//	    q->done = 1;
	    q = forward;
	 }
	 else
	 {
	  if(q->next == NULL)
       return;

      process *forward = q->next;
      append(q);
      q->next = NULL;
      q = forward;
	 }
}
void executeRR(int n, int ts)
{
	int t=0;
	while(count != n)
    {
     fun(t);   // inserts arrived processes
     roundRobin(&t, ts);
    }
}

void showResultsRR()
{
	process* temp=rr;
	printf("PID     AT    BT      CT      TAT       WT\n");
	while(temp!=NULL)
    {
     printf("%s       %d    %d        %d       %d        %d\n",temp->pid, temp->atime, temp->btemp,temp->ctime, temp->tat, temp->wtime);
     temp = temp->link;
    }
}
