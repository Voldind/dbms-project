#include "query_parser.hpp"
#include <sstream>

CommandType QueryParser::stringToCommand(const std::string& cmd) {
    if (cmd == "MPUSH") return CommandType::MPUSH;
    if (cmd == "MDEL")  return CommandType::MDEL;
    if (cmd == "MGET")  return CommandType::MGET;
    
    if (cmd == "FPUSH") return CommandType::FPUSH;
    if (cmd == "FDEL")  return CommandType::FDEL;
    if (cmd == "FGET")  return CommandType::FGET;
    
    if (cmd == "LPUSH") return CommandType::LPUSH;
    if (cmd == "LDEL")  return CommandType::LDEL;
    if (cmd == "LGET")  return CommandType::LGET;
    
    if (cmd == "QPUSH") return CommandType::QPUSH;
    if (cmd == "QPOP")  return CommandType::QPOP;
    
    if (cmd == "SPUSH") return CommandType::SPUSH;
    if (cmd == "SPOP")  return CommandType::SPOP;
    
    if (cmd == "TINSERT") return CommandType::TINSERT;
    if (cmd == "TDEL")    return CommandType::TDEL;
    if (cmd == "TGET")    return CommandType::TGET;
    if (cmd == "ISMEMBER") return CommandType::ISMEMBER;
    
    if (cmd == "PRINT") return CommandType::PRINT;
    
    return CommandType::UNKNOWN;
}

Query QueryParser::parse(const std::string& query_str) {
    Query q;
    std::stringstream ss(query_str);
    
    std::string cmd_raw;
    ss >> cmd_raw;
    q.command_raw = cmd_raw;
    q.command_type = stringToCommand(cmd_raw);
    
    if (q.command_type == CommandType::PRINT) {
        ss >> q.db_name;
        return q;
    }

    ss >> q.db_name;
    std::string arg;
    ss >> arg;

    if (!arg.empty()) {
        try {
            size_t pos;
            int val = std::stoi(arg, &pos);
            if (pos == arg.length() && val >= 0) {
                q.index = static_cast<size_t>(val);
                q.has_index = true;
                std::string extra_val;
                if (ss >> extra_val) q.value = extra_val;
            } else {
                q.value = arg;
            }
        } catch (...) {
            q.value = arg;
            std::string remainder;
            if (std::getline(ss, remainder)) q.value += remainder;
        }
    }
    return q;
}
