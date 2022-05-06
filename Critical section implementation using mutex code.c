#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>
pthread_mutex_t mutex;
int frequency[26]={0};                                            //GLOBAL SHARED MEMORY OF CHARACTER FREQUENCY ARRAY DECLARED OUTSIDE GLOBALLY.
void output(){
         printf("{");
        for(int i=0;i<26;i++){
                if(i!=25){
                        printf("%c:%d, ",i+'a',frequency[i]);
                }
                else{
                        printf("%c:%d",i+'a',frequency[i]);
                }
        }
        printf("}");
}

void histogramcalculator(char string[100]){
        int count=0;
        int index;
        if(pthread_mutex_trylock(&mutex) == 0){                                         //CRITICAL SECTION IN WHICH UPDATION OF FREQUENCY COUNTS IS DONE SATISFYING ALL THE THREE CONDITIONS OF CRITICAL SECTION.
                                                                                       //USED TRYLOCK WHICH ACTS AS NORMAL MUTEXLOCK WHEN VARIABLE RECIEVES 0 VALUE.
                if(!strcmp(string, "")) {
                        return;
        }

                while(string[count]!='\0'){
                        if((string[count]>='a' && string[count]<='z')){
                                index=string[count]-'a';
                                frequency[index]++;
                }
                        if((string[count]>='A' && string[count]<='Z')){
                                index=string[count]-'A';
                                frequency[index]++;
                }
                        count++;

        }
       }
        pthread_mutex_unlock(&mutex);

}
FILE*filepointer=NULL;
void *filereading(){
        char linewise[100];
        if(filepointer==NULL){
                printf("Error,Can't open file\n");                                     //FILEREADING FUNCTION IN WHICH MULTI THREADS SIMULTANEOUSLY READS EVERY LINE OF FUNCTION.
                return 0;
        }
        fgets(linewise,sizeof(linewise),filepointer);
        histogramcalculator(linewise);
}

int main(int argc,char * argv[]){
       filepointer=fopen("realtest","r+");
       pthread_mutex_init(&mutex, NULL);
       int k;
       if (argc!=2){
               printf(" You must enter number of threads beside ./a.out\n");
               return 0;
       }
       int noofthreads= atoi(argv[1]);
       pthread_t th[noofthreads];
       if(noofthreads<1 || noofthreads>4){                                            //PROGRAM WILL BE SHUTDOWN IF THREADS RANGE VARY THE RANGE 1,4.
            printf("Enter number of threads in range 1 to 4\n");
         return 0;
       }

       while(!feof(filepointer)) {
       for(int i=0;i<noofthreads;i++){
              k = i;
              if(!feof(filepointer))
               pthread_create(th+i,NULL,&filereading,NULL);                            //BASED ON GIVEN INPUT THREADS ARE CREATED SIMULTANEOUSLY.
                else {
                     break;


              }


//              printf("\n");
               sleep(0.00001);

       }
       for(int i=0;i<noofthreads;i++){
               if(k--)
                       break;
               pthread_join(th[i],NULL); //THREADS ARE JOINED TO STOP MAIN UNTILL COMPLETION OF ALL MULTI THREADS.
       }
       }
         pthread_mutex_destroy(&mutex);
         output();// CALLING OUTPUT OF GLOBALLY ARRAY AFTER CALCULATING FREQUENCY OF EVERY CHARACTER.
         printf("\n");
       return 0;

}
