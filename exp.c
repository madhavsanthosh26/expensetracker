#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


#define MAXDNLEN 100
#define MAXDELEN 50
#define FILENAME "expense.txt"

typedef struct{
    char description[MAXDNLEN];
    int id;
    char date[MAXDELEN];
    int amount;
} Expense;

int getid() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return 0;

    int id = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%*[^,],%*[^,],%*d", &id);
    }
    fclose(file);
    return id;
}

void addexp(const char *description, int amount){
    FILE *file = fopen(FILENAME,"a");
    if(!file){
        printf("file doesn't exist!\n");
        return;
    }
    
    //getting information about the date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[MAXDELEN];
    strftime(date, sizeof(date),"%Y-%m-%d", tm_info);

    int id = getid()+1;
    fprintf(file, "%d %s %s %d\n", id, date, description, amount);
    fclose(file);
    
    printf("Expense added successfully");
}

void listexp(){
    FILE *file = fopen(FILENAME,"r");
    if (!file){
        printf("no expenses added.\n");
        return;
    }
    printf("ID  DATE      DESCRIPTION     AMOUNT\n");
    char line[100];
    while(fgets(line,sizeof(line), file)){
        Expense e;
        e->id = 0;
        e->amount = 0;
        memset(e->date, 0,sizeof(e->date));
        memset(e->description, 0, sizeof(e->description));
        sscanf(line,"%d %[^,] %[^,] %d", &e.id, e.date, e.description, &e.amount);
        printf("%d %s %s %d\n",e.id, e.date, e.description, e.amount);
    }
    fclose(file);
}

void deletexp(int id){
    FILE *file = fopen(FILENAME,"r");
    if(!file){
        printf("No expenses to be deleted.\n");
        return;
    }
    
    FILE *temp = fopen("temp.txt","w");
     if (!temp) {
        fclose(temp);
        printf("Could not open temporary file.\n");
        return;
    }
    int found = 0;
    char line[100];
    while(fgets(line,sizeof(line),file)){
        Expense e;
        sscanf(line,"%d [^,] [^,] %d", &e.id, e.date, e.description, &e.amount);
        if (e.id != id){
            fprintf(temp, "%d %S %S %d",e.id, e.date, e.description, e.amount);
        }
        else{
            found = 1;
        }    
    }
    fclose(temp);
    fclose(file);
    remove(FILENAME);
    rename("temp.txt",FILENAME);
    if(found == 1){
        printf("Expense deleted successfully! \n");
    }
    else{
        printf("Expense with id :%d not found \n");
    }
}

int main(int argc , char *argv[]){
    if(argc<2){
        printf("correct usage: expense-tracker <command> [options] \n");
        return 1;
    }

    if(strcmp(argv[1],"add") == 0){
        char *description = NULL;
        float amount = 0;
        for (int i = 2; i<argc; i++){
            if(strcmp(argv[i],"--desc") == 0 && i+1 < argc) //move to the next argument and assign it to the description 
            {
                description = argv[++i];
            }
            if(strcmp(argv[i],"--amount")==0 && i+1 < argc){
                amount = atof(argv[++i]);
            }
        }
        if(description && amount > 0){
            addexp(description, amount);
        }
        else
            printf("provide both information");
    }

    else if((strcmp(argv[1],"delete")==0)){
        int  id = 0;
        if(argc<2){
        printf("correct usage: expense-tracker <command> [options] \n");
        return 1;
        }
        for(int i = 2; i< argc; i++){
            if (strcmp(argv[1],"delete")== 0 && i+1<argc){
                id = atoi(argv[++i]);
            }
        }    
        if(id>0){
            deletexp(id);
        }
        else{
            printf("please enter an id! \n");
        }       
    }
    else if((strcmp(argv[1],"list"))==0){
        listexp();
    }

    return 0;

}
