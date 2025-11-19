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

int state = 0;

typedef enum{START, MENU, ADD, SHOW, SEARCH, EDIT, DELETE, EXIT}State;

//structure of record
typedef struct {
    int ID;
    int age;
    double GPA;
    char* name;
    char* direction;
}Student;

//general structure for list of records
typedef struct {
    int count;
    int capacity;
    Student* student;
}List;



void AddRecord() {

};

void ShowRecord();

void SearchRecord();

void EditRecord();

void DeleteRecord();

int ExitProgram() {

    state = EXIT;
    return EXIT_SUCCESS;
};

void menu() {
    state = MENU;
    while (state != EXIT) {
        printf("EXERSICE CONSOLE PROGRAM STUDENT LIST\n");
        printf("1 - Show Students list\n");
        printf("2 - Add a Record\n");
        printf("3 - Search for a Record by ID\n");
        printf("4 - Exit Program\n");

        int choice;
        scanf("%d",&choice);
        switch (choice) {
            case 1: ShowRecord(); break;
            case 2: AddRecord(); break;
            case 3: SearchRecord(); break;
            case 4: ExitProgram(); break;
            default: printf("Invalid input\n"); break;
        }
    }
};

//initialization function
void start() {
    state = START;
    FILE* file = fopen("students.dat", "rb");
    if (file == NULL) {
        file = fopen("students.dat", "wb");
        fclose(file);
        menu();
    }else{
        fclose(file);
        menu();
    }
}

int main(int argc, char* argv[]){
    state = START;
    while (state != EXIT) {
        switch (state) {
            case START: start(); break;
                case MENU: menu(); break;
                case ADD: AddRecord(); break;
                case SEARCH: SearchRecord(); break;
                case EDIT: EditRecord(); break;
                case DELETE: DeleteRecord(); break;
                case EXIT: ExitProgram(); break;
                default: break;
        }
    }

    return EXIT_SUCCESS;
}
