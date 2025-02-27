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
#include <spdlog/spdlog.h>
#include <typeinfo>
#include "SingletonException.hpp"


namespace time_sync {
    /**
     * CARMA-Streets singleton templated class meant for extending
     * to allow for singleton scoped objects in CARMA-Streets services
     * and static access to the objects
     * 
     * @author Paul Bourelly
     * @date 3/14/2022
     */
    template <typename T, typename... Args>
    class Singleton
    {
        public:
            /**
             * Method for static singleton instance retrieval
             * @returns T singleton instance
             */ 
            static T& getSingleton() ;
            // Remove copy constructor 
            Singleton(const Singleton &) = delete;
            // Remove move constructor
            Singleton(Singleton&& ) = delete;
            // Remove copy assignment operator
            Singleton& operator=(const Singleton &) = delete;
            // Remove move assignment operator
            Singleton& operator=(const Singleton &&) = delete;
            /**
             * @brief Method to intialize Singleton with constructor params
             * ...Args of templated class.
             * 
             * @param ...args constructor parameters of templated class.
             * @return returns reference to singleton instance.
             */
            static T& create(Args...args );


        protected:
            /**
             * Protected constructor
             */ 
            Singleton() = default;
            /**
             * Protected destructor
             */        
            ~Singleton() = default;
              
            static std::unique_ptr<T> instance;        
    };

    template <typename T, typename ...Args>
    std::unique_ptr<T> Singleton<T, Args...>::instance = nullptr;
         
};

#include "internal/Singleton.tpp"
