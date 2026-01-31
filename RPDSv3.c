#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int faculty_count=0;//from abiba // have to use globally so it can be used in every function
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
struct record{//from jannat
    char year[20];
    char type[40];
    char sn[20];
    char title[400];
};
void main_menu(){//from rahat
    printf("--------------------------------\n");
    printf("          MAIN MENU             \n");
    printf("--------------------------------\n");
    printf("[1] Add Faculty\n");
    printf("[2] Display Faculty Profile\n");
    printf("[3] Add Publication\n");
    printf("[4] Display Publication Record\n");
    printf("[5] Search\n");
    printf("[6] Exit\n");
    printf("--------------------------------\n");
}
// Function to count faculty from file
void count_faculty(){ //from abiba

    char line[500];
    FILE *cf;
    cf = fopen("FacultyProfile.txt", "r");

    if (cf == NULL) {
        printf("Error Opening File!\n");
        return;
    }
    faculty_count = -3;//3 lines for the headline
    while(fgets(line, 500, cf) != NULL){
        faculty_count++;
    }
    fclose(cf);
}
void add_faculty(){ //from abiba
    struct faculty f1;

    FILE *af;
    af=fopen("FacultyProfile.txt","a");
    //fprintf(af,"--------------------------------------FACULTY INFORMATION--------------------------------------\n");
    //fprintf(af,"Faculty ID\t\tShort Name\t\tDesignation\t\t\tResearch Area\n");
    //fprintf(af,"-----------------------------------------------------------------------------------------------\n");
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
    faculty_count++;
    printf("\nRecord Saved!\n");
    printf("\nTotal Faculty Records: %d\n",faculty_count);
    printf("\nRecord Saved!\n");
}
void display_faculty(){ //from abiba

    char disfac[500];
    FILE *df;
    df=fopen("FacultyProfile.txt","r");
    while(fgets(disfac,500,df) !=0){
        printf("%s",disfac);
    }
    fclose(df);

    // Print dashes using loop
    for(int i = 0; i < 95; i++){
        printf("-");
    }
    printf("\n");
    printf("Total Faculty Records: %d\n", faculty_count);
}
void add_publication_record(){//from jannat
struct record r1;

    FILE *bf;

    bf = fopen("PublicationRecords.txt", "r");
    if (bf == NULL)
    {
    bf=fopen("PublicationRecords.txt","a");
    fprintf(bf,"--------------------------------------Publication Records--------------------------------------\n");
    fprintf(bf,"Year\t\tType\t\tFaculty\t\tTitle\n");
    fprintf(bf,"-----------------------------------------------------------------------------------------------\n");
    }
    bf = fopen("PublicationRecords.txt", "a");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter year of publication: ");
    fgets(r1.year, sizeof(r1.year), stdin);
    r1.year[strcspn(r1.year, "\n")] = 0;

    printf("Enter type of publication: ");
    fgets(r1.type, sizeof(r1.type), stdin);
    r1.type[strcspn(r1.type, "\n")] = 0;

    printf("Enter faculty short name: ");
    fgets(r1.sn, sizeof(r1.sn), stdin);
    r1.sn[strcspn(r1.sn, "\n")] = 0;

    printf("Enter title: ");
    fgets(r1.title, sizeof(r1.title), stdin);
    r1.title[strcspn(r1.title, "\n")] = 0;

    fprintf(bf,"%-15s\t%-15s\t%-15s\t%s\n", r1.year, r1.type, r1.sn, r1.title);
    fclose(bf);
    printf("\nRecord Saved!\n");
}
void display_publication_records(){//from jannat
 char dispub[500];
    FILE *bf;
    bf=fopen("PublicationRecords.txt","r");
     if (bf == NULL)
        printf("Error: File not found.\n");
    while(fgets(dispub,500,bf) !=0){
        printf("%s",dispub);
    }
    fclose(bf);
}
void search_faculty() {//from rahat
    char key[50];
    char line[500];
    int tf = 0;
    int tp = 0;
    FILE *fp;
    FILE *bf;
    int head;

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter Faculty Short Name: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    printf("\n--- SEARCH RESULTS ---\n");

    fp = fopen("FacultyProfile.txt", "r");
    if (fp != NULL) {
    head = 0;

    while (fgets(line, 500, fp) != NULL) {
    if (strstr(line, key) != NULL) {
    if (head == 0) {
    printf("\n[FACULTY PROFILE]\n");
    printf("Faculty ID\t\tShort Name\t\tDesignation\t\t\tResearch Area\n");
     printf("-----------------------------------------------------------------------------------------------\n");
    head = 1;
    }
    printf("%s", line);
    tf = 1;
    }
    }
    fclose(fp);
    }

    bf = fopen("PublicationRecords.txt", "r");
    if (bf != NULL) {
    head = 0;

    while (fgets(line, 500, bf) != NULL) {
    if (strstr(line, key) != NULL) {
    if (head == 0) {
    printf("\n[PUBLICATION RECORDS]\n");
    printf("Year\t\tType\t\tFaculty\t\tTitle\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    head = 1;
    }
    printf("%s", line);
    tp = 1;
    }
    }
    fclose(bf);
    }

    if (tf == 0 && tp == 0) {
    printf("\nNo matches found.\n");
    }

    printf("\n");
    //choice(); //it gives error as choice is not declared before this
    }
int choice(){//from rahat
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
        add_publication_record();
        choice();
        break;
    case 4:
        display_publication_records();
        choice();
        break;

      case 5:
        search_faculty();
        choice();//solved
        break;
      case 6:
        printf("Data Saved. Exited successfully!");
        return 0;
        break;
    default:
        printf("Invalid Choice!\nEnter again\n");
        choice();
        break;
    }
}

void main()
{
    header();
    printf("\n\n");
    count_faculty();
    printf("Data loaded successfully.\n");
    printf("Total Faculty Records: %d\n\n", faculty_count);//from abiba
    main_menu();
    choice();
}
