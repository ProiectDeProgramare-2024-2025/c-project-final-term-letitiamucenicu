#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Operation{
    char name[50];
    float price;
    int duration;
};

struct Appointment{
    int day, month, year, hour, minute;
    char opName[50];
};

#define MAX_OP 5
#define MAX_APP 30

struct Operation operations[MAX_OP];
struct Appointment appointments[MAX_APP];

int appointment_count = 0;

void header()
{
    printf("\n--------------\n");
    printf("Welcome to Leti's Dental Clinic! What can we help you with today?\n");
    printf("\n1 - View All Operations\n");
    printf("2 - Check Availability\n");
    printf("3 - Schedule An Appointment\n");
    printf("4 - View Appointment History\n");
    printf("5 - Save Appointment and Exit\n");
    printf("--------------\n");
}

void menu(int option)
{
    #ifdef _WIN32
        system("cls");
    #endif

    switch (option) {
            case 1:
                display_operations();
                break;
            case 2:
                check_availability();
                break;
            case 3:
                schedule_appointment();
                break;
            case 4:
                view_history();
                break;
            case 5:
                save_appointments();
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
}

void load_operations() {
    FILE *file = fopen("op_details.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    int i = 0;
    while (i < MAX_OP && fscanf(file, "%49s %f %d\n", operations[i].name, &operations[i].price, &operations[i].duration) == 3)
       { i++; }

    fclose(file);
}

void load_appointments() {
    FILE *file = fopen("app_details.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    while (fscanf(file, "%d %d %d %d %d %49s\n",
                   &appointments[appointment_count].day,
                   &appointments[appointment_count].month,
                   &appointments[appointment_count].year,
                   &appointments[appointment_count].hour,
                   &appointments[appointment_count].minute,
                   appointments[appointment_count].opName) == 6) {
        appointment_count++;
        if (appointment_count >= MAX_APP) {
            printf("Reached maximum number of appointments.\n");
            break;
        }
    }

    fclose(file);
}

void display_operations() {
    printf("\nAvailable Operations:\n");
    for (int i = 0; i < MAX_OP; i++) {
        printf("%d. %s - $%.2f - Duration: %d minutes\n", i + 1, operations[i].name, operations[i].price, operations[i].duration);
    }
    printf("\n");
}

void check_availability() {
    int day, month, year, hour, minute;
    printf("Enter the day (DD): ");
    scanf("%d", &day);
    printf("Enter the month (MM): ");
    scanf("%d", &month);
    printf("Enter the year (YYYY): ");
    scanf("%d", &year);
    printf("Enter the hour (HH): ");
    scanf("%d", &hour);
    printf("Enter the minute (MM): ");
    scanf("%d", &minute);

    if (is_date_available(day, month, year, hour, minute)) {
        printf("The office is available at \033[0;36m %02d:%02d \033[0m on \033[0;32m %02d/%02d/%d. \033[0m \n", hour, minute, day, month, year);
    } else {
        printf("Sorry, the office is not available at that time.\n");
    }
}

int is_date_available(int day, int month, int year, int hour, int minute) {
    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i].day == day && appointments[i].month == month && appointments[i].year == year &&
            appointments[i].hour == hour && appointments[i].minute == minute) {
            return 0;
        }
    }
    return 1;
}

void schedule_appointment() {
    int day, month, year, hour, minute, operation_index;
    printf("Enter the day (DD): ");
    scanf("%d", &day);
    printf("Enter the month (MM): ");
    scanf("%d", &month);
    printf("Enter the year (YYYY): ");
    scanf("%d", &year);
    printf("Enter the hour (HH): ");
    scanf("%d", &hour);
    printf("Enter the minute (MM): ");
    scanf("%d", &minute);

    if (is_date_available(day, month, year, hour, minute)) {
        display_operations();
        printf("Enter the number of the operation you want to schedule: ");
        scanf("%d", &operation_index);
        operation_index--;

        if (operation_index >= 0 && operation_index < MAX_OP) {
            appointments[appointment_count].day = day;
            appointments[appointment_count].month = month;
            appointments[appointment_count].year = year;
            appointments[appointment_count].hour = hour;
            appointments[appointment_count].minute = minute;
            strcpy(appointments[appointment_count].opName, operations[operation_index].name);
            appointment_count++;
            printf("Appointment scheduled for \033[0;35m%s \033[0m at \033[0;34m %02d:%02d \033[0m on \033[0;33m %02d/%02d/%d. \033[0m \n",
                   operations[operation_index].name, hour, minute, day, month, year);
        } else {
            printf("Invalid operation number.\n");
        }
    } else {
        printf("Sorry, the office is not available at that time.\n");
    }
}

void view_history() {
    if (appointment_count == 0) {
        printf("No appointments scheduled yet.\n");
    } else {
        printf("\nAppointment History:\n");
        for (int i = 0; i < appointment_count; i++) {
            printf("%d. %s - %02d/%02d/%d at %02d:%02d\n", i + 1,
                   appointments[i].opName,
                   appointments[i].day, appointments[i].month, appointments[i].year,
                   appointments[i].hour, appointments[i].minute);
        }
    }
    printf("\n");
}

void save_appointments() {
    FILE *file = fopen("app_details.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }


    for (int i = 0; i < appointment_count; i++) {
        fprintf(file, "%d %d %d %d %d %s\n",
                appointments[i].day,
                appointments[i].month,
                appointments[i].year,
                appointments[i].hour,
                appointments[i].minute,
                appointments[i].opName);
    }

    fclose(file);
    printf("Appointments saved successfully.\n");
}


int main()
{
    load_operations();
    load_appointments();

    int option;
    do{
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();
        printf("\n");
        menu(option);
    }while(option > 0 && option <= 5);

    return 0;
}
