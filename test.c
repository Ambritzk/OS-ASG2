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
		free(ptr->head);
		ptr->head = ptr->tail = NULL;
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


void SimulateUpperQueue(int *time, struct Process* ListOfProcesses,int TotalProcess, int *index, struct ProcessQueue* Queue){
	//subtract 1 from TotalProcess to account for the fact that array indices start at 0
	TotalProcess--;
	struct Process* CurrentProcess = NULL;
	int TimeQuantum = Queue->TimeQuantum;
	while(*index != TotalProcess || IsEmpty(Queue) == false){

		if(IsEmpty(Queue) == false || *time == ListOfProcesses[*index].arrival_time){






			
			if(*time == ListOfProcesses[*index].arrival_time){
				//A new process has arrived
				if(CurrentProcess == NULL){
					//We don't have a current process
					ListOfProcesses[*index].TimeQuantum = Queue->TimeQuantum;
					Insert(Queue,&ListOfProcesses[*index]);
					*index += 1;
				}
				else{
					//we have a newly arrived process, but we have a process that's currently executing
					//This is an interruption I suppose
					ListOfProcesses[*index].burst_time = Queue->TimeQuantum;
					Insert(Queue,&ListOfProcesses[*index]);
					*index += 1;


					CurrentProcess->interruptions++;
					CurrentProcess->interruptions %= 3;
					if(Queue->SuperiorQueue != NULL && CurrentProcess->interruptions == 0){
						Insert(Queue->SuperiorQueue,CurrentProcess);
					}
					else{
						if(CurrentProcess->disrupt_flag == 0){
							CurrentProcess->TimeQuantum = Queue->TimeQuantum;
						}
						//else it'll be the Quantum - executed time
						Insert(Queue,CurrentProcess);
					}

					CurrentProcess = NULL;
					


				}
			}
			
			if(CurrentProcess == NULL && IsEmpty(Queue) == false){
				CurrentProcess = Pop(Queue);
			}

			if(CurrentProcess != NULL){
				//This means that we have a process at hand, that should be processing
				//do dat doing
				if(CurrentProcess->TimeQuantum == 0){
					//The process has executed for the entire quantum assigned to it
					if(CurrentProcess->burst_time == 0){
						//The process has completed its execution in this time;
						free(CurrentProcess);
						CurrentProcess = NULL;
					}
					else{
						//The process still has some burst time left, must be sent to the gulag
						Insert(Queue->InferiorQueue,CurrentProcess);
					}
				}
				else if(CurrentProcess->burst_time == 0){
					//The process has completed its execution before the time quantum could expire
					free(CurrentProcess);
					CurrentProcess = NULL;
				}
				else{
					//The time quantum hasn't expired, nor has the process finished execution.
					//Must continue processing

					CurrentProcess->burst_time--;
					CurrentProcess->TimeQuantum--;
				}

			}
		}
		*time += 1;

	}
}


void main(){
	struct ProcessQueue pq = {2};
	printf("%d\n",pq.TimeQuantum);

	int number = 0;
	struct Process* ListofProcesses = ReadProcessesFromFile(&number);
	printf("%d",number);
	SortProcesses(ListofProcesses,number);
	SimulateScheduling(ListofProcesses,number);


	
	free(ListofProcesses);
	DisplayQueue(&pq);
	FreeMemoryForProcessQueue(&pq);
}
