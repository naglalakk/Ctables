/**************************************************************
*
*	Ctables : Pretty table printer written in C
*	Author  : Enkitosh
*	git 	: http://github.com/enkitosh
*	Date	: 04.03.2013
**/

#ifndef CTABLES_H_
#define CTABLES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPS 4
#define MAX_BUF 50

//Colors
//TEXT
#define BLACK "\033[30m"
#define RED "\033[31m" 
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGNETA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define DEFAULT "\033[39m"


enum {
/*
0. STRICT - User adds dimensions, table handles the indexing
   FREELY - Data is added to table and user
            handles indexing
1. Options - COLORFUL : make table sensitive to color and higlighting
             NOCOLOR  :     Prints B/W table
2. Alignment - CENTER, LEFT, RIGHT

3. ENUMERATE - Display row/column numbers along table data. Pass NONE if
               this is not desired...


*/
	FREELY = 2,
	STRICT,
	COLORFUL,
	TRANSPARENT,
	NOCOLOR,
	CENTER,
	LEFT,
	RIGHT,
	ENUMERATE,
	NONE
};

typedef enum{
	CHAR,
	INT,
	

}types_t;

typedef enum{FALSE,TRUE}bool;
bool ALLOCAT = FALSE;


typedef struct {
	char *str;
	char *color;
	int width;
	int cell_width;
	int max_cell_w;
}table_cell;


typedef struct {
	int row_dimension;
	int col_dimension;
	int index_i;
	int index_j;
	int options[MAX_OPS];
	table_cell **info;
	int capacity;

}table_t;

/*Extra functions*/

/*prints space or a given symbol*/
void ms(int space, char symbol);

/*finds the biggest string in a given row*/
int return_biggest(table_t *table, int row); 

/*Calculates width of table cells and spacing*/
int *calculate_width(table_t *table);

/*Converts Int to String*/
char *cnvrtInt(int x);

/*Converts hex value to string*/
char *cnvrtHex(int x);

/*Convert Memory Address to string*/
char *cnvrtPtr(void *ptr);

/*Operations*/

/*Initialize table*/
table_t *initialze_table(int op[], int dim_i , int dim_j);

/*Add to STRICT table*/
void add(table_t *table, char *str);

/*Add FREELY to table*/
void add_freely(table_t *table, int row, int col, char *in_str);

/*Given a row and column, color the string at given indec*/
void color_me(table_t *table,int row, int col,char *color_c);

/*Finds a string in table and colors it*/
void color_string(table_t *table, char *str_find, char *color_c);

/*Color all rows in a given column*/
void color_row(table_t *table, int col, char *color_c);

/*Color all columns in a given row*/
void color_columns(table_t *table, int row, char *color_c);

/*Prints out table*/
void print(table_t *table);

void filler_p(table_t *table);

/*Free's allocated table*/
void free_table(table_t *table);

char *cnvrtInt(int x) {

char *in_buf = (char *)malloc(15 * sizeof(char));

	sprintf(in_buf, "%d", x);

		return in_buf;

	ALLOCAT = TRUE;

}

char *cnvrtHex(int x) {
    char *hx_buf = (char *)malloc(15 * sizeof(char));

	sprintf(hx_buf, "0x%x", x);

		ALLOCAT = TRUE;
	return hx_buf;

}

char *cnvrtPtr(void *ptr){
	char *ptr_buf = (char *)malloc(15 * sizeof(char));

	sprintf(ptr_buf, "%p", ptr);

	ALLOCAT = TRUE;
		return ptr_buf;


}

void ms(int space, char symbol) {
int i;

	if(symbol == 's') {

	for(i = 0; i < space; i++) {
		printf(" ");
	    }
	}

	else {
	
	for(i = 0; i < space; i++) {
		printf("%c",symbol);
	    }
		
	}

}

int return_biggest(table_t *table, int row) {
int i;
int biggest = table -> info[0][row].width;
	for(i = 0; i < table -> row_dimension; i++) {
		if(table -> info[i][row].width > biggest) {
			biggest = table -> info[i][row].width;
		}
	}

	return biggest;

}

int *calculate_width(table_t *table) {
int i,j;
/*Nr of elements to calculate*/   
int elements = table -> col_dimension;
	/*make container*/
	int *array_biggest = (int *)malloc(elements * sizeof(int));
	
	/*Fill container with biggest element in each row*/
		for(i = 0; i < elements; i++) {
		 	array_biggest[i] = return_biggest(table,i);
		}

/*
	LEFT: Has no space at the beginning, 4 columns at the end
		|example    |
	RIGHT:
	      4 columns at the beginning
		|    example|
	CENTER:
		2 columns at beginning, 2 at the end
		|  example  |

Note: this is compared to the longest string, to make everything fit
	we have to calculate each space needed for each smaller string
	added to the table in terms of formatting specs

*/
	if(table -> options[2] > 7)  {
	    for(i = 0; i < table -> col_dimension;i++) {
		for(j = 0; j < table -> row_dimension; j++) {
			if(table -> info[j][i].width < array_biggest[i]) {
				table -> info[j][i].cell_width = ((array_biggest[i] + 4) - table -> info[j][i].width);
			}
			else {
				table -> info[j][i].cell_width = 4;
			}
			table -> info[j][i].max_cell_w = array_biggest[i] + 4;
		    }
		}
	    }
	else {
		for(i = 0; i < table -> col_dimension; i++) {
			for(j = 0; j < table -> row_dimension; j++) {
				if(table -> info[j][i].width < array_biggest[i]) {
					table -> info[j][i].cell_width = ((array_biggest[i] + 4) - table -> info[j][i].width) / 2;
	}
				else {
					table -> info[j][i].cell_width = 2;
				}
			  table -> info[j][i].max_cell_w = array_biggest[i] + 4;
			}
		}
	    }

		return array_biggest;
}

table_t *initialize_table(int op[], int dim_i,int dim_j) {
int i,j;
	table_t *new_table = (table_t *)malloc(sizeof(table_t));	    
		new_table -> info = (table_cell **)malloc(sizeof(table_cell *) * dim_i);
	for(i = 0; i < dim_i; i++) {
		new_table -> info[i] = (table_cell *)malloc(sizeof(table_cell) * dim_j);
	}
		new_table -> capacity = dim_i * dim_j;
		new_table -> row_dimension = dim_i;
		new_table -> col_dimension = dim_j;
		new_table -> index_i = 0;
		new_table -> index_j = 0;

	for(j = 0; j < MAX_OPS;j++) {
		new_table -> options[j] = op[j];
	}

	/*Set default for every cell in the table*/
	for(i = 0; i < new_table -> row_dimension; i++) {
		for(j = 0; j < new_table -> col_dimension; j++) {
			new_table -> info[i][j].str = "";	
			new_table -> info[i][j].color = DEFAULT;	
			new_table -> info[i][j].width = 0;
			new_table -> info[i][j].cell_width = 4;
		}
	}



	return new_table;
}


void add(table_t *table, char *in_str) {

    if(table -> index_i < table -> row_dimension && table -> index_j < table -> col_dimension)
    {
	table -> info[table -> index_i][table -> index_j].str = in_str;
	table -> info[table -> index_i][table -> index_j].width = strlen(in_str);

	table -> index_j++;
		if(table -> index_j == table -> col_dimension) {
			table -> index_j = 0;
			table -> index_i++;
		}	
    }
    else {
		printf("Error, not enough space allocated for table data!\n");
		exit(0);
    }

	if(ALLOCAT) {
		free(in_str);
		ALLOCAT = FALSE;
	}


}

void edit(table_t *table, int row, int col, char *edit_str) {
    if(table -> index_i < table -> row_dimension && table -> index_j < table -> col_dimension)
    {
    	table -> info[row][col].str = edit_str;
	table -> info[row][col].width = strlen(edit_str);
    }

    else {
	  printf("Could not access string at index [%i] [%i] \n", row, col);
    }


}

void add_freely(table_t *table, int row, int col, char *in_str) {

   if(table -> options[0] == FREELY) {

	if(row < table -> row_dimension && col < table -> col_dimension) {
		/*proceed*/
		table -> info[row][col].str = in_str;
		table -> info[row][col].width = strlen(in_str);

			if(ALLOCAT) {
				free(in_str);
				ALLOCAT = FALSE;
			}
	}
	
	else {
		printf("Error, not enough space allocated for table data!\n", row, col);
		exit(0);
	}
     }

    else {
		printf("Please optimize your table to be able to add data freely\n");
		printf("Pass FREELY to the first index of your options array [FREELY, ... , ... ]\n");
		exit(0);
    }


}

void color_me(table_t *table,int row, int col,char *color_c) {
	table -> info[row][col].color = color_c;

}

void color_string(table_t *table, char *str_find, char *color_c) {
int i,j;
	for(i = 0; i < table -> row_dimension; i++) {
		for(j = 0; j < table -> col_dimension; j++) {
			if(strcmp(table -> info[i][j].str, str_find) == 0) {
				table -> info[i][j].color = color_c;
			}
		}
	}
}

void color_row(table_t *table, int col, char *color_c) {
int i;
	for(i = 0; i < table -> row_dimension; i++) {
		table -> info[i][col].color = color_c;
	}
}

void color_columns(table_t *table, int row, char *color_c) {
int j;
	for(j = 0; j < table -> col_dimension; j++) {
		table -> info[row][j].color = color_c;
	}
	
}

void filler_p(table_t *table) {

	if(table -> options[1] != TRANSPARENT) {
		printf("|");
	}
		
        else {
	printf(" ");
	}


}

void print(table_t *table) {

/*Initialize variables.............*/

    int i,j;
    int *width_arr = calculate_width(table);
    int wide = 0;
    int check_size = 0;
    int wall_space = 0;

/*---------------------------------------------------*/

/*Get overall table width, add together biggest
     strings from all rows*/
	for(i = 0; i < table -> col_dimension; i++) {
		wide += width_arr[i] + 4;
	}
	/*On top of this, add */
    wide += table -> col_dimension - 1;
    

/*......Should table be enumerated?............................*/

	//ms(1,'s');
	if(table -> options[3] == ENUMERATE) {
		for(i = 0; i < table -> col_dimension; i++) {
			if(table -> options[2] == LEFT) {
				if(i == 0) {
					ms(1,'s');
				
				}
				else {
					ms(width_arr[i] + 4,'s');
				}
			printf("%i", i);
			}

			else if(table -> options[2] == RIGHT) {
				ms(width_arr[i] + 4, 's');
				printf("%i", i);

			}
		
			else if(table -> options[2] == CENTER) {
				if(i == 0) {
					ms(width_arr[0]+3,'s');
				}
				else {
				ms((width_arr[i]) + 4, 's');
				}
				printf("%i", i);

			}
		}


			printf("\n");

		if(table -> options[3] == ENUMERATE) {
		    if(table -> options[1] != TRANSPARENT)
			printf(" ");
		}
        
		if(table -> row_dimension > 10 && table -> row_dimension < 100) {
			wall_space = 1;
		}
		else if(table -> row_dimension > 100 && table -> row_dimension < 1000) {
			wall_space = 2;
		}
        
		else if(table -> row_dimension > 1000 && table -> row_dimension < 10000) {
			wall_space = 3;
		}

	}


/*------------------------------------------------------------------------------*/
    
    
/*......Check for transparency......................*/

	
	if(table -> options[1] != TRANSPARENT) {
	
		ms(wall_space, 's');
		printf("+");
		ms(wide,'-');
		printf("+\n");
	}
	
/*------------------------------------------------------*/



/*......Make proper space aligning if table is enumerated.........*/


	for(i = 0; i < table -> row_dimension; i++) {
	    if(table -> options[3] == ENUMERATE) {
            switch(wall_space) {
                case 1:
                    if(i < 10) {
                        ms(wall_space,'s');
                    }
                    break;
                case 2:
                    if(i < 10) {
                        ms(wall_space, 's');
                    }
                    else if(i < 100 && i >= 10) {
                        ms(wall_space-1, 's');
                        
                    }
                    break;
                case 3:
                    if(i < 10) {
                        ms(wall_space, 's');
                    }
                    else if(i < 100 && i >= 10) {
                        ms(wall_space-1,'s');
                    }
                    
                    else if(i < 1000 && i >= 100) {
                        ms(wall_space-2, 's');
                    }
                    break;
	
		default:
			break;
                    
            }
            	printf("%i", i);
	}

/*-----------------------------------------------------------------*/
	
	filler_p(table);	

/*Start main printing loop.................................................................................*/

    for(j = 0; j < table -> col_dimension; j++) {
		switch(table -> options[2]) {
            case LEFT:
                if( table -> options[1] == COLORFUL || table -> options[1] == TRANSPARENT) {
                    printf("%s", table -> info[i][j].color);
                    printf("%s",table -> info[i][j].str);
                    ms(table -> info[i][j].cell_width,'s');
                    printf("%s", DEFAULT);
                }
                else {
                    printf("%s",table -> info[i][j].str);
                    ms(table -> info[i][j].cell_width,'s');
                }
               
		filler_p(table); 
                
                break;
            case RIGHT:
                if( table -> options[1] == COLORFUL || table -> options[1] == TRANSPARENT) {
                    printf("%s", table -> info[i][j].color);
                    ms(table -> info[i][j].cell_width,'s');
                    printf("%s", table -> info[i][j].str);
                    printf("%s", DEFAULT);
                    
                }
                
                else {
                    ms(table -> info[i][j].cell_width,'s');
                    printf("%s", table -> info[i][j].str);
                }

               filler_p(table);
                break;
            case CENTER:
                check_size = (table -> info[i][j].cell_width +
                              table -> info[i][j].width +
                              table ->info[i][j].cell_width);
                if(check_size < table -> info[i][j].max_cell_w)
                {
                	if( table -> options[1] == COLORFUL || table -> options[1] == TRANSPARENT) {
                        printf("%s", table -> info[i][j].color);
                        ms(table -> info[i][j].cell_width,'s');
                        printf("%s", table -> info[i][j].str);
                        ms(table -> info[i][j].cell_width +
                           table -> info[i][j].max_cell_w - check_size,'s');
                        printf("%s", DEFAULT);
                        
                    }
                    
                    else {
                        ms(table -> info[i][j].cell_width,'s');
                        printf("%s", table -> info[i][j].str);
                        ms(table -> info[i][j].cell_width +
                           table -> info[i][j].max_cell_w - check_size,'s');
                    }
                }
                
                else {
               		if( table -> options[1] == COLORFUL || table -> options[1] == TRANSPARENT) {
                    
                        printf("%s", table -> info[i][j].color);
                        ms(table -> info[i][j].cell_width,'s');
                        printf("%s", table -> info[i][j].str);
                        ms(table -> info[i][j].cell_width,'s');
                        printf("%s", DEFAULT);
                   	 }
                   	 else {
                        ms(table -> info[i][j].cell_width,'s');
                        printf("%s", table -> info[i][j].str);
                        ms(table -> info[i][j].cell_width,'s');
                        
                    }
                }
                
                	filler_p(table);
               		break;
                
	    }
	}
		printf("\n");
}

/*--------------------------------------------------------------------------------------------------------*/
    
    
	if(table -> options[3] == ENUMERATE){
		ms(wall_space,'s');
		printf(" ");
	}
    
	if(table -> options[1] != TRANSPARENT) {
		printf("+");
		ms(wide,'-');
		printf("+");
	}
    
	else {
		ms(3,'s');
	}
	
    
    printf("\n");
    
	/*FREE CONTAINER*/
    free(width_arr);
    
}

void free_table(table_t *table) {
int i;
	for(i = 0; i < table -> row_dimension; i++) {
		free(table ->info[i]);
	}
	free(table -> info);
		free(table);

}


#endif
