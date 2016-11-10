#include <stdio.h>
#include <stdlib.h>



void function2(int *param) {
		int value=5;
    *param = value;
}

int main(void) {
    int variable = 111;
    int *point = &variable;

		//this gets work
    function2(&variable);
    printf("%d\n", variable);
	
    
    // gets address
    function2(point);
    printf("%d\n", point);
    
    
    return 0;
}
