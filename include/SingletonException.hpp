/**
 * Copyright (C) 2025 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
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
