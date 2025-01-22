#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int studentID;
    char studentName[50];
    char emailID[50];
    char courseID[20];
    char grade[5];
} Student;

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void createBinaryFile()
{
    FILE *fptr;
    fptr = fopen("Students.dat", "wb");
    if (fptr != NULL)
    {
        printf("Binary file created successfully.\n");
        fclose(fptr);
    }
    else
    {
        printf("Error\n");
    }
}

void Add_record()
{
    FILE *fptr;
    fptr = fopen("Students.dat", "ab");
    Student newStudent;
    printf("Enter Student ID: ");
    scanf("%d", &newStudent.studentID);
    printf("Enter Student Name: ");
    scanf(" %[^\n]", newStudent.studentName);
    printf("Enter Email ID: ");
    scanf(" %[^\n]", newStudent.emailID);
    printf("Enter Course ID: ");
    scanf(" %[^\n]", newStudent.courseID);
    printf("Enter Grade: ");
    scanf(" %[^\n]", newStudent.grade);
    fwrite(&newStudent, sizeof(Student), 1, fptr);
    fclose(fptr);
    printf("Record added successfully.\n");
}

void display_content()
{
    Student curr_record;
    FILE *fptr;
    fptr = fopen("Students.dat", "rb");

    printf("------------------------------------------------------------\n");
    while (fread(&curr_record, sizeof(Student), 1, fptr))
    {
        printf("ID: %d, Name: %s, Email: %s, Course: %s, Grade: %s\n",
               curr_record.studentID, curr_record.studentName, curr_record.emailID, curr_record.courseID, curr_record.grade);
    }
    printf("------------------------------------------------------------\n");

    fclose(fptr);
}

void seek_record()
{
    FILE *fptr;
    Student curr_record;
    int id, found = 0;

    fptr = fopen("students.dat", "rb");
    if (fptr == NULL)
    {
        printf("Error: Could not open the file!\n");
        return;
    }

    printf("Enter Student ID of the student you are looking for: ");
    scanf("%d", &id);

    while (fread(&curr_record, sizeof(Student), 1, fptr))
    {
        if (curr_record.studentID == id)
        {
            printf("Record Found:\n");
            printf("ID: %d, Name: %s, Email: %s, Course: %s, Grade: %s\n",
                   curr_record.studentID, curr_record.studentName, curr_record.emailID,
                   curr_record.courseID, curr_record.grade);
            found = 1;
            break; // Stop searching once the record is found
        }
    }

    if (!found)
    {
        printf("Record with Student ID %d not found.\n", id);
    }

    fclose(fptr);
}

void updateRecord()
{
    FILE *file = fopen("students.dat", "rb+"); // Open for reading and writing
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    int id, found = 0;
    Student student;

    printf("Enter the ID of the student to update: ");
    scanf("%d", &id);

    // Search for the record
    while (fread(&student, sizeof(Student), 1, file))
    {
        if (student.studentID == id)
        {
            found = 1;
            printf("Current details of the student:\n");
            printf("ID: %d, Name: %s, Email: %s, Course: %s, Grade: %s\n",
                   student.studentID, student.studentName, student.emailID,
                   student.courseID, student.grade);

            // Get updated information
            printf("Enter new details:\n");
            printf("Enter Name: ");
            scanf(" %[^\n]", student.studentName);
            printf("Enter Email: ");
            scanf(" %[^\n]", student.emailID);
            printf("Enter Course: ");
            scanf(" %[^\n]", student.courseID);
            printf("Enter Grade: ");
            scanf(" %[^\n]", student.grade);

            // Move the file pointer back to overwrite the current record
            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);

            printf("Record updated successfully.\n");
            break;
        }
    }

    if (!found)
    {
        printf("Record with ID %d not found.\n", id);
    }

    fclose(file);
}

void delete_record()
{
    FILE *fptr, *fptr_temp;
    Student curr_record;
    int id, found = 0;

    const char *filename = "students.dat";
    const char *temp_filename = "temp.dat";

    // Open the main file in read mode
    fptr = fopen(filename, "rb");
    if (fptr == NULL)
    {
        printf("Error: Could not open the file!\n");
        return;
    }

    // Open a temporary file in write mode
    fptr_temp = fopen(temp_filename, "wb");
    if (fptr_temp == NULL)
    {
        printf("Error: Could not create temporary file!\n");
        fclose(fptr);
        return;
    }

    // Ask user for the Student ID to delete
    printf("Enter Student ID of the student you want to delete: ");
    scanf("%d", &id);

    // Read from the original file and write all except the matching record to the temp file
    while (fread(&curr_record, sizeof(Student), 1, fptr))
    {
        if (curr_record.studentID == id)
        {
            printf("Record Found and deleted:\n");
            printf("ID: %d, Name: %s, Email: %s, Course: %s, Grade: %s\n",
                   curr_record.studentID, curr_record.studentName, curr_record.emailID,
                   curr_record.courseID, curr_record.grade);
            found = 1;
        }
        else
        {
            fwrite(&curr_record, sizeof(Student), 1, fptr_temp);
        }
    }

    // Close both files
    fclose(fptr);
    fclose(fptr_temp);

    // If no record was found, delete the temporary file
    if (!found)
    {
        printf("Record with Student ID %d not found.\n", id);
        remove(temp_filename);
    }
    else
    {
        // Remove the original file and rename the temporary file
        if (remove(filename) == 0)
        {
            if (rename(temp_filename, filename) == 0)
            {
                printf("Record deleted successfully.\n");
            }
            else
            {
                printf("Error renaming the temporary file.\n");
            }
        }
        else
        {
            printf("Error deleting the original file.\n");
        }
    }
}

int main()
{
    int choose;

    while (choose != 7)
    {

        printf("\nMAIN MENU\n");
        printf("1. Create the binary file\n");
        printf("2. Add a record to the file\n");
        printf("3. Display the contents of the file\n");
        printf("4. Seek a specific record\n");
        printf("5. Update the contents of a record\n");
        printf("6. Delete a record for a specific name\n");
        printf("7. Exit\n");
        printf("Please Enter your choice: ");
        scanf("%d", &choose);

        if (choose == 1)
        {
            createBinaryFile();
            delay(3);
            continue;
        }

        else if (choose == 2)
        {
            Add_record();
            delay(3);
            continue;
        }

        else if (choose == 3)
        {
            display_content();
            delay(3);
            continue;
        }

        else if (choose == 4)
        {
            seek_record();
            continue;
        }

        else if (choose == 5)
        {
            updateRecord();
            continue;
        }

        else if (choose == 6)
        {
            delete_record();
            continue;
        }
    }
}