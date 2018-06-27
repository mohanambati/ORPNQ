/********************************************************************************************************************************************************/
// Title:                                           
// Written By: Mohan Sai Ambati.
// Program Name: prog2.c
// 
// Description:
//	This program implements the Markov Decision Process value iteration and policy iteration algorithms to plan find optimal policy under uncertanity.
//	This program accepts reward(r) value from the user
/********************************************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define theta 0.00
float utility[36] = {0.0f};
float prevutility[9];
int actions[8], np = 0;
float r = 0;
float gama = 0.99f ;
int var = 0;
int malstates[36]={0};
int states[36] = {0};
float value[36], error =0.0f;
	int tab[36][8] = {

		{6,-1,1,-1,7,-1,-1,-1},
		{7,-1,2,0,8,-1,-1,-1},
		{8,-1,3,1,9,-1,-1,-1},
		{9,-1,4,2,10,-1,-1,-1},
		{10,-1,5,3,11,-1,-1,-1},
		{11,-1,-1,4,-1,-1,-1,-1},
		{12,0,7,-1,13,-1,-1,-1},
		{13,1,8,6,14,-1,-1,0},
		{14,2,-1,7,-1,-1,-1,1},
		{15,3,10,-1,16,-1,-1,2},
		{16,4,11,9,17,-1,-1,3},
		{17,5,-1,10,-1,-1,-1,4},
		{18,6,13,-1,19,-1,-1,-1},
		{19,7,14,12,20,-1,-1,6},
		{20,8,-1,13,-1,-1,-1,7},
		{21,9,16,-1,22,-1,-1,-1},
		{22,10,17,15,23,-1,-1,9},
		{23,11,-1,16,-1,-1,-1,10},
		{24,12,19,-1,25,-1,-1,-1},
		{25,13,20,18,26,-1,-1,12},
		{26,14,21,19,27,-1,-1,13},
		{27,15,22,20,28,-1,-1,-1},
		{28,26,23,21,29,-1,-1,15},
		{29,17,-1,22,-1,-1,-1,16},
		{30,18,-1,-1,-1,-1,-1,-1},
		{31,19,26,-1,32,-1,-1,18},
		{32,20,-1,25,-1,-1,-1,19},
		{33,21,28,-1,34,-1,-1,20},
		{34,22,-1,27,-1,-1,-1,21},
		{35,23,-1,-1,-1,-1,-1,22},
		{-1,24,31,-1,-1,-1,-1,-1},
		{-1,25,32,30,-1,-1,-1,-1},
		{-1,26,33,31,-1,-1,-1,25},
		{-1,27,34,32,-1,-1,-1,-1},
		{-1,28,35,33,-1,-1,-1,27},
		{-1,29,-1,34,-1,-1,-1,-1}
	};

	int route[36][1];
	char policy[36];
	int rewards[36];
	char policy_it[36];
	int next_node[36];
	int manhatt[36]={6,6,5,5,5,5,5,5,5,4,4,4,5,4,4,3,3,3,5,4,3,3,2,2,6,4,4,2,2,1,5,4,3,2,1,1};
	int manhatt_dis[36]={0};
	int malicious_nodes[100] ={0},c = 0;
/***********************************************************************************************************************************************************/
//    This function that initialites the utility.
/***********************************************************************************************************************************************************/

void init_environment()
{
	
	FILE *fp;
	int k =0;
	 fp = fopen("qvalues.in", "r");
		if (!fp) {
			printf("Error: File not present: %s\n");
			return;
			}
	while(!feof(fp) && k <=35){		
	fscanf(fp, "%f \n", &utility[k]);
	k++;
	}
	fclose(fp);
	
	k =0;
	 fp = fopen("nodes.in", "r");
		if (!fp) {
			printf("Error: File not present: %s\n");
			return;
			}
	while(!feof(fp) && k <=35){		
	fscanf(fp, "%d \n", &states[k]);
	k++;
	}
	fclose(fp);
	
			for(int i = 0; i < 36; i ++)
			{
				if(states[i] >= 0) manhatt_dis[i] = manhatt[i];		
				else {
				utility[i] = 0.0f;
				next_node[i] = -1;
				manhatt_dis[i] = 1000000000;
					  for(int x = 0; x < 36; x++)
					  {//printf("\n");
						for(int y = 0; y < 8 ; y ++)
						{ if(tab[x][y] == i) tab[x][y] = -1;
						 else tab[x][y] = tab[x][y];
						//printf("%d",tab[x][y]);
						}
						 
					  }
				}
			}
	
}
/***********************************************************************************************************************************************************/
//    This function that generates possible actions.
/***********************************************************************************************************************************************************/
int actionspossible(int state)
{
	for(int i = 0;i < 36; i++)
	{
		if(state == i)
		{
			for(int j = 0; j < 8; j++)
				actions[j] = 1;
		}
	}
}

float get_reward(int state)
{
	if(state != 35)
	return (10/manhatt_dis[state]);
	else return 100.0;
	
}

/***********************************************************************************************************************************************************/
//    This function that caliculates maximum rewarded action.
/***********************************************************************************************************************************************************/
float caliculatemaxaction(int state)
{ 	var = 0;
	float max ;
	float re = 0.0f;
	int share = 0;
	int success = 0, Notsuccess = 0;
     	int par, total = 0;
     	float probability_success = 0.0f;
	re = utility[state]/10;

	if( state == 35) {policy[state] = '*'; next_node[state]=35; return utility[state];}

	//printf("%d\n",state );
	for (int i = 0; i < 8; i++)
		actions[i] = 0;

	float val[8] = {0.0f};
	//printf("Hi before printing. \n");
	for(int i = 0 ; i < 8; i++)
	{
		success = 0;
		Notsuccess = 0;
	//	printf("i = %d \n",i);
		if(tab[state][i] > 0 )
		{
FILE *fp;


	if(tab[state][i] == 0) fp = fopen("0.in", "r");
	else if(tab[state][i] == 1) fp = fopen("1.in", "r");
	else if(tab[state][i] == 2) fp = fopen("2.in", "r");
	else if(tab[state][i] == 3) fp = fopen("3.in", "r");
	else if(tab[state][i] == 4) fp = fopen("4.in", "r");
	else if(tab[state][i] == 5) fp = fopen("5.in", "r");
	else if(tab[state][i] == 6) fp = fopen("6.in", "r");
	else if(tab[state][i] == 7) fp = fopen("7.in", "r");
	else if(tab[state][i] == 8) fp = fopen("8.in", "r");
	else if(tab[state][i] == 9) fp = fopen("9.in", "r");
	else if(tab[state][i] == 10) fp = fopen("10.in", "r");
	else if(tab[state][i] == 11) fp = fopen("11.in", "r");
	else if(tab[state][i] == 12) fp = fopen("12.in", "r");
	else if(tab[state][i] == 13) fp = fopen("13.in", "r");
	else if(tab[state][i] == 14) fp = fopen("14.in", "r");
	else if(tab[state][i] == 15) fp = fopen("15.in", "r");
	else if(tab[state][i] == 16) fp = fopen("16.in", "r");
	else if(tab[state][i] == 17) fp = fopen("17.in", "r");
	else if(tab[state][i] == 18) fp = fopen("18.in", "r");
	else if(tab[state][i] == 19) fp = fopen("19.in", "r");
	else if(tab[state][i] == 20) fp = fopen("20.in", "r");
	else if(tab[state][i] == 21) fp = fopen("21.in", "r");
	else if(tab[state][i] == 22) fp = fopen("22.in", "r");
	else if(tab[state][i] == 23) fp = fopen("23.in", "r");
	else if(tab[state][i] == 24) fp = fopen("24.in", "r");
	else if(tab[state][i] == 25) fp = fopen("25.in", "r");
	else if(tab[state][i] == 26) fp = fopen("26.in", "r");
	else if(tab[state][i] == 27) fp = fopen("27.in", "r");
	else if(tab[state][i] == 28) fp = fopen("28.in", "r");
	else if(tab[state][i] == 29) fp = fopen("29.in", "r");
	else if(tab[state][i] == 30) fp = fopen("30.in", "r");
	else if(tab[state][i] == 31) fp = fopen("31.in", "r");
	else if(tab[state][i] == 32) fp = fopen("32.in", "r");
	else if(tab[state][i] == 33) fp = fopen("33.in", "r");
	else if(tab[state][i] == 34) fp = fopen("34.in", "r");
	else if(tab[state][i] == 35) fp = fopen("35.in", "r");

 if (!fp) {
        printf("Error: File not present: %s\n");
		//return;
		}	
    else
     {

     	while(!feof(fp)){
     		par = -1;
     		fscanf(fp, "%d \n", &par);
     		if(par == 1) success++;
     		else if(par == 0) Notsuccess++;

     	}
     	
      	total = success+Notsuccess;
      	probability_success = (float)((float)success/(float)total);
      //	printf("state: %d -->%f\n",tab[state][i],probability_success);

      	if(probability_success >= 0.7f) share = 1; else {share = 0;

      		malicious_nodes[c] = tab[state][i];
      		c++;

      	}

     }

 fclose(fp);		



 			if(share == 1)
 			{//printf("%d share\n",tab[state][i] );
			val[i] = probability_success*(gama*(utility[tab[state][i]] + get_reward(tab[state][i])));}
			else val[i] = -1;
	
		}
		else val[i] = - 1.0f;
	}
	//printf("%d",state);
	max= val[0];
	for(int i = 0; i <= 7; i++)
	{
		//printf("%f \n", val[i] );
		if( max < val[i]) {max = val[i]; var = i; }
	}
	//printf("At state = %d, max = %d\n",state, var);
	FILE *fp;
	int succ ;
	if ((int)rand()%(10) >= 1) {if(malstates[state]) succ = 0; else succ = 1;}
	else { if(malstates[state]) succ = 1; else succ = 0;}


if(state == 0) fp = fopen("0.in", "a");
	else if(state == 1) fp = fopen("1.in", "a");
	else if(state == 2) fp = fopen("2.in", "a");
	else if(state == 3) fp = fopen("3.in", "a");
	else if(state == 4) fp = fopen("4.in", "a");
	else if(state == 5) fp = fopen("5.in", "a");
	else if(state == 6) fp = fopen("6.in", "a");
	else if(state == 7) fp = fopen("7.in", "a");
	else if(state == 8) fp = fopen("8.in", "a");
	else if(state == 9) fp = fopen("9.in", "a");
	else if(state == 10) fp = fopen("10.in", "a");
	else if(state == 11) fp = fopen("11.in", "a");
	else if(state == 12) fp = fopen("12.in", "a");
	else if(state == 13) fp = fopen("13.in", "a");
	else if(state == 14) fp = fopen("14.in", "a");
	else if(state == 15) fp = fopen("15.in", "a");
	else if(state== 16) fp = fopen("16.in", "a");
	else if(state == 17) fp = fopen("17.in", "a");
	else if(state == 18) fp = fopen("18.in", "a");
	else if(state== 19) fp = fopen("19.in", "a");
	else if(state == 20) fp = fopen("20.in", "a");
	else if(state == 21) fp = fopen("21.in", "a");
	else if(state == 22) fp = fopen("22.in", "a");
	else if(state == 23) fp = fopen("23.in", "a");
	else if(state == 24) fp = fopen("24.in", "a");
	else if(state == 25) fp = fopen("25.in", "a");
	else if(state == 26) fp = fopen("26.in", "a");
	else if(state == 27) fp = fopen("27.in", "a");
	else if(state == 28) fp = fopen("28.in", "a");
	else if(state == 29) fp = fopen("29.in", "a");
	else if(state== 30) fp = fopen("30.in", "a");
	else if(state == 31) fp = fopen("31.in", "a");
	else if(state == 32) fp = fopen("32.in", "a");
	else if(state == 33) fp = fopen("33.in", "a");
	else if(state == 34) fp = fopen("34.in", "a");
	else if(state == 35) fp = fopen("35.in", "a");
	
    if (!fp) {
        printf("Error: File not present: %s\n");
		//return;
		}	
    else
     fprintf(fp,"%d \n",succ);

 		fclose(fp);		




	if(var == 0) {policy[state] = 'U'; next_node[state]=tab[state][0];}
	else if(var == 1) {policy[state] = 'D'; next_node[state]=tab[state][1];}
	else if(var == 2) {policy[state] = 'R'; next_node[state]=tab[state][2];}
	else if(var == 3) {policy[state] = 'L'; next_node[state]=tab[state][3];}
	else if(var == 4) {policy[state] = 'x'; next_node[state]=tab[state][4];}
	else if(var == 5) {policy[state] = 'y'; next_node[state]=tab[state][5];}
	else if(var == 6) {policy[state] = 'a'; next_node[state]=tab[state][6];}
	else if(var == 7) {policy[state] = 'b'; next_node[state]=tab[state][7];}

	return max;

	}
/***********************************************************************************************************************************************************/
//    This function checks if the error is less and model is ready or not.
/***********************************************************************************************************************************************************/

int cooldown(float util1[9], float util2[9])
{
	 error = util1[0] - util2[0];
	 //printf("error : %f\n",error );
	 if(error < 0.001 && error > -0.0001) return 1;

	 return 0;

}


/***********************************************************************************************************************************************************/
//    This function that excutes valu iteration algorithm.
/***********************************************************************************************************************************************************/
void valueiteration()
{
	int stop = 0, count =0;

	//for(int i = 0; i <= 8; i++) printf("rewards[%d]: %d",i,rewards[i]);


	while(!stop){

	//for(int i = 0; i <= 8; i++) printf("utility[%d]: %f",i,utility[i]);

	//	printf("\n");

		for(int i =0 ; i <= 35; i++)	
		prevutility[i] = utility[i];


	for(int i =0; i <= 35; i++){
		if(states[i] >=0)
		//if(i == 6) utility[states[i]] =  caliculatemaxaction(states[i]) +  r;
		if(i == 35 ) utility[states[i]] =  caliculatemaxaction(states[i]) + 100;
		else utility[states[i]] =  caliculatemaxaction(states[i]);

	//printf("max action val at state[%d]: %f/n",states[i],caliculatemaxaction(states[i]));
	//printf("utility : %f \n", utility[states[i]]);
	//for(int i = 0; i <= 10000000; i++){}
	}
	if(utility[0] - prevutility[0] == 0  && count > 20 && utility[1] - prevutility[1] == 0) stop = 1;
	if(count > 10) stop = 1;

	count ++;
	}


	float max= utility[0];
	int var = 0;
	for(int i = 0; i <= 35; i++)
	{
		//printf("%f \n", val[i] );
		if( max < utility[i]) {max = utility[i]; var = i; }
	}


	//for(int i = 0; i <= 35; i ++)
	//printf("optimal policy for state %d : %c , nxt-node: %d \n", i, policy[i], next_node[i]);

}

/***********************************************************************************************************************************************************/
//    The main function that.
/***********************************************************************************************************************************************************/
int main()
{
	FILE *fp;
	
	init_environment();
	int malst = -1;
	
	fp = fopen("malstates.in", "r");
    if (!fp) {
        printf("Error: File not present: %s\n");
		return;
		}	
    else
    while(!feof(fp))
    {fscanf(fp,"%d \n",&malst);
	malstates[malst] = 1;
	}
    
    fclose(fp);




	valueiteration();

	fp = fopen("qvalues.in", "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
		return;
		}	
    else
    for(int i=0;i<36;i++)
    fprintf(fp,"%f \n",utility[i]);
    
    fclose(fp);
	
	fp = fopen("policy.in", "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
		return;
		}	
    else
    for(int i=0;i<36;i++)
    fprintf(fp,"%d \n",next_node[i]);
    
    fclose(fp);

	fp = fopen("malicious.in", "w");
    if (!fp) {
        printf("Error: File not present: %s\n");
		return;
		}	
    else
    for(int i=0;i<c;i++)
    fprintf(fp,"%d \n",malicious_nodes[i]);
    
    fclose(fp);

	return 0;
}