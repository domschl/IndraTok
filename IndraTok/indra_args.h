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
 * @param argDefs The array of predefined argument definitions.
 * @return true if the arguments were successfully parsed, false otherwise.
 */
bool indraArgParse(int argc, char *argv[], int argDefC, IndraArg argDefs[]);
