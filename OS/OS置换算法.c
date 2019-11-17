#include<stdio.h>
#define MAX 10
#define MAX_PROCESS  10
#define MAX_PRIORITY 5
struct task_struct{
     char name[10];           /*进程名称*/
     int number;              /*进程编号*/
     int come_time;           /*到达时间*/
     int begin_time;          /*开始运行时间*/
     int run_time;            /*运行时间*/
     int end_time;            /*运行结束时*/
     int round_time;          /*周转时间*/
     int priority;            /*优先级*/
     int order;               /*运行次序*/
     int rest_slice;          /*剩余时间片数*/
     int status;              /*run_flag调度标志*/
   }tasks[MAX];
   //tt 定义结构体
typedef struct QNode {
        int data;
        struct QNode *next;
}QNode,*QueuePtr;
typedef struct {
        QueuePtr front;
        QueuePtr rear;
}LinkQueue;
//pstt定义结构体
typedef struct node *PNode;
typedef struct node {
        int data;
        PNode next;
}Node;
typedef struct queue {
        PNode head;
        PNode tail;
        int Length;
        int PSlice;
}Queue;

int counter;     /*实际进程个数*/
int fcfs();      /*先来先服务*/
int ps();        /*优先级调度*/
int sjf();       /*短作业优先*/
int pstt();
int tt();
int pinput();    /*进程参数输入*/
int poutput();   /*调度结果输出*/
int clear();     /*初始化数据*/

int main(){
       int i,option;
       pinput();

       clear();
       printf("\n\nFCFS:\n");
       fcfs();
       poutput();

       clear();
       printf("\n\nSJF  :\n");
       sjf();
       poutput();

       clear();
       printf("\n\nPS  :\n");
       ps();
       poutput();

       clear();
       printf("\n\nTT    :\n");
       tt();
       poutput();

       clear();
       printf("\n\nPSTT  :\n");
       pstt();
       poutput();
}

int fcfs(){ /*先来先服务*/
        int i,j,sum;
        struct task_struct t;
        for(i=0;i<=counter-1;i++){ //对进程根据到达时间进行排序;
            for(j=0;j<counter-i-1;j++){
                if(tasks[j].come_time>tasks[j+1].come_time){
                        t=tasks[j+1];
                        tasks[j+1]=tasks[j];
                        tasks[j]=t;
                }
            }
        }     //最早到达的进程
        tasks[0].begin_time=tasks[0].come_time;
        tasks[0].end_time=tasks[0].begin_time+tasks[0].run_time;
        tasks[0].round_time=tasks[0].run_time;
        sum=tasks[0].end_time;
        tasks[0].order=1;
        for(i=1;i<counter;i++) {   //计算运行次序、运行开始时间、运行结束时间、运行 时间、周转时间、剩余时间片数
                tasks[i].order=i+1;//次序         //运行开始、结束时
                if(tasks[i].come_time<=tasks[i-1].end_time){
                    sum=sum+tasks[i].run_time;
                }
                else{
                    sum=tasks[i].come_time+tasks[i].run_time;
                }
                tasks[i].end_time=sum;
                tasks[i].begin_time=tasks[i].end_time-tasks[i].run_time;
                tasks[i].round_time=tasks[i].end_time-tasks[i].come_time;//周转时间
        }
}

int ps(){        /*优先级调度*/
        int i,j,k;
        int wait[counter];//已到达的进程队列
        int time=0,n,flag;
        for(i=0;i<counter;i++){
                wait[i]=0;
        }
        flag=0;
        for(i=0;i<counter;i++){//要执行 counter 个程序 ,一个程序运行结束后跟新到达的队列
            flag=0;
            while(flag==0){//是否有到达的程序
                for(k=0;k<counter;k++){//哪些程序到达
                    if((tasks[k].come_time<=time)&&(tasks[k].status==0)){
                        wait[k]=tasks[k].priority;//将到达的程序为其优先级
                        flag++;
                        }
                    else
                        wait[k]=0;//没到达的和已结束的设为 0
                    }
                if(flag==0)
                        time++;//如果没有到达的程序时间加一
            }
            n=0;
            for(j=0;j<counter;j++){//遍历到达队列
                if(wait[j]>wait[n]){
                        n=j;//选出优先级高的
                }
                if((wait[j]==wait[n])&&(tasks[j].come_time<tasks[n].come_time)){
                    n=j;//在优先级最高的中选出运行时间短的
                }
            }
            if(tasks[n].status==0){//修改此次运行的程序的 pcb
                tasks[n].begin_time=time;
                time=time+tasks[n].run_time;
                tasks[n].order=i+1;
                tasks[n].end_time=time;
                tasks[n].round_time=tasks[n].end_time-tasks[n].come_time;
                tasks[n].status=1;
                }
        }
}

int sjf() {       /*短作业优先*/
        int ontime,wait,i,j;
        int order;
        struct task_struct t;
        ontime = 0;      //ontime 表示 cpu 被占用时间
        wait = 0;       //等待区中的进程个数
        order=2;
        for(i=1;i<=counter-1;i++)     {   //对进程根据到达时间进行排序;
            tasks[i].order=0;
            for(j=0;j<counter-i-1;j++){
                    if(tasks[j].come_time>tasks[j+1].come_time){
                        t=tasks[j+1];
                        tasks[j+1]=tasks[j];
                        tasks[j]=t;
                    }
              }
          }
            tasks[0].begin_time=tasks[0].come_time;
            tasks[0].end_time=tasks[0].begin_time+tasks[0].run_time;
            tasks[0].round_time=tasks[0].run_time;
            ontime=tasks[0].end_time;
            tasks[0].order=1;
            for(i=1;i<=counter-1;i++){
                    if(tasks[i].come_time>=ontime){      //到达时间大于 CPU 被占用时间，则该进程到达时 CPU 被上一进程占用，需进 入等待队列中
                        if(wait==0)  {    //若该进程进入等待区时，其中无其他的进程
                            tasks[i].order = i+1;   //该进程作为下一使用 CPU 的进程
                            tasks[i].begin_time = tasks[i].come_time;
                            tasks[i].end_time = tasks[i].begin_time + tasks[i].run_time;
                            ontime=tasks[i].end_time;
                            order ++;
                            }
                         else {
                            wait++;       //该进程进入时，已有 wait 个进程处于等待区
                            tasks[i-wait+1].begin_time=ontime;  //这 wait 个进程（之前已按最短时间排好 序）中的“第一个”作为下一使用 CPU 的进程
                            tasks[i-wait+1].end_time=tasks[i-wait+1].begin_time+tasks[i-wait+1].run_time;
                            tasks[i-wait+1].round_time=tasks[i-wait+1].end_time-tasks[i-wait+1].come_time;
                            ontime=ontime+tasks[i-wait+1].run_time;
                            tasks[i-wait+1].order=order;
                            order++;
                            wait--;

                            int m = i;
                            int n;
                            int flag = 1;
                            for(;m>=(i-wait)&&flag;m--)  {        //按照最短运行时间排序
                                flag = 0;
                                for(n=i-wait;n<m;n++){
                                    if(tasks[n].run_time>tasks[n+1].run_time){
                                            t=tasks[n];
                                            tasks[n]=tasks[n+1];
                                            tasks[n+1]=t;
                                            flag = 1;
                                        }
                                    }
                               }
                        }
                }
            else{
                wait++;
                int m = i;
                int n;
                int flag = 1;
                for(;m>(i-wait+1)&&flag;m--){      //按最短运行时间排序
                    flag = 0;
                    for(n=i-wait+1;n<m;n++){
                            if(tasks[n].run_time>tasks[n+1].run_time){
                                t=tasks[n];
                                tasks[n]=tasks[n+1];
                                tasks[n+1]=t;
                                flag =1;
                                }
                          }
                    }
                }
            }
            for(i=0;i<counter;i++){
                if(tasks[i].order == 0) {
                    tasks[i].order = i+1;
                    tasks[i].begin_time  = tasks[i-1].end_time; //进程开始运行时间为上一进程运行结束的时间
                    tasks[i].end_time = tasks[i].begin_time + tasks[i].run_time;             //每个进程的结束时间为其开始时间+运行时间
                    tasks[i].round_time=tasks[i].end_time-tasks[i].come_time;   //计算周转时间=结束时间-到达时间
                    }
                }
            for(i=0;i<=counter-1;i++) {      //对进程重新根据到达时间进行排序;
                for(j=0;j<counter-i-1;j++){
                    if(tasks[j].come_time>tasks[j+1].come_time){
                        t=tasks[j+1];
                        tasks[j+1]=tasks[j];
                        tasks[j]=t;
                        }
                    }
                }
    }
void InitQueue(LinkQueue *Q){ //构造一个空队列Q
        (*Q).front=(*Q).rear=(QueuePtr)malloc(sizeof(QNode));
        if(!(*Q).front)
        exit(-2);//构造失败则退出
        (*Q).front->next=NULL;
 }
 int QueueEmpty(LinkQueue Q){// 若Q为空队列，则返回1，否则返回0
        if(Q.front->next==NULL)//头元素指针部分指向为空，判断为空队列，返回1
            return 1;
        else
            return 0;
 }
void EnQueue(LinkQueue *Q,int e){ //插入元素e为Q的新的队尾元素
        QueuePtr p=(QueuePtr)malloc(sizeof(QNode));
        if(!p) //存储分配失败
            exit(-2);//退出程序
        p->data=e;//把待插入的元素e的值赋给p的数值部分
        p->next=NULL;//由于p是新的队尾元素，所以它的指针部分指向为空
        (*Q).rear->next=p;
        (*Q).rear=p; //更改队尾元素为p
 }

int GetHead_Q(LinkQueue Q,int *e) { /* 若队列不空，则用e返回Q的队头元素，并返回1，否则返回0*/
       QueuePtr p;
       if(Q.front==Q.rear)
            return 0;
        p=Q.front->next;
        *e=p->data;
        return 1;
}
int DeQueue(LinkQueue *Q,int *e){// 若队列不空，删除Q的队头元素，用e返回其值，并返回1，否则返回0
        QueuePtr p;
        if((*Q).front==(*Q).rear)//当队头元素=队尾元素时，判断队列为空，并返回0
            return 0;
        p=(*Q).front->next; //队列不空，把队头元素指针赋给p
        *e=p->data; //e保存p数据部分的值
        (*Q).front->next=p->next;
        if((*Q).rear==p)
            (*Q).rear=(*Q).front;
        free(p);//释放队头元素
return 1;
}
pstt(){
        int max,time,task,count_task;
        int i,j,flag;
        time = 0;
        task = 0;
        count_task=0;
        while(task<counter){//当进程未执行完毕时
            flag = 0;
            for(i=0;i<counter;i++){//遍历进程
                max = 0;//最高优先级
                for(j=0;j<counter;j++){//获得最高优先级：进程可以执行，在当前时间已到达，优先级大于最大优先级
                    if((tasks[j].status==0)&&(tasks[j].come_time<=time)&&(tasks[j].priority>max))
                        max = tasks[j].priority;
                }
            if((tasks[i].status==0)&&(tasks[i].come_time<=time)){
                if(tasks[i].priority==max){//优先级最大，执行进程
                    flag = 1;//此时间片有进程执行  	 	  count_task++;
                    tasks[i].rest_slice = tasks[i].rest_slice -1;  //进程剩余时间减少 1
                    printf("%d:Process%s---No.%dslice\n",count_task,tasks[i].name,tasks[i].run_time-tasks[i].rest_slice);
                    time++;//时间片+1
                    if(tasks[i].rest_slice == 0){
                        printf("------------------------Process %s is finished\n",tasks[i].name);
                        tasks[i].end_time = time;
                        tasks[i].round_time = tasks[i].end_time -tasks[i].come_time;//周转时间
                        tasks[i].status = 1;
                        task++;    //已运行完的程序
                    }
                }
                if(flag == 0){
                    printf("%d: no process is running \n",count_task);
                    count_task++;
                    time++;//时间片+1
                }
            }
        }
    }
return 0;
}

int tt(){    /*时间片轮转*/
        int i,temp,e,ct=0;
        int k[MAX]={0};
        LinkQueue Q;
        InitQueue(&Q);//创建一个空的进程队列
        temp=tasks[0].come_time;
        for(i=0;i<counter;i++){       //找出晚到达的进程，即最大的come_time
            if(tasks[i].come_time>=temp)
                temp=tasks[i].come_time;
                tasks[i].status=1;
        }
        for(ct=0;ct<=temp;ct++){   //根据进程提交时间，依次进入队列，确定所有进程到达前的第一次执行顺序
            for(i=0;i<counter;i++){
                if(tasks[i].come_time==ct){ //如果进程提交时间等于当前依次递增的时间ct，则插入该进程
                    EnQueue(&Q,i);
                    printf("%d",i);
                    k[i]++;//记录进程重复次数
                    break;//退出当前循环
                }
                EnQueue(&Q,i);//如果没有进程提交时间等于当前 ct,即后续进程还未到达，继续插入已到达的进程
                k[i]++;
                printf("%d",i);
                ct++;
            }
       }
      for(i=0;i<counter;i++)
        printf("k[%d]=%d",i,k[i]);//输出重复次数，用于调试判断
        printf("\n"); temp=0;
        while(!QueueEmpty(Q)){   //如果队列不为空，即表明还有进程未finish
            GetHead_Q(Q,&e);   //获取当前队列的头元素，即相应进程编号-1（结构体数组的相对偏移位置）
                if(tasks[e].status==1){   //当前进程可运行标识为1
                    tasks[e].rest_slice--;//剩余时间片-1
                    printf("%d:Process 	%s----No.%dslice\n",temp,tasks[e].name,tasks[e].run_time-tasks[e].rest_slice);{    //输出执行进程的相应信息                                                    if(tasks[e].rest_slice==0)//剩余时间片为0，表明当前进程执行完毕
                    tasks[e].status=0;//置可执行标识为0
                    tasks[e].end_time=temp+1;//end_time为当前时钟记录+1
                    tasks[e].round_time=tasks[e].end_time;
                    tasks[e].come_time;//计算周转时间
                    printf("*************************Process %s is finished\n",tasks[e].name);
                }
            }
            else {
                if(k[e]>0)   //判断第一次顺序中的进程重复
                    k[e]--;
                if(k[e]==0){   //第一次因等待后续未提交进程而连续执行的进程现在不应该重复插入
                    EnQueue(&Q,e);
                }
           }
        }
        DeQueue(&Q,&e);//删掉队列头结点即已执行处理的进程
           temp++;//计时+1
}

int clear(){
        int i;
        for(i=0;i<counter;i++){
            tasks[i].begin_time=0;
            tasks[i].end_time=0;
            tasks[i].order=0;
            tasks[i].round_time=0;
            tasks[i].status=0;
            tasks[i].rest_slice=tasks[i].run_time;
        }
        return 0;
}

int pinput() {		/*进程参数手工输入   */
        int i;
        printf("please input the process counter:");
        scanf("%d",&counter);
        for(i=0;i<counter;i++){
            printf("************************** \n",i+1);
            printf("Name     :    ");
            scanf("%s",tasks[i].name);
            printf("Come_time:");
            scanf("%d",&tasks[i].come_time);
            printf("Run_time :");
            scanf("%d",&tasks[i].run_time);
            printf("Priority :");
            scanf("%d",&tasks[i].priority);
            tasks[i].begin_time=0;
            tasks[i].end_time=0;
            tasks[i].order=0;
            tasks[i].round_time=0;
            tasks[i].status=0;
            tasks[i].rest_slice=tasks[i].run_time;
        }
    return 0;
}
int poutput(){ /*调度结果输出*/
        int i;
        float turn_round_time=0,w=0;
        printf("name  come  run   priority  end  order  round  \n");
        for(i=0;i<counter;i++){
            turn_round_time+=tasks[i].round_time;
            w+=(tasks[i].round_time/tasks[i].run_time);
            printf("%3s  %3d  %4d   %5d    %4d  %4d  %4d\n",tasks[i].name,tasks[i].come_time,tasks[i].run_time,tasks[i].priority,tasks[i].end_time,tasks[i].order,tasks[i].round_time);
        }
        printf("average_turn_round_time=%5.2f\n",turn_round_time/counter);
        return 0;
}

