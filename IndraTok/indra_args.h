#include <stdbool.h>
#include <stdlib.h>  // IWYU pragma: export. // size_t

/**
 * @brief Enumeration for argument types.
 *
 * This enumeration defines the types of arguments that can be parsed.
 */
typedef enum indraArgTypes {
    BOOL,   /**< Boolean argument type */
    STRING, /**< String argument type */
    INT     /**< Integer argument type */
} IndraArgTypes;

/**
 * @brief Structure to define a command-line argument.
 *
 * This structure holds the information needed to define and parse a command-line argument.
 */
typedef struct {
    char *shortName;      /**< Short name of the argument (e.g., "-f") */
    char *name;           /**< Long name of the argument (e.g., "--file") */
    IndraArgTypes type;   /**< Type of the argument (BOOL, STRING, INT) */
    void *pValue;         /**< Pointer to the variable where the parsed value will be stored */
    size_t pValueSize;    /**< Size of the value (used for strings) */
    char *help;           /**< Help description of the argument */
} IndraArg;

bool indraArgParse(int argc, char *argv[], int argDefC, IndraArg argDefs[]);
