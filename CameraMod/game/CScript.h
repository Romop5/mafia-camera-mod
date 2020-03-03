#ifndef MAFIA_SCRIPT_H
#define MAFIA_SCRIPT_H
#include <vector>
enum ArgumentType
{
    ARG_INT,          // 4-bytes, storing DWORD
    ARG_STRING,       // Variadic size of structure: contains NULL-terminated string
    ARG_FLT_ID_OR_FLOAT, // Args contains two DWORDS, the first is type - either raw float value (0xFFFFFFFF), or int (0x0), and flt[id] - id of variable
    ARG_LET           // Special case, contains INT, flt constant / flt variable, flt constant / flt variable, operator (int)
                      // let flt[xx] = yy op zz
};

using arg_type_t = std::vector<ArgumentType>;
using command_t = std::pair<const char*, arg_type_t>;

const char* getCommandName(size_t id);
arg_type_t getCommandArgumentsType(size_t id);

size_t convertMafiaScriptConstantToID(const std::string constant);
#endif