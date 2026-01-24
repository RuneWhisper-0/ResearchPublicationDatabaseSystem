#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void header(){
    int a=0;
    while(a<=50){
    printf("=");
    a++;}
    printf("\n\033[1mRESEARCH PUBLICATION DATABASE SYSTEM\033[0m\n");
    printf("Department of Mechanical Engineering,BUET\n");
    while(a<=101){
    printf("=");
    a++;}
}
struct faculty{
    char id[20];
    char sn[10];
    char desi[40];
    char RA[200];
};
void main_menu(){
    printf("--------------------------------\n");
    printf("          MAIN MENU             \n");
    printf("--------------------------------\n");
    printf("[1] Add Faculty\n");
    printf("[2] Display Faculty Profile\n");
    printf("[3] Exit\n");
    printf("--------------------------------\n");
}
void add_faculty(){
    struct faculty f1;

    FILE *af;
    af=fopen("FacultyProfile.txt","a");
    //fprintf(af,"--------------------------------------FACULTY INFORMATION--------------------------------------\n");
    //fprintf(af,"Faculty ID\t\tShort Name\t\tDesignation\t\t\tResearch Area\n");
    //fprintf(af,"-----------------------------------------------------------------------------------------------\n");
    if(af==NULL){
       printf("Error opening file!\n");}
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter faculty id: ");
    fgets(f1.id, sizeof(f1.id),stdin);
    f1.id[strcspn(f1.id, "\n")] = 0;

    printf("Enter faculty short name: ");
    fgets(f1.sn,sizeof(f1.sn),stdin);
    f1.sn[strcspn(f1.sn, "\n")] = 0;

    printf("Enter designation: ");
    fgets(f1.desi,sizeof(f1.desi),stdin);
    f1.desi[strcspn(f1.desi, "\n")] = 0;

    printf("Enter Research Area: ");
    fgets(f1.RA,sizeof(f1.RA),stdin);
    f1.RA[strcspn(f1.RA, "\n")] = 0;

    fprintf(af,"%-15s\t\t%-15s\t\t%-25s\t%s\n", f1.id, f1.sn, f1.desi, f1.RA);
    fclose(af);
    printf("\nRecord Saved!\n");
}
void display_faculty(){
    char disfac[500];
    FILE *df;
    df=fopen("FacultyProfile.txt","r");
    if(df==NULL) {
       printf("Error opening file!\n");}

    while(fgets(disfac,500,df) !=0){
        printf("%s",disfac);
    }
    fclose(df);
}
int choice(){
    int c;
    printf("Choice: ");
    scanf("%d",&c);
    printf("\n\n");
    switch(c){
    case 1:
        add_faculty();
        choice();
        break;
    case 2:
        display_faculty();
        choice();
        break;
    case 3:
        printf("Data Saved. Exited successfully!");
        return 0;break;
    default:
        printf("Invalid Choice!\nEnter again\n");
        choice();
    }
}

void main()
{
    header();
    printf("\n\n");
    main_menu();
    choice();
}
