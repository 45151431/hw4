#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main(int argc, char* argv[])
{
	char policy[6] = {'\0'};
	int N, M, pol;
	FILE *fPtr = fopen(argv[1], "r");
	fscanf(fPtr, "Policy: %s\n", policy);
	fscanf(fPtr, "Number of Virtual Page: %d\n", &N);
	fscanf(fPtr, "Number of Physical Frame: %d\n", &M);
	fscanf(fPtr, "-----Trace-----\n");
	int ref, disk[N+1], USED[N+1], mem[M], LRU[M], i, check, fifo_i, lru_i, random_i, total_cnt, fault_cnt;
	for(i=0; i<=N; i++)
	{
		disk[i] = -1;
		USED[i] = 0;
	}
	for(i=0; i<M; i++)
	{
		mem[i] = -1;
		LRU[i] = M-1-i;
	}
	fifo_i = 0;
	total_cnt = 0;
	fault_cnt = 0;
	srand(time(NULL));
	if(strcmp(policy, "FIFO")==0)
		while(fscanf(fPtr, "Reference %d\n", &ref)!=EOF)
		{
			total_cnt++;
			check = 0;
			for(i=0; i<M; i++)
			{
				if(mem[i]==ref)
				{
					printf("Hit, %d=>%d\n", ref, i);
					check = 1;
					break;
				}
			}
			if(check==0)
			{
				int tmp1 = -1, tmp2 = -1;
				for(i=0; i<=N; i++)
					if(USED[i]==0 && mem[fifo_i]!=-1)
					{
						tmp1 = i;
						disk[i] = mem[fifo_i];
						USED[i] = 1;
						break;
					}
				for(i=0; i<=N; i++)
					if(disk[i]==ref)
					{
						tmp2 = i;
						disk[i] = -1;
						USED[i] = 0;
						break;
					}
				printf("Miss, %d, %d>>%d, %d<<%d\n", fifo_i, mem[fifo_i], tmp1, ref, tmp2);
				
				fault_cnt++;
				mem[fifo_i++] = ref;
				fifo_i %= M;
			}
		}
	else if(strcmp(policy, "LRU")==0)
		while(fscanf(fPtr, "Reference %d\n", &ref)!=EOF)
		{
			total_cnt++;
			check = 0;
			for(i=0; i<M; i++)
			{
				if(mem[i]==ref)
				{
					printf("Hit, %d=>%d\n", ref, i);
					check = 1;
					lru_i = i;
					for(i=0; i<M; i++)
						if(LRU[i]<LRU[lru_i])
							LRU[i]++;
					LRU[lru_i] = 0;
					break;
				}
			}
			if(check==0)
			{
				int tmp1 = -1, tmp2 = -1;
				for(i=0; i<M; i++)
					if(LRU[i]==M-1)
					{
						lru_i = i;
						break;
					}
				for(i=0; i<=N; i++)
					if(USED[i]==0 && mem[lru_i]!=-1)
					{
						tmp1 = i;
						disk[i] = mem[lru_i];
						USED[i] = 1;
						break;
					}
				for(i=0; i<=N; i++)
					if(disk[i]==ref)
					{
						tmp2 = i;
						disk[i] = -1;
						USED[i] = 0;
						break;
					}
				printf("Miss, %d, %d>>%d, %d<<%d\n", lru_i, mem[lru_i], tmp1, ref, tmp2);
				
				fault_cnt++;
				mem[lru_i] = ref;
				for(i=0; i<M; i++)
					if(LRU[i]!=M-1)
						LRU[i]++;
				LRU[lru_i] = 0;
			}
		}
	else if(strcmp(policy, "Random")==0)
		while(fscanf(fPtr, "Reference %d\n", &ref)!=EOF)
		{
			total_cnt++;
			check = 0;
			for(i=0; i<M; i++)
			{
				if(mem[i]==ref)
				{
					printf("Hit, %d=>%d\n", ref, i);
					check = 1;
					break;
				}
			}
			if(check==0 && total_cnt>M)
			{
				int tmp1 = -1, tmp2 = -1;
				random_i = rand()%M;
				for(i=0; i<=N; i++)
					if(USED[i]==0 && mem[random_i]!=-1)
					{
						tmp1 = i;
						disk[i] = mem[random_i];
						USED[i] = 1;
						break;
					}
				for(i=0; i<=N; i++)
					if(disk[i]==ref)
					{
						tmp2 = i;
						disk[i] = -1;
						USED[i] = 0;
						break;
					}
				printf("Miss, %d, %d>>%d, %d<<%d\n", random_i, mem[random_i], tmp1, ref, tmp2);
				fault_cnt++;
				mem[random_i] = ref;
			}
			else if(check==0 && total_cnt<=M)
			{
				int tmp1 = -1, tmp2 = -1;
				random_i = total_cnt-1;
				for(i=0; i<=N; i++)
					if(USED[i]==0 && mem[random_i]!=-1)
					{
						tmp1 = i;
						disk[i] = mem[random_i];
						USED[i] = 1;
						break;
					}
				for(i=0; i<=N; i++)
					if(disk[i]==ref)
					{
						tmp2 = i;
						disk[i] = -1;
						USED[i] = 0;
						break;
					}
				printf("Miss, %d, %d>>%d, %d<<%d\n", random_i, mem[random_i], tmp1, ref, tmp2);
				fault_cnt++;
				mem[random_i] = ref;
			}
		}
	printf("Page Fault Rate: %.3f\n", (float)fault_cnt/(float)total_cnt);
	return 0;
}
