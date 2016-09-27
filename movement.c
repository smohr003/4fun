/* This program simulate the random movement of some bots in a grid. The bots try to reach a predefined target. 

# Author:   Shahram Mohrehkesh (smohr003@odu.edu)  
# Created:  05/16/2016
#
# Copyright (C) 2016 
# For license information, see LICENSE.txt
#

*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//
//Global Variables for: Grid and the number of bots to 
//

int boundsRight;
int boundsTop;

int numOfBots;
//

//
//Print the status of grid 
//
void printGrid(int grid[boundsRight][boundsTop])
{ 
   int i = 0; 
   int j = 0; 
   for(i = 0 ; i< boundsRight; i++){
	for (j =0 ; j < boundsTop; j++){ 
	    printf("%d ", grid[i][j]); 
	}
	printf("\n"); 
   }
   printf("------------------------\n");
}

//This function  checks if bots  have reached the target on the grid
//

int checkTargetMatched(int currentGrid[boundsRight][boundsTop], int targetGrid[boundsRight][boundsTop]){

   int i = 0;
   int j = 0;
    
   for(i = 0 ; i< boundsRight; i++){
        for (j =0 ; j < boundsTop; j++){
            if (currentGrid[i][j] != targetGrid[i][j])  return 0; 
        }
        
   }
   return 1; 

}

// initializes the grid to all zero, i.e., no bots 
void setGridZero(int grid[boundsRight][boundsTop]) {

   int i = 0;
   int j = 0;
   for(i=0;i<boundsRight;i++)
   {
        for(j=0;j<boundsTop;j++)
        {
            grid[i][j]=0;
        }
    }
}

void copyGrid(int grid1[boundsRight][boundsTop], int grid2[boundsRight][boundsTop] ) {

   int i = 0;
   int j = 0;
   for(i=0;i<boundsRight;i++)
   {
        for(j=0;j<boundsTop;j++)
        {
            grid2[i][j]= grid1[i][j];
        }
    }
}




void setLoc(int grid[boundsRight][boundsTop], int numBots ){

   int i = 0;
   int j = 0;
   int n;
   int x ; int y;  
   int set; 
 
   setGridZero(grid); 

   for(n= 0; n < numBots; n++){
      set = 1 ; 
      while (set){ 
	x = rand()%boundsRight;
	y = rand()% boundsTop;  
	if (!grid[x][y]){ 
	   grid[x][y] = 1; 
	   set = 0; 
	}
      }
		
   }
}


// This function  moves the bots either left, right, up or down
//
void moveBot(int grid[boundsRight][boundsTop], int targetGrid[boundsRight][boundsTop])
{
 
    int numOpen=0;
    int done=0;
    int randomNum=0;
    int x; 
    int y;
    int openArray[4];
    int ii = 0;  
    //Loop which runs for all bots and moves in one direction depending on random rand%4.
    //Also checks to see if bot can move in the direction or if there is a bot or boundary blocking it
    // Plus checks to see if bot has reached  one of the targets, if it has not then it will move
    //And  clears the spot on the grid where it previously was if it moves
    
    int gridTemp[boundsRight][boundsTop]; 
    setGridZero(gridTemp); 

    for(x=0;x<boundsRight;x++)
    {
        for(y=0;y<boundsTop;y++) {
     
            randomNum=rand()%4;
	
            
	    if  (grid[x][y]){
    

		if (openToMove(grid, x,y, randomNum) && openToMove(gridTemp,x,y,randomNum) ) {
	 
		   updateGrid(gridTemp,x,y, randomNum) ;  
	    	}
	    	else{
		   gridTemp[x][y] = 1; 
		}
	    }
         }
     } 
     copyGrid(gridTemp, grid); 

}

	    

	    
int openToMove(int grid[boundsRight][boundsTop], int x, int y, int direction) {
  
     switch (direction) { 
        case 0 : {if (x<boundsRight-1 && grid[x+1][y]==0) return 1; break;}
        case 1 : {if (x>0             && grid[x-1][y]==0) return 1; break;}
        case 2 : {if (y< boundsTop-1  && grid[x][y+1]==0) return 1; break;}
	case 3 : {if (y>0             && grid[x][y-1]==0) return 1; break;}        
     }
     return 0;   

} 

        
       
 

void updateGrid(int grid[boundsRight][boundsTop], int x, int y, int direction){

  
   switch (direction) {
        case 0 : {grid[x+1][y]=1; break;}
        case 1 : {grid[x-1][y]=1; break;}
        case 2 : {grid[x][y+1]=1; break;}
        case 3 : {grid[x][y-1]=1; break;}

     }   

  
}

            

//This function moves  bots until they reaches target
//
int randomMoves(int grid[boundsRight][boundsTop], int targetGrid[boundsRight][boundsTop])
{
    
    long count=0;
    
    
    while(checkTargetMatched(grid,targetGrid)!=1 ) 
    {
        moveBot(grid,targetGrid);
        //	printf("%d \n", count);
        count++;
	if ( (count % 1) == 0 ) { 
	   ////printGrid(grid);
		; 
	}
    }
    return count;
}
//
//Main Function
//
int main(int argc , char**argv)
{
    //Print out curent time
    time_t now;
    time(&now);
    printf("%s", ctime(&now));
    //Set variables for use
    int avgDone=0;
    int numRuns=100;
    int i=0;
    int j=0;
    int done=0;
    double sum=0;
    long count=0;
    float avg1=0;
    float avg2=0;
    int interval; 
     
    srand((unsigned) time(&now));
   
    if (argc < 5){
        printf("Missing Argument. Correct Usage: [grid_x_size] [grid_y_size] [num_bots] [verbose_interval]\n"); 
   	exit(1); 
    } 
 
   //Set  boundary values
    boundsRight=atoi(argv[1]); 
    boundsTop= atoi(argv[2]);
    numOfBots=atoi(argv[3]);
    interval = atoi(argv[4]) ;

    //Set Grid Size
    int grid[boundsRight][boundsTop];
      
    int targetGrid[boundsRight][boundsTop]; 
    numOfBots=atoi(argv[3]); 
   


    int countSteps=0;
    while(count<numRuns && avgDone==0)
    {
 
        setLoc(grid, numOfBots);
//	printf("Initial is \n"); 
//	printGrid(grid); 
 
        setLoc(targetGrid, numOfBots); 
//	printf("Target is \n"); 
//	printGrid(targetGrid); 

        countSteps = randomMoves(grid,targetGrid);
        //printf("%d \n", count);
        //printf("%d \n", countSteps);
        if(count>0)
        {
            avg1=(sum/count);
        }
        count++;
        sum+=countSteps;
        avg2=sum/count;
        if((fabs(avg2-avg1)<0.1))
        {
            avgDone=1;
        }
       
	
	if ( (count % interval) ==0){
	  //printGrid(grid); 
	}
        
    }
    if ( numRuns == count && count !=1 ){ 
	        printf("!!!!!!!!!!!!!!!! too many runs, no convergence!!!!!!!!!!\n");
    }
    printf("%s %f \n","Sum of Number of steps needed for bots to reach target: ", sum);
    printf(" aveg 1 is %f, avg2 is %f\n", avg1,avg2); 
    printf("%s %f \n","Average Number of steps needed for bots to reach target: ", avg2);
    printf("%s %ld \n","Average was found after this number of runs: " ,count);
    time(&now);
    printf("%s", ctime(&now));
    return 0;  
}

