#include <stdio.h>
#include <stdlib.h> /* malloc() etc. */
#include <string.h>
#include <errno.h>
#include <err.h>
#include <stdbool.h>
#include "Ctables.h"

void
ms(int space, int symbol)
{
	while (space-- > 0)
		printf("%c", symbol);
}

/* MAKE sure you check for NULL returning from these */
char           *
cnvrtInt(int x)
{
	int n = 15;
	char *in_buf;

   	in_buf = malloc(n * sizeof(*in_buf));
	if(!in_buf)
		return NULL;

	snprintf(in_buf, sizeof(in_buf), "%d", x);
	return in_buf;
}

char           *
cnvrtHex(int x)
{
	int n = 15;
	char           *hx_buf;

   	hx_buf = malloc(n * sizeof(*hx_buf));
	if(!hx_buf)
		return NULL;

	snprintf(hx_buf, sizeof(hx_buf), "0x%X", x);
	return hx_buf;
}

char           *
cnvrtPtr(void *ptr)
{
	int n = 15;
	char           *ptr_buf;

   	ptr_buf = malloc(n * sizeof(*ptr_buf));
	if(!ptr_buf)
		return NULL;

	snprintf(ptr_buf, sizeof(ptr_buf), "%p", ptr);
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
	int            *array_biggest = malloc(elements * sizeof(int));

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

/* consider using a bit array for options */
table_t        *
initialize_table(int op[], int dim_i, int dim_j)
{
	int             i, j;
	table_t        *new_table = malloc(sizeof(*new_table));

	if(!new_table)
		return NULL;

	new_table->info = malloc(sizeof(table_cell_t *) * dim_i);
	for (i = 0; i < dim_i; i++) {
		new_table->info[i] = malloc(sizeof(table_cell_t) * dim_j);
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
	if(!in_str)
		errx(1, "Error, not enough space allocated for table data!");

	if (table->index_i < table->row_dimension 
			&& table->index_j < table->col_dimension) {
		table->info[table->index_i][table->index_j].str = in_str;
		table->info[table->index_i][table->index_j].width = strlen(in_str);

		table->index_j++;
		if (table->index_j == table->col_dimension) {
			table->index_j = 0;
			table->index_i++;
		}
	}

	/*
	* I'm getting rid of this,
	* user will have to handle memory himself
	* if he's using the convertion methods
	*/
		//free(in_str);
}

void
edit(table_t * table, int row, int col, char *edit_str)
{
	if(!edit_str)
		errx(1, "Error, not enough memory allocated");

	if (table->index_i >= table->row_dimension && table->index_j >= table->col_dimension)
		printf("Could not access string at index [%i] [%i] \n", row, col);

	table->info[row][col].str = edit_str;
	table->info[row][col].width = strlen(edit_str);
}

void
add_freely(table_t * table, int row, int col, char *in_str)
{

	if(!in_str) {
		errx(1, "Unable to acquire enough memory from sys.");
	}

	if (table->options[0] != FREELY) {
		fprintf(stderr, "Please optimize your table to be able to add data freely.");
		errx(1, "Pass FREELY to the first index of your options array [FREELY, ... , ... ].");
	}

	if (row < table->row_dimension && col < table->col_dimension) {
		/* proceed */
		table->info[row][col].str = in_str;
		table->info[row][col].width = strlen(in_str);

		//free(in_str);
	} else
		errx(1, "Error, not enough space allocated for table data!");
}

/* consider changing color_c to ENUM to prevent non-sensical colors */
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
	printf("%c", (table->options[1] == TRANSPARENT) ? ' ' : '|');
}

/* this function is 40% of the file
 * it HAS to be simplified */
void
print(table_t * table)
{

	/* Initialize variables............. */

	int             i, j;
	int            *width_arr = calculate_width(table);
	int             wide = 0;
	int             check_size = 0;
	int             wall_space = 0;
	int		align = table->options[2];
	bool		hasColors = 0;
	bool		isEnumerated = 0;
	bool		isTransparent = 0;

	/*
	*  Check table properties, colors, transparency etc.
	*/
	if(table->options[1] == COLORFUL) hasColors = 1;
	if(table->options[1] == TRANSPARENT) isTransparent = 1;
	if(table->options[3] == ENUMERATE) isEnumerated = 1;

	/*---------------------------------------------------*/

	/*
         * Get overall table width, add together biggest strings from all
         * rows, + 4 for each because of default cell spacing
         */
	for (i = 0; i < table->col_dimension; i++) {
		wide += width_arr[i] + 4;
	}
	/* On top of this, add */
	wide += table->col_dimension - 1;


	/* ......Should table be enumerated?............................ */

	if (isEnumerated) {
		for (i = 0; i < table->col_dimension; i++) {
			if (align == LEFT) {
				if (i == 0) {
					ms(1, ' ');

				} else {
					ms(width_arr[i] + 4, ' ');
				}
				printf("%i", i);
			} else if (align == RIGHT) {
				ms(width_arr[i] + 4, ' ');
				printf("%i", i);

			} else if (align == CENTER) {
				if (i == 0) {
					ms(width_arr[0] + 3, ' ');
				} else {
					ms((width_arr[i]) + 4, ' ');
				}
				printf("%i", i);

			}
		}


		printf("\n");
		if(!isTransparent)
			printf(" ");

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


	if (!isTransparent) {
		ms(wall_space, ' ');
		printf("+");
		ms(wide, '-');
		printf("+\n");
	}
	/*------------------------------------------------------*/



	/* ......Make proper space aligning if table is enumerated......... */


	for (i = 0; i < table->row_dimension; i++) {
		if (isEnumerated) {
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
			switch (align) {
			case LEFT:
				if (hasColors || isTransparent) {
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
				if (hasColors || isTransparent) {
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
					if (hasColors || isTransparent) {
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
					if (hasColors || isTransparent) {

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


	if (isEnumerated) {
		ms(wall_space, ' ');
		printf(" ");
	}
	if (!isTransparent) {
		printf("+");
		ms(wide, '-');
		printf("+");
	} else {
		ms(3, ' ');
	}


	putchar('\n');

	/*Turn off option flags*/
	hasColors = 0;
	isEnumerated = 0;
	isTransparent = 0;

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
