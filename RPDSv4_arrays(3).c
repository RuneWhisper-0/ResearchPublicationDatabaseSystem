#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Max array sizes
#define MAX_FACULTY 100
#define MAX_PUB 1000

//Global counters
int faculty_count = 0;
int publication_count = 0;

//Faculty structure
struct faculty{
    char id[20];
    char full_name[100];
    char sn[10];
    char desi[40];
    char RA[200];
};

//Publication structure
struct record{
    char year[20];
    char type[40];
    char sn[20];
    char title[400];
    char journal[200];
    char doi[100];
    char indexing[50];
};

//Global arrays - loaded at startup
struct faculty faculty_array[MAX_FACULTY];
struct record pub_array[MAX_PUB];

//Header display
void header(){
    int a = 0;
    while(a <= 50){
        printf("=");
        a++;
    }
    printf("\n\033[1mRESEARCH PUBLICATION DATABASE SYSTEM\033[0m\n");
    printf("Department of Mechanical Engineering, BUET\n");
    while(a <= 101){
        printf("=");
        a++;
    }
    printf("\n");
}

//Main menu
void main_menu(){
    printf("\n--------------------------------\n");
    printf("          MAIN MENU             \n");
    printf("--------------------------------\n");
    printf("[1] Add Faculty\n");
    printf("[2] Display Faculty Profile\n");
    printf("[3] Add Publication\n");
    printf("[4] Display Publication Record\n");
    printf("[5] Search\n");
    printf("[6] Generate Reports\n");
    printf("[7] Edit Record\n");
    printf("[8] Delete Record\n");
    printf("[9] Statistics Dashboard\n");
    printf("[10] Help\n");
    printf("[11] Exit\n");
    printf("--------------------------------\n");
}

//Clear input buffer
void clear_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Convert string to uppercase
void to_upper(char *str){
    for(int i = 0; str[i]; i++){
        str[i] = toupper(str[i]);
    }
}

//Clear screen - cross-platform
void clear_screen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//Pause and wait for user
void pause_screen(){
    printf("\nPress Enter to continue...");
    clear_buffer();
    getchar();
}

//Validate year - digits only, 1900-2030
int validate_year(char *year){
    for(int i = 0; year[i] != '\0'; i++){
        if(!isdigit(year[i])){
            return 0;
        }
    }
    int y = atoi(year);
    if(y < 1900 || y > 2030){
        return 0;
    }
    return 1;
}

//Trim trailing whitespace/tabs/newlines (handles Windows \r\n too)
void trim(char *str){
    int len = strlen(str);
    while(len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' ||
                      str[len-1] == '\n' || str[len-1] == '\r')){
        str[len-1] = '\0';
        len--;
    }
}

// ===================== LOAD / SAVE =====================

//Load faculty from file into array
//Format: "%-15s\t\t%-15s\t\t%-30s\t%-25s\t%s\n"
//        id \t\t sn \t\t full_name \t desi \t RA
void load_faculty(){
    FILE *fp = fopen("FacultyProfile.txt", "r");
    if(fp == NULL) return;

    char line[500];
    int line_num = 0;
    faculty_count = 0;

    while(fgets(line, 500, fp) != NULL && faculty_count < MAX_FACULTY){
        line_num++;
        if(line[0] == '\n' || line[0] == '\r') continue;  //Skip blank lines

        char temp[500];
        strcpy(temp, line);

        //Use \t as delimiter - skip empty tokens caused by double tabs
        char *tokens[10];
        int tok_count = 0;
        char *t = strtok(temp, "\t");
        while(t != NULL && tok_count < 10){
            trim(t);
            if(strlen(t) > 0){  //Skip empty tokens (from double \t\t)
                tokens[tok_count++] = t;
            }
            t = strtok(NULL, "\t");
        }

        //Need at least 5 fields: id, sn, full_name, desi, RA
        if(tok_count >= 5){
            strncpy(faculty_array[faculty_count].id,        tokens[0], sizeof(faculty_array[0].id)-1);
            strncpy(faculty_array[faculty_count].sn,        tokens[1], sizeof(faculty_array[0].sn)-1);
            strncpy(faculty_array[faculty_count].full_name, tokens[2], sizeof(faculty_array[0].full_name)-1);
            strncpy(faculty_array[faculty_count].desi,      tokens[3], sizeof(faculty_array[0].desi)-1);
            strncpy(faculty_array[faculty_count].RA,        tokens[4], sizeof(faculty_array[0].RA)-1);
            trim(faculty_array[faculty_count].RA);  //Remove trailing newline
            faculty_count++;
        }
    }
    fclose(fp);
}

//Load publications from file into array
//Format: "%-10s\t%-20s\t%-10s\t%-30s\t%-10s\t%s\n"
//        year \t type \t sn \t journal \t indexing \t title
void load_publications(){
    FILE *fp = fopen("PublicationRecords.txt", "r");
    if(fp == NULL) return;

    char line[600];
    int line_num = 0;
    publication_count = 0;

    while(fgets(line, 600, fp) != NULL && publication_count < MAX_PUB){
        line_num++;
        if(line[0] == '\n' || line[0] == '\r') continue;  //Skip blank lines
        char temp[600];
        strcpy(temp, line);

        //Skip empty tokens from any double-tab occurrences
        char *tokens[10];
        int tok_count = 0;
        char *t = strtok(temp, "\t");
        while(t != NULL && tok_count < 10){
            trim(t);
            if(strlen(t) > 0){
                tokens[tok_count++] = t;
            }
            t = strtok(NULL, "\t");
        }

        //Need at least 6 fields: year, type, sn, journal, indexing, title
        if(tok_count >= 6){
            strncpy(pub_array[publication_count].year,     tokens[0], sizeof(pub_array[0].year)-1);
            strncpy(pub_array[publication_count].type,     tokens[1], sizeof(pub_array[0].type)-1);
            strncpy(pub_array[publication_count].sn,       tokens[2], sizeof(pub_array[0].sn)-1);
            strncpy(pub_array[publication_count].journal,  tokens[3], sizeof(pub_array[0].journal)-1);
            strncpy(pub_array[publication_count].indexing, tokens[4], sizeof(pub_array[0].indexing)-1);
            strncpy(pub_array[publication_count].title,    tokens[5], sizeof(pub_array[0].title)-1);
            trim(pub_array[publication_count].title);  //Remove trailing newline
            publication_count++;
        }
    }
    fclose(fp);
}

//Save faculty array to file
void save_faculty(){
    FILE *fp = fopen("FacultyProfile.txt", "w");
    if(fp == NULL){
        printf("Error saving faculty data!\n");
        return;
    }


    //Write array
    for(int i = 0; i < faculty_count; i++){
        fprintf(fp, "%-15s\t\t%-15s\t\t%-30s\t%-25s\t%s\n",
            faculty_array[i].id,
            faculty_array[i].sn,
            faculty_array[i].full_name,
            faculty_array[i].desi,
            faculty_array[i].RA);
    }
    fclose(fp);
}

//Save publication array to file
void save_publications(){
    FILE *fp = fopen("PublicationRecords.txt", "w");
    if(fp == NULL){
        printf("Error saving publication data!\n");
        return;
    }


    //Write array
    for(int i = 0; i < publication_count; i++){
        fprintf(fp, "%-10s\t%-20s\t%-10s\t%-30s\t%-10s\t%s\n",
            pub_array[i].year,
            pub_array[i].type,
            pub_array[i].sn,
            pub_array[i].journal,
            pub_array[i].indexing,
            pub_array[i].title);
    }
    fclose(fp);
}

//Initialize files with headers if they don't exist
void initialize_files(){
    FILE *fp;

    fp = fopen("FacultyProfile.txt", "r");
    if(fp == NULL){
        fp = fopen("FacultyProfile.txt", "w");
        fclose(fp);
    } else {
        fclose(fp);
    }

    fp = fopen("PublicationRecords.txt", "r");
    if(fp == NULL){
        fp = fopen("PublicationRecords.txt", "w");
        fclose(fp);
    } else {
        fclose(fp);
    }
}

// ===================== LOOKUP HELPERS =====================

//Check if faculty ID already exists (searches array)
int faculty_id_exists(char *id){
    for(int i = 0; i < faculty_count; i++){
        if(strcmp(faculty_array[i].id, id) == 0){
            return 1;
        }
    }
    return 0;
}

//Check if faculty short name exists (searches array)
int faculty_exists(char *sn){
    for(int i = 0; i < faculty_count; i++){
        if(strcmp(faculty_array[i].sn, sn) == 0){
            return 1;
        }
    }
    return 0;
}

//Check if publication title already exists (searches array)
int publication_exists(char *title){
    for(int i = 0; i < publication_count; i++){
        if(strstr(pub_array[i].title, title) != NULL){
            return 1;
        }
    }
    return 0;
}

// ===================== CORE FUNCTIONS =====================

//Add faculty
void add_faculty(){
    if(faculty_count >= MAX_FACULTY){
        printf("\nError: Faculty database is full! (%d max)\n", MAX_FACULTY);
        return;
    }

    struct faculty f1;
    clear_buffer();

    printf("Enter faculty ID (e.g., MEF-01): ");
    fgets(f1.id, sizeof(f1.id), stdin);
    f1.id[strcspn(f1.id, "\n")] = 0;

    if(faculty_id_exists(f1.id)){
        printf("\nError: Faculty ID '%s' already exists!\n", f1.id);
        return;
    }

    printf("Enter full name: ");
    fgets(f1.full_name, sizeof(f1.full_name), stdin);
    f1.full_name[strcspn(f1.full_name, "\n")] = 0;

    if(strlen(f1.full_name) >= sizeof(f1.full_name) - 1){
        printf("Error: Name too long! Max %d characters.\n", (int)sizeof(f1.full_name)-1);
        return;
    }

    printf("Enter short name (e.g., MHK): ");
    fgets(f1.sn, sizeof(f1.sn), stdin);
    f1.sn[strcspn(f1.sn, "\n")] = 0;
    to_upper(f1.sn);

    printf("Enter designation: ");
    fgets(f1.desi, sizeof(f1.desi), stdin);
    f1.desi[strcspn(f1.desi, "\n")] = 0;

    printf("Enter research area: ");
    fgets(f1.RA, sizeof(f1.RA), stdin);
    f1.RA[strcspn(f1.RA, "\n")] = 0;

    //Add to array
    faculty_array[faculty_count] = f1;
    faculty_count++;

    //Auto-save
    save_faculty();

    printf("\n\033[1mRecord Saved!\033[0m\n");
    printf("Total Faculty Records: %d\n", faculty_count);
}

//Display all faculty from array
void display_faculty(){
    if(faculty_count == 0){
        printf("\nNo faculty records found.\n");
        return;
    }

    printf("\n--------------------------------------FACULTY INFORMATION----------------------------------------------------------------------------------------\n");
    printf("Faculty ID\t\tShort Name\t\tFull Name\t\t\tDesignation\t\t\tResearch Area\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for(int i = 0; i < faculty_count; i++){
        printf("%-15s\t\t%-15s\t\t%-30s\t%-25s\t%s\n",
            faculty_array[i].id,
            faculty_array[i].sn,
            faculty_array[i].full_name,
            faculty_array[i].desi,
            faculty_array[i].RA);
    }

    for(int i = 0; i < 95; i++) printf("-");
    printf("\nTotal Faculty Records: %d\n", faculty_count);
}

//Add publication
void add_publication_record(){
    if(publication_count >= MAX_PUB){
        printf("\nError: Publication database is full! (%d max)\n", MAX_PUB);
        return;
    }

    struct record r1;
    clear_buffer();

    printf("Enter year of publication: ");
    fgets(r1.year, sizeof(r1.year), stdin);
    r1.year[strcspn(r1.year, "\n")] = 0;

    if(!validate_year(r1.year)){
        printf("\nError: Invalid year! Must be 1900-2030, digits only.\n");
        return;
    }

    printf("Enter type (Journal/Conference/Review/Book Chapter): ");
    fgets(r1.type, sizeof(r1.type), stdin);
    r1.type[strcspn(r1.type, "\n")] = 0;

    printf("Enter faculty short name: ");
    fgets(r1.sn, sizeof(r1.sn), stdin);
    r1.sn[strcspn(r1.sn, "\n")] = 0;
    to_upper(r1.sn);

    if(!faculty_exists(r1.sn)){
        printf("\nWarning: Faculty '%s' not found in database.\n", r1.sn);
        printf("Continue anyway? (y/n): ");
        char ch;
        scanf(" %c", &ch);
        if(ch != 'y' && ch != 'Y') return;
    }

    printf("Enter journal/conference name: ");
    clear_buffer();
    fgets(r1.journal, sizeof(r1.journal), stdin);
    r1.journal[strcspn(r1.journal, "\n")] = 0;

    printf("Enter indexing (Scopus/WoS/SCI/etc): ");
    fgets(r1.indexing, sizeof(r1.indexing), stdin);
    r1.indexing[strcspn(r1.indexing, "\n")] = 0;

    printf("Enter title: ");
    fgets(r1.title, sizeof(r1.title), stdin);
    r1.title[strcspn(r1.title, "\n")] = 0;

    if(publication_exists(r1.title)){
        printf("\nWarning: A similar title already exists.\n");
        printf("Continue anyway? (y/n): ");
        char ch;
        scanf(" %c", &ch);
        if(ch != 'y' && ch != 'Y') return;
    }

    //Add to array
    pub_array[publication_count] = r1;
    publication_count++;

    //Auto-save
    save_publications();

    printf("\n\033[1mRecord Saved!\033[0m\n");
    printf("Total Publication Records: %d\n", publication_count);
}

//Display all publications from array
void display_publication_records(){
    if(publication_count == 0){
        printf("\nNo publication records found.\n");
        return;
    }

    printf("\n-------------------------------------------PUBLICATION RECORDS-------------------------------------------\n");
    printf("Year\t\tType\t\t\tFaculty\t\tJournal/Conf\t\t\tIndexing\tTitle\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");

    for(int i = 0; i < publication_count; i++){
        printf("%-10s\t%-20s\t%-10s\t%-30s\t%-10s\t%s\n",
            pub_array[i].year,
            pub_array[i].type,
            pub_array[i].sn,
            pub_array[i].journal,
            pub_array[i].indexing,
            pub_array[i].title);
    }

    for(int i = 0; i < 110; i++) printf("-");
    printf("\nTotal Publications: %d\n", publication_count);
}

// ===================== SEARCH =====================

void search_menu(){
    printf("\n--- SEARCH OPTIONS ---\n");
    printf("[1] Search by Faculty\n");
    printf("[2] Search by Year\n");
    printf("[3] Search by Publication Type\n");
    printf("[4] Search by Keyword\n");
    printf("[5] Back to Main Menu\n");
    printf("Choice: ");
}

//Search by faculty short name
void search_faculty(){
    char key[50];
    int tf = 0, tp = 0;

    clear_buffer();
    printf("Enter Faculty Short Name: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;
    to_upper(key);

    printf("\n--- SEARCH RESULTS FOR: %s ---\n", key);

    //Search faculty array
    for(int i = 0; i < faculty_count; i++){
        if(strcmp(faculty_array[i].sn, key) == 0){
            if(!tf){
                printf("\n[FACULTY PROFILE]\n");
                printf("Faculty ID\t\tShort Name\t\tFull Name\t\t\tDesignation\t\t\tResearch Area\n");
                printf("-----------------------------------------------------------------------------------------------\n");
                tf = 1;
            }
            printf("%-15s\t\t%-15s\t\t%-30s\t%-25s\t%s\n",
                faculty_array[i].id, faculty_array[i].sn,
                faculty_array[i].full_name, faculty_array[i].desi, faculty_array[i].RA);
        }
    }

    //Search publication array
    int count = 0;
    for(int i = 0; i < publication_count; i++){
        if(strcmp(pub_array[i].sn, key) == 0){
            if(!tp){
                printf("\n[PUBLICATION RECORDS]\n");
                printf("Year\t\tType\t\t\tFaculty\t\tJournal/Conf\t\t\tIndexing\tTitle\n");
                printf("-------------------------------------------------------------------------------------------------------------\n");
                tp = 1;
            }
            printf("%-10s\t%-20s\t%-10s\t%-30s\t%-10s\t%s\n",
                pub_array[i].year, pub_array[i].type, pub_array[i].sn,
                pub_array[i].journal, pub_array[i].indexing, pub_array[i].title);
            count++;
        }
    }
    if(tp) printf("\nTotal Publications: %d\n", count);
    if(!tf && !tp) printf("\nNo matches found.\n");
    printf("\n");
}

//Search by year - exact match on year field
void search_by_year(){
    char year[20];
    int found = 0, count = 0;

    clear_buffer();
    printf("Enter Year: ");
    fgets(year, sizeof(year), stdin);
    year[strcspn(year, "\n")] = 0;

    printf("\n--- PUBLICATIONS IN YEAR %s ---\n", year);

    for(int i = 0; i < publication_count; i++){
        if(strcmp(pub_array[i].year, year) == 0){
            if(!found){
                printf("Year\t\tType\t\t\tFaculty\t\tJournal/Conf\t\t\tIndexing\tTitle\n");
                printf("-------------------------------------------------------------------------------------------------------------\n");
                found = 1;
            }
            printf("%-10s\t%-20s\t%-10s\t%-30s\t%-10s\t%s\n",
                pub_array[i].year, pub_array[i].type, pub_array[i].sn,
                pub_array[i].journal, pub_array[i].indexing, pub_array[i].title);
            count++;
        }
    }

    if(found) printf("\nTotal Publications: %d\n", count);
    else printf("\nNo publications found for year %s.\n", year);
    printf("\n");
}

//Search by type
void search_by_type(){
    char type[50];
    int found = 0, count = 0;

    clear_buffer();
    printf("Enter Type (Journal/Conference/Review): ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = 0;

    printf("\n--- %s PUBLICATIONS ---\n", type);

    for(int i = 0; i < publication_count; i++){
        if(strstr(pub_array[i].type, type) != NULL){
            if(!found){
                printf("Year\t\tType\t\t\tFaculty\t\tJournal/Conf\t\t\tIndexing\tTitle\n");
                printf("-------------------------------------------------------------------------------------------------------------\n");
                found = 1;
            }
            printf("%-10s\t%-20s\t%-10s\t%-30s\t%-10s\t%s\n",
                pub_array[i].year, pub_array[i].type, pub_array[i].sn,
                pub_array[i].journal, pub_array[i].indexing, pub_array[i].title);
            count++;
        }
    }

    if(found) printf("\nTotal Publications: %d\n", count);
    else printf("\nNo %s publications found.\n", type);
    printf("\n");
}

//Search by keyword (title, journal, etc.)
void search_by_keyword(){
    char keyword[100];
    int found = 0, count = 0;

    clear_buffer();
    printf("Enter Keyword: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    printf("\n--- SEARCH RESULTS FOR KEYWORD: %s ---\n", keyword);

    for(int i = 0; i < publication_count; i++){
        if(strstr(pub_array[i].title, keyword)   != NULL ||
           strstr(pub_array[i].journal, keyword) != NULL ||
           strstr(pub_array[i].type, keyword)    != NULL){
            if(!found){
                printf("Year\t\tType\t\t\tFaculty\t\tJournal/Conf\t\t\tIndexing\tTitle\n");
                printf("-------------------------------------------------------------------------------------------------------------\n");
                found = 1;
            }
            printf("%-10s\t%-20s\t%-10s\t%-30s\t%-10s\t%s\n",
                pub_array[i].year, pub_array[i].type, pub_array[i].sn,
                pub_array[i].journal, pub_array[i].indexing, pub_array[i].title);
            count++;
        }
    }

    if(found) printf("\nTotal Results: %d\n", count);
    else printf("\nNo matches found for '%s'.\n", keyword);
    printf("\n");
}

//Search submenu
int search_submenu(){
    int c;
    search_menu();
    scanf("%d", &c);
    printf("\n");

    switch(c){
        case 1:
            search_faculty();
            pause_screen();
            clear_screen();
            search_submenu();
            break;
        case 2:
            search_by_year();
            pause_screen();
            clear_screen();
            search_submenu();
            break;
        case 3:
            search_by_type();
            pause_screen();
            clear_screen();
            search_submenu();
            break;
        case 4:
            search_by_keyword();
            pause_screen();
            clear_screen();
            search_submenu();
            break;
        case 5:
            return 0;
        default:
            printf("Invalid choice!\n");
            pause_screen();
            clear_screen();
            search_submenu();
            break;
    }
    return 0;
}

// ===================== REPORTS =====================

//Faculty-wise report - sorted by publication count
void faculty_wise_report(){
    char names[MAX_FACULTY][100];
    int counts[MAX_FACULTY];

    printf("\n--- FACULTY-WISE PUBLICATION REPORT ---\n");

    if(faculty_count == 0){
        printf("No faculty data found.\n");
        return;
    }

    //Count publications per faculty from array
    for(int i = 0; i < faculty_count; i++){
        strcpy(names[i], faculty_array[i].full_name);
        counts[i] = 0;
        for(int j = 0; j < publication_count; j++){
            if(strcmp(pub_array[j].sn, faculty_array[i].sn) == 0){
                counts[i]++;
            }
        }
    }

    //Sort by count (descending)
    for(int i = 0; i < faculty_count - 1; i++){
        for(int j = i + 1; j < faculty_count; j++){
            if(counts[i] < counts[j]){
                int tmp = counts[i]; counts[i] = counts[j]; counts[j] = tmp;
                char tmpName[100];
                strcpy(tmpName, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], tmpName);
            }
        }
    }

    printf("%-30s\t%-20s\n", "Faculty Name", "Total Publications");
    printf("----------------------------------------------------------\n");

    int total = 0;
    for(int i = 0; i < faculty_count; i++){
        printf("%-30s\t%d\n", names[i], counts[i]);
        total += counts[i];
    }

    printf("----------------------------------------------------------\n");
    printf("Department Total: %d\n\n", total);
}

//Year-wise report
void year_wise_report(){
    int years[200];
    int year_counts[200] = {0};
    int unique_years = 0;

    if(publication_count == 0){
        printf("\nNo publication data found.\n");
        return;
    }

    //Collect unique years from array
    for(int i = 0; i < publication_count; i++){
        int y = atoi(pub_array[i].year);
        if(y < 1900 || y > 2030) continue;

        int found = 0;
        for(int j = 0; j < unique_years; j++){
            if(years[j] == y){
                year_counts[j]++;
                found = 1;
                break;
            }
        }
        if(!found){
            years[unique_years] = y;
            year_counts[unique_years] = 1;
            unique_years++;
        }
    }

    //Sort by year ascending
    for(int i = 0; i < unique_years - 1; i++){
        for(int j = i + 1; j < unique_years; j++){
            if(years[i] > years[j]){
                int tmp = years[i]; years[i] = years[j]; years[j] = tmp;
                tmp = year_counts[i]; year_counts[i] = year_counts[j]; year_counts[j] = tmp;
            }
        }
    }

    printf("\n--- YEAR-WISE PUBLICATION COUNT ---\n");
    printf("Year\t\tNumber of Publications\n");
    printf("----------------------------------------\n");
    for(int i = 0; i < unique_years; i++){
        printf("%d\t\t%d\n", years[i], year_counts[i]);
    }
    printf("----------------------------------------\n\n");
}

//Type-wise report
void type_wise_report(){
    int journal = 0, conference = 0, review = 0, other = 0;

    if(publication_count == 0){
        printf("\nNo publication data found.\n");
        return;
    }

    for(int i = 0; i < publication_count; i++){
        if(strstr(pub_array[i].type, "Journal") != NULL)         journal++;
        else if(strstr(pub_array[i].type, "Conference") != NULL) conference++;
        else if(strstr(pub_array[i].type, "Review") != NULL)     review++;
        else                                                       other++;
    }

    printf("\n--- PUBLICATION TYPE SUMMARY ---\n");
    printf("Type\t\t\tCount\n");
    printf("----------------------------------------\n");
    printf("Journal\t\t\t%d\n", journal);
    printf("Conference\t\t%d\n", conference);
    printf("Review\t\t\t%d\n", review);
    if(other > 0) printf("Other\t\t\t%d\n", other);
    printf("----------------------------------------\n\n");
}

void report_menu(){
    printf("\n--- GENERATE REPORTS ---\n");
    printf("[1] Faculty-wise Publication Report (Sorted)\n");
    printf("[2] Year-wise Publication Count\n");
    printf("[3] Publication Type Summary\n");
    printf("[4] All Reports\n");
    printf("[5] Back to Main Menu\n");
    printf("Choice: ");
}

int report_submenu(){
    int c;
    report_menu();
    scanf("%d", &c);
    printf("\n");

    switch(c){
        case 1:
            faculty_wise_report();
            pause_screen();
            clear_screen();
            report_submenu();
            break;
        case 2:
            year_wise_report();
            pause_screen();
            clear_screen();
            report_submenu();
            break;
        case 3:
            type_wise_report();
            pause_screen();
            clear_screen();
            report_submenu();
            break;
        case 4:
            faculty_wise_report();
            year_wise_report();
            type_wise_report();
            pause_screen();
            clear_screen();
            report_submenu();
            break;
        case 5:
            return 0;
        default:
            printf("Invalid choice!\n");
            pause_screen();
            clear_screen();
            report_submenu();
            break;
    }
    return 0;
}

// ===================== EDIT =====================

//Edit faculty by ID
void edit_faculty(){
    char id[20];
    clear_buffer();
    printf("Enter Faculty ID to edit: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    //Find in array
    int idx = -1;
    for(int i = 0; i < faculty_count; i++){
        if(strcmp(faculty_array[i].id, id) == 0){
            idx = i;
            break;
        }
    }

    if(idx == -1){
        printf("\nFaculty ID '%s' not found.\n", id);
        return;
    }

    printf("\nCurrent record:\n");
    printf("ID: %s | Name: %s | SN: %s | Desi: %s | RA: %s\n",
        faculty_array[idx].id, faculty_array[idx].full_name,
        faculty_array[idx].sn, faculty_array[idx].desi, faculty_array[idx].RA);
    printf("\nEnter new details:\n");

    printf("Enter full name: ");
    fgets(faculty_array[idx].full_name, sizeof(faculty_array[idx].full_name), stdin);
    faculty_array[idx].full_name[strcspn(faculty_array[idx].full_name, "\n")] = 0;

    printf("Enter short name: ");
    fgets(faculty_array[idx].sn, sizeof(faculty_array[idx].sn), stdin);
    faculty_array[idx].sn[strcspn(faculty_array[idx].sn, "\n")] = 0;
    to_upper(faculty_array[idx].sn);

    printf("Enter designation: ");
    fgets(faculty_array[idx].desi, sizeof(faculty_array[idx].desi), stdin);
    faculty_array[idx].desi[strcspn(faculty_array[idx].desi, "\n")] = 0;

    printf("Enter research area: ");
    fgets(faculty_array[idx].RA, sizeof(faculty_array[idx].RA), stdin);
    faculty_array[idx].RA[strcspn(faculty_array[idx].RA, "\n")] = 0;

    //Auto-save
    save_faculty();
    printf("\n\033[1mFaculty record updated!\033[0m\n");
}

//Edit publication by keyword
void edit_publication(){
    char keyword[100];
    clear_buffer();
    printf("Enter keyword from title to edit: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    //Find in array
    int idx = -1;
    for(int i = 0; i < publication_count; i++){
        if(strstr(pub_array[i].title, keyword) != NULL){
            idx = i;
            break;
        }
    }

    if(idx == -1){
        printf("\nKeyword '%s' not found.\n", keyword);
        return;
    }

    printf("\nCurrent record:\n");
    printf("[%s] %s - %s | %s | %s\n",
        pub_array[idx].year, pub_array[idx].title,
        pub_array[idx].sn, pub_array[idx].journal, pub_array[idx].indexing);
    printf("\nEnter new details:\n");

    printf("Enter year: ");
    fgets(pub_array[idx].year, sizeof(pub_array[idx].year), stdin);
    pub_array[idx].year[strcspn(pub_array[idx].year, "\n")] = 0;

    if(!validate_year(pub_array[idx].year)){
        printf("Invalid year! Edit cancelled.\n");
        //Reload from file to restore
        load_publications();
        return;
    }

    printf("Enter type: ");
    fgets(pub_array[idx].type, sizeof(pub_array[idx].type), stdin);
    pub_array[idx].type[strcspn(pub_array[idx].type, "\n")] = 0;

    printf("Enter faculty short name: ");
    fgets(pub_array[idx].sn, sizeof(pub_array[idx].sn), stdin);
    pub_array[idx].sn[strcspn(pub_array[idx].sn, "\n")] = 0;
    to_upper(pub_array[idx].sn);

    printf("Enter journal/conference: ");
    fgets(pub_array[idx].journal, sizeof(pub_array[idx].journal), stdin);
    pub_array[idx].journal[strcspn(pub_array[idx].journal, "\n")] = 0;

    printf("Enter indexing: ");
    fgets(pub_array[idx].indexing, sizeof(pub_array[idx].indexing), stdin);
    pub_array[idx].indexing[strcspn(pub_array[idx].indexing, "\n")] = 0;

    printf("Enter title: ");
    fgets(pub_array[idx].title, sizeof(pub_array[idx].title), stdin);
    pub_array[idx].title[strcspn(pub_array[idx].title, "\n")] = 0;

    //Auto-save
    save_publications();
    printf("\n\033[1mPublication record updated!\033[0m\n");
}

void edit_menu(){
    printf("\n--- EDIT RECORD ---\n");
    printf("[1] Edit Faculty\n");
    printf("[2] Edit Publication\n");
    printf("[3] Back to Main Menu\n");
    printf("Choice: ");
}

int edit_submenu(){
    int c;
    edit_menu();
    scanf("%d", &c);
    printf("\n");

    switch(c){
        case 1:
            edit_faculty();
            pause_screen();
            clear_screen();
            edit_submenu();
            break;
        case 2:
            edit_publication();
            pause_screen();
            clear_screen();
            edit_submenu();
            break;
        case 3:
            return 0;
        default:
            printf("Invalid choice!\n");
            pause_screen();
            clear_screen();
            edit_submenu();
            break;
    }
    return 0;
}

// ===================== DELETE =====================

//Delete faculty by ID - array shift
void delete_faculty(){
    char id[20];
    clear_buffer();
    printf("Enter Faculty ID to delete: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;

    int idx = -1;
    for(int i = 0; i < faculty_count; i++){
        if(strcmp(faculty_array[i].id, id) == 0){
            idx = i;
            break;
        }
    }

    if(idx == -1){
        printf("\nFaculty ID '%s' not found.\n", id);
        return;
    }

    printf("Are you sure you want to delete faculty '%s'? (y/n): ", id);
    char confirm;
    scanf(" %c", &confirm);
    if(confirm != 'y' && confirm != 'Y'){
        printf("Deletion cancelled.\n");
        return;
    }

    //Shift array left
    for(int i = idx; i < faculty_count - 1; i++){
        faculty_array[i] = faculty_array[i + 1];
    }
    faculty_count--;

    //Auto-save
    save_faculty();
    printf("\n\033[1mFaculty record deleted!\033[0m\n");
    printf("Total Faculty Records: %d\n", faculty_count);
}

//Delete publication by keyword - array shift
void delete_publication(){
    char keyword[100];
    clear_buffer();
    printf("Enter keyword from title to delete: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    int idx = -1;
    for(int i = 0; i < publication_count; i++){
        if(strstr(pub_array[i].title, keyword) != NULL){
            idx = i;
            break;
        }
    }

    if(idx == -1){
        printf("\nKeyword '%s' not found.\n", keyword);
        return;
    }

    printf("Are you sure you want to delete publication matching '%s'? (y/n): ", keyword);
    char confirm;
    scanf(" %c", &confirm);
    if(confirm != 'y' && confirm != 'Y'){
        printf("Deletion cancelled.\n");
        return;
    }

    //Shift array left
    for(int i = idx; i < publication_count - 1; i++){
        pub_array[i] = pub_array[i + 1];
    }
    publication_count--;

    //Auto-save
    save_publications();
    printf("\n\033[1mPublication record deleted!\033[0m\n");
    printf("Total Publication Records: %d\n", publication_count);
}

void delete_menu(){
    printf("\n--- DELETE RECORD ---\n");
    printf("[1] Delete Faculty\n");
    printf("[2] Delete Publication\n");
    printf("[3] Back to Main Menu\n");
    printf("Choice: ");
}

int delete_submenu(){
    int c;
    delete_menu();
    scanf("%d", &c);
    printf("\n");

    switch(c){
        case 1:
            delete_faculty();
            pause_screen();
            clear_screen();
            delete_submenu();
            break;
        case 2:
            delete_publication();
            pause_screen();
            clear_screen();
            delete_submenu();
            break;
        case 3:
            return 0;
        default:
            printf("Invalid choice!\n");
            pause_screen();
            clear_screen();
            delete_submenu();
            break;
    }
    return 0;
}

// ===================== STATS & HELP =====================

//Statistics dashboard
void show_statistics(){
    printf("\n");
    for(int i = 0; i < 50; i++) printf("=");
    printf("\n          SYSTEM STATISTICS\n");
    for(int i = 0; i < 50; i++) printf("=");
    printf("\n\n");

    printf("Total Faculty: %d\n", faculty_count);
    printf("Total Publications: %d\n\n", publication_count);

    if(faculty_count > 0){
        float avg = (float)publication_count / faculty_count;
        printf("Average Publications per Faculty: %.2f\n\n", avg);
    }

    //Find min/max year from array
    if(publication_count > 0){
        int max_year = 0, min_year = 3000;
        for(int i = 0; i < publication_count; i++){
            int y = atoi(pub_array[i].year);
            if(y > 1900 && y < 2030){
                if(y > max_year) max_year = y;
                if(y < min_year) min_year = y;
            }
        }
        if(max_year > 0){
            printf("Most Recent Publication: %d\n", max_year);
            printf("Oldest Publication:      %d\n", min_year);
            printf("Publication Span:        %d years\n\n", max_year - min_year);
        }
    }

    //Top 3 faculty
    printf("--- TOP 3 MOST PRODUCTIVE FACULTY ---\n");
    char names[MAX_FACULTY][100];
    int counts[MAX_FACULTY];

    for(int i = 0; i < faculty_count; i++){
        strcpy(names[i], faculty_array[i].full_name);
        counts[i] = 0;
        for(int j = 0; j < publication_count; j++){
            if(strcmp(pub_array[j].sn, faculty_array[i].sn) == 0){
                counts[i]++;
            }
        }
    }

    //Sort descending
    for(int i = 0; i < faculty_count - 1; i++){
        for(int j = i + 1; j < faculty_count; j++){
            if(counts[i] < counts[j]){
                int tmp = counts[i]; counts[i] = counts[j]; counts[j] = tmp;
                char tmpN[100];
                strcpy(tmpN, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], tmpN);
            }
        }
    }

    int show = (faculty_count < 3) ? faculty_count : 3;
    for(int i = 0; i < show; i++){
        printf("%d. %-30s - %d publications\n", i+1, names[i], counts[i]);
    }

    printf("\n");
    for(int i = 0; i < 50; i++) printf("=");
    printf("\n\n");
}

//Help
void show_help(){
    printf("\n");
    for(int i = 0; i < 60; i++) printf("=");
    printf("\n                         HELP GUIDE\n");
    for(int i = 0; i < 60; i++) printf("=");
    printf("\n\n");

    printf("1. ADD FACULTY\n");
    printf("   - Register new faculty with unique ID\n");
    printf("   - Short names auto-converted to uppercase\n");
    printf("   - Duplicate IDs not allowed\n\n");

    printf("2. DISPLAY FACULTY PROFILE\n");
    printf("   - View all registered faculty members\n\n");

    printf("3. ADD PUBLICATION\n");
    printf("   - Year must be 1900-2030, digits only\n");
    printf("   - Links to existing faculty (warning if not found)\n");
    printf("   - Checks for duplicate titles\n\n");

    printf("4. DISPLAY PUBLICATION RECORD\n");
    printf("   - View all registered publications\n\n");

    printf("5. SEARCH\n");
    printf("   - By Faculty: Finds profile + all their publications\n");
    printf("   - By Year: Exact year match\n");
    printf("   - By Type: Journal/Conference/Review\n");
    printf("   - By Keyword: Searches title, journal, type\n\n");

    printf("6. GENERATE REPORTS\n");
    printf("   - Faculty-wise: Publication count (sorted)\n");
    printf("   - Year-wise: Chronological count\n");
    printf("   - Type-wise: Breakdown by type\n\n");

    printf("7. EDIT RECORD\n");
    printf("   - Faculty: Search by ID\n");
    printf("   - Publication: Search by keyword in title\n\n");

    printf("8. DELETE RECORD\n");
    printf("   - Confirmation required\n");
    printf("   - Deletion is permanent!\n\n");

    printf("9. STATISTICS DASHBOARD\n");
    printf("   - Averages, year range, top 3 faculty\n\n");

    printf("TIPS:\n");
    printf("- Short names are case-insensitive (mhk = MHK)\n");
    printf("- Data auto-saves after every add/edit/delete\n");
    printf("- All data persists between program runs\n\n");

    for(int i = 0; i < 60; i++) printf("=");
    printf("\n\n");
}

// ===================== MAIN CHOICE (unchanged recursion) =====================

int choice(){
    int c;
    printf("Choice: ");
    scanf("%d", &c);
    printf("\n");

    switch(c){
        case 1:
            add_faculty();
            pause_screen();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 2:
            display_faculty();
            pause_screen();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 3:
            add_publication_record();
            pause_screen();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 4:
            display_publication_records();
            pause_screen();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 5:
            clear_screen();
            search_submenu();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 6:
            clear_screen();
            report_submenu();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 7:
            clear_screen();
            edit_submenu();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 8:
            clear_screen();
            delete_submenu();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 9:
            show_statistics();
            pause_screen();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 10:
            show_help();
            pause_screen();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
        case 11:
            clear_screen();
            printf("\033[1mAll records saved successfully.\033[0m\n");
            printf("Program terminated normally.\n");
            for(int i = 0; i < 50; i++) printf("-");
            printf("\n");
            return 0;
        default:
            printf("Invalid Choice!\nEnter again\n");
            pause_screen();
            clear_screen();
            header();
            main_menu();
            choice();
            break;
    }
    return 0;
}

// ===================== MAIN =====================

void main(){
    header();
    printf("\n");

    initialize_files();    //Create files if they don't exist
    load_faculty();        //Load file → faculty_array
    load_publications();   //Load file → pub_array

    printf("\033[1mData loaded successfully.\033[0m\n");
    printf("Total Faculty Records: %d\n", faculty_count);
    printf("Total Publication Records: %d\n\n", publication_count);

    main_menu();
    choice();
}
