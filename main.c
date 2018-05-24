#include <fcntl.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "utils.c"


int commandsinline=0;
#define PARAMS 64
/*
  Function Declarations for builtin shell commands:
 */
void cdcommand(char *line);
void helpcommand();
int exitcommand();
void redir(char* line);
void bg(char* line);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char* commands[]= {
  "cd",
  "help",
  "QUIT",
  "REDIR",
  "BG"
};

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
void cdcommand(char *line)
{
  if (line == NULL) {
    fprintf(stderr, "Escriba el directorio después de cd\n");
  } else {
    if (chdir(line) != 0) {
      perror("lsh");
    }
  }
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int startsh(char *line)
{
  pid_t pid, wpid;
  int status;
  bool m=false;

    m=false;
    char* complete= malloc(strlen(line)*sizeof(char));
    strcpy(complete,line);

    char* command = strsep(&line, " ");

    //Si el comando ingresado es cd
    if(!strcmp(command ,commands[0]) && !m){    
      cdcommand(line);
      m=true;
    }
    //Si el comando ingresado es help
    if(!strcmp(command,commands[1]) && !m){
       printf("Este lindo shell es propiedad de Omar y Valentina\n");
  	printf("Escriba por favor el comando seguido de parametros u otros programas\n");
  	printf("Recuerde usar caracteres como '|' '&&' o ';'\n");
	printf("O escribir '--help' al lado del comando.\n");
	printf("Por favor, evitar usar init 0 :c.\n");
      m=true;
    }
    //Si el comando ingresado es QUIT
    if(!strcmp(command,commands[2]) && !m){
      return 0;
    }

    //Si el comando ingresado es REDIR
    if(!strcmp(command,commands[3]) && !m){
      m=true;
      redir(line);
    }

    //Si el comando ingresado es BG
    if(!strcmp(command,commands[4]) && !m){
      m=true;
      bg(line);
    }    

    pid=fork();

    if(pid==0){
      char *execArgs[64];
      trimargs(line, execArgs);
      execvp(command, execArgs);
      m=true;
      exit(0);
    }
    else{     
        while ((wpid = wait(&status)) > 0);
    }

  return 1;
}

/***********************************************/
//	LEER LA LINEA INGRESADA
#define LSH_RL_BUFSIZE 1024
char *readlinesh(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void redir(char* line){

    bool inputmod=false;
    bool outputmod=false;
    int fdi, fdo;
    char* input=getinput(line);
    char* output=getoutput(line);
    char* newline=trimredir(line);
    char* args[64];
    trimargs(newline, args);

      pid_t pid, wpid;
      int status;

      pid=fork();

    if(pid==0){

      if(input != NULL){
        fdi = open(input, O_RDONLY);        
        dup2(fdi, STDIN_FILENO);
        close(fdi);
      }
      if(output != NULL){
        fdo = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0666);        
        dup2(fdo, STDOUT_FILENO);
        close(fdo);
      } 
      execvp(args[0], args);
      exit(0);
    }
    else{     
        while ((wpid = wait(&status)) > 0);
    }    
}
/***********************************************/
//		FUNCION PARA CORRER PIPES
int pipes(char ** comline, char ** comline2){
	int fd[2];
	pipe(fd);
	int i;
	pid_t pid = fork();
	//Error case
	if(pid == -1){
	    printf("Fork error");
	    return 1;
	}
	//Hijo
	//Correr segundo comando
	if(pid == 0){
	close(fd[1]);
	dup2(fd[0],0);
	execvp(comline2[0],comline2);
	return 0;
	}
	//padre
	//El padre se encargara de correr el primer comando
	else{
	close(fd[0]);
	dup2(fd[1],1);
	execvp(comline[0],comline);
	return 0;
	}	
	
}

void bg(char* line){
  char* args[64];
  trimargs(line, args);
  pid_t pid, pid2;
  int status;

  pid=fork();

  if(pid==0){
    pid2=fork();
    if(pid2==0){
      trimargs(line, args);
      execvp(args[0],args);
    }
  }

}


/**
   @return status code
 */
//1267507
int main(int argc, char **argv)
{
  // Se realiza un ciclo infinito mientras status determine que el usuario no desea salir .
  char * comline [64];//arreglo de 64 espacion para comandos
  char * comaux1 [64] = {0};//arreglos auxiliares para pipes
  char * comaux2 [64] = {0};
  char *line;//linea que se inserta
  int status,cant=0, aux=0,aux2=0;//cant: cantidad de comandos insertada
  char** commands;
  bool pipe = false, amper=false, dot=false;

  do {
    commandsinline=0;
    printf("\n> ");
    line = readlinesh();
    char *cline= malloc(strlen(line)*sizeof(char));
    strcpy(cline, line);
    //trimcommands(line);
    cant=trimargs(line, comline);
	/*// BLOQUE DE PRUEBA DE ENTRADA DE LINEA Y TRIM2 CORRECTO
	for(int i=0; i<63; i++) { 
        printf("%s",comline[i]);
  	if(comline[i] == NULL) break;
	}*/
    //regularcomm(comline);
    for (int i =0; i <cant; i++) { //    
        if (strcmp(comline[i], "|") == 0) {    
            aux = i;
	           pipe=true;      
         //  printf("pipe found\n");
           break;
        }               
    }

    if(pipe==true){//HERE
	//printf("entre a pipe true\n");
	for (int i=0; i<aux; i++) {
	comaux1[i]=comline[i];
	//printf("%s",comaux1[i]);
	//printf("\n");
	}
        aux2=0;
	for (int i=aux+1; i< cant; i++) {  //empieza en la posicion despues del pipe y sigue hasta que se acaba   

	//printf("Here2: \n");        
	comaux2[aux2]=comline[i];
	//printf("%s",comaux2[aux2]);
	//printf("\n");
                aux2++;
        }     
	if(pipes(comaux1,comaux2)==0) {
	//printf("ejecuto\n");	
	break;
	}
    }
    else if(pipe==false){
    //printf("entre a pipe false\n");
    status=startsh(cline);
    }
  
  } while (status);

  return 1;
}

