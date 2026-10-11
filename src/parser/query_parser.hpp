#pragma once
#include <string>

// все возможные команды
enum class CommandType {
    MPUSH, MDEL, MGET,
    FPUSH, FDEL, FGET,
    LPUSH, LDEL, LGET,
    QPUSH, QPOP,
    SPUSH, SPOP,
    TINSERT, TDEL, TGET, ISMEMBER,
    PRINT,
    UNKNOWN
};

struct Query {
    CommandType command_type = CommandType::UNKNOWN;
    std::string command_raw;
    std::string db_name;
    std::string value;
    size_t index = 0;
    bool has_index = false;
};

class QueryParser {
public:
    static Query parse(const std::string& query_str);
private:
    static CommandType stringToCommand(const std::string& cmd);
};
