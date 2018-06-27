

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char *argv[])
{
	/*printf("argc : %d\n", argc );

	for(int i =0; i < argc; i++)
		printf("%s\n",argv[i] );*/

if(argc < 2)
{
	printf("usage : %s <Malicious Nodes> \n", argv[0]);
	return -1;
}


	FILE *fp;
	fp = fopen("malstates.in", "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
		return;
		}	
    else
    for(int i=1;i<argc;i++)
    fprintf(fp,"%d \n",atoi(argv[i]));
    
    fclose(fp);


return 0;
    }