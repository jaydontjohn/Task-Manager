# Task Manager

A command-line task manager written in C. Add tasks with due dates, mark them complete, and delete them. Tasks are saved to a file so they are still there when you close and reopen the program.

## Features

- Add tasks with an optional due date
- View pending tasks or all tasks
- Mark tasks as complete
- Delete tasks
- Tasks are saved to `tasks.txt` and loaded automatically on startup

## Build & Run

**Requirements:** GCC compiler and Make

```bash
make
./task_manager
```

Or without Make:

```bash
gcc -Wall -o task_manager task_manager.c
./task_manager
```

To clean up the built files:

```bash
make clean
```

## Usage

```
-----------------------------
       Task Manager
-----------------------------
1. Add a task
2. View pending tasks
3. View all tasks
4. Mark task as complete
5. Delete a task
6. Quit
-----------------------------
```

Enter a number and follow the prompts. Tasks are saved automatically after every change.

## How tasks are stored

Each task is saved as 4 lines in `tasks.txt`:

```
1
Buy groceries
2026-03-10
0
```

In order: ID, title, due date, and done status (0 = pending, 1 = complete). This makes the file easy to read and edit manually if needed.

## Concepts used

- Arrays and parallel array data storage
- File I/O with `fgets` and `fprintf`
- Functions and prototypes
- String handling with `strcpy`, `strcspn`, and `strlen`
- Input handling with `scanf` and `fgets`