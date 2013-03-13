#include "Ctables.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv[]) {
	char *p = "hello";
	int size = 50;
	int hx = 0x23;
	
	int ops[MAX_OPS] = {STRICT, COLORFUL, CENTER, ENUMERATE};
	table_t *tabl = initialize_table(ops, 3, 3);
	
	add(tabl, "Memory Address");
	/*Give it a color*/
	color_me(tabl, 0, 0, RED);
	add(tabl, "Size");

	add(tabl, "Difference");
	color_me(tabl, 0,2, BLUE);

		/*Memory Address*/
		add(tabl,cnvrtPtr(p));
		
		/*Size of header/pointer*/
		add(tabl, cnvrtInt(size));
	
		/*Test Hex Values*/
		add(tabl, cnvrtHex(hx));


	print(tabl);

	free_table(tabl);
	
	

return (0);
}
