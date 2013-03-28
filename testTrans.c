#include "Ctables.h"

int main() {
int i,j;
int c,k;
	int ops[MAX_OPS] = {FREELY, COLORFUL, CENTER, ENUMERATE};
	table_t *new_table = initialize_table(ops, 10 , 10);

/*make table full of dots*/
	for(i = 0; i < 10; i++) {
		for(j = 0; j < 10; j++) {
			add_freely(new_table,i,j,".");
				
		}
	
	}	

/*Coloring dots*/
for(c = 0; c < 10; c++) {
	for(k = 0; k < 10; k++) {
		switch(k) {
		case 0:
			color_me(new_table, c,k, RED);
			break;
		case 1:
			color_me(new_table, c,k, BLUE);
			break;
		case 2:
			color_me(new_table, c,k, GREEN);
			break;
		case 3:
			color_me(new_table, c,k, YELLOW);
			break;
		case 4:
			color_me(new_table, c,k, CYAN);
			break;
		case 5:
			color_me(new_table, c,k, MAGNETA);
			break;
		case 6:
			color_me(new_table, c,k, WHITE);
			break;
		case 7:
			color_me(new_table, c,k, DEFAULT);
			break;
		case 8:	
			color_me(new_table, c,k, RED);
			break;
		case 9:
			color_me(new_table, c,k, GREEN);
			break;
		case 10:
			color_me(new_table, c,k, YELLOW);
			break;
		}
	}
	
}

	print(new_table);

		free_table(new_table);

	return 0;
}
