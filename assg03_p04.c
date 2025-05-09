#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_graph(){
    FILE *fp = fopen("city_graph.txt", "r");
    if (fp == NULL){
        printf("Cannot open file.\n");
        return;
    }
    else{
        char buffer[500]; // support map with up to 500 spaces
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
    }
    }
    fclose(fp);
}

int main(){

    while (1){
        printf("--- VinDelivery Route System ---\n");
        printf("1. Display graph\n");
        printf("2. BFS traversal\n");
        printf("3. DFS traversal\n");
        printf("4. Find shortest path (Dijkstra)\n");
        printf("5. Add city / add road\n");
        printf("6. Detect cycle\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch (choice){
            case 1:
                display_graph();
                break;
            case 2:
                ;
                return 0;
            case 3:
                ;
                return 0;
            case 4:
                ;
                return 0;
            case 5:
                ;
                return 0;
            case 6: 
                ;
                return 0;
            case 7: 
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
    
    return 0;
}