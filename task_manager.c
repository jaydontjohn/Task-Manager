#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TASKS 100
#define MAX_LEN   100

//store task in arrays
int  task_count = 0;
int  ids[MAX_TASKS];
char titles[MAX_TASKS][MAX_LEN];
char due_dates[MAX_TASKS][MAX_LEN];
int  done[MAX_TASKS];   // 0 = pending, 1 = complete
int  next_id = 1;

//function prototypes
void load_tasks(void);
void save_tasks(void);
void add_task(void);
void list_tasks(int show_all);
void complete_task(void);
void delete_task(void);
void clear_input(void);

//Main menu

int main(void) {
    load_tasks();

    int choice;
    while (1) {
        printf("-----------------------------\n");
        printf("       Task Manager      \n");
        printf("-----------------------------\n");
        printf("1. Add a task\n");
        printf("2. View pending tasks\n");
        printf("3. View all tasks\n");
        printf("4. Mark task as complete\n");
        printf("5. Delete a task\n");
        printf("6. Quit\n");
        printf("-----------------------------\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            clear_input();
            continue;
        }
        clear_input();
        printf("\n");

        switch (choice) {
            case 1: add_task();      break;
            case 2: list_tasks(0);   break;
            case 3: list_tasks(1);   break;
            case 4: complete_task(); break;
            case 5: delete_task();   break;
            case 6: printf("Goodbye!\n"); return 0;
            default: printf("Please enter a number between 1 and 6.\n\n");
        }
    }
}

//functions
void load_tasks(void) {
    //file
    // The file stores each task as 4 lines: id, title, due date, done status
    // Example:
    //   1
    //   Buy groceries
    //   2026-03-10
    //   0
    FILE *fp = fopen("tasks.txt", "r");
    if (!fp) return;   // no file yet on first run

    char line[MAX_LEN];

    while (task_count < MAX_TASKS) {
        // Read ID
        if (!fgets(line, MAX_LEN, fp)) break;
        ids[task_count] = atoi(line);

        // Read title
        if (!fgets(titles[task_count], MAX_LEN, fp)) break;
        titles[task_count][strcspn(titles[task_count], "\n")] = '\0';

        // Read due date
        if (!fgets(due_dates[task_count], MAX_LEN, fp)) break;
        due_dates[task_count][strcspn(due_dates[task_count], "\n")] = '\0';

        // Read done status
        if (!fgets(line, MAX_LEN, fp)) break;
        done[task_count] = atoi(line);

        // Track the highest ID so new tasks get a unique ID
        if (ids[task_count] >= next_id)
            next_id = ids[task_count] + 1;

        task_count++;
    }

    fclose(fp);
}

void save_tasks(void) {
    FILE *fp = fopen("tasks.txt", "w");
    if (!fp) { printf("Error: could not save tasks.\n"); return; }

    for (int i = 0; i < task_count; i++) {
        fprintf(fp, "%d\n",  ids[i]);
        fprintf(fp, "%s\n",  titles[i]);
        fprintf(fp, "%s\n",  due_dates[i]);
        fprintf(fp, "%d\n",  done[i]);
    }

    fclose(fp);
}

//Helpers

// Clears any leftover characters in the input buffer
void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Features 

void add_task(void) {
    if (task_count >= MAX_TASKS) {
        printf("Task list is full.\n");
        return;
    }

    printf("Title: ");
    fgets(titles[task_count], MAX_LEN, stdin);
    titles[task_count][strcspn(titles[task_count], "\n")] = '\0'; //strip newline

    if (strlen(titles[task_count]) == 0) {
        printf("Title cannot be empty.\n");
        return;
    }

    printf("Due date (YYYY-MM-DD, or press Enter to skip): ");
    fgets(due_dates[task_count], MAX_LEN, stdin);
    due_dates[task_count][strcspn(due_dates[task_count], "\n")] = '\0';

    ids[task_count]  = next_id++;
    done[task_count] = 0;
    task_count++;

    save_tasks();
    printf("Task added.\n");
}

void list_tasks(int show_all) {
    int shown = 0;
    printf("\n");
    printf("%-4s  %-10s  %-35s  %s\n", "ID", "Status", "Title", "Due Date");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < task_count; i++) {
        if (!show_all && done[i]) continue;   // skip completed if only showing pending

        char status[10];
        if (done[i]) {
            strcpy(status, "Done");
        } else {
            strcpy(status, "Pending");
        }
        char date[MAX_LEN];
        if (strlen(due_dates[i]) > 0) {
            strcpy(date, due_dates[i]);
        } else {
            strcpy(date, "-");
        }

        printf("%-4d  %-10s  %-35s  %s\n", ids[i], status, titles[i], date);
        shown++;
    }

    if (shown == 0)
        printf("No tasks to show.\n");
    printf("\n");
}

void complete_task(void) {
    list_tasks(0);
    printf("Enter task ID to mark complete (0 to cancel): ");

    int id;
    if (scanf("%d", &id) != 1) { clear_input(); return; }
    clear_input();

    if (id == 0) return;

    for (int i = 0; i < task_count; i++) {
        if (ids[i] == id) {
            if (done[i]) {
                printf("Task #%d is already complete.\n", id);
            } else {
                done[i] = 1;
                save_tasks();
                printf("Task #%d marked as complete.\n", id);
            }
            return;
        }
    }
    printf("Task #%d not found.\n", id);
}

void delete_task(void) {
    list_tasks(1);
    printf("Enter task ID to delete (0 to cancel): ");

    int id;
    if (scanf("%d", &id) != 1) { clear_input(); return; }
    clear_input();

    if (id == 0) return;

    for (int i = 0; i < task_count; i++) {
        if (ids[i] == id) {
            // Shift everything after this index one spot left to fill the gap
            for (int j = i; j < task_count - 1; j++) {
                ids[j] = ids[j + 1];
                strcpy(titles[j], titles[j + 1]);
                strcpy(due_dates[j], due_dates[j + 1]);
                done[j] = done[j + 1];
            }
            task_count--;
            save_tasks();
            printf("Task #%d deleted.\n", id);
            return;
        }
    }
    printf("Task #%d not found.\n", id);
}