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
#include "main.h"

#define SIZE_OF_ARG 2

int stringLength(char *str)
{
  int length = 0;
  while (str[length] != '\0')
  {
    length++;
  }

  return length;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("test: %s --\n", argv[0]);
    return 1;
  }

  char *input = argv[1];
  printf("User input is %s and  lenght is %i\n", input, stringLength(input));

  char town[100];          // One line of input file
  char recomand[128] = ""; // Variable where will be first chars of towns
  int foundedTowns = 0;    // If we can't find word, we have to print "Not found"

  while (fgets(town, sizeof(town), stdin) != NULL)
  {
    char isSimilar = 1; // If word is similar, we want to add char to recomand

    // For each letter in user input. If
    for (int i = 0; i < stringLength(input); i++)
    {
      if (input[i] != town[i])
      {
        isSimilar = 0;
        break;
      }

      // printf("%c %c\n", input[i], town[i]); // Tisk znaků
    }
    if (isSimilar)
    {
      printf("%s", town);
      printf(" index %c \n", town[stringLength(input)]);
      foundedTowns++;
      recomand[stringLength(recomand)] = town[stringLength(input)];
    }
  }

  if (foundedTowns)
  {
    printf("Found %i\n", foundedTowns);
    printf("Recomand: %s\n", recomand);
  }
  else
  {
    printf("Not found\n");
  }

  return 0;
}