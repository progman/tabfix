#include <stdio.h>
#include <string.h>

int main()
{
	for(;;)
	{
		{
			const char *p = "this is                long string";
	        int size;
    	    size = strlen(p);
//    	    size++;
			printf("string size is %d\n", size);
		}
		break; //        is break;
	}

    return 0;
}
