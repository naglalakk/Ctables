#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

#include "Ctables.h"

bool            ALLOCAT = false;

void
ms(int space, int symbol)
{
	while (space-- > 0)
		printf("%c", symbol);
}

char           *
cnvrtInt(int x)
{
	char           *in_buf = (char *) malloc(15 * sizeof(char));

	snprintf(in_buf, sizeof(in_buf), "%d", x);

	ALLOCAT = true;
	return in_buf;
}

char           *
cnvrtHex(int x)
{
	char           *hx_buf = (char *) malloc(15 * sizeof(char));

	snprintf(hx_buf, sizeof(hx_buf), "0x%X", x);

	ALLOCAT = true;
	return hx_buf;
}

char           *
cnvrtPtr(void *ptr)
{
	char           *ptr_buf = (char *) malloc(15 * sizeof(char));

	snprintf(ptr_buf, sizeof(ptr_buf), "%p", ptr);

	ALLOCAT = true;
	return ptr_buf;
}

int
return_biggest(table_t * table, int row)
{
	int             i, biggest;

	biggest = table->info[0][row].width;
	for (i = 0; i < table->row_dimension; i++)
		if (table->info[i][row].width > biggest)
			biggest = table->info[i][row].width;

	return biggest;
}

/*		LEFT: Has no space at the beginning, 4 columns at the end
 *               |example    |
 *      RIGHT:
 *             4 columns at the beginning
 *               |    example|
 *      CENTER:
 *               2 columns at beginning, 2 at the end
 *               |  example  |
 *
 *		Note: this is compared to the longest string, to make everything fit
 *      we have to calculate each space needed for each smaller string
 *      added to the table in terms of formatting specs
 */

int            *
calculate_width(table_t * table)
{
	int             i, j;

	/* Nr of elements to calculate */
	int             elements = table->col_dimension;

	/* make container */
	int            *array_biggest = (int *) malloc(elements * sizeof(int));

	/* Fill container with biggest element in each row */
	for (i = 0; i < elements; i++)
		array_biggest[i] = return_biggest(table, i);

	for (i = 0; i < table->col_dimension; i++) {
		for (j = 0; j < table->row_dimension; j++) {

			if (table->options[2] > 7) {
				if (table->info[j][i].width < array_biggest[i])
					table->info[j][i].cell_width = ((array_biggest[i] + 4) - table->info[j][i].width);
				else
					table->info[j][i].cell_width = 4;
				table->info[j][i].max_cell_w = array_biggest[i] + 4;
			} else {
				if (table->info[j][i].width < array_biggest[i])
					table->info[j][i].cell_width = ((array_biggest[i] + 4) - table->info[j][i].width) / 2;
				else
					table->info[j][i].cell_width = 2;
				table->info[j][i].max_cell_w = array_biggest[i] + 4;
			}
		}
	}

	return array_biggest;
}

table_t        *
initialize_table(int op[], int dim_i, int dim_j)
{
	int             i, j;
	table_t        *new_table = (table_t *) malloc(sizeof(table_t));

	new_table->info = (table_cell_t **) malloc(sizeof(table_cell_t *) * dim_i);
	for (i = 0; i < dim_i; i++) {
		new_table->info[i] = (table_cell_t *) malloc(sizeof(table_cell_t) * dim_j);
	}
	new_table->capacity = dim_i * dim_j;
	new_table->row_dimension = dim_i;
	new_table->col_dimension = dim_j;
	new_table->index_i = 0;
	new_table->index_j = 0;

	for (j = 0; j < MAX_OPS; j++) {
		new_table->options[j] = op[j];
	}

	/* Set default for every cell in the table */
	for (i = 0; i < new_table->row_dimension; i++) {
		for (j = 0; j < new_table->col_dimension; j++) {
			new_table->info[i][j].str = "";
			new_table->info[i][j].color = DEFAULT;
			new_table->info[i][j].width = 0;
			new_table->info[i][j].cell_width = 4;
		}
	}

	return new_table;
}


void
add(table_t * table, char *in_str)
{
	if (table->index_i < table->row_dimension && table->index_j < table->col_dimension) {
		table->info[table->index_i][table->index_j].str = in_str;
		table->info[table->index_i][table->index_j].width = strlen(in_str);

		table->index_j++;
		if (table->index_j == table->col_dimension) {
			table->index_j = 0;
			table->index_i++;
		}
	} else {
		printf("Error, not enough space allocated for table data!\n");
		exit(0);
	}

	if (ALLOCAT) {
		free(in_str);
		ALLOCAT = false;
	}
}

void
edit(table_t * table, int row, int col, char *edit_str)
{
	if (table->index_i < table->row_dimension && table->index_j < table->col_dimension) {
		table->info[row][col].str = edit_str;
		table->info[row][col].width = strlen(edit_str);
	} else {
		printf("Could not access string at index [%i] [%i] \n", row, col);
	}


}

void
add_freely(table_t * table, int row, int col, char *in_str)
{

	if (table->options[0] == FREELY) {

		if (row < table->row_dimension && col < table->col_dimension) {
			/* proceed */
			table->info[row][col].str = in_str;
			table->info[row][col].width = strlen(in_str);

			if (ALLOCAT) {
				free(in_str);
				ALLOCAT = false;
			}
		} else {
			printf("Error, not enough space allocated for table data!\n");
			exit(0);
		}
	} else {
		printf("Please optimize your table to be able to add data freely\n");
		printf("Pass FREELY to the first index of your options array [FREELY, ... , ... ]\n");
		exit(0);
	}


}

void
color_me(table_t * table, int row, int col, char *color_c)
{
	table->info[row][col].color = color_c;

}

void
color_string(table_t * table, char *str_find, char *color_c)
{
	int             i, j;

	for (i = 0; i < table->row_dimension; i++) {
		for (j = 0; j < table->col_dimension; j++) {
			if (strcmp(table->info[i][j].str, str_find) == 0) {
				table->info[i][j].color = color_c;
			}
		}
	}
}

void
color_row(table_t * table, int col, char *color_c)
{
	int             i;

	for (i = 0; i < table->row_dimension; i++) {
		table->info[i][col].color = color_c;
	}
}

void
color_columns(table_t * table, int row, char *color_c)
{
	int             j;

	for (j = 0; j < table->col_dimension; j++) {
		table->info[row][j].color = color_c;
	}

}

void
filler_p(table_t * table)
{

	if (table->options[1] != TRANSPARENT) {
		printf("|");
	} else {
		printf(" ");
	}


}

void
print(table_t * table)
{

	/* Initialize variables............. */

	int             i, j;
	int            *width_arr = calculate_width(table);
	int             wide = 0;
	int             check_size = 0;
	int             wall_space = 0;

	/*---------------------------------------------------*/

	/*
         * Get overall table width, add together biggest strings from all
         * rows
         */
	for (i = 0; i < table->col_dimension; i++) {
		wide += width_arr[i] + 4;
	}
	/* On top of this, add */
	wide += table->col_dimension - 1;


	/* ......Should table be enumerated?............................ */

	//ms(1, ' ');
	if (table->options[3] == ENUMERATE) {
		for (i = 0; i < table->col_dimension; i++) {
			if (table->options[2] == LEFT) {
				if (i == 0) {
					ms(1, ' ');

				} else {
					ms(width_arr[i] + 4, ' ');
				}
				printf("%i", i);
			} else if (table->options[2] == RIGHT) {
				ms(width_arr[i] + 4, ' ');
				printf("%i", i);

			} else if (table->options[2] == CENTER) {
				if (i == 0) {
					ms(width_arr[0] + 3, ' ');
				} else {
					ms((width_arr[i]) + 4, ' ');
				}
				printf("%i", i);

			}
		}


		printf("\n");

		if (table->options[3] == ENUMERATE) {
			if (table->options[1] != TRANSPARENT)
				printf(" ");
		}
		if (table->row_dimension > 10 && table->row_dimension < 100) {
			wall_space = 1;
		} else if (table->row_dimension > 100 && table->row_dimension < 1000) {
			wall_space = 2;
		} else if (table->row_dimension > 1000 && table->row_dimension < 10000) {
			wall_space = 3;
		}
	}
	/*------------------------------------------------------------------------------*/


	/* ......Check for transparency...................... */


	if (table->options[1] != TRANSPARENT) {

		ms(wall_space, ' ');
		printf("+");
		ms(wide, '-');
		printf("+\n");
	}
	/*------------------------------------------------------*/



	/* ......Make proper space aligning if table is enumerated......... */


	for (i = 0; i < table->row_dimension; i++) {
		if (table->options[3] == ENUMERATE) {
			switch (wall_space) {
			case 1:
				if (i < 10) {
					ms(wall_space, ' ');
				}
				break;
			case 2:
				if (i < 10) {
					ms(wall_space, ' ');
				} else if (i < 100 && i >= 10) {
					ms(wall_space - 1, ' ');

				}
				break;
			case 3:
				if (i < 10) {
					ms(wall_space, ' ');
				} else if (i < 100 && i >= 10) {
					ms(wall_space - 1, ' ');
				} else if (i < 1000 && i >= 100) {
					ms(wall_space - 2, ' ');
				}
				break;

			default:
				break;

			}
			printf("%i", i);
		}
		/*-----------------------------------------------------------------*/

		filler_p(table);

		/*
		 * Start main printing
		 * loop.......................................................
		 * ..........................
		 */

		for (j = 0; j < table->col_dimension; j++) {
			switch (table->options[2]) {
			case LEFT:
				if (table->options[1] == COLORFUL || table->options[1] == TRANSPARENT) {
					printf("%s", table->info[i][j].color);
					printf("%s", table->info[i][j].str);
					ms(table->info[i][j].cell_width, ' ');
					printf("%s", DEFAULT);
				} else {
					printf("%s", table->info[i][j].str);
					ms(table->info[i][j].cell_width, ' ');
				}

				filler_p(table);

				break;
			case RIGHT:
				if (table->options[1] == COLORFUL || table->options[1] == TRANSPARENT) {
					printf("%s", table->info[i][j].color);
					ms(table->info[i][j].cell_width, ' ');
					printf("%s", table->info[i][j].str);
					printf("%s", DEFAULT);

				} else {
					ms(table->info[i][j].cell_width, ' ');
					printf("%s", table->info[i][j].str);
				}

				filler_p(table);
				break;
			case CENTER:
				check_size = (table->info[i][j].cell_width +
					      table->info[i][j].width +
					      table->info[i][j].cell_width);
				if (check_size < table->info[i][j].max_cell_w) {
					if (table->options[1] == COLORFUL || table->options[1] == TRANSPARENT) {
						printf("%s", table->info[i][j].color);
						ms(table->info[i][j].cell_width, ' ');
						printf("%s", table->info[i][j].str);
						ms(table->info[i][j].cell_width +
						   table->info[i][j].max_cell_w - check_size, ' ');
						printf("%s", DEFAULT);

					} else {
						ms(table->info[i][j].cell_width, ' ');
						printf("%s", table->info[i][j].str);
						ms(table->info[i][j].cell_width +
						   table->info[i][j].max_cell_w - check_size, ' ');
					}
				} else {
					if (table->options[1] == COLORFUL || table->options[1] == TRANSPARENT) {

						printf("%s", table->info[i][j].color);
						ms(table->info[i][j].cell_width, ' ');
						printf("%s", table->info[i][j].str);
						ms(table->info[i][j].cell_width, ' ');
						printf("%s", DEFAULT);
					} else {
						ms(table->info[i][j].cell_width, ' ');
						printf("%s", table->info[i][j].str);
						ms(table->info[i][j].cell_width, ' ');

					}
				}

				filler_p(table);
				break;

			}
		}
		printf("\n");
	}

	/*--------------------------------------------------------------------------------------------------------*/


	if (table->options[3] == ENUMERATE) {
		ms(wall_space, ' ');
		printf(" ");
	}
	if (table->options[1] != TRANSPARENT) {
		printf("+");
		ms(wide, '-');
		printf("+");
	} else {
		ms(3, ' ');
	}


	printf("\n");

	/* FREE CONTAINER */
	free(width_arr);

}

void
free_table(table_t * table)
{
	int             i;

	for (i = 0; i < table->row_dimension; i++) {
		free(table->info[i]);
	}
	free(table->info);
	free(table);

}
