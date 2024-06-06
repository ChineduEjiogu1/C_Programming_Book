#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELD_LENGTH 100

typedef struct
{
    int Rank;
    char Title[MAX_FIELD_LENGTH];
    char Genre[MAX_FIELD_LENGTH];
    char Description[MAX_FIELD_LENGTH];
    char Director[MAX_FIELD_LENGTH];
    char Actors[MAX_FIELD_LENGTH];
    char Year[MAX_FIELD_LENGTH];
    char Runtime[MAX_FIELD_LENGTH];
    char Rating[MAX_FIELD_LENGTH];
    char Votes[MAX_FIELD_LENGTH];
    char Revenue[MAX_FIELD_LENGTH];
    char Metascore[MAX_FIELD_LENGTH];
} Movie;

typedef struct
{
    char first_name[50];
    char last_name[50];
    char name_suffix[10];
    char email_address[100];
    char street_address[100];
    char state[10];
    char zipcode[10];
    char city[50];
    char card_number[20];
    char card_expr[10];
    char card_sec[5];
} PurchaseInfo;

Movie *movie_db = NULL;
int movie_count = 0;
int movie_capacity = 0;
PurchaseInfo purchase_info;
Movie movie_to_purchase;
int ok_to_purchase = 0;

void trim_whitespace(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = 0;
}

char *read_field(char **rest)
{
    char *field = *rest;
    if (*field == '"')
    {
        field++;
        char *end = strchr(field, '"');
        while (end && *(end + 1) == '"')
        {
            end = strchr(end + 2, '"');
        }
        if (end)
        {
            *end = '\0';
            *rest = end + 2;
        }
        else
        {
            *rest = field + strlen(field);
        }
    }
    else
    {
        char *end = strchr(field, ',');
        if (end)
        {
            *end = '\0';
            *rest = end + 1;
        }
        else
        {
            *rest = field + strlen(field);
        }
    }
    trim_whitespace(field);
    return field;
}

void add_movie(Movie movie)
{
    if (movie_count == movie_capacity)
    {
        movie_capacity = movie_capacity == 0 ? 10 : movie_capacity * 2;
        movie_db = realloc(movie_db, sizeof(Movie) * movie_capacity);
        if (!movie_db)
        {
            perror("Memory reallocation failed");
            exit(EXIT_FAILURE);
        }
    }
    movie_db[movie_count++] = movie;
}

void load_movies(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file); // Skip header

    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        Movie movie = {0};
        char *rest = line;
        int field_index = 0;

        while (*rest)
        {
            char *token = read_field(&rest);
            switch (field_index)
            {
            case 0:
                movie.Rank = atoi(token);
                break;
            case 1:
                strncpy(movie.Title, token, MAX_FIELD_LENGTH);
                break;
            case 2:
                strncpy(movie.Genre, token, MAX_FIELD_LENGTH);
                break;
            case 3:
                strncpy(movie.Description, token, MAX_FIELD_LENGTH);
                break;
            case 4:
                strncpy(movie.Director, token, MAX_FIELD_LENGTH);
                break;
            case 5:
                strncpy(movie.Actors, token, MAX_FIELD_LENGTH);
                break;
            case 6:
                strncpy(movie.Year, token, MAX_FIELD_LENGTH);
                break;
            case 7:
                strncpy(movie.Runtime, token, MAX_FIELD_LENGTH);
                break;
            case 8:
                strncpy(movie.Rating, token, MAX_FIELD_LENGTH);
                break;
            case 9:
                strncpy(movie.Votes, token, MAX_FIELD_LENGTH);
                break;
            case 10:
                strncpy(movie.Revenue, token, MAX_FIELD_LENGTH);
                break;
            case 11:
                strncpy(movie.Metascore, token, MAX_FIELD_LENGTH);
                break;
            }
            field_index++;
        }
        add_movie(movie);
    }

    fclose(file);
}

void get_input(const char *prompt, char *input, size_t size)
{
    printf("%s", prompt);
    fgets(input, size, stdin);
    input[strcspn(input, "\n")] = '\0';
}

int regex_match(const char *pattern, const char *str)
{
    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    ret = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return !ret;
}

int validate_alpha_input(const char *input)
{
    return regex_match("^[A-Za-z]+$", input);
}

int validate_email(const char *email)
{
    return regex_match("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}$", email);
}

int validate_zipcode(const char *zipcode)
{
    return regex_match("^[0-9]{5}(-[0-9]{4})?$", zipcode);
}

int validate_credit_card(const char *card_number)
{
    return regex_match("^[0-9]{16}$", card_number);
}

int validate_card_expr(const char *card_expr)
{
    return regex_match("^(0[1-9]|1[0-2])/[0-9]{2}$", card_expr);
}

int validate_card_sec(const char *card_sec)
{
    return regex_match("^[0-9]{3,4}$", card_sec);
}

void lookup_movies_by_title(const char *title)
{
    printf("\nPerforming lookup by Title\n");
    printf("==========================\n");
    char lowercase_title[MAX_FIELD_LENGTH];
    for (int i = 0; title[i]; i++)
    {
        lowercase_title[i] = tolower(title[i]);
    }
    lowercase_title[strlen(title)] = '\0';

    int suggestions_count = 0;
    Movie suggestions[10];

    for (int i = 0; i < movie_count; i++)
    {
        char lowercase_db_title[MAX_FIELD_LENGTH];
        for (int j = 0; movie_db[i].Title[j]; j++)
        {
            lowercase_db_title[j] = tolower(movie_db[i].Title[j]);
        }
        lowercase_db_title[strlen(movie_db[i].Title)] = '\0';

        if (strstr(lowercase_db_title, lowercase_title))
        {
            suggestions[suggestions_count++] = movie_db[i];
            printf("%d. %s\n", suggestions_count, movie_db[i].Title);
            if (suggestions_count >= 10)
                break;
        }
    }

    if (suggestions_count == 0)
    {
        printf("Movie not found.\n");
        return;
    }

    int choice;
    printf("\nEnter the number of the movie you want to select: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > suggestions_count)
    {
        printf("Invalid choice.\n");
        return;
    }

    movie_to_purchase = suggestions[choice - 1];
    ok_to_purchase = 1;
}

void lookup_movies_by_year(const char *year)
{
    printf("\nPerforming lookup by Year\n");
    printf("==========================\n");
    int found = 0;
    for (int i = 0; i < movie_count; i++)
    {
        if (strcmp(movie_db[i].Year, year) == 0)
        {
            printf("%d. %s\n", i + 1, movie_db[i].Title);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No movies found for the year %s.\n", year);
        return;
    }

    int choice;
    printf("\nEnter the number of the movie you want to select: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > movie_count || strcmp(movie_db[choice - 1].Year, year) != 0)
    {
        printf("Invalid choice.\n");
        return;
    }

    movie_to_purchase = movie_db[choice - 1];
    ok_to_purchase = 1;
}

void prompt_billing_info()
{
    const struct
    {
        const char *prompt;
        char *field;
        size_t size;
        int (*validator)(const char *);
        const char *error_msg;
    } prompts[] = {
        {"Enter first name: ", purchase_info.first_name, sizeof(purchase_info.first_name), validate_alpha_input, "Invalid first name. Please enter only alphabetic characters.\n"},
        {"Enter last name: ", purchase_info.last_name, sizeof(purchase_info.last_name), validate_alpha_input, "Invalid last name. Please enter only alphabetic characters.\n"},
        {"Enter name suffix (optional): ", purchase_info.name_suffix, sizeof(purchase_info.name_suffix), NULL, NULL},
        {"Enter email address: ", purchase_info.email_address, sizeof(purchase_info.email_address), validate_email, "Invalid email address. Please enter a valid email.\n"},
        {"Enter street address: ", purchase_info.street_address, sizeof(purchase_info.street_address), NULL, NULL},
        {"Enter city: ", purchase_info.city, sizeof(purchase_info.city), validate_alpha_input, "Invalid city name. Please enter only alphabetic characters.\n"},
        {"Enter state (2-letter abbreviation): ", purchase_info.state, sizeof(purchase_info.state), validate_alpha_input, "Invalid state abbreviation. Please enter a valid 2-letter state abbreviation.\n"},
        {"Enter ZIP code: ", purchase_info.zipcode, sizeof(purchase_info.zipcode), validate_zipcode, "Invalid ZIP code. Please enter a valid ZIP code (5 or 9 digits).\n"},
        {"Enter credit card number (16 digits): ", purchase_info.card_number, sizeof(purchase_info.card_number), validate_credit_card, "Invalid credit card number. Please enter a valid 16-digit credit card number.\n"},
        {"Enter credit card expiration date (MM/YY): ", purchase_info.card_expr, sizeof(purchase_info.card_expr), validate_card_expr, "Invalid expiration date. Please enter a valid expiration date (MM/YY).\n"},
        {"Enter credit card security code (CVV): ", purchase_info.card_sec, sizeof(purchase_info.card_sec), validate_card_sec, "Invalid CVV. Please enter a valid CVV (3 or 4 digits).\n"}};

    for (int i = 0; i < sizeof(prompts) / sizeof(prompts[0]); i++)
    {
        do
        {
            get_input(prompts[i].prompt, prompts[i].field, prompts[i].size);
        } while (prompts[i].validator && !prompts[i].validator(prompts[i].field) && printf("%s", prompts[i].error_msg));
    }
}

void print_movie_info(const Movie *movie)
{
    printf("Movie Details:\n");
    printf("==============\n");
    printf("Title: %s\n", movie->Title);
    printf("Genre: %s\n", movie->Genre);
    printf("Description: %s\n", movie->Description);
    printf("Director: %s\n", movie->Director);
    printf("Actors: %s\n", movie->Actors);
    printf("Year: %s\n", movie->Year);
    printf("Runtime: %s\n", movie->Runtime);
    printf("Rating: %s\n", movie->Rating);
    printf("Votes: %s\n", movie->Votes);
    printf("Revenue: %s\n", movie->Revenue);
    printf("Metascore: %s\n", movie->Metascore);
    printf("Total Price: $15.00\n");
}

void confirm_purchase()
{
    printf("\nMovie Purchase Confirmation\n");
    printf("===========================\n");
    printf("You have successfully purchased the movie '%s'!\n", movie_to_purchase.Title);
    printf("A confirmation email will be sent to %s.\n\n", purchase_info.email_address);

    print_movie_info(&movie_to_purchase);

    printf("\nBilling Information:\n");
    printf("====================\n");
    printf("Name: %s %s %s\n", purchase_info.first_name, purchase_info.last_name, purchase_info.name_suffix);
    printf("Email: %s\n", purchase_info.email_address);
    printf("Address: %s, %s, %s, %s\n", purchase_info.street_address, purchase_info.city, purchase_info.state, purchase_info.zipcode);
    printf("Credit Card: **** **** **** %s\n", purchase_info.card_number + 12); // Only show last 4 digits
}

int main()
{
    load_movies("IMDB-Movie-Data.csv");

    char title[MAX_FIELD_LENGTH];
    char year[MAX_FIELD_LENGTH];

    printf("Do you want to lookup by title or by year? (Enter 'title' or 'year'): ");
    char choice[10];
    get_input("", choice, sizeof(choice));

    if (strcasecmp(choice, "title") == 0)
    {
        printf("Enter the movie title: ");
        get_input("", title, sizeof(title));
        lookup_movies_by_title(title);
    }
    else if (strcasecmp(choice, "year") == 0)
    {
        printf("Enter the movie year: ");
        get_input("", year, sizeof(year));
        lookup_movies_by_year(year);
    }
    else
    {
        printf("Invalid choice.\n");
        return 1;
    }

    if (ok_to_purchase)
    {
        prompt_billing_info();
        confirm_purchase();
    }

    free(movie_db);
    return 0;
}
