#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

int N;

#define INFINITY 100000
#define TABLE_FINAL 2
#define TABLE_LOC 1

int **RefTable;
int ***InputTable;
int **BkTable;
int **FinalTable;

int **LocalTable;
int **CopyTable;

int **pa;

int Update=1;
int RowUpdated=0;
int NoFurtherRowUpdate = 0;
int ServicedRows[10000];
int ServicedRowsLen=0;
int RefFrameUpdated = 1;
int FirstTime = 0;
int MainLoopCont = 1;
int RouterValidity;
int userInp=0;
int GlobalStart;

struct router
{
	int dist[10000];
	int from[10000];
}rt[10000];


int bpcost[10000];

struct bkupcur
{
	int index;
	int cost[10000];
}bpcur[10000];
int bpcurIndex=-1;

/* Shortest path */
int pIndex=1;
int de=0;

int mm,nn,noofterms,pcount=0;
int aa[10000];
int delay[10000],delay1[10000];
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

void permute(int *a,int m,int n)
{
	int i,j;
	if(m==n)
	{
		if(GlobalStart == aa[0])
		{
			for(i=0;i<n;i++)
			{
				pa[pIndex][i+1] = aa[i];    	
			}
			pIndex++;
		}
	}
	else
	{
		for(j=n;j<m;j++)
		{
			swap(&a[j],&a[n]);
			permute(a,m,n+1);
			swap(&a[j],&a[n]);
		}
	}
}

int ShortestPath(int start, int dest, int selTable)
{

	/* Shortest path */
	int i,j,k;
	int fact;
	int lfact,tmp;
	int **paths;
	int **paths1;
	int index=0;
	int arrInd=0;
	int flag = 0;

	/* Finding factorial */
	fact = 1;
	for(i=1;i<=N;i++)
		fact*=i; 

	//MANI
	//fact = 10000;
	
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

	/* Shortest path */

	for(i=1;i<=N;i++)
		for(j=1;j<=N;j++)
		{
			if(i != j)
			{
				if((LocalTable[i][j] == -1) || (LocalTable[i][j] == 0))
					LocalTable[i][j] = INFINITY;
			}
		}	

#ifdef DEBUG			
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

		for(i=1;i<=fact-1;i++)
			for(j=1;j<=fact;j++)
				paths[i][j] = 0;

		noofterms = N;
		for(i=0;i<N;i++)
			aa[i] = i+1;

		pIndex=1;
		de=0;
		pcount=0;

		permute(&aa[0],N,0);

#ifdef DEBUG
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
#endif
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
			for(j=2;j<=N;j++)
			{
				arrInd = pa[i][j];
				if(LocalTable[tmp][arrInd])
				{
					flag=1;
					de=de+LocalTable[tmp][arrInd];
					tmp=arrInd;
					paths[index][j] = tmp;
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

		lfact = 1;
		for(i=1;i<=N-1;i++)
			lfact*=i; 

		//MANI
		//lfact = 10000;
		
		for(i=2;i<=lfact;i++)
		{
			if(delay[i-1] != delay[i])
			{
				j=j+1;
				delay1[j]=delay[i];

				for(k=1;k<=N;k++)
					paths1[j][k] = paths[i][k];
			}
		}

		target=minimum(delay1,j);
		for(i=1;i<=j;i++)
		{
			if (delay1[i]==target)
				break;
		}
		shortind=i;

#ifdef DEBUG
		/* displaying all the paths and also the selected one */
		if((userInp == 5) && (selTable == TABLE_FINAL))
		{
			for(i=1;i<=j;i++)
			{
				for(k=1;k<=N;k++)
				{
					if (paths1[i][k] !=0)
						printf("%d->",paths1[i][k]);
					else
						break;
				}
				printf("\b\b : delay = %d\n",delay1[i]);
			}
		}
#endif

		if((userInp == 5) && (selTable == TABLE_FINAL))
		{
			/*shortest path n its delay*/
			printf(" ");
			printf("The shortest path is :");
			for(j=1;j<=N;j++)
			{
				if (paths1[shortind][j] !=0)
					printf("%d->",paths1[shortind][j]);
				else
					break;
			}
			printf("\b\b : delay = %d\n\n",delay1[shortind]);
		}
		return delay1[shortind];
}


int main()
{
	int i,j,k,m,p;
	int SerLen;
	int Proceed;
	int shrtRet;
	int src,dst;

	char *newline = NULL; 
	char FileName[100];
	FILE *fp;
	int locLen;
	int Cnt1,Cnt2,nog=0,spaces,Cnt3;
	char line[500];
	char tempStrVal[20];
	int InputObtained = 0;
	int RouterInput=0,SourceInput=0,DestInput=0;
	int FileValid = 0;

	while(userInp != 6)/* Exit condition */
	{
		printf("1 - LOAD FILE \n");
		printf("2 - OUTPUT ROUTING PROCESS FOR A SPECIFIC ROUTER \n");		
		printf("3 - COMPUTE FINAL ROUTING TABLE \n");
		printf("4 - COST CHANGE OR LINK FAILURE \n");
		printf("5 - OUTPUT OPTIMAL PATH AND MINIMUM COST \n");
		printf("6 - EXIT \n");		
		scanf("%d",&userInp);

		FileValid = 0;

		if(userInp == 6)
		{
			return 0;
		}
		else if(userInp == 1)			
		{
			printf("Please load original routing table data file: \n");
			scanf("%s",&FileName);
#ifdef DEBUG			
			printf("FileName = \"%s\"\n", FileName);
#endif

#if 1
			fp = fopen(FileName,"r");
			if(fp == NULL)
			{
				printf("Error in opening file\n\n");
				//getch();
				//return 0;
			}
			else
			{
#ifdef DEBUG				
				printf("File successfully opened\n");
#endif
				FileValid = 1;
			}

			if(FileValid == 1)			
			{
				/* Find the value of N */
				while ( fgets ( line, sizeof line, fp ) != NULL ) /* read a line */
				{
					for(Cnt1 = 0;Cnt1 < strlen(line)-1;Cnt1++)
					{
						if(line[Cnt1] == ' ')
							nog++;
					}
					break;
				}
				fclose(fp);

				nog++;

				N = nog;/* Number of nodes */

				/* Dynamic multidimensional memory allocation */
				RefTable = (int **)malloc(nog * sizeof(int *));
				for(Cnt1 = 0; Cnt1 < nog; Cnt1++)
					RefTable[Cnt1] = malloc(nog * sizeof(int));

				fp = fopen(FileName,"r");
				Cnt1 = 0;
				Cnt2=0;
				Cnt3=0;
				line[0] = '\0';
				/* Filling the array */
				while ( fgets ( line, sizeof line, fp ) != NULL ) /* read a line */
				{
					spaces = nog;
					Cnt2=0;
					Cnt3=0;
					locLen = strlen(line) - 1;
					tempStrVal[0] = '\0';
					while(spaces)
					{
						if(line[Cnt3] != ' ')
						{
							strncat(tempStrVal,&line[Cnt3],1);
						}
						else
						{
							RefTable[Cnt1][Cnt2] = atoi(tempStrVal);
							spaces--;
							Cnt2++;
							tempStrVal[0] = '\0';
						}
						Cnt3++;
						if(Cnt3 == locLen)
						{
							RefTable[Cnt1][Cnt2] = atoi(tempStrVal);
							spaces--;
							Cnt2++;
							tempStrVal[0] = '\0';
							break;
						}
					}
					Cnt1++;
				}
				InputObtained = 1;
				printf("Original routing table is as follows:\n");
				for(i=0; i < N;i++)
				{
					for(j=0;j < N;j++)
					{
						printf("%d ",RefTable[i][j]);
					}	
					printf("\n");
				}
				printf("\n");
			}
#endif		

		}
		else if((userInp == 2) || (userInp == 3) || (userInp == 4) || (userInp == 5))
		{
			if(InputObtained != 1)
			{
				printf("No input file is loaded. Enter a valid input \n\n");
			}
			else
			{
				/* Variable reinitialization */
				nog = 0;
				MainLoopCont = 1;
				FirstTime = 0;

				fp = fopen(FileName,"r");
				if(fp == NULL)
				{
					printf("Error in opening file\n");
					getch();
					return 0;
				}
				else
#ifdef DEBUG					
					printf("File successfully opened\n");
#endif
				/* Find the value of N */
				while ( fgets ( line, sizeof line, fp ) != NULL ) /* read a line */
				{
					for(Cnt1 = 0;Cnt1 < strlen(line)-1;Cnt1++)
					{
						if(line[Cnt1] == ' ')
							nog++;
					}
					break;
				}
				fclose(fp);

				nog++;

				N = nog;/* Number of nodes */

				/* Dynamic multidimensional memory allocation */
				RefTable = (int **)malloc(nog * sizeof(int *));
				for(Cnt1 = 0; Cnt1 < nog; Cnt1++)
					RefTable[Cnt1] = malloc(nog * sizeof(int));

				fp = fopen(FileName,"r");
				Cnt1 = 0;
				Cnt2=0;
				Cnt3=0;
				line[0] = '\0';
				/* Filling the array */
				while ( fgets ( line, sizeof line, fp ) != NULL ) /* read a line */
				{
					spaces = nog;
					Cnt2=0;
					Cnt3=0;
					locLen = strlen(line) - 1;
					tempStrVal[0] = '\0';
					while(spaces)
					{
						if(line[Cnt3] != ' ')
						{
							strncat(tempStrVal,&line[Cnt3],1);
						}
						else
						{
							RefTable[Cnt1][Cnt2] = atoi(tempStrVal);
							spaces--;
							Cnt2++;
							tempStrVal[0] = '\0';
						}
						Cnt3++;
						if(Cnt3 == locLen)
						{
							RefTable[Cnt1][Cnt2] = atoi(tempStrVal);
							spaces--;
							Cnt2++;
							tempStrVal[0] = '\0';
							break;
						}
					}
					Cnt1++;
				}
				InputObtained = 1;

				if(userInp == 2)
				{
					printf("Please input a router number:\n");
					scanf("%d",&RouterInput);
					if(RouterInput < 1 || RouterInput > N)
					{
						printf("Invalid router entry\n");
						RouterValidity = 0;
					}
					else
					{
						printf("Each updating step of the router is given below:\n\n");
						RouterValidity = 1;
						RouterInput--;
					}
				}
				else if(userInp == 5)
				{
					printf("Please input the source and destination router number:\n");
					scanf("%d %d",&SourceInput,&DestInput);
				}
				else if(userInp == 4)
				{
					printf("Cost change or link failure not supported \n\n");
				}

				InputTable =(int ***) malloc(N* sizeof(int **));

				for(i=0;i<N;i++)
				{
					InputTable[i]=(int **)malloc(N * sizeof(int *));
					for(j=0;j<N;j++)
						InputTable[i][j]=(int *)malloc(N * sizeof(int));
				}

				/* Dynamic multidimensional memory allocation */
				BkTable = (int **)malloc(N * sizeof(int *));
				for(i = 0; i < N; i++)
					BkTable[i] = malloc(N * sizeof(int));

				/* Dynamic multidimensional memory allocation */
				FinalTable = (int **)malloc((N+1) * sizeof(int *));
				for(i = 0; i <= (N+1); i++)
					FinalTable[i] = malloc((N+1) * sizeof(int));

				/* Dynamic multidimensional memory allocation */
				CopyTable = (int **)malloc((N+1) * sizeof(int *));
				for(i = 0; i <= (N+1); i++)
					CopyTable[i] = malloc((N+1) * sizeof(int));

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

				if(userInp == 2)
				{
					/* Printing the array to see if the input is correct */
					for(i=0;i<N;i++)
					{
						if((i == RouterInput) && (RouterValidity))
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
#ifdef DEBUG
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
#endif
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
#ifdef DEBUG						

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

#endif
						/* Distance vector array */
						for(i=0;i<N;i++)
						{
							for(j=0;j<N;j++)
							{
								rt[i].dist[j] = InputTable[m][i][j];
								rt[i].from[j]=j;
							}
						}

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
#ifdef DEBUG
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
									GlobalStart = src;
									shrtRet = ShortestPath(src,dst,TABLE_LOC);	
									InputTable[m][m][i-1] = shrtRet;
								}
							}
#ifdef DEBUG
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
#endif

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

#ifdef DEBUG
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
#endif
							if(userInp == 2)
							{
								if((MainLoopCont) && (RouterInput == m) && (RouterValidity))
								{
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
							}

							bpcur[bpcurIndex].index = m;											
							for(p=0;p<N;p++)
							{
								bpcur[bpcurIndex].cost[p] = InputTable[m][m][p];
							}
							bpcurIndex++;

							if(N-1 != m)
							{
								for(p=0;p<N;p++)
								{
									InputTable[m][m][p] = bpcost[p];
								}
							}
#ifdef DEBUG			
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

				if(userInp == 3)
				{
					/* Display */
					//printf("++++++++++++++FINAL ROUTING TABLE+++++++++++++++++\n");
					printf("The final routing table is shown below:\n\n");
					for(i=1;i<=N;i++)
					{
						for(j=1;j<=N;j++)
						{
							printf("%d ",FinalTable[i][j]);
						}
						printf("\n");
					}			
					printf("\n");			
				}

				if(userInp == 5)
				{
					if(SourceInput < 1 || SourceInput > N || DestInput > N || DestInput < 1)
					{
						printf("Invalid start and destination nodes\n");
						//break;
					}
					else
					{
						if(SourceInput != DestInput)		
						{
							GlobalStart = SourceInput;
							shrtRet = ShortestPath(SourceInput,DestInput,TABLE_FINAL);
						}
						else
						{
							printf("Invalid start and destination nodes\n");
							//break;
						}
#ifdef DEBUG
						printf("\n The shortest path between the selected nodes = %d\n",shrtRet);
#endif
					}
				}
			}
		}
		else
		{
			printf("Invalid Input. Please enter again\n");
		}
	}
	getch();/* _getch() */
	return 0;	
}

