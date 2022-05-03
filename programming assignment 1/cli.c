#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <stdbool.h>


int main(){
FILE* file;
char line[256];
   
        file = fopen("./commands.txt","r");


if(file==NULL){
        perror("Unable to open file");
        exit(1);
}
while(fgets(line, sizeof(line), file)){
    

        char *myargs[10];
        char *word = strtok(line," ");
        int i = 0;
        while(word != NULL){
            
            int k,j;
            long unsigned int len = strlen(word);
            for(k = 0; k < len; k++)
          {
              if(word[k] == '\n' || word[k] == '\r' || word[k] == '"'  )
              {
                  for(j = k; j < len; j++)
                  {
                      word[j] = word[j + 1];
                  }
                  len--;
                  k--;
              }
          }
                myargs[i]=word;
                word =strtok(NULL, " ");
               i++;
  
        }
    int n=0;
    int x;
    bool io = false;
    char *myargs2[]={"null","null","null","null","null","null","null","null"} ;
    char *myargs3[i+1];
    myargs3[i]=NULL;
    
    bool inp=false;
    while (n<i) {
        char* str = strdup(myargs[n]);

        if (n==0) {
            myargs2[0]=strdup(str);
        }
        if (*str == '-') {
            myargs2[2]=strdup(str);
        }
       if (*str == '"') {
            myargs2[3]=str;
        }
        if (n==1  && *str != '-' && *str !='&' ) {
            myargs2[1]=strdup(str);
            inp=true;
        }
        if(n==2 && *str != '-' && *str !='&' && !inp && strlen(str) != 0 ) {
            
            myargs2[1]=strdup(str);
            
            
        }
        if ((strchr(str, '>') !=NULL  || strchr(str, '<') !=NULL)) {
            myargs2[4]=strdup(str);
            x = n;
            io=true;
        }
        if (io) {
            myargs2[5]=strdup(myargs[n]);
        }
        if (*str =='&') {
            myargs2[6]=strdup(str);
        }
        
        n++;
    }
    printf("%s\n", "----------");
    
    if(strcmp( myargs2[0], "null") != 0){
        printf("%s","Command: ");
        printf("%s\n", myargs2[0]);
    }else{
        printf("%s\n","Command:");
    }
    
    if(strcmp( myargs2[1], "null") != 0){
        printf("%s","Inputs: ");
        printf("%s\n", myargs2[1]);
    }else{
        printf("%s\n","Inputs:");
    }
    
    if(strcmp( myargs2[2], "null") != 0){
        printf("%s","Options: ");
        printf("%s\n", myargs2[2]);
    }else{
        printf("%s\n","Options:");
    }
    
    if(strcmp( myargs2[4], "null") != 0){
        printf("%s","Redirection: ");
        printf("%s\n", myargs2[4]);
    }else{
        printf("%s\n","Redirection: -");
    }
    
    if(strcmp( myargs2[6], "null") != 0){
        printf("%s\n","Background: y");
    }else{
        printf("%s\n","Background: n");
    }
    printf("%s\n", "----------");
    
    int r=0;
    int p =0;
    int io2= false;
    while (r<7) {
        if (strchr(myargs2[r], '<') == NULL && strchr(myargs2[r], '>') ==NULL && !io2){
            if(strcmp( myargs2[r], "null") != 0 && strchr(myargs2[r], '&') == NULL ){

                myargs3[p]=myargs2[r];
                p++;
            }
       
        }else{
            io2=true;
        }
        r++;
    }
    
    while (p<i) {
        myargs3[p]=NULL;
        p++;
    }
    int status;
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
     
        if (strchr(myargs2[4], '>') !=NULL ) {
            
            close(STDOUT_FILENO);
            open(myargs2[5], O_CREAT|O_WRONLY|O_TRUNC);
        }else if (strchr(myargs2[4], '<') !=NULL ){
  
            int fd = open(myargs2[5], O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        //printf("%s","Child: ");
        //printf("hello world (pid:%d)\n", (int) getpid());

        execvp(myargs3[0], myargs3);
    } else {
        
        //printf("%s","Parent: ");
        //printf("hello world (pid:%d)\n", (int) getpid());
        if (strcmp( myargs2[6], "null") == 0) {
            waitpid(rc,&status,0);
        }
     
      
    }

     
}

fclose(file);
    
return 0;
}
