/**
 * @file main.c
 * @author Frantisek Spunda <xspundaf00>
 * @date 2023-28-10
 * @brief Description of your project
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 102     // in windows is end of line \r\n and we wanna support addreses of length 100
#define MAX_RECOMAND_LENGTH 128 // max length of recomandation comes from ASCII table that has 128 characters
#define MAX_UNIQUE_ADRESSES 10  // no reason for number 10, but bcs max length is 100, max possible addresses is 100 x 128 and we cant handle that

typedef struct
{
  int found;
  char recomand[MAX_RECOMAND_LENGTH];
  char last_found[MAX_LINE_LENGTH][MAX_UNIQUE_ADRESSES];
} Recomandation;

void string_remove_diacritics(char *str);
void char_remove(char *str, unsigned int index);
char string_compare(char *input, char *str);
void arg_join(char *outputString, int argc, char *argv[]);
void string_trim(char *str, int from_end);
void string_order(char *string);
void find_recomandations(Recomandation *recomandation, char *input, char *address);
void print_recomandation(Recomandation *recomandation, char user_input[MAX_LINE_LENGTH]);
int format_check(char *input);
void string_tolower(char *string);

int main(int argc, char **argv)
{
  char user_input[MAX_LINE_LENGTH] = "";
  arg_join(user_input, argc, argv);
  string_remove_diacritics(user_input);
  string_trim(user_input, 0);

  char address[MAX_LINE_LENGTH]; // Variable for one line of file

  Recomandation recomandation = {
      .found = 0,
      .recomand = "",
  };

  // Check if any addresses have been passed to program
  if (isatty(STDIN_FILENO))
  {
    fprintf(stderr, "There is no file with addresses. Pass it as parameter of program (program < addresses.txt)\n");
    return 1;
  }

  while (fgets(address, sizeof(address), stdin) != NULL) // Get one line of file
  {
    if (format_check(address))
    {
      string_remove_diacritics(address); // We have to remove diacritics since we are removing diacritics from user input
      string_trim(address, 1);
      find_recomandations(&recomandation, user_input, address);
    }
    else
      return 1;
  }

  print_recomandation(&recomandation, user_input);

  return 0;
}

/**
 * @brief Check format of input data
 *
 */
int format_check(char *input)
{
  for (int i = 0; i < MAX_LINE_LENGTH; i++)
    if (input[i] == '\n')
      return 1;

  fprintf(stderr, "Address \"%s\" is not valid. Address have to be smaller then %i chars", input, MAX_LINE_LENGTH - 2);
  return 0;
}

/**
 * @brief Function for printing recomandations in right format
 *
 * @param recomandation
 * @param user_input
 */
void print_recomandation(Recomandation *recomandation, char user_input[MAX_LINE_LENGTH])
{
  int duplicit = 1;
  int same_as_arg = -1;
  for (int i = 0; i < recomandation->found - 1; i++)
  {
    char tolower_recomendation[MAX_RECOMAND_LENGTH], tolower_recomendation_next[MAX_RECOMAND_LENGTH];
    strcpy(tolower_recomendation, recomandation->last_found[i]);
    strcpy(tolower_recomendation_next, recomandation->last_found[i + 1]);
    string_tolower(tolower_recomendation);
    string_tolower(tolower_recomendation_next);

    // Find duplicit values
    if (strcmp(tolower_recomendation, tolower_recomendation_next))
      duplicit = 0;

    // Check if we found exact same value as user input
    if (!strcmp(tolower_recomendation, user_input))
      same_as_arg = i;
  }

  if (recomandation->found > 1 && !duplicit)
  {
    if (same_as_arg >= 0)
      printf("Found: %s\n", recomandation->last_found[same_as_arg]);

    string_order(recomandation->recomand);
    printf("Enable: %s\n", recomandation->recomand);
  }
  else if (recomandation->found == 1 || (recomandation->found != 0 && duplicit))
    printf("Found: %s\n", recomandation->last_found[recomandation->found - 1]);
  else
    printf("Not found\n");
}

void string_tolower(char *string)
{
  for (int i = 0; string[i]; i++)
    string[i] = tolower(string[i]);
}

/**
 * @brief Function for order string from A to Z. Its basic selection sort.
 *
 * @param string
 */
void string_order(char *string)
{
  int string_len = strlen(string);
  for (int i = 0; i < string_len; i++)
  {
    int min = i;

    for (int j = i + 1; j < string_len; j++)
    {
      if (string[j] < string[min])
        min = j;
    }
    if (min != i)
    {
      int temp = string[i];
      string[i] = string[min];
      string[min] = temp;
    }
  }
}

/**
 * @brief Remove spaces, tabs, newlines etc. from start and end of string.
 *
 * @param str
 */
void string_trim(char *str, int from_end)
{
  int start = 0;
  int end = strlen(str) - 1;

  while ((isspace(str[start]) && str[start] != '\n'))
    start++;

  if (from_end)
    while ((isspace(str[end]) && end > start))
      end--;

  if (start > 0 || (unsigned int)end < strlen(str) - 1)
  {
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
  }
}

/**
 * @brief Find recomandations for one line. If input is similar to line, increase found, found following character and save last address.
 *
 * @param recomandation
 * @param input
 * @param line
 */
void find_recomandations(Recomandation *recomandation, char *input, char *line)
{
  if (string_compare(input, line))
  {
    strcpy(recomandation->last_found[recomandation->found], line); // Save last line. If we will found one, we will print it
    recomandation->found++;                                        // If we found similar line, increase found

    // Add character to recomandation if it's not in recomand
    if (!strrchr(recomandation->recomand, toupper(line[strlen(input)])))
    {
      recomandation->recomand[strlen(recomandation->recomand)] = toupper(line[strlen(input)]);
    }
  }
}

/**
 * @brief Join all user arguments into one string
 *
 * @param outputString
 * @param argc number of arguments
 * @param argv arguments
 */
void arg_join(char *outputString, int argc, char **argv)
{
  // Join user arguments
  for (int i = 1; i < argc; i++)
  {
    strcat(outputString, argv[i]);
    if (i < argc - 1)
      strcat(outputString, " ");
  }
}

/**
 * @brief Check if string is similar to input string
 *
 * @param input string that should be similar
 * @param str string to compare
 * @return char - 1 if similar, 0 if not
 */
char string_compare(char *input, char *str)
{
  for (unsigned int i = 0; i < strlen(input); i++)
  {
    if (tolower(input[i]) != tolower(str[i]))
    {
      return 0;
    }
  }

  return 1;
}

/**
 * @brief Remove character from string. Basicly it will shift all characters after index
 *
 * @param str
 * @param index
 */
void char_remove(char *str, unsigned int index)
{
  if (index < strlen(str))
  {
    for (int i = index; str[i] != '\0'; i++)
    {
      str[i] = str[i + 1];
    }
  }
}

/**
 * @brief Remove diacritics from string. String will be modified in place.
 *
 * @param str
 */
void string_remove_diacritics(char *str)
{
  char diacritics[] = "áÁčČďĎéÉěĚíÍňŇóÓřŘšŠťŤúÚůŮýÝžŽ";
  char withoutDiacritics[] = "a A c C d D e E e E i I n N o O r R s S t T u U u U y Y z Z ";

  for (unsigned int i = 0; str[i]; i++)
  {
    for (int j = 0; diacritics[j]; j++)
    {
      if (str[i] == diacritics[j] && str[i + 1] == diacritics[j + 1])
      {
        str[i] = withoutDiacritics[j];
        char_remove(str, i + 1);
        break;
      }
    }
  }
}