#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

int N;

#define INFINITY 100000
#define NODES 10000
#define TABLE_FINAL 2
#define TABLE_LOC 1
#define TABLE_INT 3

#define OPT_LOAD_FILE 1
#define OPT_SPECIFIC_ROUTER 2
#define OPT_FINAL_ROUTING_TABLE 3
#define OPT_LINK_FAILURE 4
#define OPT_OPTIMAL_PATH_COST 5
#define OPT_EXIT 6

int **RefTable;
int ***InputTable;
int **BkTable;
int **FinalTable;
int **LclTable;
int **ChkTable;
int **LocalTable;
int **CopyTable;
int Update=1;
int RowUpdated=0;
int NoFurtherRowUpdate = 0;
int ServicedRows[NODES];
int ServicedRowsLen=0;
int RefFrameUpdated = 1;
int FirstTime = 0;
int MainLoopCont = 1;
int RouterValidity;
int userInp=0;
int bpcost[NODES];

struct bkupcur
{
	int index;
	int cost[NODES];
}bpcur[NODES];
int bpcurIndex=-1;

#if 1 /* new short */
#define MAX NODES
#define TEMP 0
#define PERM 1

int LinkPath[MAX];
int shortdist;
int numPaths;

struct node
{
	int predecessor;
	int dist; /*minimum distance of node from source*/
	int status;
};

int non;

int findpath(int s,int d,int path[MAX],int *sdist,int selTable)
{
	struct node state[MAX];
	int i,min,count=0,current,newdist,u,v;
	*sdist=0;

	
	if(selTable == TABLE_FINAL)
	{
		LocalTable = FinalTable;
	}
	else if(selTable == TABLE_LOC)
	{
		LocalTable = CopyTable;
	}
	else if(selTable == TABLE_INT)
	{
		LocalTable = ChkTable;
	}

	/* Make all nodes temporary */
	for(i=1;i<=non;i++)
	{
		state[i].predecessor=0;
		state[i].dist = INFINITY;
		state[i].status = TEMP;
	}

	/*Source node should be permanent*/
	state[s].predecessor=0;
	state[s].dist = 0;
	state[s].status = PERM;

	/*Starting from source node until destination is found*/
	current=s;
	while(current!=d)
	{
		for(i=1;i<=non;i++)
		{
			/*Checks for adjacent temporary nodes */
			if ( LocalTable[current][i] > 0 && state[i].status == TEMP )
			{
				newdist=state[current].dist + LocalTable[current][i];
				/*Checks for Relabeling*/
				if( newdist < state[i].dist )
				{
					state[i].predecessor = current;
					state[i].dist = newdist;
				}
			}
		}/*End of for*/

		/*Search for temporary node with minimum distand make it current
		node*/
		min=INFINITY;
		current=0;
		for(i=1;i<=non;i++)
		{
			if(state[i].status == TEMP && state[i].dist < min)
			{
				min = state[i].dist;
				current=i;
			}
		}/*End of for*/

		if(current==0) /*If Source or Sink node is isolated*/
			return 0;
		state[current].status=PERM;
	}/*End of while*/

	/* Getting full path in array from destination to source */
	while( current!=0 )
	{
		count++;
		path[count]=current;
		current=state[current].predecessor;
	}

	/*Getting distance from source to destination*/
	for(i=count;i>1;i--)
	{
		u=path[i];
		v=path[i-1];
		*sdist+= LocalTable[u][v];
	}
	return (count);
}/*End of findpath()*/ 

#endif /* new  short*/

int main()
{
	int i,j,k,m,p,r,s;
	int SerLen;
	int Proceed;
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

	while(userInp != OPT_EXIT)/* Exit condition */
	{
		printf("1 - LOAD FILE \n");
		printf("2 - OUTPUT ROUTING PROCESS FOR A SPECIFIC ROUTER \n");		
		printf("3 - COMPUTE FINAL ROUTING TABLE \n");
		printf("4 - COST CHANGE OR LINK FAILURE \n");
		printf("5 - OUTPUT OPTIMAL PATH AND MINIMUM COST \n");
		printf("6 - EXIT \n");		
		scanf("%d",&userInp);

		FileValid = 0;

		if(userInp == OPT_EXIT)
		{
			return 0;
		}
		else if(userInp == OPT_LOAD_FILE)			
		{
			if(InputObtained == 0)		
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
					printf("\nError in opening file\n\n");
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
			}

			if((FileValid == 1)	&& (InputObtained == 0))		
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
				non = nog;

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
				printf("Original routing table is as follows:\n\n");
				for(i=0; i < N;i++)
				{
					printf("Router %d: ",i+1);				
					for(j=0;j < N;j++)
					{
						printf("%d ",RefTable[i][j]);
					}	
					printf("\n");
				}
				printf("\n");
			}
			else
			{
				printf("\nOne input file is already opened.\n");
				printf("To open another file, please exit and re-run the program\n\n");				
			}
#endif		

		}
		else if((userInp == OPT_SPECIFIC_ROUTER) || 
				(userInp == OPT_FINAL_ROUTING_TABLE) || 
				(userInp == OPT_LINK_FAILURE) || 
				(userInp == OPT_OPTIMAL_PATH_COST))
		{
			if(InputObtained != 1)
			{
				printf("\nNo input file is loaded. Please load the input file before performing these operations \n\n");
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

				if(userInp == OPT_SPECIFIC_ROUTER)
				{
					printf("\nPlease input a router number:\n");
					scanf("%d",&RouterInput);
					if(RouterInput < 1 || RouterInput > N)
					{
						printf("Invalid router entry\n");
						RouterValidity = 0;
					}
					else
					{
						printf("Each step of the updating process of this router is:\n\n");
						RouterValidity = 1;
						RouterInput--;
					}
				}
				else if(userInp == OPT_OPTIMAL_PATH_COST)
				{
					printf("\nPlease input the source and destination router number:\n");
					scanf("%d %d",&SourceInput,&DestInput);
				}
				else if(userInp == OPT_LINK_FAILURE)
				{
					printf("\nCost change or link failure not supported \n\n");
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
				ChkTable = (int **)malloc((N+1) * sizeof(int *));
				for(i = 0; i <= (N+1); i++)
					ChkTable[i] = malloc((N+1) * sizeof(int));

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
				
#ifdef DEBUG
				printf("RefTable is: \n\n");
				for(i=0;i<N;i++)
				{
					for(j=0;j<N;j++)
					{
							printf("%d ",RefTable[i][j]);
					}
					printf("\n");
				}
				printf("\n");
#endif

				if(userInp == OPT_SPECIFIC_ROUTER)
				{				
					/* Printing the array to see if the input is correct */
					for(i=0;i<N;i++)
					{						
						if((i == RouterInput) && (RouterValidity))
						{						
							for(j=0;j<N;j++)
							{
								if(RefTable[RouterInput][j] != -1)										
								{								
									printf("Router %d: ",j+1);												
									for(k=0;k<N;k++)
									{	
#if 0												
										if(InputTable[i][j][k] == -1)
										{
											shortdist = 0;
											numPaths = findpath(j+1,k+1,LinkPath,&shortdist,TABLE_INT);
											
											if(shortdist != 0)
											{											
												InputTable[i][j][k] = shortdist;
												printf("%d ",shortdist);
											}
											else
												printf("%d ",InputTable[i][j][k]);
										}									
#else										
										printf("%d ",InputTable[i][j][k]);
#endif
									}
									printf("\n");
								}
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
									/* shrtRet = ShortestPath(src,dst,TABLE_LOC);	*/
									numPaths = findpath(src,dst,LinkPath,&shortdist,TABLE_LOC);
									if(shortdist != 0)
									{
										InputTable[m][m][i-1] = shortdist;
									}
									else
									{
										InputTable[m][m][i-1] = -1;
									}
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
							if(userInp == OPT_SPECIFIC_ROUTER)
							{
								if((MainLoopCont) && (RouterInput == m) && (RouterValidity))
								{
#if 1
									for(r=1;r<=N;r++)
									{
										for(s=1;s<=N;s++)
										{	
											ChkTable[r][s] = InputTable[m][r-1][s-1];
										}
									}					
#endif									
								
									/* Display */
									for(i=0;i<N;i++)
									{
										if(RefTable[m][i] != -1)										
										{
											printf("Router %d: ",i+1);
											for(j=0;j<N;j++)
											{
#if 1
												if(InputTable[m][i][j] == 0)
													printf("%d ",InputTable[m][i][j]);	
												else if(InputTable[m][i][j] == -1)
												{
													shortdist = 0;
													numPaths = findpath(i+1,j+1,LinkPath,&shortdist,TABLE_INT);													
													if(shortdist != 0)
													{											
														InputTable[i][j][k] = shortdist;
														printf("%d ",shortdist);
													}
													else
														printf("%d ",InputTable[m][i][j]);
												}
												else
													printf("%d ",InputTable[m][i][j]);	
#else
												printf("%d ",InputTable[m][i][j]);
#endif
											}
											printf("\n");
										}
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

				if(userInp == OPT_FINAL_ROUTING_TABLE)
				{
					LclTable = FinalTable;
					for(i=1;i<=N;i++)
					{
						for(j=1;j<=N;j++)
						{
							if(LclTable[i][j] == -1)
							{
								shortdist = 0;
								numPaths = findpath(i,j,LinkPath,&shortdist,TABLE_FINAL);
								if(shortdist != 0)
									LclTable[i][j] = shortdist;
								else
									LclTable[i][j] = -1;
							}					
						}
					}			

					/* Display */
					//printf("++++++++++++++FINAL ROUTING TABLE+++++++++++++++++\n");
					printf("\nFinal routing table computed by the DV algorithm is:\n\n");
					for(i=1;i<=N;i++)
					{
						printf("Router %d: ",i);
						for(j=1;j<=N;j++)
						{
							printf("%d ",LclTable[i][j]);
						}
						printf("\n");
					}			
					printf("\n");			
				}

				if(userInp == OPT_OPTIMAL_PATH_COST)
				{
					if(SourceInput < 1 || SourceInput > N || DestInput > N || DestInput < 1)
					{
						printf("Invalid input for the nodes\n");
						//break;
					}
					else
					{
						if(SourceInput != DestInput)		
						{
							/* shrtRet = ShortestPath(SourceInput,DestInput,TABLE_FINAL); */
							numPaths = findpath(SourceInput,DestInput,LinkPath,&shortdist,TABLE_FINAL);
							if(shortdist != 0)
							{								
								printf("\nThe shortest path from %d to %d is ",SourceInput,DestInput);
								for(i=numPaths;i>1;i--)
									printf("%d-",LinkPath[i]);
								printf("%d",LinkPath[i]);
								printf(", total cost is %d\n", shortdist);
								printf("\n");

							}
							else
							{
								printf("\nThere is no path from source to destination node\n");
							}
						}
						else
						{
							printf("\nInvalid start and destination nodes\n");
							//break;
						}
#ifdef DEBUG
						printf("\nThe shortest path between the selected nodes = %d\n",shrtRet);
#endif
					}
				}
			}
		}
		else
		{
			printf("Invalid Input. Please enter again\n\n");
		}
		fflush(stdin);
	}
	getch();/* _getch() */
	return 0;	
}
 
