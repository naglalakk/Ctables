/**************************************************************
*
*	Ctables.h
*
*
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPS 4

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
	FREELY = 2,
	STRICT,
	COLORFUL,
	NOCOLOR,
	CENTER,
	LEFT,
	RIGHT,
	ENUMERATE,
	NONE
};

typedef enum{FALSE,TRUE}bool;

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
void ms(int space, char symbol); /*prints space or a given symbol*/
int return_biggest(table_t *table, int row); /*finds the biggest string in a given row*/
int *calculate_width(table_t *table);

/*Operations*/
table_t *initialze_table(int op[], int dim_i , int dim_j);
void add(table_t *table, char *str);
void add_freely(table_t *table, int row, int col, char *in_str);
void color_me(table_t *table,int row, int col,char *color_c);
void color_string(table_t *table, char *str_find, char *color_c);
void color_row(table_t *table, int row, char *color_c);
void color_columns(table_t *table, int row, char *color_c);
void print(table_t *table);
void free_table(table_t *table);


/*initialize a table, what kind it is

	Op: fill in the array with your options, pass it to the struct

	0. STRICT - User adds dimensions, table handles the indexing
	   FREELY - Data is added to table and user
		    handles indexing
	1. Options - COLORFUL : make table sensitive to color and higlighting
		     NOCOLOR : 	Prints B/W table
	2. Alignment - CENTER, LEFT, RIGHT

	3. ENUMERATE - Display row/column numbers along table data. Pass NONE if
		       this is not desired...

	example:
		int options[MAX_OPS] = {STRICT, COLORFUL, CENTER, ENUMERATE};
	--create new table
		table_t *neu_tableu = initialize_table(options,5,5);
			makes a colorful/enumerated new table of size 5 * 5
		

		
*/
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
	if(table -> options[2] > 6)  {
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
int mul = dim_i * dim_j;
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
	}
	
	else {
		printf("Error, not enough space allocated for table data!\n");
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

/*Finds a string in table and colors it*/
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

/*Color all rows in a given column*/
void color_row(table_t *table, int col, char *color_c) {
int i;
	for(i = 0; i < table -> row_dimension; i++) {
		table -> info[i][col].color = color_c;
	}
}

/*Color all columns in a given row*/
void color_columns(table_t *table, int row, char *color_c) {
int j;
	for(j = 0; j < table -> col_dimension; j++) {
		table -> info[row][j].color = color_c;
	}
	
}

void print(table_t *table) {
int i,j;
int *width_arr = calculate_width(table);
int wide = 0;
int check_size;
	/*Get overall table width, add together biggest
	  strings from all rows*/
	for(i = 0; i < table -> col_dimension; i++) {	
		wide += width_arr[i] + 4;
	}
	/*On top of this, add */
		wide += table -> col_dimension - 1;

	/*Should table be enumerated?*/
	if(table -> options[3] == ENUMERATE) {
		for(i = 0; i < table -> col_dimension; i++) {
			ms(width_arr[i] + 4,'s');
			printf("%i", i);
		}
		printf("\n");
		printf(" ");
	}
	printf("+");
	ms(wide,'-');
	printf("+\n");


	for(i = 0; i < table -> row_dimension; i++) {
	    if(table -> options[3] == ENUMERATE) {
			printf("%i", i);	
		}
		printf("|");
	    for(j = 0; j < table -> col_dimension; j++) {
		switch(table -> options[2]) {
		case LEFT:
		if(table -> options[1] == COLORFUL) {
			printf("%s", table -> info[i][j].color);
			printf("%s",table -> info[i][j].str);
			ms(table -> info[i][j].cell_width,'s');
			printf("%s", DEFAULT);
		}
		else {
			printf("%s",table -> info[i][j].str);
			ms(table -> info[i][j].cell_width,'s');
		}
			printf("|");
			break;
		case RIGHT:
		if(table -> options[1] == COLORFUL) {
			printf("%s", table -> info[i][j].color);
			ms(table -> info[i][j].cell_width,'s');
			printf("%s", table -> info[i][j].str);
			printf("%s", DEFAULT);

		}
		else {
			ms(table -> info[i][j].cell_width,'s');
			printf("%s", table -> info[i][j].str);
		}
			printf("|");
			break;
		case CENTER:
		check_size = (table -> info[i][j].cell_width + 
		table -> info[i][j].width + 
			table ->info[i][j].cell_width);
		if(check_size < table -> info[i][j].max_cell_w) 
		 {
		    if(table -> options[1] == COLORFUL) {
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
		    if(table -> options[1] == COLORFUL) {
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
			printf("|");
			break;
	
		}
	    }
		printf("\n");
	}

	if(table -> options[3] == ENUMERATE) {
		printf(" ");
	}
	printf("+");
	ms(wide,'-');
	printf("+");

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
