
#include <stdio.h>
#include <stdlib.h>
char** maze;              //2D array to store the maze
int** visited;
int rows;
int cols;
int start_row;
int start_col;

enum terrain                         //assigns value to
{
    empty,                          //empty box
    wall,							//closed path
    goal,							//destination
    crumb							//marker,such as bread crumbs
};

void alloc_maze()                                  //allocating a 2D array with size equal to that of the maze
{
    maze = malloc(rows * sizeof(char*));

    int i;
    for (i = 0; i < rows; ++i)
    {
        maze[i] = malloc(cols * sizeof(char*));
    }
}


void alloc_visited()
{
    visited = malloc(rows * sizeof(int*));
    int i;
    for (i = 0; i < rows; ++i)
    {
        visited[i] = malloc(cols * sizeof(int*));
    }
}

void get_maze(char* file_name)                              //reading the maze from the file
{
    char c;
    char rows_s[3] = { '\0' };          //no. of rows(should be a two digit no.)
    char cols_s[3] = { '\0' };			//no. of columns(should be a two digit no.)
    int rows_i = 0;
    int cols_i = 0;
    int swap = 0;                       //flag to let know when to start reading the no. of columns

    FILE* maze_file = fopen(file_name, "r");      //reading the file

    if (maze_file)
    {
        while ((c = getc(maze_file)) != EOF)
        {
            if (c == '\n')
            {
                break;
            }
            else if (c == ',')
            {
                swap = 1;
            }
            else if (!swap)
            {
                rows_s[rows_i] = c;                            //taking the input for no. of rows
                rows_i++;
            }
            else
            {
                cols_s[cols_i] = c; 						   //taking the input for no. of columns
                cols_i++;
            }
        }
    }
    else
    {
        printf("No such file!");
        return;
    }

    rows = atoi(rows_s);                  //converting string to integer
    cols = atoi(cols_s);				  //converting string to integer

    alloc_maze();

    int i,j;

    for (i = 0; i < rows; ++i)                                          //copying the maze in the 2D array
    {
        for (j = 0; j < cols; ++j)
        {

            if ((c = getc(maze_file)) == '\n')
            {
                c = getc(maze_file);
            }

            maze[i][j] = c;

            if (c =='s')
            {
                start_row = i;
                start_col = j;
            }
        }
    }

    fclose(maze_file);
}

void init_visited()                          //assigning the matrix input with integer values for ease
{
    alloc_visited();

    int i, j;
    for (i = 0; i < rows; ++i)
    {
        for (j = 0; j < cols; ++j)
        {
            if (maze[i][j] == '+')
            {
                visited[i][j] = wall;
            }
            else if (maze[i][j] == 'g')
            {
                visited[i][j] = goal;
            }
            else
            {
                visited[i][j] = empty;
            }
        }
    }
}

void print_maze()
{
    int i, j;
    for (i = 0; i < rows; ++i)
    {
        for (j = 0; j < cols; ++j)
        {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void add_crumbs()                  //placing dots to the required path
{
    int i, j;
    for (i = 0; i < rows; ++i)
    {
        for (j = 0; j < cols; ++j)
        {
            if (maze[i][j] != 's' && visited[i][j] == crumb)
            {
                maze[i][j] = '.';
            }
        }
    }
}

int dfs(int row, int col)                                 //the depth first search code
{
    int* current = &visited[row][col];

    if (*current == goal)
    {
        return 1;
    }

    if (*current == empty)
    {
        *current = wall;

        if (dfs(row, col - 1))
        {
            *current = crumb;
            return 1;
        }
        if (dfs(row + 1, col))
        {
            *current = crumb;
            return 1;
        }
        if (dfs(row, col + 1))
        {
            *current = crumb;
            return 1;
        }
        if (dfs(row - 1, col))
        {
            *current = crumb;
            return 1;
        }
    }

    return 0;
}

int main()
{
    get_maze("maze.txt");
    init_visited();

    print_maze();

    if (!dfs(start_row, start_col))
    {
        printf("No path to the goal could be found.\n");
    }
    else
    {
        add_crumbs();
        print_maze();
    }
    return 0;
}
