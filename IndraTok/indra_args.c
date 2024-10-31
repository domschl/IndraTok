#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "indra_args.h"

/**
 * @brief Parses command-line arguments based on predefined argument definitions.
 *
 * This function processes command-line arguments provided to the program and matches them
 * against a set of predefined argument definitions. It supports string, boolean, and integer
 * argument types. If the `-h` or `--help` flag is provided, it prints the usage information.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @param argDefC The number of predefined argument definitions.
 * @param argDefs The array of predefined argument definitions of type `IndraArg`.
 * @return true if the arguments were successfully parsed, false otherwise.
 */
bool indraArgParse(int argc, char *argv[], int argDefC, IndraArg argDefs[]) {
  // Check for help flag and print usage information if found
  for (int i=1; i<argc; i++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      printf("Usage:\n");
      printf("    %s \n", argv[0]);
      for (int j=0; j<argDefC; j++) {
        char *tp;
        switch (argDefs[j].type) {
        case STRING:
          tp="String";
          break;
        case BOOL:
          tp="Bool";
          break;
        case INT:
          tp="Int";
          break;
        }
        printf("        %2s %10s <%s> - %s\n", argDefs[j].shortName, argDefs[j].name, tp, argDefs[j].help);
      }
      return false;
    }
  }

  // Initialize boolean arguments to false
  for (int i=0; i<argDefC; i++) {
    if (argDefs[i].type == BOOL && argDefs[i].pValue != NULL) *((bool *)(argDefs[i].pValue)) = false;
  }
  bool found = false;

  // Parse the command-line arguments
  for (int i=1; i<argc; i++) {
    for (int j=0; j<argDefC; j++) {
      if (!strcmp(argv[i], argDefs[j].shortName) || !strcmp(argv[i], argDefs[j].name)) {
        found = true;
        switch (argDefs[j].type) {
        case STRING:
          if (i>=argc-1) {
            printf("Missing argument for parameter: %s\nUse -h for help\n", argv[i]);
            return false;
          }
          strncpy(argDefs[j].pValue,argv[i+1], argDefs[j].pValueSize);
          ((char *)(argDefs[j].pValue))[argDefs[j].pValueSize-1]=0;
          i++;
          break;
        case BOOL:
          *((bool *)(argDefs[j].pValue)) = true;
          break;
        case INT:
          if (i>=argc-1) {
            printf("Missing argument for parameter: %s\nUse -h for help\n", argv[i]);
            return false;
          }
          *((int *)(argDefs[j].pValue)) = atoi(argv[i+1]);
          i++;
          break;
        }
        break;
      }
    }
    if (!found) {
      printf("Unknown parameter: %s\nUse -h for help\n", argv[i]);
      return false;
    }
  }
  return true;
}
