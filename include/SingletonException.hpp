
#pragma once

#include <stdexcept>

namespace time_sync {
    /**
     * @brief Streets Singleton Exception thrown when user attempts to retreive
     * singleton before calling create to initialize it.
     */ 
    class SingletonException : public std::runtime_error{
        public:
            /**
             * @brief Destructor.
             */ 
            ~SingletonException() override;
            /**
             * @brief Constructor. 
             * @param msg String exception message.
             */  
            explicit SingletonException(const std::string &msg );
    };
}
