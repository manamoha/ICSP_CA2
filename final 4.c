/*
* 2021 Fall
* Computer Assginment 2 Phase 1
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_COMMAND_LENGTH 1000
#define FALSE 0
#define TRUE 1
#define NOT_SIGNED_IN -1
#define ADMIN_ID -2
#define MAX_STR_LEN 100

//file addresses
#define USERS_PATH "users.txt"
#define HALLS_PATH "hall_report.txt"
#define FOODS_PATH "food_report.txt"

//commands
#define SIGNUP "signup"
#define LOGIN "login"
#define LOGOUT "logout"
#define ADD_HALL "add_hall"
#define ADD_FOOD "add_food"
#define ADD_COUPON "add_coupon"
#define GET_MONEY "get_money"
#define SHOW_MONEY "show_money"
#define MENU "menu"
#define RESERVE "reserve"
#define RESERVE_DISCOUNT "reserve_discount"
#define HALL_REPORT "hall_report"
#define FOOD_REPORT "food_report"
#define SHOW_RESERVED "show_reserved"
#define EXIT "exit"


//Defining Structs

typedef struct Hall {
    char name[MAX_STR_LEN];
    int hall_id;
    int capacity;
    int reserved[7];
    int profit[7];
    struct Hall* next;
} Hall;

typedef struct Food {
    char name[MAX_STR_LEN];
    int food_id;
    int price;
    int capacity;
    int hall_id;
    int day;
    int reserved;
    int profit;
    struct Food* next;
} Food;

typedef struct Coupon {
    int coupon_id;
    int percentage;
    int capacity;
    int used;
    struct Coupon* next;
} Coupon;

//for each student's reserved foods
typedef struct Reserved {
    int food_id;
    struct Reserved* next;
} Reserved;

typedef struct Student {
    char username[MAX_STR_LEN];
    int student_id;
    char password[MAX_STR_LEN];
    int money;
    Reserved* reserved[7];
    struct Student* next;
} Student;

typedef struct Menu {
    Food* item;
    struct Menu* next;
}Menu;


//prints
void print_welcome(char username[MAX_STR_LEN])
{
    printf("%s", "welcome ");
    printf("%s", username);
    printf("%s", "\n");
}

void print_done()
{
    printf("%s", "done\n");
}

void print_user_already_exists()
{
    printf("%s", "user already exists\n");
}

void print_wrong_user_pass()
{
    printf("%s", "wrong username or pass\n");
}

void print_access_denied()
{
    printf("%s", "access denied\n");
}

void print_not_unique_hall_id()
{
    printf("%s", "hall id is not unique\n");
}

void print_not_unique_food_id()
{
    printf("%s", "food id is not unique\n");
}

void print_not_unique_coupon_id()
{
    printf("%s", "coupon id is not unique\n");
}

void print_wrong_hall_id()
{
    printf("%s", "wrong hall id\n");
}

void print_not_possible()
{
    printf("not possible\n");
}


int is_empty(char* str)
{
    if (strlen(str) <= 1)
        return TRUE;
    else
        return FALSE;
}


//finds the student/hall/food/coupon with mentioned id and returns the node
Student* find_student(Student* students, int student_id)
{
    Student* temp = students->next;
    while (temp != NULL)
    {
        if (temp->student_id == student_id)
            break;
        temp = temp->next;
    }
    return temp;
}

Hall* find_hall(Hall* halls, int hall_id)
{
    Hall* temp = halls->next;
    while (temp != NULL)
    {
        if (temp->hall_id == hall_id)
            break;
        temp = temp->next;
    }
    return temp;
}

Food* find_food(Food* foods, int food_id)
{
    Food* temp = foods->next;
    while (temp != NULL)
    {
        if (temp->food_id == food_id)
            break;
        temp = temp->next;
    }
    return temp;
}

Coupon* find_coupon(Coupon* coupons, int coupon_id)
{
    Coupon* temp = coupons->next;
    while (temp != NULL)
    {
        if (temp->coupon_id == coupon_id)
            break;
        temp = temp->next;
    }
    return temp;
}


//free functions
void free_reserved(Reserved* reserved)
{
    if (reserved == NULL)
        return;
    free_reserved(reserved->next);
    free(reserved);
}

void free_reserved_arr(Reserved** reserved)
{
    for (int i = 6; i >= 0; i--)
        free_reserved(reserved[i]);
}

void free_students_reserved(Student* students)
{
    while (students != NULL)
    {
        free_reserved_arr(students->reserved);

        students = students->next;
    }
}

void free_student(Student* student)
{
    if (student == NULL)
        return;
    free_student(student->next);
    free(student);
}

void free_menu(Menu* menu)
{
    if (menu == NULL)
        return;
    free_menu(menu->next);
    free(menu);
}

void free_menu_arr(Menu** menu)
{
    for (int i = 6; i >= 0; i--)
        free_menu(menu[i]);
}

void free_hall(Hall* hall)
{
    if (hall == NULL)
        return;
    free_hall(hall->next);
    free(hall);
}

void free_food(Food* food)
{
    if (food == NULL)
        return;
    free_food(food->next);
    free(food);
}

void free_coupon(Coupon* coupon)
{
    if (coupon == NULL)
        return;
    free_coupon(coupon->next);
    free(coupon);
}

void free_all_memmories(Student* students, Hall* halls, Food* foods, Coupon* coupons, Menu** menu)
{
    free_students_reserved(students->next);
    free_student(students);
    free_menu_arr(menu);
    free_hall(halls);
    free_food(foods);
    free_coupon(coupons);
}


/*admin section*/


/*menu*/

//creates a new menu node and sets the next pointer to NULL
Menu* new_menu_node()
{
    Menu* Mn = malloc(sizeof(Menu));
    Mn->next = NULL;
    return Mn;
}

//sets each element of the array to a menu node (each of them will be a head of a linked list)
void initialize_menu_arr(Menu** arr)
{
    for (int i = 0; i < 7; i++)
        arr[i] = new_menu_node();
}

//adds a new food to menu array
void add_to_menu(Food* item, Menu** menu, int day)
{
    Menu* Mn = new_menu_node();
    Mn->item = item;

    Menu* temp = menu[day - 1];
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = Mn;
}


/*halls*/

//creates a new hall node and sets the next pointer to NULL
Hall* new_hall_node()
{
    Hall* hl = malloc(sizeof(Hall));
    hl->next = NULL;
    return hl;
}

//creates a hall with given informations and adds it to the list
void add_hall_node(char name[MAX_STR_LEN], int hall_id, int capacity, Hall* halls)
{
    Hall* hl = new_hall_node();
    strcpy(hl->name, name);
    hl->capacity = capacity;
    hl->hall_id = hall_id;
    for (int i = 0; i < 7; i++)
    {
        hl->profit[i] = 0;
        hl->reserved[i] = 0;
    }

    Hall* temp = NULL;
    temp = halls;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = hl;
}

//if the hall_id is not already used, it will add a new hall to the list
int add_hall(char name[MAX_STR_LEN], int hall_id, int capacity, Hall* halls)
{
    Hall* temp = halls->next;
    //checking if the hall_id already exists, if so it returns FALSE
    while (temp != NULL)
    {
        if (temp->hall_id == hall_id)
            return FALSE;
        temp = temp->next;
    }

    add_hall_node(name, hall_id, capacity, halls);

    return TRUE;
}

//gets the data from command line and passes it to add_hall function
int add_hall_command(char cmd[MAX_COMMAND_LENGTH], Hall* halls)
{
    char* token = strtok(cmd, " ");
    //Skip the first token
    token = strtok(NULL, " ");

    char name[MAX_STR_LEN];
    int hall_id;
    int capacity;

    int i = 0;
    while (token) {

        switch (i)
        {
        case 0:
            strcpy(name, token);
            break;
        case 1:
            hall_id = atoi(token);
            break;
        case 2:
            capacity = atoi(token);
            break;
        }

        i += 1;
        token = strtok(NULL, " ");
    }

    return add_hall(name, hall_id, capacity, halls);
}



/*foods*/

//creates a new food node and sets the next pointer to NULL
Food* new_food_node()
{
    Food* fd = malloc(sizeof(Food));
    fd->next = NULL;
    return fd;
}

//creates a food with given informations and adds it to the list
//also it passes the food to add_to_menu function so that it would be added to menu array as well
void add_food_node(char name[MAX_STR_LEN], int food_id, int price, int capacity, int hall_id, int day, Food* foods, Menu** menu)
{
    Food* fd = new_food_node();
    strcpy(fd->name, name);
    fd->food_id = food_id;
    fd->price = price;
    fd->capacity = capacity;
    fd->hall_id = hall_id;
    fd->day = day;
    fd->reserved = 0;
    fd->profit = 0;

    Food* temp = foods;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = fd;

    add_to_menu(fd, menu, day);
}

//if the hall exists and the food_id hasn't already been used, it will add a new food to the list
int add_food(char name[MAX_STR_LEN], int food_id, int price, int capacity, int hall_id, int day
    , Food* foods, Hall* halls, Menu** menu)
{
    // Here we check if hall_id exists
    int hall_found = FALSE;
    Hall* temp1 = halls->next;
    while (temp1 != NULL)
    {
        if (temp1->hall_id == hall_id)
        {
            hall_found = TRUE;
            break;
        }
        temp1 = temp1->next;
    }
    if (hall_found == FALSE)
    {
        print_wrong_hall_id();
        return FALSE;
    }

    // Here we check if food_id exists
    Food* temp2 = foods->next;
    while (temp2 != NULL)
    {
        if (temp2->food_id == food_id)
        {
            print_not_unique_food_id();
            return FALSE;
        }
        temp2 = temp2->next;
    }

    add_food_node(name, food_id, price, capacity, hall_id, day, foods, menu);

    return TRUE;
}

//gets the data from command line and passes it to add_food function
int add_food_command(char cmd[MAX_COMMAND_LENGTH], Food* foods, Hall* halls, Menu** menu)
{
    char* token = strtok(cmd, " ");
    //Skip the first token
    token = strtok(NULL, " ");

    char name[MAX_STR_LEN];
    int food_id;
    int price;
    int capacity;
    int hall_id;
    int day;

    int i = 0;
    while (token) {

        switch (i)
        {
        case 0:
            strcpy(name, token);
            break;
        case 1:
            food_id = atoi(token);
            break;
        case 2:
            price = atoi(token);
            break;
        case 3:
            capacity = atoi(token);
            break;
        case 4:
            hall_id = atoi(token);
            break;
        case 5:
            day = atoi(token);
            break;
        }

        i += 1;
        token = strtok(NULL, " ");
    }

    return add_food(name, food_id, price, capacity, hall_id, day, foods, halls, menu);
}



/*coupons*/

//creates a new coupon node and sets the next pointer to NULL
Coupon* new_coupon_node()
{
    Coupon* cp = malloc(sizeof(Coupon));
    cp->next = NULL;
    return cp;
}

//creates a coupon with given informations and adds it to the list
void add_coupon_node(int coupon_id, int percentage, int capacity, Coupon* coupons)
{
    Coupon* cp = new_coupon_node();
    cp->coupon_id = coupon_id;
    cp->percentage = percentage;
    cp->capacity = capacity;
    cp->used = 0;

    Coupon* temp = coupons;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = cp;
}

////if the coupon_id hasn't already been used, it will add a new coupon to the list
int add_coupon(int coupon_id, int percentage, int capacity, Coupon* coupons)
{
    Coupon* temp = coupons->next;
    while (temp != NULL)
    {
        if (temp->coupon_id == coupon_id)
            return FALSE;
    }

    add_coupon_node(coupon_id, percentage, capacity, coupons);

    return TRUE;
}

//gets the data from command line and passes it to add_coupon function
int add_coupon_command(char cmd[MAX_COMMAND_LENGTH], Coupon* coupons)
{
    char* token = strtok(cmd, " ");
    //Skip the first token
    token = strtok(NULL, " ");

    int coupon_id;
    int percentage;
    int capacity;

    int i = 0;
    while (token) {

        switch (i)
        {
        case 0:
            coupon_id = atoi(token);
            break;
        case 1:
            percentage = atoi(token);
            break;
        case 2:
            capacity = atoi(token);
            break;
        }

        i += 1;
        token = strtok(NULL, " ");
    }

    return add_coupon(coupon_id, percentage, capacity, coupons);
}


/*reports*/

//food report
void food_report(Food* foods)
{
    FILE* fptr;
    fptr = fopen(FOODS_PATH, "w");

    //checks if the list is empty
    if (foods == NULL)
        fprintf(fptr, "there's no food available");

    //puts each food's info in a string and prints it in the file
    while (foods != NULL)
    {
        char str[MAX_COMMAND_LENGTH];

        char food_id_string[MAX_STR_LEN];
        sprintf(food_id_string, "%d", foods->food_id);

        char capacity_string[MAX_STR_LEN];
        sprintf(capacity_string, "%d", foods->capacity);

        char reserved_string[MAX_STR_LEN];
        sprintf(reserved_string, "%d", foods->reserved);

        char profit_string[MAX_STR_LEN];
        sprintf(profit_string, "%d", foods->profit);

        strcpy(str, food_id_string);
        strcat(str, " ");
        strcat(str, capacity_string);
        strcat(str, " ");
        strcat(str, reserved_string);
        strcat(str, " ");
        strcat(str, profit_string);
        strcat(str, "\n");

        fputs(str, fptr);

        foods = foods->next;
    }

    fclose(fptr);
}


//hall report
void hall_report(Hall* halls)
{
    FILE* fptr;
    fptr = fopen(HALLS_PATH, "w");

    //checks if the list is empty
    if (halls == NULL)
        fprintf(fptr, "there's no hall");

    //for each hall, first puts it's info in the string and then adds each day's data to the end of the string and prints it in the file
    while (halls != NULL)
    {
        char str[MAX_COMMAND_LENGTH];

        char hall_id_string[MAX_STR_LEN];
        sprintf(hall_id_string, "%d", halls->hall_id);

        char capacity_string[MAX_STR_LEN];
        sprintf(capacity_string, "%d", halls->capacity);

        strcpy(str, hall_id_string);
        strcat(str, " ");
        strcat(str, halls->name);
        strcat(str, " ");
        strcat(str, capacity_string);
        strcat(str, "\n");

        for (int i = 0; i < 7; i++)
        {
            char reserved_string[MAX_STR_LEN];
            sprintf(reserved_string, "%d", halls->reserved[i]);

            char profit_string[MAX_STR_LEN];
            sprintf(profit_string, "%d", halls->profit[i]);

            char day_string[2];
            sprintf(day_string, "%d", i + 1);

            strcat(str, day_string);
            strcat(str, " ");

            strcat(str, reserved_string);
            strcat(str, " ");

            strcat(str, profit_string);
            strcat(str, "\n");
        }

        fputs(str, fptr);

        halls = halls->next;
    }

    fclose(fptr);
}



/* students section */

/*get money*/

//finds the student with given id, and addes to their money
void get_money(Student* students, int student_id, int money)
{
    Student* std = find_student(students, student_id);
    std->money = std->money + money;
}

//gets the data from command line and passes it to get_money function
void get_money_command(char cmd[MAX_COMMAND_LENGTH], Student* students, int student_id)
{
    char* token = strtok(cmd, " ");
    token = strtok(NULL, " ");

    int money;
    money = atoi(token);
    get_money(students, student_id, money);
}

/*reserve*/

//creates a new reserved node and sets the next pointer to NULL
Reserved* new_reserved_node()
{
    Reserved* rev = malloc(sizeof(Reserved));
    rev->next = NULL;
    return rev;
}

//creates a reserved node with given informations and adds it to the list of that day
void add_to_reserved(Student* student, int food_id, int day)
{
    Reserved* rev = new_reserved_node();

    rev->food_id = food_id;

    Reserved* temp = student->reserved[day - 1];
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = rev;
}

//normal reserve
int reserve(Student* students, int student_id, Food* foods, Hall* halls, int food_id)
{
    //checks if the food exist and the food and hall are not full, and if the student has enough money
    Food* fd = find_food(foods, food_id);
    if (fd == NULL)
        return 0;
    if (fd->reserved == fd->capacity)
        return 0;

    Hall* hl = find_hall(halls, fd->hall_id);
    if (hl->capacity == hl->reserved)
        return 0;

    Student* std = find_student(students, student_id);
    if (std->money < fd->price)
        return 0;

    //everything is okay so we're reserving the food
    fd->reserved += 1;
    fd->profit += fd->price;
    hl->reserved[fd->day] += 1;
    hl->profit[fd->day] += fd->price;
    std->money -= fd->price;

    //add to student's reserved array
    add_to_reserved(std, food_id, fd->day);

    return 1;
}

//gets the data from command line and passes it to reserve function
int reserve_command(char cmd[MAX_COMMAND_LENGTH], Student* students, int student_id, Food* foods, Hall* halls)
{
    char* token = strtok(cmd, " ");
    token = strtok(NULL, " ");

    int food_id;
    food_id = atoi(token);
    return reserve(students, student_id, foods, halls, food_id);
}

//reserve with discount
int reserve_discount(Student* students, int student_id, Food* foods, Hall* halls, Coupon* coupons, int food_id, int coupon_code)
{
    //checks if the food and coupon exist and the food, hall and coupon are not full, and if the student has enough money
    Food* fd = find_food(foods, food_id);
    if (fd == NULL)
        return 0;
    if (fd->reserved == fd->capacity)
        return 0;

    Hall* hl = find_hall(halls, fd->hall_id);
    if (hl->capacity == hl->reserved)
        return 0;

    Coupon* cp = find_coupon(coupons, coupon_code);
    if (cp == NULL)
        return 0;
    if (cp->capacity == cp->used)
        return 0;

    Student* std = find_student(students, student_id);
    int pay_price = (fd->price) * (cp->percentage) / 100;
    if (std->money < pay_price)
        return 0;

    //everything is okay so we're reserving the food

    fd->reserved += 1;
    fd->profit += pay_price;
    hl->reserved[fd->day] += 1;
    hl->reserved[fd->day] += pay_price;
    std->money -= pay_price;
    cp->used += 1;
    
    //add to student's reserved array
    add_to_reserved(std, food_id, fd->day);

    return 1;
}

//gets the data from command line and passes it to reserve_discount function
int reserve_discount_command(char cmd[MAX_COMMAND_LENGTH], Student* students, int student_id, Food* foods, Hall* halls, Coupon* coupons)
{
    char* token = strtok(cmd, " ");
    token = strtok(NULL, " ");

    int food_id;
    int coupon_code;

    int i = 0;
    while (token) {

        switch (i)
        {
        case 0:
            food_id = atoi(token);
            break;
        case 1:
            coupon_code = atoi(token);
            break;
        }

        i += 1;
        token = strtok(NULL, " ");
    }

    return reserve_discount(students, student_id, foods, halls, coupons, food_id, coupon_code);
}

/*show data*/

//shows the menu
void show_menu(Menu** menu, Hall* halls)
{
    for (int i = 0; i < 7; i++)
    {
        printf("day %d:\n", i + 1);
        Menu* temp = menu[i]->next;
        while (temp != NULL)
        {
            Hall* hl = find_hall(halls, temp->item->hall_id);
            printf("%s %d %d %s %d\n", temp->item->name, temp->item->price, temp->item->food_id, hl->name, (temp->item->capacity) - (temp->item->reserved));
            temp = temp->next;
        }
    }
}

//shows the student's money
void show_money(Student* students, int student_id)
{
    Student* std = find_student(students, student_id);
    printf("%d\n", std->money);
}

//shows the student's reserved foods for each day
void show_reserved(Reserved** reserved, Hall* halls, Food* foods)
{
    for (int i = 0; i < 7; i++)
    {
        printf("day %d:\n", i + 1);

        Reserved* temp = reserved[i]->next;
        while (temp != NULL)
        {
            Food* fd = find_food(foods, temp->food_id);
            Hall* hl = find_hall(halls, fd->hall_id);
            printf("%s %d %d %s\n", fd->name, fd->price, fd->food_id, hl->name);
            temp = temp->next;
        }
    }
}

//finds the student with given id and passes it to show_reserved function
void show_reserved_command(Student* students, Hall* halls, Food* foods, int student_id)
{
    Student* std = find_student(students, student_id);
    show_reserved(std->reserved, halls, foods);
}


/*no one logged in*/


/*signup*/

//creates an empty student node and sets the next pointer to NULL
Student* new_student_node()
{
    Student* std = malloc(sizeof(Student));
    std->next = NULL;
    return std;
}

//writes new user to file
void save_new_user(char username[MAX_STR_LEN], int student_id, char password[MAX_STR_LEN])
{
    char str[MAX_COMMAND_LENGTH];

    char student_id_string[MAX_STR_LEN];
    sprintf(student_id_string, "%d", student_id);

    strcpy(str, username);
    strcat(str, " ");
    strcat(str, student_id_string);
    strcat(str, " ");
    strcat(str, password);
    strcat(str, "\n");

    // Saving the result
    FILE* fptr;
    fptr = fopen(USERS_PATH, "a");
    fputs(str, fptr);
    fclose(fptr);
}

//creates a new student with given informations and addes it to the list
void add_student_node(char username[MAX_STR_LEN], int student_id, char password[MAX_STR_LEN], Student* students)
{
    Student* std = new_student_node();
    strcpy(std->username, username);
    strcpy(std->password, password);
    std->student_id = student_id;
    std->money = 0;
    for (int i = 0; i < 7; i++)
    {
        std->reserved[i] = new_reserved_node();
    }

    Student* temp = NULL;
    temp = students;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = std;
}

//if the student_id doesn't already exist, it will add the new student to the list
// Returns true if the signup is successful
int signup(char username[MAX_STR_LEN], int student_id, char password[MAX_STR_LEN], Student* students, int save_result)
{
    Student* temp = students->next;
    while (temp != NULL)
    {
        // Check whether username or student_id exists
        if (strcmp(temp->username, username) == 0 || temp->student_id == student_id)
            return FALSE;
        temp = temp->next;
    }
    add_student_node(username, student_id, password, students);
    if (save_result == TRUE)
        save_new_user(username, student_id, password);
    return TRUE;
}

// This function tokenizes signup command
int signup_command(char cmd[MAX_COMMAND_LENGTH], Student* students)
{
    char* token = strtok(cmd, " ");
    //Skip the first token
    token = strtok(NULL, " ");

    char username[MAX_STR_LEN];
    int student_id;
    char password[MAX_STR_LEN];

    int i = 0;
    while (token) {

        switch (i)
        {
        case 0:
            strcpy(username, token);
            break;
        case 1:
            student_id = atoi(token);
            break;
        case 2:
            strcpy(password, token);
            break;
        }

        i += 1;
        token = strtok(NULL, " ");
    }
    int save_result = TRUE;
    return signup(username, student_id, password, students, save_result);
}


/*login*/

// Returns student_id if the username & password is correct otherwise it returns FALSE
int login(char username[MAX_STR_LEN], char password[MAX_STR_LEN], Student* students)
{

    // Check if the entered user & pass are admin or not
    char admin[MAX_STR_LEN] = "admin";
    if (strcmp(username, admin) == 0 && strcmp(password, admin) == 0)
    {
        print_welcome(username);
        return ADMIN_ID;
    }

    // Iterate through students array
    Student* temp = students->next;
    while (temp != NULL)
    {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
        {
            print_welcome(username);
            return temp->student_id;
        }
        temp = temp->next;
    }

    return FALSE;
}
//changed

// This function tokenizes login command
int login_command(char cmd[MAX_COMMAND_LENGTH], Student* students)
{
    char* token = strtok(cmd, " ");
    //Skip the first token
    token = strtok(NULL, " ");

    char username[MAX_STR_LEN];
    char password[MAX_STR_LEN];

    int i = 0;
    while (token) {

        switch (i)
        {
        case 0:
            strcpy(username, token);
            break;
        case 1:
            strcpy(password, token);
            break;
        }

        i += 1;
        token = strtok(NULL, " ");
    }

    return login(username, password, students);
}



void run(Student* students, Hall* halls, Food* foods, Coupon* coupons, Menu** menu)
{
    // Current logged in user student_id
    int logged_in_user = NOT_SIGNED_IN;
    int end = 0;

    while (!end)
    {
        char cmd[MAX_COMMAND_LENGTH] = "";

        fgets(cmd, MAX_COMMAND_LENGTH, stdin); // read from stdin

        if (!is_empty(cmd))
            // Remove Next Line(\n)
            cmd[strcspn(cmd, "\n")] = 0;

        char cmd_copy[MAX_COMMAND_LENGTH];
        strcpy(cmd_copy, cmd);

        char* p = strtok(cmd_copy, " ");

        // Signup/Login Commands
        if (logged_in_user == NOT_SIGNED_IN)
        {
            //Signup command
            if (strcmp(p, SIGNUP) == 0)
            {
                if (signup_command(cmd, students) == TRUE)
                {
                    print_done();
                }
                else
                    print_user_already_exists();
            }
            //Login command
            else if (strcmp(p, LOGIN) == 0)
            {
                int log = login_command(cmd, students);
                if (log == FALSE)
                    print_wrong_user_pass();
                else
                    logged_in_user = log;
            }
            //exit command
            else if (strcmp(p, EXIT) == 0)
            {
                free_all_memmories(students, halls, foods, coupons, menu);
                end = 1;
            }
            else
            {
                print_access_denied();
            }
        }
        // Admin Commands
        else if (logged_in_user == ADMIN_ID)
        {
            //Logout command
            if (strcmp(p, LOGOUT) == 0)
            {
                logged_in_user = NOT_SIGNED_IN;
                print_done();
            }
            //Add Hall
            else if (strcmp(p, ADD_HALL) == 0)
            {
                if (add_hall_command(cmd, halls) == TRUE)
                {
                    print_done();
                }
                else
                    print_not_unique_hall_id();
            }
            //Add Food
            else if (strcmp(p, ADD_FOOD) == 0)
            {
                if (add_food_command(cmd, foods, halls, menu) == TRUE)
                {
                    print_done();
                }
            }
            //Add Coupon
            else if (strcmp(p, ADD_COUPON) == 0)
            {
                if (add_coupon_command(cmd, coupons) == TRUE)
                {
                    print_done();
                }
                else
                {
                    print_not_unique_coupon_id();
                }
            }
            //Food report
            else if (strcmp(p, FOOD_REPORT) == 0)
            {
                food_report(foods->next);
                print_done();
            }
            //Hall report
            else if (strcmp(p, HALL_REPORT) == 0)
            {
                hall_report(halls->next);
                print_done();
            }
            //exit command
            else if (strcmp(p, EXIT) == 0)
            {
                free_all_memmories(students, halls, foods, coupons, menu);
                end = 1;
            }
            else
            {
                print_access_denied();
            }
        }
        // User Commands
        else
        {
            //Logout command
            if (strcmp(p, LOGOUT) == 0)
            {
                logged_in_user = NOT_SIGNED_IN;
                print_done();
            }
            //get money command
            else if (strcmp(p, GET_MONEY) == 0)
            {
                get_money_command(cmd, students, logged_in_user);
                print_done();
            }
            //show money command
            else if (strcmp(p, SHOW_MONEY) == 0)
            {
                show_money(students, logged_in_user);
            }
            //menu command
            else if (strcmp(p, MENU) == 0)
            {
                show_menu(menu, halls);
            }
            //reserve command
            else if (strcmp(p, RESERVE) == 0)
            {
                if (reserve_command(cmd, students, logged_in_user, foods, halls))
                    print_done();
                else
                    print_not_possible();
            }
            //reserve discount command
            else if (strcmp(p, RESERVE_DISCOUNT) == 0)
            {
                if (reserve_discount_command(cmd, students, logged_in_user, foods, halls, coupons))
                    print_done();
                else
                    print_not_possible();
            }
            //show reserved command
            else if (strcmp(p, SHOW_RESERVED) == 0)
            {
                show_reserved_command(students, halls, foods, logged_in_user);
            }
            //exit command
            else if (strcmp(p, EXIT) == 0)
            {
                free_all_memmories(students, halls, foods, coupons, menu);
                end = 1;
            }
            else
            {
                print_access_denied();
            }
        }

        continue;
    }
}

int file_exists(char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void read_students(Student* students)
{
    // Create one if it doesn't exist
    if (file_exists(USERS_PATH) == FALSE)
    {
        FILE* file_ptr = fopen(USERS_PATH, "w");
        fclose(file_ptr);
    }
    else {
        FILE* fp = fopen(USERS_PATH, "r");

        char buffer[MAX_COMMAND_LENGTH];
        while (fgets(buffer, MAX_COMMAND_LENGTH, fp))
        {
            // Remove trailing newline
            buffer[strcspn(buffer, "\n")] = 0;

            char* token = strtok(buffer, " ");

            char username[MAX_STR_LEN];
            int student_id;
            char password[MAX_STR_LEN];

            int i = 0;
            while (token) {

                switch (i)
                {
                case 0:
                    strcpy(username, token);
                    break;
                case 1:
                    student_id = atoi(token);
                    break;
                case 2:
                    strcpy(password, token);
                    break;
                }

                i += 1;
                token = strtok(NULL, " ");
            }

            int save_result = FALSE;
            signup(username, student_id, password, students, save_result);
            }

        fclose(fp);
    }
}

int main()
{
    Student* students = new_student_node();   //student_arr_head
    Hall* halls = new_hall_node();       //hall_arr_head
    Food* foods = new_food_node();            //food_arr_head
    Coupon* coupons = new_coupon_node();      //coupon_arr_head

    Menu** menu[7];
    initialize_menu_arr(menu);

    read_students(students);

    run(students, halls, foods, coupons, menu);

    return 0;
}