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
