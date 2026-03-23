#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

struct Process{
	char pid[2];
	int arrival_time;
	int burst_time;
	bool disrupt_flag;
	int interruptions;
	int TimeQuantum;
	struct Process* next;
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

void DisplayProcess(struct Process *ptr){
	printf("Process Name: %s, arv = %d, brst = %d, dsrpt %d\n", ptr->pid, ptr->arrival_time, ptr->burst_time, ptr->disrupt_flag);
}

//Below is the code for processQueue
struct ProcessQueue{
	int TimeQuantum;
	struct Process* head;
	struct Process* tail;

	
};

void Insert(struct ProcessQueue* ptr, struct Process* proc){
	
	struct Process* newProcess = proc;
	if (newProcess == NULL) return;
	Deepcopy(newProcess, proc);

	if(ptr->head == NULL){
		ptr->head = ptr->tail = proc;
	}
	else{
		ptr->tail->next = proc;
		ptr->tail = proc;
	}
}
void PopAndDelete(struct ProcessQueue* ptr){
	if(ptr->head == NULL) return;
	if(ptr->head->next == NULL){
		free(ptr->head);
		ptr->head = ptr->tail = NULL;
	}
	else{
		struct Process* temp = ptr->head;
		ptr->head = temp->next;
		free(temp);
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
void TemporaryExecution(int* time, struct Process* ListOfProcesses, int ind, int num, int id,struct ProcessQueue* q1, struct ProcessQueue* q2, struct ProcessQueue* q3){
	struct Process* proc = NULL;
	int time_start = -1;
	if(id == 2){
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
					printf("Q2 executed %s for %d units -> %s finished execution\n",proc->pid, *time - time_start, proc->pid);
					proc = NULL;	
				}
				else if(proc->TimeQuantum == 0){
					if(proc != NULL){
						printf("Q2 executed %s for %d units -> %s moved to Q3\n",proc->pid, *time - time_start, proc->pid);
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
							printf("Q2 executes %s for %d units -> %s is interrupted and moves to Q1\n",proc->pid, *time - time_start, proc->pid);
							Insert(q1,proc);
						}
						else{
							if(proc->disrupt_flag == 0){
								proc->TimeQuantum = q2->TimeQuantum;
							}
							printf("Q2 executes %s for %d units -> %s is interrupted and moves to Q2\n",proc->pid, *time - time_start, proc->pid);
							Insert(q2,proc);
						}
						return;
					}
				}
				
			}
		}
		if(*time < ListOfProcesses[ind].arrival_time){
			if(IsEmpty(q3)){
				return;
			}
			else{
				TemporaryExecution(time,ListOfProcesses,ind,num,3,q1,q2,q3);
			}
		}
	}
	else{
		while(!IsEmpty(q3) || proc != NULL){ //  This queue operates on FCFS basis
			if(proc == NULL){
				proc = Pop(q3);
				time_start = *time;
			}
			else{
				proc->burst_time--;
				*time += 1;
				if(proc->burst_time == 0){
					printf("Q3 executes %s for %d units -> %s finishes execution\n",proc->pid,*time - time_start,proc->pid);
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
							printf("Q3 executes %s for %d units -> %s is interrupted and pushed into Q2\n", proc->pid,*time - time_start,proc->pid);
							Insert(q2,proc);
						}
						else{
							printf("Q3 executes %s for %d -> %s is interrupted and pushed to Q3\n",proc->pid,*time - time_start, proc->pid);
							Insert(q3,proc);
						}
						return;
					}
				}
				
			}
		}
	}
}


void InitialScheduling(int*time, struct Process* ListOfProcesses, int num,struct ProcessQueue* q1,struct ProcessQueue* q2,struct ProcessQueue* q3){
	int ind = 0;
	struct Process* Proc = NULL;
	int time_start = -1;
	while(ind < num){
		if(*time == ListOfProcesses[ind].arrival_time){
			if(Proc != NULL){
				Proc->interruptions++;
				Proc->interruptions %= 3;
				if(Proc->disrupt_flag == 0){
					Proc->TimeQuantum = q1->TimeQuantum;
				}
				printf("Time:%d Q1 executed %s for %d units -> %s is interrupted and moved to Q1\n",*time,Proc->pid,*time - time_start,Proc->pid);
			}
			Insert(q1,&ListOfProcesses[ind++]);
			if(Proc != NULL){
				Insert(q1,Proc);
				Proc = NULL;
			}
			printf("Time:%d, %s arrived\n",*time,ListOfProcesses[ind - 1].pid);
			if(ind == num) return; // if it's the last process, then we return
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
					Proc = NULL;
				}				
				if(Proc != NULL && Proc->TimeQuantum == 0){
					printf("Time:%d, Q1 executes %s for %d units -> %s moves to Q2\n",*time,Proc->pid,*time - time_start,Proc->pid);
					Insert(q2,Proc);
					Proc = NULL;
				}

			}
			else{
				TemporaryExecution(time,ListOfProcesses,ind,num,2,q1,q2,q3);
				if(*time < ListOfProcesses[ind].arrival_time){
					printf("Q1 remains idle for %d units since no process in any of the queues\n", ListOfProcesses[ind].arrival_time - *time);
					*time = ListOfProcesses[ind].arrival_time;
				}
			}
		}
	}


}




void SimulateQueue(int* time, int id, struct ProcessQueue* q1,struct ProcessQueue* q2,struct ProcessQueue* q3){
	int time_start = 0;
	struct ProcessQueue* q = (id == 1)?q1:q2;
	struct Process* proc = NULL;
	while(!IsEmpty(q) || proc != NULL){
		if(proc == NULL){
			proc = Pop(q);
			proc->TimeQuantum = q->TimeQuantum;
			time_start = *time;
		}
		else{
			proc->burst_time--;
			proc->TimeQuantum--;
			*time += 1;
			if(proc->burst_time == 0){
				printf("Time:%d, ",*time);
				if(id == 1){
					printf("Q1 ");
				}
				else{
					printf("Q2 ");
				}
				printf("executes %s for %d units -> %s finishes execution\n",proc->pid,*time - time_start,proc->pid);
				proc = NULL;
				
			}
			if(proc != NULL && proc->TimeQuantum == 0){
				if(id == 1){
					printf("Time:%d, Q1 executes %s for %d units -> %s moves to Q2\n",*time,proc->pid,*time - time_start, proc->pid);
					Insert(q2,proc);
				}
				else{
					printf("Time:%d, Q2 executes %s for %d units -> %s moves to Q3\n",*time,proc->pid,*time - time_start, proc->pid);
					Insert(q3,proc);
				}
				proc = NULL;
				
			}
			
		}
	}
}

void SimulateLowest(int* time, struct ProcessQueue* q3){
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
				proc = NULL;
			}
		}
	}
}


void final(int*time, struct Process* ListOfProcesses, int num,struct ProcessQueue* q1,struct ProcessQueue* q2,struct ProcessQueue* q3){
	
	InitialScheduling(time,ListOfProcesses,num,q1,q2,q3);
	while(!IsEmpty(q1) || !IsEmpty(q2) || !IsEmpty(q3)){
		SimulateQueue(time,1,q1,q2,q3);
		SimulateQueue(time,2,q1,q2,q3);
		SimulateLowest(time,q3);
	}
}



void main(){
	struct ProcessQueue pq = {2};
	printf("%d\n",pq.TimeQuantum);

	int number = 0;
	struct Process* ListOfProcesses = ReadProcessesFromFile(&number);
	printf("%d",number);
	SortProcesses(ListOfProcesses,number);
//	SimulateScheduling(ListOfProcesses,number);
	int time = 0;
	struct ProcessQueue q1 = {2,NULL,NULL};
	struct ProcessQueue q2 = {4,NULL,NULL};
	struct ProcessQueue q3 = {2,NULL,NULL};
	final(&time,ListOfProcesses,number,&q1,&q2,&q3);
	printf("We here\n");

	
	free(ListOfProcesses);
}
