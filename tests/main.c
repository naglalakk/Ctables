#include "Ctables.h"

int
main(void)
{
	int             i, j;
	int             ops[MAX_OPS] = {STRICT, COLORFUL, RIGHT, ENUMERATE};
	int             ops_2[MAX_OPS] = {FREELY, NOCOLOR, CENTER, NONE};

	/* EXAMPLE 1 */

	/* Initialize table */
	table_t        *newer_table = initialize_table(ops, 7, 7);
	for (i = 0; i < 7; i++)
		for (j = 0; j < 7; j++)
			add(newer_table, "Sweet");

	/* Color each row differently */
	color_row(newer_table, 0, RED);
	color_row(newer_table, 1, GREEN);
	color_row(newer_table, 2, YELLOW);
	color_row(newer_table, 3, BLUE);
	color_row(newer_table, 4, MAGNETA);
	color_row(newer_table, 5, CYAN);
	color_row(newer_table, 6, WHITE);

	/* Print our table */
	print(newer_table);
	/* Free it */
	free_table(newer_table);

	/* EXAMPLE 2 */
	/* Act freely, No colors, Center-alignment, No enumeration */
	table_t        *neu_tableu = initialize_table(ops_2, 5, 2);

	/* If we want indexing we have to use the add_freely function */
	add_freely(neu_tableu, 0, 0, "Hello");
	add_freely(neu_tableu, 1, 0, "I am");
	add_freely(neu_tableu, 2, 0, "a very");
	add_freely(neu_tableu, 3, 0, "fine");
	add_freely(neu_tableu, 4, 0, "table");

	/* Column width will be adjusted to string length */
	add_freely(neu_tableu, 0, 1, "This is a totally acceptable length");
	add_freely(neu_tableu, 1, 1, "So is this");
	add_freely(neu_tableu, 2, 1, "...");
	add_freely(neu_tableu, 3, 1, "au");
	add_freely(neu_tableu, 4, 1, "revoir");

	print(neu_tableu);

	free_table(neu_tableu);

	return (0);
}
