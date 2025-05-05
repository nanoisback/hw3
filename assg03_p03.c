#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct order {
    int ID;
    char name[101];
    unsigned int priority; // 1 - 5
} order;

typedef struct {
    order *data;
    int size;
    int capacity;
} MinHeap;

void swap(order *a, order *b) {
    order temp = *a;
    *a = *b;
    *b = temp;
}

MinHeap* create_heap(int capacity) {
    MinHeap *heap = malloc(sizeof(MinHeap));
    heap->data = malloc(capacity * sizeof(order));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify_up(MinHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->data[parent].priority > heap->data[index].priority) {
            swap(&heap->data[parent], &heap->data[index]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapify_down(MinHeap *heap, int index) {
    int left, right, smallest;
    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        smallest = index;

        if (left < heap->size && heap->data[left].priority < heap->data[smallest].priority)
            smallest = left;
        if (right < heap->size && heap->data[right].priority < heap->data[smallest].priority)
            smallest = right;

        if (smallest != index) {
            swap(&heap->data[smallest], &heap->data[index]);
            index = smallest;
        } else {
            break;
        }
    }
}

void insert_heap(MinHeap *heap, order value) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, heap->capacity * sizeof(order));
    }
    heap->data[heap->size] = value;
    heapify_up(heap, heap->size);
    heap->size++;
}

order extract_min(MinHeap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        exit(1);
    }
    order min = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapify_down(heap, 0);
    return min;
}

void print_heap_sorted(MinHeap *heap) {
    // Make a copy of the heap
    MinHeap temp;
    temp.size = heap->size;
    temp.capacity = heap->capacity;
    temp.data = malloc(temp.capacity * sizeof(order));
    memcpy(temp.data, heap->data, heap->size * sizeof(order));

    printf("(Display current heap)\n");
    while (temp.size > 0) {
        order o = extract_min(&temp);
        printf("ID: %d, Customer: %s, Priority: %u\n", o.ID, o.name, o.priority);
    }
    free(temp.data);
}

void change_priority(MinHeap *heap, int ID, unsigned int new_priority) {
    // Temporary array to hold orders
    order *temp = malloc(heap->size * sizeof(order));
    int n = 0;
    int found = 0;

    // Extract all orders
    while (heap->size > 0) {
        order o = extract_min(heap);
        if (o.ID == ID) {
            o.priority = new_priority;
            found = 1;
        }
        temp[n++] = o;
    }

    // Re-insert all orders
    for (int i = 0; i < n; i++) {
        insert_heap(heap, temp[i]);
    }

    free(temp);

    if (!found) {
        printf("Order with ID %d not found.\n", ID);
    } else {
        printf("Priority of order with ID %d changed to %u.\n", ID, new_priority);
    }
}

// Example usage
int main() {
    MinHeap *heap = create_heap(20);
    while(1){
        printf("--- VinFood Order System ---\n");
        printf("1. Insert new order\n");
        printf("2. Display heap\n");
        printf("3. Process next order\n");
        printf("4. Update priority\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                int ID;
                char name[101];
                unsigned int priority;
                printf("Enter Order ID: ");
                scanf("%d", &ID);
                printf("Enter Customer Name: ");
                scanf("%s", name);
                printf("Enter Priority (1-5): ");
                scanf("%u", &priority);

                if (priority < 1 || priority > 5) {
                    printf("Invalid priority. Please enter a value between 1 and 5.\n");
                    break;
                }

                order new_order = {ID, "", priority};
                strcpy(new_order.name, name);
                insert_heap(heap, new_order);
                break;
            }
            case 2:
                print_heap_sorted(heap);
                break;
            case 3: {
                if (heap->size > 0) {
                    order next_order = extract_min(heap);
                    printf("Processing order: \n");
                    printf("Order ID: %d, Customer: %s, Priority: %u\n", next_order.ID, next_order.name, next_order.priority);
                    printf("Heap is automatically reorganized: \n");
                    print_heap_sorted(heap);
                } else {
                    printf("No orders to process.\n");
                }
                break;
            }
            case 4: {
                int ID;
                unsigned int new_priority;
                printf("Enter Order ID to update: ");
                scanf("%d", &ID);
                printf("Enter new priority (1-5): ");
                scanf("%u", &new_priority);

                if (new_priority < 1 || new_priority > 5) {
                    printf("Invalid priority. Please enter a value between 1 and 5.\n");
                    break;
                }

                change_priority(heap, ID, new_priority);
                break;
            }
            case 5:
                FILE *fp = fopen("orders_heap.txt", "w");
                if (!fp) {
                    printf("Error opening file for writing.\n");
                    break;
                }
                MinHeap temp;
                temp.size = heap->size;
                temp.capacity = heap->capacity;
                temp.data = malloc(temp.capacity * sizeof(order));
                memcpy(temp.data, heap->data, heap->size * sizeof(order));
                while (temp.size > 0) {
                    order o = extract_min(&temp);
                    fprintf(fp, "Order ID: %d, Customer: %s, Priority: %u\n", o.ID, o.name, o.priority);
                }
                free(temp.data);
                fclose(fp);

                free(heap->data);
                free(heap);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

