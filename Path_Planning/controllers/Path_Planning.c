#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include<malloc.h>
#include<limits.h>
#include <webots/robot.h>
#include <webots/motor.h>
#define MAP_FILE_NAME "../../Map4.txt"
#define PATH_PLAN_FILE_NAME "../../PathPlanFound.txt"
#include <time.h>

#define TIME_STEP 64
#define LSIZ 50 
#define RSIZ 50 

#define N 5
#define M 9
#define x_max N - 1
#define x_min 0
#define y_max M - 1
#define y_min 0
#define DATA_SIZE 1000
int min_num_of_turns = INT_MAX;
char shortest_str[100];
int shortest_strl = 0;
int shortest_path_matrix[N][M];
int count_f=1;

struct graph_node {
	int y_value;
	int x_value;
	struct graph_node* next_nodes[4];
	struct graph_node* parent_node;
};

struct graph_node* build_a_node(int y, int x, struct graph_node* parent) {
	struct graph_node* current_node = (struct graph_node*)malloc(sizeof(struct graph_node));
	current_node->y_value = y;
	current_node->x_value = x;
	current_node->parent_node = parent;
	for(int i = 0; i < 4; i++)
		current_node -> next_nodes[i] = NULL;
	return current_node;
}

void print_path_as_map(int matrix[][M], char direction) {
	int i, j;
	int first_digit, second_digit;
	char map_file_char;
	FILE* map_file = fopen(MAP_FILE_NAME, "r");
	for(i = 0; i < N; i++) {
		for(j = 0; j < 4 * M + 2; j++) {
			fscanf(map_file, "%c", &map_file_char);
			printf("%c", map_file_char);
		}
		for(j = 0; j < 4 * M; j++) {
			fscanf(map_file, "%c", &map_file_char);
		
			if(matrix[i][j/4] != -1 && matrix[i][j/4] != -2) {
				if(j % 4 == 2) {
					first_digit = matrix[i][j/4] / 10 + 48;
					if(first_digit == '0') {
						printf("%c", map_file_char);
					}
					else {
						printf("%c", first_digit);
					}
				}			
				else if(j % 4 == 3) {
					second_digit = matrix[i][j/4] % 10 + 48;
					printf("%c", second_digit);				
				} else {
					printf("%c", map_file_char);
				}
			} else if(matrix[i][j/4] == -2) {
				if(j % 4 == 2) {
					printf("%c", direction);				
				} else {
					printf("%c", map_file_char);
				}
			} else {
				printf("%c", map_file_char);
			}
		}	
		/*fscanf(map_file, "%c", &map_file_char);
		printf("%c", map_file_char);
		*/
		fscanf(map_file, "%c", &map_file_char);
		printf("%c", map_file_char);

		fscanf(map_file, "%c", &map_file_char);
		printf("%c", map_file_char);

	}

	for(j = 0; j < 4 * M + 3; j++) {
		fscanf(map_file, "%c", &map_file_char);
		printf("%c", map_file_char);
	}
	
	printf("\n");
	
	fclose(map_file);
}

void print_path(int disp[][M], struct graph_node* curr_node, int start_x, int start_y, char direction) {
	struct graph_node* node_to_print = curr_node;
	int matrix[N][M];
	int child_x_value, child_y_value, parent_x_value, parent_y_value;
	int old_x_change = 0, old_y_change = 0, curr_x_change, curr_y_change;
	int ddx, ddy;
	int num_of_turns = 0;
	char str[100];
	int strl = 0;
	
	for(int i = 0; i < N; i++)
		for(int j = 0; j < M; j++)
			matrix[i][j] = -1;
		
	printf("--- Path %d ---\n",count_f);
	count_f++;
	char dir='D';
	if(direction == 'v')
	dir='S';
	if(direction == '^')
	dir='N';
	if(direction == '>')
	dir='E';
	if(direction == '<')
	dir='W';
	
	
	
	
	
	
	while(node_to_print != NULL) {
		matrix[node_to_print->x_value][node_to_print->y_value] = disp[node_to_print->x_value][node_to_print->y_value];
		child_x_value = node_to_print->x_value;
		child_y_value = node_to_print->y_value;
		node_to_print = node_to_print -> parent_node;

		if(node_to_print != NULL) {			
			
			parent_x_value = node_to_print->x_value;
			parent_y_value = node_to_print->y_value;

			curr_x_change = parent_x_value - child_x_value;
			curr_y_change = parent_y_value - child_y_value;
			
			ddx = curr_x_change - old_x_change;
			ddy = curr_y_change - old_y_change;
			
			if(ddx == 0 && ddy == 0)
				str[strl++] = 'F';


			if((curr_x_change == -1 && curr_y_change == 0 && old_x_change == 0 && old_y_change == 1) ||
				(curr_x_change == 0 && curr_y_change == -1 && old_x_change == -1 && old_y_change == 0) ||
				(curr_x_change == 1 && curr_y_change == 0 && old_x_change == 0 && old_y_change == -1) ||
				(curr_x_change == 0 && curr_y_change == 1 && old_x_change == 1 && old_y_change == 0)) {
				str[strl++] = 'F';
				str[strl++] = 'R';
				num_of_turns++;
			}
			
			if((curr_x_change == 0 && curr_y_change == 1 && old_x_change == -1 && old_y_change == 0) ||
				(curr_x_change == -1 && curr_y_change == 0 && old_x_change == 0 && old_y_change == -1) ||
				(curr_x_change == 0 && curr_y_change == -1 && old_x_change == 1 && old_y_change == 0) ||
				(curr_x_change == 1 && curr_y_change == 0 && old_x_change == 0 && old_y_change == 1)) {
				str[strl++] = 'F';
				str[strl++] = 'L';
				num_of_turns++;
			}

			old_x_change = curr_x_change;
			old_y_change = curr_y_change;
		}
	}

	str[strl++] = 'F';

	if(old_x_change == 0 && old_y_change == -1 && direction == 'v')
	{
		str[strl++] = 'L';
		dir='S';
		}
	if(old_x_change == 0 && old_y_change == 1 && direction == 'v')
	{
		str[strl++] = 'R';	
		dir='S';
		}
	if(old_x_change == 0 && old_y_change == 1 && direction == '^')
	{
		str[strl++] = 'R';
		dir='N';	
		}
	if(old_x_change == 0 && old_y_change == -1 && direction == '^')
	{
		str[strl++] = 'L';
		dir='N';
		}
	if(old_x_change == 1 && old_y_change == 0 && direction == '>')
	{      
		str[strl++] = 'L';
		dir='E';	
		}
	if(old_x_change == -1 && old_y_change == 0 && direction == '>')
	{
		str[strl++] = 'R';
		dir='E';
		}	
	if(old_x_change == -1 && old_y_change == 0 && direction == '<')
	{
		str[strl++] = 'L';	
		dir='W';
		}
	if(old_x_change == 1 && old_y_change == 0 && direction == '<')
	{
		str[strl++] = 'R';
		dir='W';	
		}	
	//Other cases

	
	str[strl++] = dir;
	//Other cases
	
	str[strl++] = start_y + 48;
	str[strl++] = start_x + 48;
	/////////////////////////////////////////////
	// PATH FOR ALL THE GRAPH
	/*for(int i = strl-1; i >=0; i--) {
		printf("%c", str[i]);
	}

	printf("\n");*/
	////////////////////////////////////////////
	matrix[start_x][start_y] = -2;

	if(num_of_turns < min_num_of_turns) {
		for(int i = strl-1; i >=0; i--) {
			shortest_str[strl - i - 1] = str[i];
		}
		
		shortest_strl = strl;
		
		for(int i = 0; i < N; i++)
			for(int j = 0; j < M; j++)
				shortest_path_matrix[i][j] = matrix[i][j];
		
		min_num_of_turns = num_of_turns;
	}

	print_path_as_map(matrix, direction);
}

void traverse_graph(int disp[][M], int horizontal_wall[][M], int vertical_wall[][M+1], int start_x, int start_y, int curr_x, int curr_y, char direction, int parent_value, int end_value, int valid_update, struct graph_node* curr_node) {
	if(curr_x < x_min || curr_x > x_max || curr_y > y_max || curr_y < y_min)
		return;

	if(disp[curr_x][curr_y] != parent_value + valid_update)
		return;
	
	if(disp[curr_x][curr_y] == end_value) {
		print_path(disp, curr_node, start_x, start_y, direction);
		return;
	}

	int next_x, next_y;

	next_x = curr_x-1;
	next_y = curr_y;
	if(horizontal_wall[curr_x][curr_y] == 0) {
		struct graph_node* child_node_1 = build_a_node(next_y, next_x, curr_node);
		curr_node -> next_nodes[0] = child_node_1;
		traverse_graph(disp, horizontal_wall, vertical_wall, start_x, start_y, next_x, next_y, direction, disp[curr_x][curr_y], end_value, valid_update, child_node_1);
	}
	
	next_x = curr_x+1;
	next_y = curr_y;
	if(horizontal_wall[next_x][curr_y] == 0) {
		struct graph_node* child_node_2 = build_a_node(next_y, next_x, curr_node);
		curr_node -> next_nodes[1] = child_node_2;
		traverse_graph(disp, horizontal_wall, vertical_wall, start_x, start_y, next_x, next_y, direction, disp[curr_x][curr_y], end_value, valid_update, child_node_2);
	}
	
	next_x = curr_x;
	next_y = curr_y-1;
	if(vertical_wall[curr_x][curr_y] == 0) {
		struct graph_node* child_node_3 = build_a_node(next_y, next_x, curr_node);
		curr_node -> next_nodes[2] = child_node_3;
		traverse_graph(disp, horizontal_wall, vertical_wall, start_x, start_y, next_x, next_y, direction, disp[curr_x][curr_y], end_value, valid_update, child_node_3);
	}
	
	next_x = curr_x;
	next_y = curr_y+1;
	if(vertical_wall[curr_x][next_y] == 0) {
		struct graph_node* child_node_4 = build_a_node(next_y, next_x, curr_node);
		curr_node -> next_nodes[3] = child_node_4;
		traverse_graph(disp, horizontal_wall, vertical_wall, start_x, start_y, next_x, next_y, direction, disp[curr_x][curr_y], end_value, valid_update, child_node_4);
	}
}


int main(int argc, char **argv) {
   wb_robot_init();
  char line[RSIZ][LSIZ];
    FILE *fptr = NULL; 
    FILE *fPtr = NULL;
    int i=0, tot=0,len=0,count=0,num_wall=0,num_wall_02=0,num_wall_03=0,flag=0,flag2=0,flag3=0,flag4=0,row=0,row_02=0;
    char var,var2,var27;
    char direction='q';
    int start_x=99, start_y=99;
    int HorizontalWall[6][9],VerticalWall[5][10],SpotRobot[5][10],CellValues[5][9];
    int GoalCellValue=0,CurrentExploredValue=0,MazeValueChanged=1,h=0,h2=0;
    for(int q=0;q<5;q++)
    {
    for(int q2=0;q2<9;q2++)
    {
    CellValues[q][q2]=99;
    }
    }
CellValues[2][4]=GoalCellValue;
         
    
    printf("--- Task 1 ---\n");
    fptr = fopen(MAP_FILE_NAME, "r");
    while(fgets(line[i], LSIZ, fptr)) 
	{
	len=strlen(line[i]);
       
   //code to extract vertical and horizontal wall matrix
        if(i%2==0)
        {
        row=i/2;
         
        for(int k =1; k<len;k++)
        {
         var=line[i][k];
        if(var=='-')
        {
        flag=0;
        count=count+1;
        if(count>0 && count%3==0){
        HorizontalWall[row][num_wall]=1;
        num_wall++;
        }
        }
         if(var==' '){
        count=0;
        if(flag>0 && flag%4==0){
        HorizontalWall[row][num_wall]=0;
        //printf(" length %d\n", num_wall);
        num_wall++;
        }
        flag++;
        }
        if(num_wall==9)
        {
        num_wall=0;
        }
        }
        }
        // horizontal walls ends here 
        
        // vertical walls starts here
        else
        {
        row_02=(i+1)/2 -1;
       // printf(" length  %d\n", len);
        flag2=0;
         for(int k2 =0; k2<len;k2++)
        {
        var2=line[i][k2];
        if(var2=='|')
        {
        flag2=0;
        VerticalWall[row_02][num_wall_02]=1;
        num_wall_02++;
        }
         if(var2=='v')
        {
        flag2++;
        }
         if(var2=='<')
        {
        flag2++;
        }
        if(var2=='>')
        {
        flag2++;
        }
         if(var2=='^')
        {
        flag2++;
        }
        if(var2==' ')
        {
        flag2++;
        if( flag2>0 && flag2%4==0){
        VerticalWall[row_02][num_wall_02]=0;
        num_wall_02++;
        flag2=0; 
        }
        }
       if(num_wall_02==10)
        {
        num_wall_02=0;
        }
        }
        
        flag3=0;
        for(int k7 =0; k7<len;k7++)
        {
       var27=line[i][k7];
       flag3++;
       if(var27=='v')
       {
       flag4=9;
       }
       if(var27=='>')
       {
       flag4=7;
       }
       if(var27=='^')
       {
       flag4=5;
       }
         if(var27=='<')
       {
       flag4=3;
       //printf(" l value%d\n", flag4);
       }
       if(flag3>0 && flag3%4==0)
       {
        flag3=0;
       SpotRobot[row_02][num_wall_03]=0;
       if(flag4==9)
       {
       SpotRobot[row_02][num_wall_03]=9;
       direction='v';
       flag4=0;
       }
        if(flag4==7)
       {
       SpotRobot[row_02][num_wall_03]=7;
       direction='>';
       flag4=0;
       }
        if(flag4==5)
       {
       SpotRobot[row_02][num_wall_03]=5;
       direction='^';
       flag4=0;
       }
        if(flag4==3)
       {
       SpotRobot[row_02][num_wall_03]=3;
       direction='<';
       flag4=0;
       }
       num_wall_03++;
       }
       if(num_wall_03==9)
        {
        num_wall_03=0;
        flag3=0;
        }
        }
        
        
        
        
        }
        // it ends here 
     line[i][len-1] = '\0';
    i++;
    }
    
    
    
    //print of the array
    
     /*  printf("Two Dimensional horizontal   wall array elements:\n");
   for(int m=0; m<6; m++) {
      for(int j=0;j<9;j++) {
         printf("%d ", HorizontalWall[m][j]);
         if(j==8){
            printf("\n");  
         }
         }
         }
              printf("Two Dimensional vertical wall array elements:\n");
   for(int m2=0; m2<5; m2++) {
      for(int j2=0;j2<10;j2++) {
         printf("%d ", VerticalWall[m2][j2]);
         if(j2==9){
            printf("\n");  
         }
         }
         } 
               printf("Two Dimensional robot spot array elements:\n");*/
   for(int m2=0; m2<5; m2++) {
      for(int j2=0;j2<9;j2++) {
       if (SpotRobot[m2][j2]==3||SpotRobot[m2][j2]==5||SpotRobot[m2][j2]==7||SpotRobot[m2][j2]==9)
       {
       start_x=m2;
       start_y=j2;
       }
        
        // printf("%d ", SpotRobot[m2][j2]);
        /* if(j2==8){
            printf("\n");  
         }*/
         }
         }
        /* for(int tt=0;tt<3;tt++)
         {
        printf("%c "start_position[tt]);
        }*/
        //printf("\n");  
   
    
    tot = i; 
    for(i = 0; i < tot; ++i)
    {
        printf(" %s\n", line[i]);
    }
    printf("--- Task 2 ---\n");  
    int counter=0;
    //flood fill algorithm
    while(MazeValueChanged!=0)
    {
    //printf("Counter  %d \n", counter);
    MazeValueChanged=0;
    for(h=0;h<5;h++)
    {
    for(h2=0;h2<9;h2++)
    {
   // printf("cell values %d, currentExlored looking for value %d \n", CellValues[h][h2],CurrentExploredValue);
   // printf("row value %d, colm value %d \n", h,h2);
   
    //printf("At 5 colm value %02d \n", h2);
    if(CellValues[h][h2]==CurrentExploredValue)
    {
    //LEFT
    if(h2>0 && VerticalWall[h][h2]==0 && CellValues[h][h2-1]==99)
    {
    CellValues[h][h2-1]=CellValues[h][h2]+1;
    }
    
    //RIGHT
    if(h2<9 && VerticalWall[h][h2+1]==0 && CellValues[h][h2+1]==99)
    {
    CellValues[h][h2+1]=CellValues[h][h2]+1;
    }
    
    // UP
    if(h>0 && HorizontalWall[h][h2]==0 && CellValues[h-1][h2]==99)
    {
    CellValues[h-1][h2]=CellValues[h][h2]+1;
    }
    
    //Down
     if(h<4 && HorizontalWall[h+1][h2]==0 && CellValues[h+1][h2]==99)
    {
    CellValues[h+1][h2]=CellValues[h][h2]+1;
    }
    
    MazeValueChanged=1;
   // printf("%02d \n", CurrentExploredValue);
    //CurrentExploredValue++;
  
    }
    
    }
    }
    CurrentExploredValue++;
    counter++;
    }
    // done with finding the matrix
    
    
    /* for(int q3=0;q3<5;q3++)
    {
    for(int q4=0;q4<9;q4++)
    {
     printf("%02d ", CellValues[q3][q4]);//Cell Values
         if(q4==8){
            printf("\n");  
         }
    }
    }*/
 


	int valid_update = -1;
	int end_value = 0;

	struct graph_node* graph = build_a_node(start_y, start_x, NULL);
	struct graph_node* current_node = graph;
	
	
	traverse_graph(CellValues, HorizontalWall, VerticalWall, start_x, start_y, start_x, start_y, direction, CellValues[start_x][start_y] + 1, end_value, valid_update, current_node);
      	printf("--- Task 3 ---\n");
       print_path_as_map(shortest_path_matrix, direction);
       printf("Steps: %d\n",shortest_strl-3);
       printf("Path: ");
	for(int i = 0; i < shortest_strl; i++) {
		printf("%c", shortest_str[i]);
	}
	printf("\n");
      printf("--- Task 4 ---\n");
   
	
       fPtr = fopen(PATH_PLAN_FILE_NAME, "w");


    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create  file.\n");
        exit(EXIT_FAILURE);
    }
     fputs(shortest_str, fPtr);
     fclose(fPtr);
     printf("File: ../../PathPlanFound.txt");
      printf("\n");
       printf("Path: ");
    	//printf("--- Task 3 ---\n");
	for(int i = 0; i < shortest_strl; i++) {
		printf("%c", shortest_str[i]);
	}
	printf("\n");

  
    while (wb_robot_step(TIME_STEP) != -1);
    return 0;
}