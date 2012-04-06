#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define N 4
#define ROUTER_SEL 0/* Starts with 0 */
#define STARTNODE 1/* Starts with 1 */
#define ENDNODE 4/* Starts with 1 */

//int RefTable[N][N] = {{0,2,7},{2,0,1},{7,1,0}};
//int RefTable[N][N] = {{0,2,-1},{2,0,1},{-1,1,0}};
int RefTable[N][N] = {{0,3,5,99},{3,0,99,1},{5,4,0,2},{99,1,2,0}};
//int RefTable[N][N] = {{0,4,-1,-1},{4,0,8,3},{-1,8,0,1},{-1,3,1,0}};

#define INFINITY 100000
#define TABLE_FINAL 2
#define TABLE_LOC 1

int (*InputTable)[N][N];
int **BkTable;
int **FinalTable;

int **LocalTable;
int **CopyTable;

//int start,dest;
int **pa;

int Update=1;
int RowUpdated=0;
int NoFurtherRowUpdate = 0;
int ServicedRows[100];
int ServicedRowsLen=0;
int RefFrameUpdated = 1;
int FirstTime = 0;
int MainLoopCont = 1;

struct router
{
    int dist[N];
	int from[N];
}rt[N];


int bpcost[N];

struct bkupcur
{
	int index;
	int cost[N];
}bpcur[N];
int bpcurIndex=-1;

#if 1/* Shortest path */
int pIndex=1;
int de=0;

int mm,nn,noofterms,pcount=0;
int aa[100];
int delay[100],delay1[100];
int shortind,target;

int minimum(int a[],int len)
{
	int i,min;
	min = a[1];
	if(len == 1)
		return min;
	for(i=2;i<=len;i++)
	{
		if(a[i] < min)
			min = a[i];
	}
	return min;
}

void display()
{
	int x;
	for(x=0;x<noofterms;x++)
	//printf("%d  ",aa[x]);
		pa[pIndex][x+1] = aa[x];

	pIndex++;
//	printf("\n");
	pcount++;
}

void swap(int *p1,int *p2)
{
	int temp;
	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

int noofpermutations()
{
	int permutations=1,x;
	for(x=1;x<=noofterms;x++)
		permutations=permutations*x;
	return permutations;
}

void permute()
{
	int y;
	while(pcount<noofpermutations())
	{   
		for(y=0;y<noofterms-1;y++)
		{
			swap(&aa[y],&aa[y+1]);
			display();
		}
		swap(&aa[0],&aa[1]);
		display();
		for(y=noofterms-1;y>0;y--)
		{
			swap(&aa[y],&aa[y-1]);
			display();
		}
		swap(&aa[noofterms-1],&aa[noofterms-2]);
		display();
	}
}

int ShortestPath(int start, int dest, int selTable)
{

#if 1/* Shortest path */
	//int **FinalTable;
	int i,j,k;
	int fact,lfact,tmp;
	int **paths;
	int **paths1;
	int index=0;
	int arrInd=0;
	int flag = 0;

/* Finding factorial */
fact = 1;
for(i=1;i<=N;i++)
	fact*=i; 

if(selTable == TABLE_FINAL)
{
	LocalTable = FinalTable;
}
else if(selTable == TABLE_LOC)
{
	LocalTable = CopyTable;
}

/* Dynamic multidimensional memory allocation */
paths = (int **)malloc((fact) * sizeof(int *));
for(i = 0; i <= fact; i++)
	paths[i] = malloc((fact+1) * sizeof(int));

/* Dynamic multidimensional memory allocation */
paths1 = (int **)malloc((fact) * sizeof(int *));
for(i = 0; i <= fact; i++)
	paths1[i] = malloc((fact+1) * sizeof(int));

/* Dynamic multidimensional memory allocation */
pa = (int **)malloc((fact+1) * sizeof(int *));
for(i = 0; i <= (fact+1); i++)
	pa[i] = malloc((N+1) * sizeof(int));

#endif

#if 1/* Shortest path */

	for(i=1;i<=N;i++)
		for(j=1;j<=N;j++)
		{
			if(i != j)
			{
				if((LocalTable[i][j] == -1) || (LocalTable[i][j] == 0))
				LocalTable[i][j] = INFINITY;
			}
		}		

//	printf("Factorial = %d \n",fact);

#if 0			
		/* Display */
		printf("TABLE FOR SHORTEST CALCULATION \n");
		for(i=1;i<=N;i++)
		{
			for(j=1;j<=N;j++)
			{
				printf("%d ",CopyTable[i][j]);
			}
			printf("\n");
		}			
		printf("\n");			
#endif

//	if(CopyTable[start][dest] == INFINITY)
//		return 0;
	
	for(i=1;i<=fact-1;i++)
		for(j=1;j<=fact;j++)
			paths[i][j] = 0;

	noofterms = N;
	for(i=0;i<N;i++)
		aa[i] = i+1;

	pIndex=1;
	de=0;
	pcount=0;

	permute();
/*
	printf("Permutation Array \n");
	for(i=1;i<=fact;i++)
	{
		for(j=1;j<=N;j++)
		{
				printf("%d\t",pa[i][j]);
		}
		printf("\n");
	}
	printf("\n");
*/
	for(i=1;i<=fact;i++)
	{
		if(pa[i][1] != start)
		{
			continue;
		}
		de=0;
		tmp=pa[i][1];
		index=index+1;
		paths[index][1]=tmp;
		for(j=2;j<=N;j++)//for j=2:n
		{
			arrInd = pa[i][j];//if gr(temp,p(i,j))>0
			if(LocalTable[tmp][arrInd])
			{
				flag=1;
				de=de+LocalTable[tmp][arrInd];//de=de+gr(temp,p(i,j));
				tmp=arrInd;
				paths[index][j] = tmp;//paths(index,j)=temp;
				if (tmp == dest)
				{
					delay[index]=de;
					break;
				}
			}	
			else
			{
				flag=0;
				index=index-1;
				break;
			}
		}

	}

	j=1;
	delay1[j]=delay[1];
	for(i=1;i<=N;i++)
		paths1[j][i] = paths[1][i];
	//paths1(j,:)=paths(1,:);
	lfact = 1;
	for(i=1;i<=N-1;i++)
		lfact*=i; 

	for(i=2;i<=lfact;i++)//for i=2:factorial(n-1)
	{
		if(delay[i-1] != delay[i])//if delay(i-1)~=delay(i)
		{
			j=j+1;
			delay1[j]=delay[i];
			
			for(k=1;k<=N;k++)
				paths1[j][k] = paths[i][k];
			//paths1(j,:)=paths(i,:);
		}
	}

	target=minimum(delay1,j);
	for(i=1;i<=j;i++)//for i=1:length(delay1)
	{
		if (delay1[i]==target)
			break;
	}
	shortind=i;

	// displaying all the paths and also the selected one
	/*
	for(i=1;i<=j;i++)//for	i=1:length(delay1)
	{
		for(k=1;k<=N;k++)//for j=1:n
		{
			if (paths1[i][k] !=0)
				printf("%d->",paths1[i][k]);
			else
				break;
		}
		printf("\b\b : delay = %d\n",delay1[i]);
	}
	*/
	//shortest path n its delay
	/*
	printf(" ");
	printf("the shortest path is :");
	for(j=1;j<=N;j++)//for j=1:n
	{
			if (paths1[shortind][j] !=0)
				printf("%d->",paths1[shortind][j]);
			else
				break;
	}
	printf("\b\b : delay = %d\n",delay1[shortind]);
	*/
#endif

	return delay1[shortind];
}

#endif

int main()
{
	int i,j,k,m,p;
	int SerLen;
	int Proceed;
	int count;
	int shrtRet;
	int src,dst;
	
	/* Dynamic Memory Allocation */
	InputTable = (int (*)[N][N])malloc(N*N*N*sizeof(int));

	/* Dynamic multidimensional memory allocation */
	BkTable = (int **)malloc(N * sizeof(int *));
	for(i = 0; i < N; i++)
		BkTable[i] = malloc(N * sizeof(int));

	/* Dynamic multidimensional memory allocation */
	FinalTable = (int **)malloc((N+1) * sizeof(int *));
	for(i = 0; i <= (N+1); i++)
		FinalTable[i] = malloc((N+1) * sizeof(int));

#if 1
	/* Dynamic multidimensional memory allocation */
	CopyTable = (int **)malloc((N+1) * sizeof(int *));
	for(i = 0; i <= (N+1); i++)
		CopyTable[i] = malloc((N+1) * sizeof(int));
#endif

	/* -1 initialization */
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			for(k=0;k<N;k++)
				InputTable[i][j][k] = -1;

	/* Filling sample data into the Input Table */
	k = -1;
	for(i=0;i<N;i++)
	{
		k++;
		for(j=0;j<N;j++)
		{
			InputTable[k][k][j] = RefTable[i][j];
		}
	}

	/* Printing the array to see if the input is correct */
	printf("Original Array \n");
	for(i=0;i<N;i++)
	{
		if(i == ROUTER_SEL)
		{
			for(j=0;j<N;j++)
			{
				for(k=0;k<N;k++)
				{
					printf("%d\t",InputTable[i][j][k]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}

	/* State updation */
	while(MainLoopCont)
	{
		if(FirstTime)
		{
			for(i=0;i<N;i++)
			{
				for(j=0;j<N;j++)
				{
					InputTable[i][i][j] = bpcur[i].cost[j];
				}
			}
/*			
			printf("FIRST TIME\n");
			for(i=0;i<N;i++)
			{
				for(j=0;j<N;j++)
				{
					for(k=0;k<N;k++)
					{
						printf("%d ",InputTable[i][j][k]);
					}
					printf("\n");
				}
				printf("\n");
			}
			printf("\n");
*/			
		}

		FirstTime++;

		bpcurIndex = 0;
		RefFrameUpdated = 0;
		MainLoopCont = 0;
		for(m=0;m<N;m++)/* Reference frame */
		{			
			ServicedRowsLen = 0;
			for(i=0;i<N;i++)/* Looping frame */
			{
				if(m != i)
				{
					for(p=0;p<N;p++)
					{
						bpcost[p] = InputTable[m][m][p];
					}				
					
					for(j=0;j<N;j++)/* Looping row */
					{
						if(m != j)
						{
							Proceed = 1;
							for(SerLen=0;SerLen < ServicedRowsLen;SerLen++)
							{
								if(ServicedRows[SerLen] == j)
								{
									Proceed = 0;
									break;
								}
							}
							if(Proceed == 1)
							{
								//ServicedRows[ServicedRowsLen++] = j;
								for(k=0;k<N;k++)/* Looping column */
								{
									if(InputTable[m][j][k] != InputTable[i][j][k])
									{
										RowUpdated = 1;
										ServicedRows[ServicedRowsLen++] = j;
										NoFurtherRowUpdate = 1;
										break;
									}

								}
								if(RowUpdated)
								{
									for(k=0;k<N;k++)/* Update table */
										InputTable[m][j][k] = InputTable[i][j][k];
									RowUpdated = 0;
									RefFrameUpdated = 1;
									if(MainLoopCont == 0)
										MainLoopCont = 1;
								}
							}
						}
						if(NoFurtherRowUpdate)
						{
							NoFurtherRowUpdate = 0;
							break;
						}
						
					}
				}
			}
#if 1 
for(i=0;i<N;i++)
	for(j=0;j<N;j++)
		BkTable[i][j] = 0;
	
for(i=0;i<N;i++)
{
	if(i != m)
	{
		for(j=0;j<N;j++)
		{
			BkTable[i][j] = InputTable[m][i][j];
		}
	}
}
/*
for(i=0;i<N;i++)
{
	if(i == m)
	{
		for(j=0;j<N;j++)
		{
			if(i == j)
				InputTable[m][i][j]=0;
			else
				InputTable[m][i][j]=INFINITY;
		}
	}
}
*/
/*
printf("INPUT TABLE %d \n",m+1);
for(i=0;i<N;i++)
{
	for(j=0;j<N;j++)
	{
		printf("%d ",InputTable[m][i][j]);
	}
	printf("\n");
}
printf("\n");
*/

/* Distance vector array */
for(i=0;i<N;i++)
{
	for(j=0;j<N;j++)
	{
		rt[i].dist[j] = InputTable[m][i][j];
		rt[i].from[j]=j;
	}
}

#if 0
do
{ 
	count=0;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			for(k=0;k<N;k++)
			{
				#if 0
				if(InputTable[m][i][k]+rt[j].dist[k] > 0)
				{
					if(rt[i].dist[j] > InputTable[m][i][k]+rt[j].dist[k])
					{					
						rt[i].dist[j]=rt[i].dist[k]+rt[j].dist[k];
						rt[i].from[j]=k;
						count++;
					}
				}
				else
				{	
					if(InputTable[m][i][k]+rt[j].dist[k] != 0)
						rt[i].dist[j] = InputTable[m][i][k]+rt[j].dist[k];
					//count++;
				}
				#else
				if(rt[i].dist[j] > InputTable[m][i][k]+rt[j].dist[k])
				{					
					rt[i].dist[j]=rt[i].dist[k]+rt[j].dist[k];
					rt[i].from[j]=k;
					count++;
				}
				#endif
			}
		}			
	}
	
}while(count!=0);

for(i=0;i<N;i++)
{
	if(i == m)
	{
		for(j=0;j<N;j++)
		{
			InputTable[m][i][j] = rt[i].dist[j];
		}
	}
}
#else
	for(i=1;i<=N;i++)
	{
		for(j=1;j<=N;j++)
		{	
			CopyTable[i][j] = InputTable[m][i-1][j-1];
		}
	}

	for(i=1;i<=N;i++)
		for(j=1;j<=N;j++)
		{
			if(i != j)
			{
				if((CopyTable[i][j] == -1) || (CopyTable[i][j] == 0))
				CopyTable[i][j] = INFINITY;
			}
		}	
#if 0
	printf("Copy Table \n");
	for(i=1;i<=N;i++)
	{
		for(j=1;j<=N;j++)
		{
			printf("%d ",CopyTable[i][j]);			
		}
		printf("\n");
	}
	printf("\n");
#endif
	for(i=1;i<=N;i++)
	{
		src = m+1;
		dst = i;
		if((src != dst) && (CopyTable[src][dst] != INFINITY))
		{
			shrtRet = ShortestPath(src,dst,TABLE_LOC);	
			InputTable[m][m][i-1] = shrtRet;
		}
	}
/*
	printf("AFTER SHORTEST CALCULATION FOR m = %d \n",m+1);
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("%d ",InputTable[m][i][j]);
		}
		printf("\n");
	}
	printf("\n");
*/
#endif

/*
printf("DEBUG2 %d \n",m+1);
for(i=0;i<N;i++)
{
	for(j=0;j<N;j++)
	{
		printf("%d ",InputTable[m][i][j]);
	}
	printf("\n");
}
printf("\n");
*/


for(i=0;i<N;i++)
{
	if(i != m)
	{
		for(j=0;j<N;j++)
		{
			InputTable[m][i][j] = BkTable[i][j];
		}
	}
}

/*
printf("DEBUG3 %d \n",m+1);
for(i=0;i<N;i++)
{
	for(j=0;j<N;j++)
	{
		printf("%d ",InputTable[m][i][j]);
	}
	printf("\n");
}
printf("\n");
*/
#endif	

#if 1			
if((MainLoopCont) && (ROUTER_SEL == m))
{
		printf("Expected State Update\n");
		/* Display */
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				printf("%d ",InputTable[m][i][j]);
			}
			printf("\n");
		}			
		printf("\n");			
}
#endif	

//if(RefFrameUpdated)
//{
	bpcur[bpcurIndex].index = m;											
	for(p=0;p<N;p++)
	{
		bpcur[bpcurIndex].cost[p] = InputTable[m][m][p];
	}
	bpcurIndex++;
//}	

if(N-1 != m)
{
		for(p=0;p<N;p++)
		{
			InputTable[m][m][p] = bpcost[p];
		}
}
#if 0			
		/* Display */
		printf("Restoring for next iteration \n");
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				printf("%d ",InputTable[m][i][j]);
			}
			printf("\n");
		}			
		printf("\n");			
#endif	

		}
	}

	for(i=1;i<=N;i++)
	{
		for(j=1;j<=N;j++)
		{	
			FinalTable[i][j] = InputTable[0][i-1][j-1];
		}
	}

#if 1			
	/* Display */
	printf("++++++++++++++FINAL ROUTING TABLE+++++++++++++++++\n");
	for(i=1;i<=N;i++)
	{
		for(j=1;j<=N;j++)
		{
			printf("%d ",FinalTable[i][j]);
		}
		printf("\n");
	}			
	printf("\n");			
#endif

	if(STARTNODE != ENDNODE)		
		shrtRet = ShortestPath(STARTNODE,ENDNODE,TABLE_FINAL);
	else
		printf("Invalid start and destination nodes\n");

	printf("\n The shortest path between the selected nodes = %d\n",shrtRet);
	getch();/* _getch() */
	return 0;
}

