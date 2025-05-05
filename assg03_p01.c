#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct customer{
    char name[31];
    char id[11];
    int points;
} customer;

typedef struct node{
    customer data;
    struct node *next;
} node;

node *make_node(node *head, customer new_customer) {
    node *new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        return head;
    }
    new_node->data = new_customer;
    new_node->next = head;
    return new_node;
}

void insert_node(node **head, customer new_customer) {
    node *new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    new_node->data = new_customer;
    new_node->next = *head;
    *head = new_node;
}

int hash_function(char *id) {
    int hash = 0;
    for (int i = 0; i < strlen(id); i++) {
        hash += id[i];
    }
    return hash % 100; // Hash table expected size 1000, each key 10 customers
}

void insert_into_hash_table(node **hash_table, customer new_customer) {
    int index = hash_function(new_customer.id);
    insert_node(&hash_table[index], new_customer);
}

void search_in_hash_table(node **hash_table, char *id) {
    int index = hash_function(id);
    node *current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->data.id, id) == 0) {
            printf("Customer found: %s, ID: %s, Points: %d\n", current->data.name, current->data.id, current->data.points);
            return;
        }
        current = current->next;
    }
    printf("Customer with ID %s not found\n", id);
}

void update_points_in_hash_table(node **hash_table, char *id, int points) {
    int index = hash_function(id);
    node *current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->data.id, id) == 0) {
            current->data.points += points;
            printf("Updated points for customer ID %s: %d\n", id, current->data.points);
            return;
        }
        current = current->next;
    }
    printf("Customer with ID %s not found for update\n", id);
}

void delete_from_hash_table(node **hash_table, char *id) {
    int index = hash_function(id);
    node *current = hash_table[index];
    node *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->data.id, id) == 0) {
            if (prev == NULL) {
                hash_table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Deleted customer with ID %s\n", id);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Customer with ID %s not found for deletion\n", id);
}



int main(){
    node *hash_table[100] = {NULL}; // Hash table with 100 slots
    printf("Loading customers from file mode?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Enter your choice: ");
    char choice = scanf("%c", &choice);
    if (choice == '1') {
        FILE *file = fopen("customers.txt", "r");
        if (file == NULL) {
            printf("Error opening file\n");
            return 1;
        }
        customer new_customer;
        while (fscanf(file, "%s %s %d", new_customer.name, new_customer.id, &new_customer.points) != EOF) {
            insert_into_hash_table(hash_table, new_customer);
        }
        fclose(file);
    } else {
        printf("No customers loaded from file.\n");
    while(1){
        printf("---VinAir Loyalty Program---\n");
        printf("1. Insert customer\n");
        printf("2. Search customer\n");
        printf("3. Delete customer\n");
        printf("4. Display all customers\n");
        printf("5. Save and exit\n");

        char menu_choice;
        scanf(" %c", &menu_choice);
        switch (menu_choice) {
            case '1': {
                printf("Enter ID, name, and points: ");
                customer new_customer;
                scanf("%s %s %d", new_customer.id, new_customer.name, &new_customer.points);

                // Check if customer exists
                int index = hash_function(new_customer.id);
                node *current = hash_table[index];
                int found = 0;
                while (current != NULL) {
                    if (strcmp(current->data.id, new_customer.id) == 0) {
                        current->data.points = new_customer.points; // Update points
                        printf("Updated points for customer ID %s: %d\n", new_customer.id, new_customer.points);
                        found = 1;
                        break;
                    }
                    current = current->next;
                }
                if (!found) {
                    insert_into_hash_table(hash_table, new_customer);
                    printf("Added new customer: %s, ID: %s, Points: %d\n", new_customer.name, new_customer.id, new_customer.points);
                }
                break;
            }
            case '2': {
                char id[11];
                printf("Enter customer ID to search: ");
                scanf("%s", id);
                search_in_hash_table(hash_table, id);
                break;
            }
            case '3': {
                char id[11];
                printf("Enter customer ID to delete: ");
                scanf("%s", id);
                delete_from_hash_table(hash_table, id);
                break;
            }
            case '4': {
                for (int i = 0; i < 100; i++) {
                    node *current = hash_table[i];
                    while (current != NULL) {
                        printf("ID: %s, Name: %s, Points: %d\n", current->data.id, current->data.name, current->data.points);
                        current = current->next;
                    }
                }
                break;
            }
            case '5': {
                FILE *file = fopen("customers_text.txt", "w");
                if (file == NULL) {
                    printf("Error opening file for writing\n");
                    return 1;
                }
                for (int i = 0; i < 100; i++) {
                    node *current = hash_table[i];
                    while (current != NULL) {
                        fprintf(file, "%s %s %d\n", current->data.name, current->data.id, current->data.points);
                        current = current->next;
                    }
                }
                fclose(file);
                return 0;
            }
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}}