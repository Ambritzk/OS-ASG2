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
	int Qid;
	struct Process* head;
	struct Process* tail;
	struct ProcessQueue* InferiorQueue;
	struct ProcessQueue* SuperiorQueue;

	
};

void Insert(struct ProcessQueue* ptr, struct Process* proc){
	
	struct Process* newProcess = (struct Process* ) malloc(sizeof(struct Process));
	if (newProcess == NULL) return;
	Deepcopy(newProcess, proc);

	if(ptr->head == NULL){
		ptr->head = ptr->tail = newProcess;
	}
	else{
		ptr->tail->next = newProcess;
		ptr->tail = newProcess;
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


void Scheduling(int*time, struct Process* ListOfPRocesses, int num,struct ProcessQueue* q,struct ProcessQueue* q2,struct ProcessQueue* q3){
	int ind = 0;
	struct Process* Proc = NULL;
	while(ind < num){
		if(*time == ListOfPRocesses[ind].arrival_time){
			if(Proc != NULL){ // if a process is currently executing, we push it back into the queu
				Proc->burst_time--;
				Proc->TimeQuantum--;
				if(Proc->disrupt_flag){
					Proc->TimeQuantum = q->TimeQuantum;
				}
				Proc->interruptions++;
				Insert(q,Proc);
				Proc = NULL;
			}
			Insert(q,&ListOfPRocesses[ind++]);
		}
		else{
			if(!IsEmpty(q)){
				if(Proc == NULL){
					Proc = Pop(q);
					Proc->TimeQuantum = q->TimeQuantum;
				}
				Proc->burst_time--;
				Proc->TimeQuantum--;
				if(Proc->TimeQuantum == 0){ // if time quantum ran out, but burst time didn't
					if(Proc->burst_time != 0){
						Insert(q2,Proc);
					}
					Proc = NULL;
				}
				else if(Proc->burst_time == 0){ // if burst time ran out but time quantum didn't
					Proc = NULL;
				}				
			}
			else{
				if(!IsEmpty(q2)){
					struct Process* p2 = NULL;
				}
				else{

				}
			}
		}
	}
}


void TemporaryExecution(int* time, struct Process* ListOfProcesses, int ind, int num, int id,struct ProcessQueue* q1, struct ProcessQueue* q2, struct ProcessQueue* q3){
	struct Process* proc = NULL;
	int time_spent = -1;
	bool TimeToQuit = false;
	if(id == 2){
		while(!IsEmpty(q2)){
			if(TimeToQuit){
				return;
			}
			if(proc == NULL){
				proc = Pop(q2);
				proc->TimeQuantum = q2->TimeQuantum;
			}
			else{
				proc->burst_time--;
				proc->TimeQuantum--;
				if(proc->burst_time == 0){
					proc = NULL;	
				}
				else if(proc->TimeQuantum == 0){
					if(proc != NULL){
						Insert(q3,proc);
						proc = NULL;
					}
				}

				if(*time == ListOfProcesses[ind].arrival_time){
					if(proc->burst_time > 0){
						proc->interruptions += 1;
						proc->interruptions %= 3;
						if(proc->interruptions == 0){
							Insert(q1,proc);
						}
						else{
							if(proc->disrupt_flag){
								proc->TimeQuantum = q2->TimeQuantum;
							}
							Insert(q2,proc);
						}
					}
				}
				*time += 1;
			}
		}
	}
	else{
		while(!IsEmpty(q3)){
			if(TimeToQuit){
				return;
			}
			if(proc == NULL){
				proc = Pop(q3);
				time_spent = *time;
			}
			else{
				proc->burst_time--;
				if(proc->burst_time == 0){
					printf("Q3 executes %s for %d units -> %s finishes execution\n",proc->pid,*time - time_spent,proc->pid);
					proc = NULL;
				}
				if(*time == ListOfProcesses[ind].arrival_time){
					if(proc->burst_time != 0){
						proc->interruptions += 1;
						proc->interruptions %= 3;
						if(proc->interruptions == 0){
							printf("Q3 executes %s for %d units -> %s is interrupted and pushed into Q2\n", proc->pid,*time - time_spent,proc->pid);
							Insert(q2,proc);
						}
						else{
							printf("Q3 executes %s for %d -> %s is interrupted and pushed to Q3\n",proc->pid,*time - time_spent, proc->pid);
							Insert(q3,proc);
						}
					}
				}
				*time += 1;
			}
		}
	}
}
void main(){
	struct ProcessQueue pq = {2};
	printf("%d\n",pq.TimeQuantum);

	int number = 0;
	struct Process* ListofProcesses = ReadProcessesFromFile(&number);
	printf("%d",number);
	SortProcesses(ListofProcesses,number);
//	SimulateScheduling(ListofProcesses,number);
	int time = 0;
	SimulateLowestQueue(&time,&pq,ListofProcesses);

	
	free(ListofProcesses);
	FreeMemoryForProcessQueue(&pq);
}
