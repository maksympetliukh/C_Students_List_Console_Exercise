# STUDENTS LIST
## Description
 The current program is designed for students data storage and operations with it.

 ## Allowed operations:
 - add records;
 - view records;
 - edit records;
 - delete records;
 - searching for a record with 2 different options (ID and GPA values);

## Project goal
 The current project is an exercise in basic C-language programming, project architecture
design and dynamic memory management.
 The following dynamic memory allocation methods are implemented:
- **add** - implementation of memory allocation for a dynamic array of structures (malloc);
- **edit** - resizing of fields in the selected array of structures (realloc);
- **delete** - implementation of memory deallocation (free);
 The architecture was designed by me. All code was written by me, following instructions and
suggestions of LLM (e.g., on file handling), without the LLM producing code directly.
 
## Used LLM:
- ChatGPT(GPT-5.2);
- Claude(Anthropic);
- Grok(xAI);

 ## Compilation example
 To compile and run the program with GCC:
```bash
gcc students_list.c -o students_list
./students_list

