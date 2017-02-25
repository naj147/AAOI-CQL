#include <stdio.h>
#include <stdlib.h>




int main(int argc,char * argv){

const char* hexstring = "7bc35368 d70ff85c b113f15d b113f15d";
char * lol=(char*)malloc(4028*sizeof(char));
long int number = (int)strtol(hexstring,NULL,16);
printf("%ld",number);
sprintf(lol,"%lx",number);
printf("\n what the heck : %s",lol);


return 0;}
