#pragma once
#include <string>

enum class CommandType {
    // 1. МАССИВ (M)
    MPUSH,
    MDEL,
    MGET,
    MSET,
    MSIZE,
    MCLEAR,

    // 2. ОДНОСВЯЗНЫЙ СПИСОК (F)
    FPUSHHEAD,
    FPUSHTAIL,
    FADD,
    FADDBEFORE,
    FDELHEAD,
    FDELTAIL,
    FDEL,
    FDELBEFORE,
    FFIND,
    FVALDEL,
    FGET,
    FSIZE,
    FCLEAR,

    // 3. ДВУСВЯЗНЫЙ СПИСОК (L)
    LPUSHHEAD,
    LPUSHTAIL,
    LADD,
    LADDBEFORE,
    LDELHEAD,
    LDELTAIL,
    LDEL,
    LDELBEFORE,
    LFIND,
    LVALDEL,
    LGET,
    LSIZE,
    LCLEAR,

    // 4. СТЕК (S)
    SPUSH,
    SPOP,
    SSIZE,
    SCLEAR,

    // 5. ОЧЕРЕДЬ (Q)
    QPUSH,
    QPOP,
    QSIZE,
    QCLEAR,

    // 6. ПОЛНОЕ БИНАРНОЕ ДЕРЕВО (T)
    TINSERT,
    TFIND,
    TCHECK,
    TCLEAR,

    // Вспомогательные методы
    PRINT,
    HELP,
    UNKNOWN
};

struct Query {
    CommandType command_type = CommandType::UNKNOWN;
    std::string command_raw;
    std::string db_name;
    std::string target_value; // целевое значение для операций списка ДО / ПОСЛЕ
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