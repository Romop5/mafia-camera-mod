#ifndef MAFIA_SCRIPT_H
#define MAFIA_SCRIPT_H
#include <vector>
enum ArgumentType
{
    ARG_INT,          // 4-bytes, storing DWORD
    ARG_STRING,       // Variadic size of structure: contains NULL-terminated string
    ARG_INT_OR_FLOAT, // Args contains two DWORDS, the first is type - either float (0xFFFFFFFF), or int (0x0), and the value word
};

using arg_type_t = std::vector<ArgumentType>;
using command_t = std::pair<const char*, arg_type_t>;

const char* getCommandName(size_t id);
arg_type_t getCommandArgumentsType(size_t id);
#endif