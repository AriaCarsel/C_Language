#include<stdio.h>
#define MAX 10
#define MAX_PROCESS  10
#define MAX_PRIORITY 5
struct task_struct{
     char name[10];           /*��������*/
     int number;              /*���̱��*/
     int come_time;           /*����ʱ��*/
     int begin_time;          /*��ʼ����ʱ��*/
     int run_time;            /*����ʱ��*/
     int end_time;            /*���н���ʱ*/
     int round_time;          /*��תʱ��*/
     int priority;            /*���ȼ�*/
     int order;               /*���д���*/
     int rest_slice;          /*ʣ��ʱ��Ƭ��*/
     int status;              /*run_flag���ȱ�־*/
   }tasks[MAX];
   //tt ����ṹ��
typedef struct QNode {
        int data;
        struct QNode *next;
}QNode,*QueuePtr;
typedef struct {
        QueuePtr front;
        QueuePtr rear;
}LinkQueue;
//pstt����ṹ��
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

int counter;     /*ʵ�ʽ��̸���*/
int fcfs();      /*�����ȷ���*/
int ps();        /*���ȼ�����*/
int sjf();       /*����ҵ����*/
int pstt();
int tt();
int pinput();    /*���̲�������*/
int poutput();   /*���Ƚ�����*/
int clear();     /*��ʼ������*/

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
       printf("\n\nTT    :�\\n");
       tt();
       poutput();

       clear();
       printf("\n\nPSTT  :\n");
       pstt();
       poutput();
}

int fcfs(){ /*�����ȷ���*/
        int i,j,sum;
        struct task_struct t;
        for(i=0;i<=counter-1;i++){ //�Խ��̸��ݵ���ʱ���������;
            for(j=0;j<counter-i-1;j++){
                if(tasks[j].come_time>tasks[j+1].come_time){
                        t=tasks[j+1];
                        tasks[j+1]=tasks[j];
                        tasks[j]=t;
                }
            }
        }     //���絽��Ľ���
        tasks[0].begin_time=tasks[0].come_time;
        tasks[0].end_time=tasks[0].begin_time+tasks[0].run_time;
        tasks[0].round_time=tasks[0].run_time;
        sum=tasks[0].end_time;
        tasks[0].order=1;
        for(i=1;i<counter;i++) {   //�������д������п�ʼʱ�䡢���н���ʱ�䡢���� ʱ�䡢��תʱ�䡢ʣ��ʱ��Ƭ��
                tasks[i].order=i+1;//����         //���п�ʼ������ʱ
                if(tasks[i].come_time<=tasks[i-1].end_time){
                    sum=sum+tasks[i].run_time;
                }
                else{
                    sum=tasks[i].come_time+tasks[i].run_time;
                }
                tasks[i].end_time=sum;
                tasks[i].begin_time=tasks[i].end_time-tasks[i].run_time;
                tasks[i].round_time=tasks[i].end_time-tasks[i].come_time;//��תʱ��
        }
}

int ps(){        /*���ȼ�����*/
        int i,j,k;
        int wait[counter];//�ѵ���Ľ��̶���
        int time=0,n,flag;
        for(i=0;i<counter;i++){
                wait[i]=0;
        }
        flag=0;
        for(i=0;i<counter;i++){//Ҫִ�� counter ������ ,һ���������н�������µ���Ķ���
            flag=0;
            while(flag==0){//�Ƿ��е���ĳ���
                for(k=0;k<counter;k++){//��Щ���򵽴�
                    if((tasks[k].come_time<=time)&&(tasks[k].status==0)){
                        wait[k]=tasks[k].priority;//������ĳ���Ϊ�����ȼ�
                        flag++;
                        }
                    else
                        wait[k]=0;//û����ĺ��ѽ�������Ϊ 0
                    }
                if(flag==0)
                        time++;//���û�е���ĳ���ʱ���һ
            }
            n=0;
            for(j=0;j<counter;j++){//�����������
                if(wait[j]>wait[n]){
                        n=j;//ѡ�����ȼ��ߵ�
                }
                if((wait[j]==wait[n])&&(tasks[j].come_time<tasks[n].come_time)){
                    n=j;//�����ȼ���ߵ���ѡ������ʱ��̵�
                }
            }
            if(tasks[n].status==0){//�޸Ĵ˴����еĳ���� pcb
                tasks[n].begin_time=time;
                time=time+tasks[n].run_time;
                tasks[n].order=i+1;
                tasks[n].end_time=time;
                tasks[n].round_time=tasks[n].end_time-tasks[n].come_time;
                tasks[n].status=1;
                }
        }
}

int sjf() {       /*����ҵ����*/
        int ontime,wait,i,j;
        int order;
        struct task_struct t;
        ontime = 0;      //ontime ��ʾ cpu ��ռ��ʱ��
        wait = 0;       //�ȴ����еĽ��̸���
        order=2;
        for(i=1;i<=counter-1;i++)     {   //�Խ��̸��ݵ���ʱ���������;
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
                    if(tasks[i].come_time>=ontime){      //����ʱ����� CPU ��ռ��ʱ�䣬��ý��̵���ʱ CPU ����һ����ռ�ã���� ��ȴ�������
                        if(wait==0)  {    //���ý��̽���ȴ���ʱ�������������Ľ���
                            tasks[i].order = i+1;   //�ý�����Ϊ��һʹ�� CPU �Ľ���
                            tasks[i].begin_time = tasks[i].come_time;
                            tasks[i].end_time = tasks[i].begin_time + tasks[i].run_time;
                            ontime=tasks[i].end_time;
                            order ++;
                            }
                         else {
                            wait++;       //�ý��̽���ʱ������ wait �����̴��ڵȴ���
                            tasks[i-wait+1].begin_time=ontime;  //�� wait �����̣�֮ǰ�Ѱ����ʱ���ź� ���еġ���һ������Ϊ��һʹ�� CPU �Ľ���
                            tasks[i-wait+1].end_time=tasks[i-wait+1].begin_time+tasks[i-wait+1].run_time;
                            tasks[i-wait+1].round_time=tasks[i-wait+1].end_time-tasks[i-wait+1].come_time;
                            ontime=ontime+tasks[i-wait+1].run_time;
                            tasks[i-wait+1].order=order;
                            order++;
                            wait--;

                            int m = i;
                            int n;
                            int flag = 1;
                            for(;m>=(i-wait)&&flag;m--)  {        //�����������ʱ������
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
                for(;m>(i-wait+1)&&flag;m--){      //���������ʱ������
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
                    tasks[i].begin_time  = tasks[i-1].end_time; //���̿�ʼ����ʱ��Ϊ��һ�������н�����ʱ��
                    tasks[i].end_time = tasks[i].begin_time + tasks[i].run_time;             //ÿ�����̵Ľ���ʱ��Ϊ�俪ʼʱ��+����ʱ��
                    tasks[i].round_time=tasks[i].end_time-tasks[i].come_time;   //������תʱ��=����ʱ��-����ʱ��
                    }
                }
            for(i=0;i<=counter-1;i++) {      //�Խ������¸��ݵ���ʱ���������;
                for(j=0;j<counter-i-1;j++){
                    if(tasks[j].come_time>tasks[j+1].come_time){
                        t=tasks[j+1];
                        tasks[j+1]=tasks[j];
                        tasks[j]=t;
                        }
                    }
                }
    }
void InitQueue(LinkQueue *Q){ //����һ���ն���Q
        (*Q).front=(*Q).rear=(QueuePtr)malloc(sizeof(QNode));
        if(!(*Q).front)
        exit(-2);//����ʧ�����˳�
        (*Q).front->next=NULL;
 }
 int QueueEmpty(LinkQueue Q){// ��QΪ�ն��У��򷵻�1�����򷵻�0
        if(Q.front->next==NULL)//ͷԪ��ָ�벿��ָ��Ϊ�գ��ж�Ϊ�ն��У�����1
            return 1;
        else
            return 0;
 }
void EnQueue(LinkQueue *Q,int e){ //����Ԫ��eΪQ���µĶ�βԪ��
        QueuePtr p=(QueuePtr)malloc(sizeof(QNode));
        if(!p) //�洢����ʧ��
            exit(-2);//�˳�����
        p->data=e;//�Ѵ������Ԫ��e��ֵ����p����ֵ����
        p->next=NULL;//����p���µĶ�βԪ�أ���������ָ�벿��ָ��Ϊ��
        (*Q).rear->next=p;
        (*Q).rear=p; //���Ķ�βԪ��Ϊp
 }

int GetHead_Q(LinkQueue Q,int *e) { /* �����в��գ�����e����Q�Ķ�ͷԪ�أ�������1�����򷵻�0*/
       QueuePtr p;
       if(Q.front==Q.rear)
            return 0;
        p=Q.front->next;
        *e=p->data;
        return 1;
}
int DeQueue(LinkQueue *Q,int *e){// �����в��գ�ɾ��Q�Ķ�ͷԪ�أ���e������ֵ��������1�����򷵻�0
        QueuePtr p;
        if((*Q).front==(*Q).rear)//����ͷԪ��=��βԪ��ʱ���ж϶���Ϊ�գ�������0
            return 0;
        p=(*Q).front->next; //���в��գ��Ѷ�ͷԪ��ָ�븳��p
        *e=p->data; //e����p���ݲ��ֵ�ֵ
        (*Q).front->next=p->next;
        if((*Q).rear==p)
            (*Q).rear=(*Q).front;
        free(p);//�ͷŶ�ͷԪ��
return 1;
}
pstt(){
        int max,time,task,count_task;
        int i,j,flag;
        time = 0;
        task = 0;
        count_task=0;
        while(task<counter){//������δִ�����ʱ
            flag = 0;
            for(i=0;i<counter;i++){//��������
                max = 0;//������ȼ�
                for(j=0;j<counter;j++){//���������ȼ������̿���ִ�У��ڵ�ǰʱ���ѵ�����ȼ�����������ȼ�
                    if((tasks[j].status==0)&&(tasks[j].come_time<=time)&&(tasks[j].priority>max))
                        max = tasks[j].priority;
                }
            if((tasks[i].status==0)&&(tasks[i].come_time<=time)){
                if(tasks[i].priority==max){//���ȼ����ִ�н���
                    flag = 1;//��ʱ��Ƭ�н���ִ��  	 	  count_task++;
                    tasks[i].rest_slice = tasks[i].rest_slice -1;  //����ʣ��ʱ����� 1
                    printf("%d:Process%s---No.%dslice\n",count_task,tasks[i].name,tasks[i].run_time-tasks[i].rest_slice);
                    time++;//ʱ��Ƭ+1
                    if(tasks[i].rest_slice == 0){
                        printf("------------------------Process %s is finished\n",tasks[i].name);
                        tasks[i].end_time = time;
                        tasks[i].round_time = tasks[i].end_time -tasks[i].come_time;//��תʱ��
                        tasks[i].status = 1;
                        task++;    //��������ĳ���
                    }
                }
                if(flag == 0){
                    printf("%d: no process is running \n",count_task);
                    count_task++;
                    time++;//ʱ��Ƭ+1
                }
            }
        }
    }
return 0;
}

int tt(){    /*ʱ��Ƭ��ת*/
        int i,temp,e,ct=0;
        int k[MAX]={0};
        LinkQueue Q;
        InitQueue(&Q);//����һ���յĽ��̶���
        temp=tasks[0].come_time;
        for(i=0;i<counter;i++){       //�ҳ�����Ľ��̣�������come_time
            if(tasks[i].come_time>=temp)
                temp=tasks[i].come_time;
                tasks[i].status=1;
        }
        for(ct=0;ct<=temp;ct++){   //���ݽ����ύʱ�䣬���ν�����У�ȷ�����н��̵���ǰ�ĵ�һ��ִ��˳��
            for(i=0;i<counter;i++){
                if(tasks[i].come_time==ct){ //��������ύʱ����ڵ�ǰ���ε�����ʱ��ct�������ý���
                    EnQueue(&Q,i);
                    printf("%d",i);
                    k[i]++;//��¼�����ظ�����
                    break;//�˳���ǰѭ��
                }
                EnQueue(&Q,i);//���û�н����ύʱ����ڵ�ǰ ct,���������̻�δ������������ѵ���Ľ���
                k[i]++;
                printf("%d",i);
                ct++;
            }
       }
      for(i=0;i<counter;i++)
        printf("k[%d]=%d",i,k[i]);//����ظ����������ڵ����ж�
        printf("\n"); temp=0;
        while(!QueueEmpty(Q)){   //������в�Ϊ�գ����������н���δfinish
            GetHead_Q(Q,&e);   //��ȡ��ǰ���е�ͷԪ�أ�����Ӧ���̱��-1���ṹ����������ƫ��λ�ã�
                if(tasks[e].status==1){   //��ǰ���̿����б�ʶΪ1
                    tasks[e].rest_slice--;//ʣ��ʱ��Ƭ-1
                    printf("%d:Process 	%s----No.%dslice\n",temp,tasks[e].name,tasks[e].run_time-tasks[e].rest_slice);{    //���ִ�н��̵���Ӧ��Ϣ                                                    if(tasks[e].rest_slice==0)//ʣ��ʱ��ƬΪ0��������ǰ����ִ�����
                    tasks[e].status=0;//�ÿ�ִ�б�ʶΪ0
                    tasks[e].end_time=temp+1;//end_timeΪ��ǰʱ�Ӽ�¼+1
                    tasks[e].round_time=tasks[e].end_time;
                    tasks[e].come_time;//������תʱ��
                    printf("*************************Process %s is finished\n",tasks[e].name);
                }
            }
            else {
                if(k[e]>0)   //�жϵ�һ��˳���еĽ����ظ�
                    k[e]--;
                if(k[e]==0){   //��һ����ȴ�����δ�ύ���̶�����ִ�еĽ������ڲ�Ӧ���ظ�����
                    EnQueue(&Q,e);
                }
           }
        }
        DeQueue(&Q,&e);//ɾ������ͷ��㼴��ִ�д���Ľ���
           temp++;//��ʱ+1
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

int pinput() {		/*���̲����ֹ�����   */
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
int poutput(){ /*���Ƚ�����*/
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

