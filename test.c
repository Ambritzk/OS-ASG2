#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

struct Process{
	char* pid;
	int arrival_time;
	int burst_time;
	bool disrupt_flag;
	int interruptions;
	int TimeQuantum;
	struct Process* next;
	int original_burst;
	int completion_time;
};
void Deepcopy(struct Process* dest, struct Process *source){
	dest->arrival_time = source->arrival_time;
	dest->burst_time = source->burst_time;
	dest->disrupt_flag = source->disrupt_flag;
	strcpy(dest->pid, source->pid);
	dest->next = NULL;
	dest->interruptions = source->interruptions;
	dest->TimeQuantum = source->TimeQuantum;
}


struct  GanttNode{
	char* name;
	int start;
	int end;
	struct GanttNode* next;
};

struct GanttQueue{
	struct GanttNode* head;
	struct GanttNode* tail;	
};
bool IsChartEmpty(struct GanttQueue* q){
	return (q->head == NULL);
}

void GanttInsert(struct GanttQueue* q,struct Process* proc, int start, int end){
	
	struct GanttNode* node = (struct GanttNode*) malloc(sizeof(struct GanttNode));
	
	if(end == -1){
		node->name = (char* ) malloc(sizeof(char) * (5));
		strcpy(node->name,"Idle");
	}
	else{
		
		node->name = (char*) malloc(sizeof(char) * (strlen(proc->pid) + 1));
		
		strcpy(node->name,proc->pid);
	}
		
	node->end = end;
	node->start = start;
	node->next = NULL;

	
	if(IsChartEmpty(q)){
		
		q->head = q->tail = node;
	}
	else{
		q->tail->next = node;
		q->tail = node;
	}
	

}
void GanttDisplay(struct GanttQueue* q){
	if(q == NULL || IsChartEmpty(q)){
		return;
	}
	struct GanttNode* temp = q->head;
	while(temp != NULL){
		printf("%s\t|",temp->name);
		temp = temp->next;
	}
	temp = q->head;
	printf("\n");
	while(temp != NULL){
		if(temp->start == -1){
			printf("%d\t|",temp->start);
		}
		else{
			printf("%d\t|",temp->start);
		}
		if(temp->next == NULL){
			printf("%d",temp->end);
		}
		temp = temp->next;
	}

}
void FreeChart(struct GanttQueue* q){
	if(IsChartEmpty(q)){
		return;
	}
	struct GanttNode* temp = q->head;
	while(temp != NULL){
		free(temp->name);
		free(temp);
		temp = temp->next;
	}


}

//Below is the code for processQueue
struct ProcessQueue{
	int TimeQuantum;
	struct Process* head;
	struct Process* tail;
	
	
};
void DisplayProcess(struct Process* ptr){
	if(ptr == NULL) return;
	printf("%s, ",ptr->pid);
}
void Insert(struct ProcessQueue* ptr, struct Process* proc){
	
	if(proc == NULL)return;

	if(ptr->head == NULL){
		ptr->head = ptr->tail = proc;
	}
	else{
		ptr->tail->next = proc;
		ptr->tail = proc;
	}
}

struct Process* Pop(struct ProcessQueue* ptr){
	if(ptr->head == NULL) return NULL;
	if(ptr->head->next == NULL){
		struct Process* temp = ptr->head;
		ptr->head = ptr->tail = NULL;
		return temp;
	}
	else{
		struct Process* temp = ptr->head;
		ptr->head = temp->next;
		temp->next = NULL;
		return temp;
	}
}
void DisplayQueue(struct ProcessQueue* ptr){
	if(ptr->head == NULL){
		printf("Empty\n");
		return;
	}
	
	struct Process* temporary_ptr = ptr->head;
	while(temporary_ptr != NULL){
		DisplayProcess(temporary_ptr);
		temporary_ptr = temporary_ptr->next;
	}

 }
bool IsEmpty(struct ProcessQueue* ptr){
	return ptr->head == NULL;
}

void FreeMemoryForProcessQueue(struct ProcessQueue* ptr){
	if(ptr->head == NULL){
		return;
	}
	struct Process* temp = ptr->head;
	while(ptr->head != NULL){
		ptr->head = ptr->head->next;
		free(temp);
		temp = ptr->head;
	}
	ptr->tail = NULL;
}

//code for ProcessQueue ends here



struct Process* ReadProcessesFromFile(int* sizeOfList){
	char buffer[256];
	FILE* fp = fopen("processes.txt","r");

	struct Process* ListOfProcesses = NULL;
	*sizeOfList = 0;
	int ind = 0;
	if(fp == NULL){
		printf("Couldn't open \"processes.txt\".");
		return NULL;
	}

	while(fgets(buffer,sizeof(buffer),fp) != NULL){
		char* ptr;
		char* pid;
		char* arv;
		char* brst;
		char* dsprs;


		

		for(int i = 1; i <= 4; i++){
			if(i == 1){
				ptr = strtok(buffer," ");
			}
			else{
				ptr = strtok(NULL," ");
			}
			
			if(ptr == NULL){
				break;
			}
			if(i == 1){
				pid = ptr;
			}
			if(i == 2){
				arv = ptr;
			}
			if(i == 3){
				brst = ptr;
			}
			if(i == 4){
				dsprs = ptr;
				break;
			}
		}

		if(arv == NULL || brst == NULL || dsprs == NULL || pid == NULL){
			return NULL;
		}
	
		if(ListOfProcesses == NULL){
			ListOfProcesses = (struct Process*) malloc(sizeof(struct Process) * (ind + 1));
			ListOfProcesses[ind].arrival_time = atoi(arv);
			ListOfProcesses[ind].burst_time = atoi(brst);
			ListOfProcesses[ind].disrupt_flag = atoi(dsprs);
			ListOfProcesses[ind].interruptions = 0;
			ListOfProcesses[ind].TimeQuantum = - 1;
			ListOfProcesses[ind].original_burst = atoi(brst);
			ListOfProcesses[ind].pid = (char*) malloc(sizeof(char) * (strlen(pid) + 1));
			strcpy(ListOfProcesses[ind].pid,pid);

			*sizeOfList = ++ind;
		}
		else{
			
			ListOfProcesses = realloc(ListOfProcesses, sizeof(struct Process) * (ind + 1));
			ListOfProcesses[ind].arrival_time = atoi(arv);
			ListOfProcesses[ind].burst_time = atoi(brst);
			ListOfProcesses[ind].disrupt_flag = atoi(dsprs);
			ListOfProcesses[ind].interruptions = 0;
			ListOfProcesses[ind].TimeQuantum = - 1;
			ListOfProcesses[ind].original_burst = atoi(brst);
			ListOfProcesses[ind].pid = (char*) malloc(sizeof(char) * (strlen(pid) + 1));
			strcpy(ListOfProcesses[ind].pid,pid);
			*sizeOfList = ++ind;
		}
	}

	return ListOfProcesses;
}


void SortProcesses(struct Process* ListOfProcesses, int num){
	for(int i = 0; i < num - 1; i++){
		for(int j = 0; j < num - 1 - i; j++){
			if(ListOfProcesses[j].arrival_time > ListOfProcesses[j + 1].arrival_time){
				struct Process temp = ListOfProcesses[j];
				ListOfProcesses[j] = ListOfProcesses[j + 1];
				ListOfProcesses[j + 1] = temp;
			}
		}
	}
}
void TemporaryExecution(int* time, struct Process* ListOfProcesses, int ind, int num, int id,struct ProcessQueue* q1, struct ProcessQueue* q2, struct ProcessQueue* q3,struct GanttQueue* gq){

	if(*time == ListOfProcesses[ind].arrival_time){
		return;
	}
	
	struct Process* proc = NULL;
	int time_start = -1;
	if(id == 2){
		if(IsEmpty(q2)){
			return;
		}
		while(ind < num && (!IsEmpty(q2) || proc != NULL)){
			
			if(proc == NULL){
				proc = Pop(q2);
				proc->TimeQuantum = q2->TimeQuantum;
				time_start = *time;
			}
			else{
				proc->burst_time--;
				proc->TimeQuantum--;
				*time += 1;
				if(proc->burst_time == 0){
					printf("Time:%d, Q2 executed %s for %d units -> %s finished execution\n",*time,proc->pid, *time - time_start, proc->pid);
					GanttInsert(gq,proc,time_start,*time);
					proc->completion_time = *time;
					proc = NULL;	
				}
				else if(proc->TimeQuantum == 0){
					if(proc != NULL){
						printf("Time:%d, Q2 executed %s for %d units -> %s moved to Q3\n",*time,proc->pid, *time - time_start, proc->pid);
						GanttInsert(gq,proc,time_start,*time);
						Insert(q3,proc);
						proc = NULL;
					}
				}

				if(*time == ListOfProcesses[ind].arrival_time){
					if(proc == NULL){
						return;
					}
					if(proc->burst_time > 0){
						
						proc->interruptions += 1;
						proc->interruptions %= 3;
						if(proc->interruptions == 0){
							printf("Time:%d, Q2 executes %s for %d units -> %s is interrupted and moves to Q1 (PROMOTED!)\n",*time,proc->pid, *time - time_start, proc->pid);
							GanttInsert(gq,proc,time_start,*time);
							Insert(q1,proc);
						}
						else{
							if(proc->disrupt_flag == 0){
								proc->TimeQuantum = q2->TimeQuantum;
							}
							printf("Time:%d, Q2 executes %s for %d units -> %s is interrupted and moves to Q2\n",*time,proc->pid, *time - time_start, proc->pid);
							GanttInsert(gq,proc,time_start,*time);
							Insert(q2,proc);
						}
						return;
					}
				}
				
			}
		}
	}
	else{
		if(IsEmpty(q3)){
			return;
		}
		while(!IsEmpty(q3) || proc != NULL){ //  This queue operates on FCFS basis
			if(proc == NULL){
				proc = Pop(q3);
				time_start = *time;
			}
			else{
				proc->burst_time--;
				*time += 1;
				if(proc->burst_time == 0){
					printf("Time:%d, Q3 executes %s for %d units -> %s finishes execution\n",*time,proc->pid,*time - time_start,proc->pid);
					GanttInsert(gq,proc,time_start,*time);
					
					proc->completion_time = *time;
					proc = NULL;
				}
				if(*time == ListOfProcesses[ind].arrival_time){
					if(proc == NULL){
						return;
					}

					if(proc->burst_time != 0){
						proc->interruptions += 1;
						proc->interruptions %= 3;
						if(proc->interruptions == 0){
							printf("Time:%d, Q3 executes %s for %d units -> %s is interrupted and pushed into Q2 (PROMOTED!)\n",*time,proc->pid,*time - time_start,proc->pid);
							GanttInsert(gq,proc,time_start,*time);
							Insert(q2,proc);
						}
						else{
							printf("Time:%d, Q3 executes %s for %d -> %s is interrupted and pushed to Q3\n",*time,proc->pid,*time - time_start, proc->pid);
							GanttInsert(gq,proc,time_start,*time);
							Insert(q3,proc);
						}
						return;
					}
				}
				
			}
		}
	}
}


void InitialScheduling(int*time, struct Process* ListOfProcesses, int num,struct ProcessQueue* q1,struct ProcessQueue* q2,struct ProcessQueue* q3,struct GanttQueue* gq){
	int ind = 0;
	struct Process* Proc = NULL;
	int time_start = -1;
	int ab = -1;
	while(ind <= num){
		if(Proc == NULL && ind == num) return;
		if(*time == ListOfProcesses[ind].arrival_time){
			
			while(*time == ListOfProcesses[ind].arrival_time){

				Insert(q1,&ListOfProcesses[ind++]);
				
				printf("Time:%d, %s arrived\n",*time,ListOfProcesses[ind - 1].pid);
				if(ind == num && Proc == NULL) return; // if it's the last process, then we return since the next check will lead to a segmentation error
			}
			
		}
		else{
			if(!IsEmpty(q1) || Proc != NULL){
				if(Proc == NULL){
					Proc = Pop(q1);
					Proc->TimeQuantum = q1->TimeQuantum;
					time_start = *time;
				}

				Proc->burst_time--;
				Proc->TimeQuantum--;
				*time += 1;
				

				
				if(Proc->burst_time == 0){
					printf("Time:%d, Q1 executes %s for %d -> %s finishes execution\n",*time,Proc->pid,*time - time_start,Proc->pid);
					GanttInsert(gq,Proc,time_start,*time);
					Proc->completion_time = *time;
					Proc = NULL;
				}				


				if(Proc != NULL && Proc->TimeQuantum == 0){
					
					printf("Time:%d, Q1 executes %s for %d units -> %s moves to Q2\n",*time,Proc->pid,*time - time_start,Proc->pid);
					GanttInsert(gq,Proc,time_start,*time);
					Insert(q2,Proc);
					Proc = NULL;
				}

				if(Proc == NULL && ind == num){
					return;
				} 
				
				
			}
			else{
				TemporaryExecution(time,ListOfProcesses,ind,num,2,q1,q2,q3,gq);
				TemporaryExecution(time,ListOfProcesses,ind,num,3,q1,q2,q3,gq);
				if(*time < ListOfProcesses[ind].arrival_time){
					printf("Q1 remains idle for %d units since no process in any of the queues\n", ListOfProcesses[ind].arrival_time - *time);
					GanttInsert(gq,NULL,*time,-1);
					*time = ListOfProcesses[ind].arrival_time; // next iteration will handle this
				}
			}
		}
	}


}

void SimulateQueue(int* time, int id, struct ProcessQueue* q1,struct ProcessQueue* q2,struct ProcessQueue* q3,struct GanttQueue* gq){
	
	int time_start = 0;
	struct Process* proc = NULL;
	if(id == 1){
		while(!IsEmpty(q1) || proc != NULL){
			if(proc == NULL){
				proc = Pop(q1);
				proc->TimeQuantum = q1->TimeQuantum;
				time_start = *time;
			}
			else{
				proc->TimeQuantum--;
				proc->burst_time--;
				*time += 1;
				if(proc->burst_time == 0){
					printf("Time:%d, Q1 executed %s for %d units -> %s finished execution\n",*time,proc->pid,*time - time_start,proc->pid);
					GanttInsert(gq,proc,time_start,*time);
					proc->completion_time = *time;
					proc = NULL;
				}
				if(proc != NULL && proc->TimeQuantum == 0){
					printf("Time:%d, Q1 executed %s for %d units -> %s moved to Q2\n",*time,proc->pid,*time - time_start,proc->pid);
					GanttInsert(gq,proc,time_start,*time);
					Insert(q2,proc);
					proc = NULL;
				}


			}
		}
	}
	else if(id == 2){
		while(!IsEmpty(q2) || proc != NULL){
			if(proc == NULL){
				proc = Pop(q2);
				proc->TimeQuantum = q2->TimeQuantum;
				time_start = *time;
			}
			else{
				proc->TimeQuantum--;
				proc->burst_time--;
				*time += 1;
				if(proc->burst_time == 0){
					printf("Time:%d, Q2 executed %s for %d units -> %s finished execution\n",*time,proc->pid,*time - time_start,proc->pid);
					GanttInsert(gq,proc,time_start,*time);
					proc->completion_time = *time;
					proc = NULL;
				}
				if(proc != NULL && proc->TimeQuantum == 0){
					printf("Time:%d, Q2 executed %s for %d units -> %s moved to Q3\n",*time,proc->pid,*time - time_start,proc->pid);
					GanttInsert(gq,proc,time_start,*time);
					Insert(q3,proc);
					proc = NULL;
				}


			}
		}
	}



}

void SimulateLowest(int* time, struct ProcessQueue* q3,struct GanttQueue* gq){
	if(IsEmpty(q3)){
		return;
	}
	int time_start = 0;
	struct Process* proc = NULL;
	
	while(!IsEmpty(q3) || proc != NULL){
		
		if(proc == NULL){
			proc = Pop(q3);
			time_start = *time;
		}
		else{
			proc->burst_time--;
			*time += 1;
			if(proc->burst_time == 0){
				printf("Time:%d, Q3 executes %s for %d units -> %s finishes execution\n",*time,proc->pid, *time - time_start, proc->pid);
				GanttInsert(gq,proc,time_start,*time);
				proc->completion_time = *time;
				proc = NULL;
			}
		}
	}
}


void final(int*time, struct Process* ListOfProcesses, int num,struct ProcessQueue* q1,struct ProcessQueue* q2,struct ProcessQueue* q3, struct GanttQueue* gq){
	
	InitialScheduling(time,ListOfProcesses,num,q1,q2,q3,gq);
	while(!IsEmpty(q1) || !IsEmpty(q2) || !IsEmpty(q3)){
		SimulateQueue(time,1,q1,q2,q3,gq);
		SimulateQueue(time,2,q1,q2,q3,gq);

		SimulateLowest(time,q3,gq);
	}
}

void FinalTable(struct Process* ListOfProcesses, int sized){
	printf("P\tAT\tBT\tCT\tTAT\tWT\n");
	for(int i = 0; i < sized; i++){
		struct Process* proc = &ListOfProcesses[i];
		int tat = proc->completion_time - proc->arrival_time;
		int wait = tat - proc->original_burst;
		printf("%s\t%d\t%d\t%d\t%d\t%d\n",proc->pid, proc->arrival_time, proc->original_burst,proc->completion_time,tat,wait);
	}
}



void main(){
	struct ProcessQueue pq = {2};
	struct GanttQueue gq = {NULL,NULL};
	int number = 0;
	struct Process* ListOfProcesses = ReadProcessesFromFile(&number);
	SortProcesses(ListOfProcesses,number);
	int time = 0;
	struct ProcessQueue q1 = {2,NULL,NULL};
	struct ProcessQueue q2 = {4,NULL,NULL};
	struct ProcessQueue q3 = {-1,NULL,NULL};




	final(&time,ListOfProcesses,number,&q1,&q2,&q3,&gq);
	printf("GANTT CHART\n");
	GanttDisplay(&gq);
	printf("\nFINAL TABLE\n");




	FinalTable(ListOfProcesses,number);
	
	for(int i = 0; i < number; i++){
		free(ListOfProcesses[i].pid);
	}


	FreeChart(&gq);
	free(ListOfProcesses);

}
