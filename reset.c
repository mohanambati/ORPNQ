

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void update(char *s)
{
    FILE *fp;
        fp = fopen(s, "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
        return;
        }   
    else
    fprintf(fp,"%d \n",1);
    
    fclose(fp);
}

int main()
{
	FILE *fp;
	fp = fopen("qvalues.in", "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
		return;
		}	
    else
    for(int i=0;i<36;i++)
    fprintf(fp,"%d \n",0);
    
    fclose(fp);

    fp = fopen("malstates.in", "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
        return;
        }   
    else
    fprintf(fp,"%c \n",' ');
    
    fclose(fp);

update("0.in");
update("1.in");
update("2.in");
update("3.in");
update("4.in");
update("5.in");
update("6.in");
update("7.in");
update("8.in");
update("9.in");
update("10.in");
update("11.in");
update("12.in");
update("13.in");
update("14.in");
update("15.in");
update("16.in");
update("17.in");
update("18.in");
update("19.in");
update("20.in");
update("21.in");
update("22.in");
update("23.in");
update("24.in");
update("25.in");
update("26.in");
update("27.in");
update("28.in");
update("28.in");
update("29.in");
update("30.in");
update("31.in");
update("32.in");
update("33.in");
update("34.in");
update("35.in");


	return 0;
}