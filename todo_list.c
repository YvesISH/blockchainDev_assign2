#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum for task status
typedef enum {
    PENDING,
    IN_PROGRESS,
    COMPLETED
} TaskStatus;

// Struct for a Task
typedef struct Task {
    int task_id;            // Unique identifier
    char description[256];  // Task description
    TaskStatus status;      // Task status
    unsigned char hash[SHA256_DIGEST_LENGTH];  // Task hash
    struct Task* next;      // Pointer for a linked list
} Task;

// Function prototypes
void compute_hash(Task *task, unsigned char *output_buffer);
void add_task(Task **head, int id, const char *desc, TaskStatus status);
void update_task(Task *head, int id, TaskStatus new_status);
void delete_task(Task **head, int id);
void list_tasks(Task *head);
void link_tasks(Task *head);
void menu(Task **task_list);
void handle_error(const char* msg);

// Function to compute the SHA-256 hash for a task
void compute_hash(Task *task, unsigned char *output_buffer) {
    char data_to_hash[512];
    sprintf(data_to_hash, "%d%s%d", task->task_id, task->description, task->status);
    SHA256((unsigned char*)data_to_hash, strlen(data_to_hash), output_buffer);
}

// Function to add a new task
void add_task(Task **head, int id, const char *desc, TaskStatus status) {
    Task *new_task = (Task*)malloc(sizeof(Task));
    if (new_task == NULL) {
        handle_error("Failed to allocate memory for new task.");
        return;
    }
    
    new_task->task_id = id;
    strcpy(new_task->description, desc);
    new_task->status = status;
    new_task->next = *head;

    // Compute and store hash
    compute_hash(new_task, new_task->hash);

    // Add to the front of the list
    *head = new_task;
}

// Function to update the status of a task
void update_task(Task *head, int id, TaskStatus new_status) {
    Task *current = head;

    while (current != NULL) {
        if (current->task_id == id) {
            current->status = new_status;

            // Recompute hash after update
            unsigned char old_hash[SHA256_DIGEST_LENGTH];
            memcpy(old_hash, current->hash, SHA256_DIGEST_LENGTH);

            compute_hash(current, current->hash);

            // Compare hashes
            if (memcmp(old_hash, current->hash, SHA256_DIGEST_LENGTH) != 0) {
                printf("Warning: Task ID %d hash has changed, integrity verified.\n", id);
            }
            return;
        }
        current = current->next;
    }

    printf("Task ID %d not found.\n", id);
}

// Function to delete a task from the list
void delete_task(Task **head, int id) {
    Task *temp = *head, *prev = NULL;

    if (temp != NULL && temp->task_id == id) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->task_id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Task ID %d not found.\n", id);
        return;
    }

    prev->next = temp->next;
    free(temp);
}

// Function to display all tasks
void list_tasks(Task *head) {
    Task *current = head;

    while (current != NULL) {
        printf("Task ID: %d\n", current->task_id);
        printf("Description: %s\n", current->description);
        printf("Status: %d\n", current->status);

        printf("Hash: ");
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%02x", current->hash[i]);
        }
        printf("\n\n");

        current = current->next;
    }
}

// Function to simulate linking tasks like a blockchain
void link_tasks(Task *head) {
    Task *current = head;
    Task *prev = NULL;

    while (current != NULL) {
        if (prev != NULL) {
            unsigned char combined_data[SHA256_DIGEST_LENGTH * 2];

            // Concatenate previous task's hash with current task data
            memcpy(combined_data, prev->hash, SHA256_DIGEST_LENGTH);
            memcpy(combined_data + SHA256_DIGEST_LENGTH, current->hash, SHA256_DIGEST_LENGTH);

            // Hash this combined data
            SHA256(combined_data, SHA256_DIGEST_LENGTH * 2, current->hash);
        }

        prev = current;
        current = current->next;
    }
}

// Function to handle errors gracefully
void handle_error(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

// Function to present a menu to the user
void menu(Task **task_list) {
    int choice, id, status;
    char description[256];

    while (1) {
        printf("Menu:\n");
        printf("1. Add Task\n");
        printf("2. Update Task Status\n");
        printf("3. Delete Task\n");
        printf("4. List Tasks\n");
        printf("5. Link Tasks (Blockchain-like)\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter task ID: ");
                scanf("%d", &id);
                printf("Enter task description: ");
                scanf("%s", description);
                add_task(task_list, id, description, PENDING);
                break;
            case 2:
                printf("Enter task ID: ");
                scanf("%d", &id);
                printf("Enter new status (0 for PENDING, 1 for IN_PROGRESS, 2 for COMPLETED): ");
                scanf("%d", &status);
                update_task(*task_list, id, status);
                break;
            case 3:
                printf("Enter task ID to delete: ");
                scanf("%d", &id);
                delete_task(task_list, id);
                break;
            case 4:
                list_tasks(*task_list);
                break;
            case 5:
                link_tasks(*task_list);
                printf("Tasks linked successfully (Blockchain-like feature applied).\n");
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
}

// Main function
int main() {
    Task *task_list = NULL;
    menu(&task_list);
    return 0;
}