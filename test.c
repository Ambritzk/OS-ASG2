#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

struct Process{
	char pid[2];
	int arrival_time;
	int burst_time;
	bool disrupt_flag;
	struct Process* next;
};
void Deepcopy(struct Process* dest, struct Process *source){
	dest->arrival_time = source->arrival_time;
	dest->burst_time = source->burst_time;
	dest->disrupt_flag = source->disrupt_flag;
	strcpy(dest->pid, source->pid);
	dest->next = NULL;
}

void DisplayProcess(struct Process *ptr){
	printf("Process Name: %s, arv = %d, brst = %d, dsrpt %d\n", ptr->pid, ptr->arrival_time, ptr->burst_time, ptr->disrupt_flag);
}

//Below is the code for processQueue
struct ProcessQueue{

	struct Process* head;
	struct Process* tail;
	
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
void Pop(struct ProcessQueue* ptr){
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



void ReadProcessesFromFile(struct ProcessQueue* q){
	char buffer[256];
	FILE* fp = fopen("processes.txt","r");
	if(fp == NULL){
		printf("Couldn't open \"processes.txt\".");
		return;
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
	
		struct Process newP;
		strcpy(newP.pid,pid);

		newP.arrival_time = atoi(arv);
		newP.burst_time = atoi(brst);
		newP.disrupt_flag = atoi(dsprs);
		Insert(q,&newP);
	}
}


void main(){
	struct ProcessQueue pq;
	ReadProcessesFromFile(&pq);
	DisplayQueue(&pq);
	FreeMemoryForProcessQueue(&pq);
}
