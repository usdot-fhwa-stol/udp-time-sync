#include "SingletonException.hpp"

namespace time_sync {
    // Constructor
    SingletonException::SingletonException(const std::string &msg ): std::runtime_error(msg){};
    // Destructor
    SingletonException::~SingletonException() = default;
}
