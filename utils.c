/***********************************************/
//  DE LINEA DE COMANDO A VECTOR DE COMANDOS
int trimargs(char* line, char** comline) {//comline is our vector for all the arguments and commands inserted on the line
    int cant=0;//cantidad de palabras en la linea
    for(int i=0; i<63; i++) { //we'll admit at max 64 words between commands and arguments in one line
        comline[i] = strsep(&line, " ");//adds the next found word to the vector
        cant=i;
        if(comline[i] == NULL) break;//if we reach EOF
    }
    return(cant);//we return yhe cuantitie
};//after this we should check first for the simple commands,then for the | & or ; 
//omar note: still needs to be tested

char* trimredir(char* s){
    char* file= malloc(1*sizeof(char));
    char c=' ';
    int cont=10;
    int pos;
    int sizefile=1;
    for(int i=0;i<strlen(s);i++){
        if((s[i]==' ' && s[i+1]=='I') && (s[i+2]=='N' && s[i+3]=='P')){
            return file;
        }
        if((s[i]==' ' && s[i+1]=='O') && (s[i+2]=='U' && s[i+3]=='T')){
            return file;
        }
        file[i]=s[i];
        sizefile++;
        file=realloc(file, sizefile*sizeof(char));
    }
    return NULL;    
}

char* getinput(char* s){
	char* file= malloc(1*sizeof(char));
	char c=' ';
	int cont=10;
	int pos;
	int sizefile=0;
	for(int i=0;i<strlen(s);i++){
    	if(s[i]=='I' && s[i+1]=='N'){
    		c=s[i+cont];
    		sizefile=0;
    		pos=i;
    		while(c!=' ' && c!='\0'){
    			sizefile++;
    			file=realloc(file, sizefile*sizeof(char));
    			file[sizefile-1]=c;
    			cont++;
    			i++;
    			c=s[pos+cont];
    		}
    		return file;
    	}
    }
    return NULL;	
}

char* getoutput(char* s){
	char* file= malloc(1*sizeof(char));
	char c=' ';
	int cont=11;
	int pos;
	int sizefile=0;
	for(int i=0;i<strlen(s);i++){
    	if(s[i]=='O' && s[i+1]=='U'){
    		c=s[i+cont];
    		sizefile=0;
    		pos=i;
    		while(c!=' ' && c!='\0'){
    			sizefile++;
    			file=realloc(file, sizefile*sizeof(char));
    			file[sizefile-1]=c;
    			cont++;
    			i++;
    			c=s[pos+cont];
    		}
    		return file;
    	}
    }
    return NULL;	
}

char* trimfile(char* s){
	char* file= malloc(1*sizeof(char));
	for(int i=0;i<strlen(s);i++){
    	if(s[i]=='I' && s[i+1]=='N'){
    		int size=(strlen(s)-(i+9));
    		file=malloc((size)*(sizeof(char)));
    		for(int j=0;j< size ;j++){
    			file[j]=s[i+j+10];
    		}
    		return file;
    	}
    	if(s[i]=='O' && s[i+1]=='U'){
    		int size=(strlen(s)-(i+10));
    		file=malloc((size)*(sizeof(char)));
    		for(int j=0;j< size ;j++){
    			file[j]=s[i+j+11];
    		}
    		return file;
    	}
    }
    return file;
}