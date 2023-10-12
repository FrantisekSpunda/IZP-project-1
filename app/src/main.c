/**
 * @file main.c
 * @author Frantisek Spunda (xspundaf00)
 * @date 2023-12-10
 * @brief Description of your project
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 100
#define MAX_RECOMAND_LENGTH 128 // max length of recomandation comes from ASCII table that has 128 characters

typedef struct
{
  int found;
  char recomand[MAX_RECOMAND_LENGTH];
  char last_found[MAX_LINE_LENGTH];
} Recomandation;

void diacriticsrm(char *str);
void charrm(char *str, unsigned int index);
char strcompare(char *input, char *str);
void argjoin(char *outputString, int argc, char *argv[]);
void strtrim(char *str);
void find_recomandations(Recomandation *recomandation, char *input, char *address);

int main(int argc, char **argv)
{
  time_t start, end;
  double cpu_time_used;
  start = clock();

  char user_input[MAX_LINE_LENGTH] = "";
  argjoin(user_input, argc, argv);
  strtrim(user_input);
  diacriticsrm(user_input);

  char address[MAX_LINE_LENGTH]; // Variable for one line of file

  Recomandation recomandation = {
      .found = 0,
      .last_found = "",
      .recomand = "",
  };

  while (fgets(address, sizeof(address), stdin) != NULL) // Get one line of file
  {
    strtrim(address);
    diacriticsrm(address); // We have to remove diacritics since we are removing diacritics from user input
    find_recomandations(&recomandation, user_input, address);
  }

  end = clock();
  if (recomandation.found > 1)
    printf("\033[0;34m Enable: \033[0m%s\n", recomandation.recomand);
  else if (recomandation.found == 1)
    printf("\033[0;32m Found: \033[0m%s\n", recomandation.last_found);
  else
    printf("Not found\n");

  cpu_time_used = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

  printf("Time: %f ms\n", cpu_time_used);

  return 0;
}

/**
 * @brief Remove spaces, tabs, newlines etc. from start and end of string.
 *
 * @param str string to trim
 */
void strtrim(char *str)
{
  int start = 0;
  int end = strlen(str) - 1;

  while ((isspace(str[start]) && str[start] != '\n'))
    start++;

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
 * @param recomandation struct of all returning data { found, recomand, last_found }
 * @param input
 * @param line
 */
void find_recomandations(Recomandation *recomandation, char *input, char *line)
{
  if (strcompare(input, line))
  {
    recomandation->found++; // If we found similar line, increase found

    strcpy(recomandation->last_found, line); // Save last line. If we will found one, we will print it

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
 * @param outputString string where will be joined all arguments
 * @param argc number of arguments
 * @param argv arguments
 */
void argjoin(char *outputString, int argc, char **argv)
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
char strcompare(char *input, char *str)
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
 * @param str string to remove character from
 * @param index index of character to remove
 */
void charrm(char *str, unsigned int index)
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
 * @param str string to remove diacritics from
 */
void diacriticsrm(char *str)
{
  char diacritics[] = "áÁčČďĎéÉěĚíÍňŇóÓřŘšŠťŤúÚůŮýÝžŽ";
  char withoutDiacritics[] = "a A c C d D e E e E i I n N o O r R s S t T u U u U y Y z Z ";

  // Projdeme všechny znaky v řetězci
  for (unsigned int i = 0; i < strlen(str); i++)
  {
    // Pokud je znak diakritický, nahradíme ho odpovídajícím ne-diakritickým znakem
    for (int j = 0; diacritics[j]; j++)
    {
      if (str[i] == diacritics[j] && str[i + 1] == diacritics[j + 1])
      {
        str[i] = withoutDiacritics[j];
        charrm(str, i + 1);
        break; // Pokud byl znak nalezen a nahrazen, ukončíme smyčku
      }
    }
  }
}