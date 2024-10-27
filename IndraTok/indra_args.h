#include <stdbool.h>
#include <stdlib.h>

typedef enum indraArgTypes {BOOL, STRING, INT} IndraArgTypes;

typedef struct {
  char *shortName;
  char *name;
  IndraArgTypes type;
  void *pValue;
  size_t pValueSize;
  char *help;
} IndraArg;

bool indraArgParse(int argc, char *argv[], int argDefC, IndraArg argDefs[]);

