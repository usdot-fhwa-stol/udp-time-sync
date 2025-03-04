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
            static T& getSingleton()  {
                if ( !instance ) {
                    throw SingletonException("Singleton has not been created");
                } 
                char strAddress[20];
                snprintf(strAddress,sizeof(strAddress) ,"%p",std::addressof(instance) );
                SPDLOG_TRACE("Singleton class : {0}.", typeid(instance.get()).name() );
                SPDLOG_TRACE("Singleton address: {0}", strAddress);
                return *instance;
            };
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
            static T& create(Args...args ) {
                if (instance != nullptr){
                    SPDLOG_WARN("Recreating Singleton of type {0}!", typeid(instance.get()).name());
                    // Reset unique ptr
                    
                    instance.reset( new T(args...) );
                  
                  }
                  else {
                    SPDLOG_INFO("Initializing Singleton of type {0}!", typeid(instance.get()).name());
                    instance = std::unique_ptr<T>( new T(args...) );
                  }
                  
                  return *instance;
            };


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

