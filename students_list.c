/*
* Students List
 * -------------
 * Exercise console program for work with students data
 *
 * Program features:
 * - add record
 * - edit record
 * - delete record
 * - view the list of records
 * - search for a record by ID
 *
 * Author: Maksym Petliukh
 * 2025
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int state = 0;
unsigned int read_count = 0;
unsigned int read_capacity = 0;
unsigned int read_record = 0;
unsigned int read_id = 0;
unsigned int read_gpa = 0;
unsigned int read_name = 0;
int selected_index = -1;

typedef enum{START, MENU, SHOW, ADD, EDIT, DELETE, SAVE, SEARCH, END}State;

typedef struct {
    unsigned int id;
    unsigned int gpa;
    char* name;
}Student;

typedef struct {
    unsigned int count;
    unsigned int capacity;
    Student* students;
}List;

List* list = NULL;

void start();
void menu();
void show();
void add();
void edit();
void delete();
void save();
void search();
void end();
void clear();

int main(int argc, char** argv) {
    //Call  initialization function
    start();
    while (state != END){
        //Call the necessary action by switching states
        switch (state) {
            case MENU: menu(); break;
            case SHOW: show(); break;
            case ADD: add(); break;
            case EDIT: edit(); break;
            case DELETE: delete(); break;
            case SAVE: save(); break;
            case SEARCH: search(); break;
            case END: end(); break;
            default: menu(); break;
        }
    }
}

void start() {
    state = START;

    printf("STUDENTS LIST CONSOLE EXERCISE PROGRAM\n");

    //open data file
    FILE* file = fopen("students.dat", "rb");
    if (file) {
        //memory allocation for students list structure
        list = (List*)malloc(sizeof(List));
        list->count = 0;
        list->capacity = 0;
        list->students = NULL;

        //Read fields
        read_count = fread(&list->count, sizeof(unsigned int), 1, file);
        read_capacity = fread(&list->capacity, sizeof(unsigned int), 1, file);

        //Memory allocation for record structure
        list->students = (Student*)malloc(list->capacity * sizeof(Student));

        //Checking for a correct memory allocation
        if (read_count != 1 || read_capacity != 1 || list->count > list->capacity) {
            free(list->students);
            list->students = NULL;
            list->capacity = list->count= 0;
        }

        unsigned int len;
        for (size_t i = 0; i < list->capacity; i++) {list->students[i].name = NULL;}

        for (int i = 0; i < list->count; i++) {
            //read record structure fields
            read_id = fread(&list->students[i].id, sizeof(unsigned int), 1, file);
            read_gpa = fread(&list->students[i].gpa, sizeof(unsigned int), 1, file);

            //read a name length in a current record
            fread(&len, sizeof(unsigned int), 1, file);

            //memory allocation for name array
            list->students[i].name = malloc(len + 1);

            //read name field from current record
            read_name = fread(list->students[i].name, 1, len,  file);
            list->students[i].name[len] = '\0';
        }
        fclose(file);
    }else {
        //Create .dat file if it doesn't exist
        file = fopen("students.dat", "wb");

        //Memory allocation for a list structure
        list = (List*)malloc(sizeof(List));
        list->count = 0;
        list->capacity = 0;
        list->students = NULL;
        fclose(file);
    }


    //Change state for switch a mode
    state = MENU;
}

void menu() {
    clear();

    int input;

    while (1) {
        printf("\nPlease choose the action and press Enter: ");
        printf("\n1 - Show students list\n");
        printf("2 - Add record\n");
        printf("3 - Search record\n");
        printf("0 - Exit\n");
        printf("---------------------------------\n");

        if (scanf("%d", &input) != 1) {
            printf("Incorrect input!\nPress Enter to continue");
            while (getchar() != '\n');
            getchar();
            continue;
        }

            while (getchar() != '\n');

            //Choose a state
            switch (input) {
                case 1: state = SHOW; return;
                case 2: state = ADD; return;
                case 3: state = SEARCH; return;
                case 0: state = END; return;
                default: return;
            }
        }
    }

void add() {
    clear();
    if (list->count == list->capacity) {
        unsigned int new_capacity = (list->capacity == 0) ? 8 : list->capacity * 2;
        Student* new_students = (Student*)realloc(list->students, new_capacity * sizeof(Student));
        if (new_students == NULL) {
            printf("Out of memory!\n");
            printf("Press Enter to continue");
            getchar();
            return;
        }

        list->students = new_students;
        list->capacity = new_capacity;
    }

    unsigned int temp_id;
    unsigned int temp_gpa;
    char* temp_name = NULL;
    size_t size = 0;

    printf("Enter the student id: ");
    scanf("%u", &temp_id);
    while (getchar() != '\n');

    printf("Enter the student GPA: ");
    scanf("%u", &temp_gpa);
    while (getchar() != '\n');

    printf("Enter the student name: ");
    ssize_t check = getline(&temp_name, &size, stdin);

    if (check <= 0) {
        printf("Incorrect input!\n");
        printf("Press Enter to continue");
        getchar();
        return;
    }

    if (temp_name[strlen(temp_name) - 1] == '\n') {
        temp_name[strlen(temp_name) - 1] = '\0';
    }

    if (strlen(temp_name) == 0) {
        printf("Name cannot be empty!\n");
        free(temp_name);
        state = MENU;
        return;
    }

    list->students[list->count].id = temp_id;
    list->students[list->count].gpa = temp_gpa;
    list->students[list->count].name = temp_name;

    list->count++;

    state = SAVE;
}

void show() {
    clear();

    printf("===STUDENTS LIST===\n\n");

    for (size_t i = 0; i < list->count; i++) {
        printf("\n%zu. %s\nID: %u\nGPA: %u\n",
            i, list->students[i].name,
            list->students[i].id,
            list->students[i].gpa);
    }

    if (list->count == 0) {
        printf("No records found!\n");
        printf("Press Enter to continue");
        selected_index = -1;
        state = MENU;
        getchar();
        return;
    }

    int choice;
    printf("Choose the record: ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    if (choice < 0 || choice >= list->count) {
        printf("Incorrect input!\n");
        printf("Press Enter to continue");
        getchar();
        return;
    }

    selected_index = choice;

    clear();
    printf("===CURRENT RECORD===\n\n");

    printf("%s\nID: %u\nGPA: %u\n",
        list->students[choice].name,
        list->students[choice].id,
        list->students[choice].gpa);

    printf("\n1. Edit record\n");
    printf("2. Delete record\n");
    printf("3. Back to the menu\n");
    printf("4. Exit program\n");

    int action;
    scanf("%d", &action);
    while (getchar() != '\n');

    switch (action) {
        case 1: state = EDIT; return;
        case 2:  state = DELETE; return;
        case 3: selected_index = -1; state = MENU; return;
        case 4: state = END; return;
    }
}

void edit() {
    clear();

    if (selected_index < 0 || selected_index >= list->count) {
        printf("No record selected!\nPress Enter to continue");
        getchar();
        state = MENU;
        return;
    }

    int i = selected_index;

    unsigned int temp_id;
    unsigned int temp_gpa;
    char* temp_name = NULL;
    size_t size = 0;
    unsigned int unique;

    do {
        unique = 0;
        printf("Enter new ID: ");
        scanf("%u", &temp_id);
        while (getchar() != '\n');

        for (size_t j = 0; j < list->count; j++) {
            if (j != i && list->students[j].id == temp_id) {
                unique = 1;
                printf("This ID already exists!\n");
                break;
            }
        }
    } while (unique);

    printf("Enter new GPA: ");
    scanf("%u", &temp_gpa);
    while (getchar() != '\n');

    printf("Enter new name: ");
    ssize_t check = getline(&temp_name, &size, stdin);
    if (check <= 0) {
        printf("Incorrect input!\nPress Enter to continue");
        getchar();
        return;
    }
    if (temp_name[strlen(temp_name) - 1] == '\n') {
        temp_name[strlen(temp_name) - 1] = '\0';
    }
    if (strlen(temp_name) == 0) {
        free(temp_name);
        printf("Name cannot be empty!\n");
        state = MENU;
        return;
    }

    free(list->students[i].name);
    list->students[i].id = temp_id;
    list->students[i].gpa = temp_gpa;
    list->students[i].name = temp_name;

    state = SAVE;
}

void save() {
    FILE* f = fopen("students.dat", "wb");
    if (!f){printf("Can't open file!\n"); return;}

    read_count = fwrite(&list->count, sizeof(unsigned int), 1, f);
    read_capacity = fwrite(&list->capacity, sizeof(unsigned int), 1, f);

    for (size_t i = 0; i < list->count; i++) {
        fwrite(&list->students[i].id, sizeof(unsigned int), 1, f);
        fwrite(&list->students[i].gpa, sizeof(unsigned int), 1, f);

        unsigned int len = strlen(list->students[i].name) + 1;

        fwrite(&len, sizeof(unsigned int), 1, f);
        fwrite(list->students[i].name, sizeof(char), len, f);
    }

    fclose(f);
    state = MENU;
    selected_index = -1;
}

void delete() {
    clear();

    if (selected_index < 0 || selected_index >= list->count) {
        printf("No record selected!\nPress Enter to continue");
        getchar();
        state = MENU;
        return;
    }

    int i = selected_index;

    if (list->students[i].name)
        free(list->students[i].name);

    for (size_t j = i; j < list->count - 1; j++) {
        list->students[j] = list->students[j + 1];
    }

    selected_index = -1;
    list->count--;

    printf("Record deleted successfully.\nPress Enter to continue");
    getchar();

    state = SAVE;
}

void end() {
    save();
    printf("Press Enter to exit");
    getchar();
    exit(EXIT_SUCCESS);
}

void clear() {
    printf("\033[H\033[J");
    fflush(stdout);
}

void search() {
    clear();

    printf("====SEARCHING====\n");
    printf("Choose an option: \n");
    printf("1 - ID\n");
    printf("2 - GPA\n");
    printf("0 - Back to menu\n");
    printf("----------------------------\n\n");

    int option;
    scanf("%i", &option);
    while (getchar() != '\n');

    if (option == 0) {
        selected_index = -1;
        state = MENU;
        return;
    }

    if (option < 1 || option > 2) {
        printf("Incorrect input!\nPress Enter to continue");
        getchar();
        state = SEARCH;
        return;
    }

    if (option == 1) {
        unsigned int search_id;
        printf("Enter the ID: ");
        scanf("%u", &search_id);
        while (getchar() != '\n');

        int found = -1;
        for (size_t i = 0; i < list->count; i++) {
            if (list->students[i].id == search_id) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Record not found!\nPress Enter to continue");
            getchar();
            state = MENU;
            return;
        }

        selected_index = found;

        clear();
        printf("====RECORD FOUND!====\n\n");
        printf("%s\nID: %u\nGPA: %u\n",
            list->students[found].name, list->students[found].id, list->students[found].gpa);

        printf("1 - Edit record\n");
        printf("2 - Delete record\n");
        printf("0 - Back to menu\n");

        int action;
        scanf("%i", &action);
        while (getchar() != '\n');

        switch (action) {
            case 1:state = EDIT; return;
            case 2:state = DELETE; return;
            case 0: selected_index = -1; state = MENU; return;
            default: selected_index = -1; state = MENU; return;
        }
    }

    if (option == 2) {
        unsigned int search_gpa;
        printf("Enter the GPA: ");
        scanf("%u", &search_gpa);
        while (getchar() != '\n');

        int match_count = 0;
        for (size_t i = 0; i < list->count; i++) {
            if (list->students[i].gpa == search_gpa) {
                match_count++;
            }
        }

        if (match_count == 0) {
            printf("Record not found!\nPress Enter to continue");
            getchar();
            state = MENU;
            return;
        }

        clear();
        printf("====SEARCH RESULTS====\n\n");
        int* matches = malloc(match_count * sizeof(int));
        int match_index = 0;

        for (size_t i = 0; i < list->count; i++) {
            if (list->students[i].gpa == search_gpa) {
                matches[match_index] = i;
                printf("%d. %s\nID: %u\nGPA: %u\n", match_index,
                    list->students[i].name,
                    list->students[i].id, list->students[i].gpa);
                match_index++;
            }
        }

        printf("Choose a record (0-%d) or -1 to go back: \n", match_count - 1);
        int action;
        scanf("%i", &action);
        while (getchar() != '\n');

       if (action == -1) {
           free(matches);
           state = MENU;
           return;
       }

        if (action < 0 || action >= match_count) {
            printf("Record not found!\nPress Enter to continue");
            getchar();
            state = MENU;
            return;
        }

        selected_index = matches[action];
        free(matches);

        clear();
        printf("====SELECTED RECORD====\n\n");

        printf("%s\nID: %u\nGPA: %u\n",
            list->students[selected_index].name,
            list->students[selected_index].id,
            list->students[selected_index].gpa);

        printf("1 - Edit record\n");
        printf("2 - Delete record\n");
        printf("0 - Back to menu\n");

        int action2;
        scanf("%i", &action2);
        while (getchar() != '\n');

        switch (action2) {
            case 1:state = EDIT; return;
            case 2: state = DELETE; return;
            case 0: selected_index = -1; state = MENU; return;
            default: selected_index = -1; state = MENU; return;
        }
    }
}