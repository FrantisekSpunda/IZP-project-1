/**
 * @file main.c
 * @author Your name
 * @date 2023-10-10
 * @brief Description of your project
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

void removeDiacritics(char *str)
{
  for (int i = 0; i < 10; i++)
  {
    str[i] = tolower(str[i]);
  }
}

void stringLowercase(char *string);

int main(int argc, char *argv[])
{
  char user_input[100] = "";
  for (int i = 1; i < argc; i++)
  {
    strcat(user_input, argv[i]);
    if (i < argc - 1)
      strcat(user_input, " ");
  }

  char address[100];          // Variable for one line of file
  char recomand[128] = "";    // Variable for recommended characters
  int found = 0;              // Number of recomandations we found
  char lastAddress[100] = ""; // Keep last address for printing single address

  while (fgets(address, sizeof(address), stdin) != NULL) // Get one line of file
  {
    char isSimilar = 1; // If word is similar, we want to add character to recomand

    // Check if user input is similar to address
    for (unsigned int i = 0; i < strlen(user_input); i++)
    {
      if (tolower(user_input[i]) != tolower(address[i]))
      {
        isSimilar = 0;
        break;
      }
    }

    if (isSimilar)
    {
      // Save last address. If we will found one, we will print it
      strcpy(lastAddress, address);
      found++;

      // Add character to recomand if it's not in recomand
      if (!strrchr(recomand, toupper(address[strlen(user_input)])))
        recomand[strlen(recomand)] = toupper(address[strlen(user_input)]);
    }
  }

  if (found == 1)
  {
    printf("\033[0;32m Found: \033[0m%s\n", lastAddress);
  }
  else if (found > 1)
  {
    printf("\033[0;34m Enable: \033[0m%s\n", recomand);
  }
  else
  {
    printf("Not found\n");
  }

  return 0;
}

void stringLowercase(char *string)
{
  for (unsigned int i = 0; i < strlen(string); i++)
  {
    string[i] = tolower(string[i]);
  }
}