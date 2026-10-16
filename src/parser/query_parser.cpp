#include "query_parser.hpp"

#include <sstream>

CommandType QueryParser::stringToCommand(const std::string& cmd) {
    // --- Массив ---
    if (cmd == "MPUSH") return CommandType::MPUSH;
    if (cmd == "MDEL") return CommandType::MDEL;
    if (cmd == "MGET") return CommandType::MGET;
    if (cmd == "MSET") return CommandType::MSET;
    if (cmd == "MSIZE") return CommandType::MSIZE;
    if (cmd == "MCLEAR") return CommandType::MCLEAR;

    // --- Односвязный список ---
    if (cmd == "FPUSHHEAD") return CommandType::FPUSHHEAD;
    if (cmd == "FPUSHTAIL") return CommandType::FPUSHTAIL;
    if (cmd == "FADD") return CommandType::FADD;
    if (cmd == "FADDBEFORE") return CommandType::FADDBEFORE;
    if (cmd == "FDELHEAD") return CommandType::FDELHEAD;
    if (cmd == "FDELTAIL") return CommandType::FDELTAIL;
    if (cmd == "FDEL") return CommandType::FDEL;
    if (cmd == "FDELBEFORE") return CommandType::FDELBEFORE;
    if (cmd == "FFIND") return CommandType::FFIND;
    if (cmd == "FVALDEL") return CommandType::FVALDEL;
    if (cmd == "FGET") return CommandType::FGET;
    if (cmd == "FSIZE") return CommandType::FSIZE;
    if (cmd == "FCLEAR") return CommandType::FCLEAR;

    // --- ДВУСВЯЗНЫЙ СПИСОК ---
    if (cmd == "LPUSHHEAD") return CommandType::LPUSHHEAD;
    if (cmd == "LPUSHTAIL") return CommandType::LPUSHTAIL;
    if (cmd == "LADD") return CommandType::LADD;
    if (cmd == "LADDBEFORE") return CommandType::LADDBEFORE;
    if (cmd == "LDELHEAD") return CommandType::LDELHEAD;
    if (cmd == "LDELTAIL") return CommandType::LDELTAIL;
    if (cmd == "LDEL") return CommandType::LDEL;
    if (cmd == "LDELBEFORE") return CommandType::LDELBEFORE;
    if (cmd == "LFIND") return CommandType::LFIND;
    if (cmd == "LVALDEL") return CommandType::LVALDEL;
    if (cmd == "LGET") return CommandType::LGET;
    if (cmd == "LSIZE") return CommandType::LSIZE;
    if (cmd == "LCLEAR") return CommandType::LCLEAR;

    // --- СТЕК ---
    if (cmd == "SPUSH") return CommandType::SPUSH;
    if (cmd == "SPOP") return CommandType::SPOP;
    if (cmd == "SSIZE") return CommandType::SSIZE;
    if (cmd == "SCLEAR") return CommandType::SCLEAR;

    // --- ОЧЕРЕДЬ ---
    if (cmd == "QPUSH") return CommandType::QPUSH;
    if (cmd == "QPOP") return CommandType::QPOP;
    if (cmd == "QSIZE") return CommandType::QSIZE;
    if (cmd == "QCLEAR") return CommandType::QCLEAR;

    // --- ДЕРЕВО ---
    if (cmd == "TINSERT") return CommandType::TINSERT;
    if (cmd == "TFIND") return CommandType::TFIND;
    if (cmd == "TCHECK") return CommandType::TCHECK;
    if (cmd == "TCLEAR") return CommandType::TCLEAR;

    if (cmd == "HELP" || cmd == "--help" || cmd == "-h")
        return CommandType::HELP;
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
    if (q.command_type == CommandType::PRINT ||
        q.command_type == CommandType::HELP) {
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
