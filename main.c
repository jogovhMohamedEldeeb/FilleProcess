#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct record
{
    char first_name[30];
    char last_name[30];
    int id;
};

typedef struct record record;
typedef struct record* recordPtr;

void write_file(FILE *fptr, recordPtr ptr);
void read_file(FILE *fptr, recordPtr ptr);
int search_record(FILE *fptr, int id, recordPtr result);
int remove_record(const char *file_name, int id);
void print_record(recordPtr ptr);

int main(void)
{
    record rec;
    recordPtr recPtr = &rec;
    char file_name[30];
    int choice;
    FILE *fptr;
    int search_id;

    printf("Enter file name: ");
    if (fgets(file_name, sizeof(file_name), stdin) != NULL) 
    {
        file_name[strcspn(file_name, "\n")] = '\0';
    } 
    else 
    {
        printf("Error reading file name.\n");
        return 1;
    }

    while (1) 
    {
        printf("Enter choice (1: Write, 2: Read, 3: Search, 4: Remove, 5: Terminate): ");
        if (scanf("%d", &choice) != 1) {
            printf("Error reading choice. Please enter a number.\n");
            return 1;
        }

        switch (choice) 
        {
            case 1: // Write
                printf("Enter record first name, last name, id: \n");
                if (scanf("%29s %29s %d", rec.first_name, rec.last_name, &rec.id) != 3) 
                {
                    printf("Error reading record data. Please enter valid values.\n");
                    while (fgetc(stdin) != '\n'); // Clear input buffer
                    break;
                }

                fptr = fopen(file_name, "a");
                if (fptr == NULL)
                {
                    printf("Error opening file for writing.\n");
                    break;
                }

                write_file(fptr, &rec);
                fclose(fptr);
                printf("Record written successfully.\n");
                break;

            case 2: // Read
                fptr = fopen(file_name, "r");
                if (fptr == NULL)
                {
                    printf("Error opening file for reading.\n");
                    break;
                }

                while (fscanf(fptr, "%29s %29s %d", rec.first_name, rec.last_name, &rec.id) != EOF) 
                {
                    print_record(&rec);
                }

                fclose(fptr);
                break;

            case 3: // Search
                printf("Enter ID to search: ");
                if (scanf("%d", &search_id) != 1) 
                {
                    printf("Error reading search ID. Please enter a number.\n");
                    break;
                }

                fptr = fopen(file_name, "r");
                if (fptr == NULL) 
                {
                    printf("Error opening file for search.\n");
                    break;
                }

                if (search_record(fptr, search_id, recPtr)) 
                {
                    printf("Record with ID %d found!\n", search_id);
                    print_record(recPtr);
                } 
                else 
                {
                    printf("Record with ID %d not found.\n", search_id);
                }

                fclose(fptr);
                break;

            case 4: // Remove
                printf("Enter ID to remove: ");
                if (scanf("%d", &search_id) != 1) 
                {
                    printf("Error reading remove ID. Please enter a number.\n");
                    break;
                }

                if (remove_record(file_name, search_id)) 
                {
                    printf("Record with ID %d removed successfully.\n", search_id);
                } 
                else 
                {
                    printf("Record with ID %d not found.\n", search_id);
                }
                break;

            case 5: // Terminate
                return 0;

            default:
                printf("Invalid choice. Please enter 1, 2, 3, 4, or 5.\n");
        }
    }

    return 0;
}

void write_file(FILE *fptr, recordPtr ptr)
{
    fprintf(fptr, "%-20s %-20s %d\n", ptr->first_name, ptr->last_name, ptr->id);
}

void read_file(FILE *fptr, recordPtr ptr)
{
    fscanf(fptr, "%29s %29s %d", ptr->first_name, ptr->last_name, &ptr->id);
}

void print_record(recordPtr ptr)
{
    printf("First Name: %s\nLast Name: %s\nID: %d\n", ptr->first_name, ptr->last_name, ptr->id);
}

int search_record(FILE *fptr, int id, recordPtr result)
{
    record temp;
    while (fscanf(fptr, "%29s %29s %d", temp.first_name, temp.last_name, &temp.id) != EOF) 
    {
        if (temp.id == id) 
        {
            *result = temp;
            return 1;
        }
    }
    return 0;
}

int remove_record(const char *file_name, int id)
{
    FILE* fptr = fopen(file_name, "r");
    if (fptr == NULL) 
    {
        printf("Error opening file for reading.\n");
        return 0;
    }

    FILE* temp_fptr = fopen("temp.dat", "w");
    if (temp_fptr == NULL) 
    {
        printf("Error opening temporary file for writing.\n");
        fclose(fptr);
        return 0;
    }

    record temp;
    int found = 0;

    while (fscanf(fptr, "%29s %29s %d", temp.first_name, temp.last_name, &temp.id) != EOF) 
    {
        if (temp.id != id) 
        {
            fprintf(temp_fptr, "%-20s %-20s %d\n", temp.first_name, temp.last_name, temp.id);
        } 
        else 
        {
            found = 1;
        }
    }

    fclose(fptr);
    fclose(temp_fptr);

    if (found) 
    {
        remove(file_name);
        rename("temp.dat", file_name);
    } 
    else 
    {
        remove("temp.dat");
    }

    return found;
}
