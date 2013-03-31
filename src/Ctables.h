/*
 * Ctables : Pretty table printer written in C
 * Author  : Enkitosh
 * git 	: http://github.com/enkitosh
 * Date	: 2013-Mar-04
 */

#ifndef CTABLES_H_
#define CTABLES_H_

#define MAX_OPS 4
#define MAX_BUF 50

/* COLORS */
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
 * 0. STRICT - User adds dimensions, table handles the indexing
 *    FREELY - Data is added to table and user
 *    handles indexing
 * 1. Options - COLORFUL : make table sensitive to color and
 *    higlighting
 *    NOCOLOR  :     Prints B/W table
 * 2. Alignment - CENTER, LEFT, RIGHT
 *
 * 3. ENUMERATE - Display row/column numbers
 *    along table data. Pass NONE if
 *    this is not desired...
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

typedef struct table_cell_s {
    char     *str;
    char     *color;
    int       width;
    int       cell_width;
    int       max_cell_w;
}         table_cell;

typedef struct table_s {
    int       row_dimension;
    int       col_dimension;
    int       index_i;
    int       index_j;
    int       options[MAX_OPS];
    table_cell **info;
    int       capacity;
}         table_t;

/* prints given symbol */
void      ms(int space, int symbol);
/* finds the biggest string in a given row */
int       return_biggest(table_t * table, int row);
/* Calculates width of table cells and spacing */
int      *calculate_width(table_t * table);
/* Converts Int to String */
char     *cnvrtInt(int x);
/* Converts hex value to string */
char     *cnvrtHex(int x);
/* Convert Memory Address to string */
char     *cnvrtPtr(void *ptr);

/* Operations */
table_t  *initialize_table(int op[], int dim_i, int dim_j);

/* Add to STRICT table */
void      add(table_t * table, char *str);

/* Add FREELY to table */
void      add_freely(table_t * table, int row, int col, char *in_str);

/* Given a row and column, color the string at given index */
void      color_me(table_t * table, int row, int col, char *color_c);

/* Finds a string in table and colors it */
void      color_string(table_t * table, char *str_find, char *color_c);

/* Color all rows in a given column */
void      color_row(table_t * table, int col, char *color_c);

/* Color all columns in a given row */
void      color_columns(table_t * table, int row, char *color_c);

/* Prints out table */
void      print(table_t * table);
void      filler_p(table_t * table);

/* Free's allocated table */
void      free_table(table_t * table);

#endif					/* CTABLES_H_ */
