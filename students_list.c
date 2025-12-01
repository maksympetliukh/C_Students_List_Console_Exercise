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

typedef enum{START, MENU, SHOW, ADD, EDIT, DELETE, SAVE, END}State;

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
void end();

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
            case END: end(); break;
            default: menu(); break;
        }
    }
}

void start() {
    state = START;
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
    int input;

    while (1) {
        printf("STUDENTS LIST CONSOLE EXERCISE PROGRAM\n");
        printf("Please choose the action and press Enter: ");
        printf("\n1 - Show students list\n");
        printf("2 - Add record\n");
        printf("3 - Edit record\n");
        printf("4 - Delete record\n");
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
                case 3: state = EDIT; return;
                case 4: state = DELETE; return;
                case 0: state = END; return;
                default: return;
            }
        }
    }

void add() {
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
    if (list->count <= 0) {
        printf("No records found!\n"); state = MENU;
    }else {
        for (size_t i = 0; i < list->count; i++) {
            printf("%zu. Name: %s\nID: %u\nGPA: %u\n====================\n", i, list->students[i].name, list->students[i].id, list->students[i].gpa );
        }
    }

    state = MENU;
}

void edit() {
    unsigned int input;
    unsigned int temp_id;
    unsigned int temp_gpa;
    char* temp_name = NULL;
    size_t size = 0;
    unsigned int flag = 0;
    unsigned int unique = 0;

    printf("Enter the student ID: ");
    scanf("%u", &input);
    while (getchar() != '\n');

    for (size_t i = 0; i < list->count; i++) {
        if (list->students[i].id == input) {

            do {
                unique = 0;
                printf("Enter the new ID: ");
                scanf("%u", &temp_id);
                while (getchar() != '\n');

                for (size_t j = 0; j < list->count; j++) {
                    if (j != i && list->students[j].id == temp_id) {
                        unique = 1;
                        printf("This ID is already exists!\n");
                        break;
                    }
                }
            }while (unique);

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
                free(temp_name);
                printf("Name cannot be empty!\n");
                state = MENU;
                return;
            }

            free(list->students[i].name);
            list->students[i].id = temp_id;
            list->students[i].gpa = temp_gpa;
            list->students[i].name = temp_name;

            flag = 1;
            break;
        }
    }

    if (flag == 0) {
        printf("Student not found!\n");
    }

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
}

void delete() {
    unsigned int input;
    int found = 0;

    printf("Enter the student ID: ");
    scanf("%u", &input);
    while (getchar() != '\n');

    for (size_t i = 0; i < list->count; i++) {
        if (list->students[i].id == input) {

            if (list->students[i].name)
                free(list->students[i].name);

            for (size_t j = i; j < list->count - 1; j++) {
                list->students[j] = list->students[j + 1];
            }

            list->count--;
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student not found!\n");
    } else {
        printf("Student deleted successfully.\n");
    }

    state = SAVE;
}

void end() {
    save();
    printf("Press Enter to exit");
    getchar();
    exit(EXIT_SUCCESS);
}