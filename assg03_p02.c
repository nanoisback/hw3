#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct patient{
    int ID;
    char name[101];
    unsigned int critical_level; // 1 - 10
} patient;

typedef struct Node{
    patient data;
    struct Node *left;
    struct Node *right;
} Node;

void inorder(Node *root){
    if(root == NULL){
        return;
    }
    inorder(root->left);
    printf("%d %s %u\n", root->data.ID, root->data.name, root->data.critical_level);
    inorder(root->right);
}

void preorder(Node *root){
    if(root == NULL){
        return;
    }
    preorder(root->left);
    preorder(root->right);
    printf("%d %s %u\n", root->data.ID, root->data.name, root->data.critical_level);
}

void postorder(Node *root){
    if(root == NULL){
        return;
    }
    printf("%d %s %u\n", root->data.ID, root->data.name, root->data.critical_level);
    postorder(root->left);
    postorder(root->right);
}

void collect_patients(Node *root, patient *buffer, int *idx) {  // take patients into buffer
    if (root == NULL) return;
    buffer[(*idx)++] = root->data;
    collect_patients(root->left, buffer, idx);
    collect_patients(root->right, buffer, idx);
}

int compare_critical(const void *a, const void *b) {
    const patient *pa = (const patient *)a;
    const patient *pb = (const patient *)b;
    return (int)pa->critical_level - (int)pb->critical_level;
}

int tree_size(Node *root) {
    if (root == NULL)
        return 0;
    return 1 + tree_size(root->left) + tree_size(root->right);
}

void levelorder(Node *root){
    int size = tree_size(root);
    patient temp_buffer[size];
    int idx = 0;
    collect_patients(root, temp_buffer, &idx);

    qsort(temp_buffer, size, sizeof(patient), compare_critical);

    for (int i = 0; i < size; i++) {
        printf("%d %s %u\n", temp_buffer[i].ID, temp_buffer[i].name, temp_buffer[i].critical_level);
    }
} // some kind of queue hopefully

void print_critical_patients(Node *root) {
    if (root == NULL) return;
    if (root->data.critical_level >= 8) {
        printf("CRITICAL PATIENTS: %d %s %u\n", root->data.ID, root->data.name, root->data.critical_level);
    }
    print_critical_patients(root->left);
    print_critical_patients(root->right);
}

Node *getNewNode(patient val){
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = val;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node *insert(Node *root, patient val){
    if(root == NULL){
        root = getNewNode(val);
    }
    else if(val.ID <= root->data.ID){
        root->left = insert(root->left, val);
    }
    else{
        root->right = insert(root->right, val);
    }
    return root;
}

int search_and_update(Node *root, int val){
    if(root == NULL){
        return 0;
    }
    else if(root->data.ID == val){
        printf("Patient found: %s, ID: %d, Critical Level: %d\n", root->data.name, root->data.ID, root->data.critical_level);
        printf("Enter new critical level (1-10): ");
        unsigned int new_level;
        scanf("%u", &new_level);
        if (new_level == 10){
            printf("Immediate attention required.\n");
        }
        else
        if(new_level >= 1 && new_level < 10){
            root->data.critical_level = new_level;
            printf("Critical level updated to %u\n", new_level);
        }
        else{
            printf("Invalid critical level. Please enter a value between 1 and 10.\n");
        }
    }
    else if(val <= root->data.ID){
        return search_and_update(root->left, val);
    }
    else{
        return search_and_update(root->right, val);
    }
}


Node* findMin(Node* root) {
    while (root->left != NULL)
    root = root->left;
    return root;
}

Node* delete_node(Node* root, int id) {
    if (root == NULL) return NULL;

    if (id < root->data.ID) {
        root->left = delete_node(root->left, id);
    } else if (id > root->data.ID) {
        root->right = delete_node(root->right, id);
    } else {
        // Node to be deleted found
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        } else {
            Node* temp = findMin(root->right);
            root->data = temp->data;
            root->right = delete_node(root->right, temp->data.ID);
        }
    }
    return root;
}

int main(){
    Node *root = NULL;
    int choice, id, critical_level;
    char name[101];
    patient new_patient;

    while(1){
        printf("1. Insert Patient\n2. Search and Update Patient\n3. Delete Patient\n4. Traversals\n5. Critical Patients Report\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                patient new_patient;
                printf("Enter Admission ID, Name, Critical Level: ");
                scanf("%d %s %u", &new_patient.ID, new_patient.name, &new_patient.critical_level);
                if (new_patient.critical_level < 1 || new_patient.critical_level > 10) {
                    printf("Invalid critical level. Please enter a value between 1 and 10.\n");
                    break;
                }
                if (new_patient.critical_level == 10) {
                    printf("Immediate attention required.\n");
                }
                root = insert(root, new_patient);
                printf("Patient inserted\n");
                break;
                
            case 2:
                printf("Enter ID to search: ");
                scanf("%d", &id);
                search_and_update(root, id);
                break;
            case 3:
                printf("Enter ID to delete: ");
                scanf("%d", &id);
                root = delete_node(root, id);
                break;
            case 4:
                printf("--- Inorder Traversal --- \n");
                inorder(root);
                printf("--- Preorder Traversal --- \n");
                preorder(root);
                printf("--- Postorder Traversal --- \n");
                postorder(root);
                printf("--- Level Order Traversal --- \n");
                levelorder(root);
                break;
            case 5:
                printf("--- Critical Patients (Level >= 8) --- \n");
                print_critical_patients(root);
                break;
            case 6: {
                FILE *file = fopen("hospital_bst.txt", "w");
                if (!file) {
                    printf("Error opening file.\n");
                    break;
                }

                // Collect all patients into a buffer
                int size = tree_size(root);
                patient temp_buffer[size];
                int idx = 0;
                collect_patients(root, temp_buffer, &idx);

                // Sort by critical level
                qsort(temp_buffer, size, sizeof(patient), compare_critical);

                // Write to file
                for (int i = 0; i < size; i++) {
                    fprintf(file, "%d %s %u\n", temp_buffer[i].ID, temp_buffer[i].name, temp_buffer[i].critical_level);
                }
                fclose(file);
                printf("Data saved to hospital_bst.txt. Exiting...\n");
                exit(0);
            }
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
    return 0;
}