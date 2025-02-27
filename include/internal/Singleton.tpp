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
namespace time_sync {  
    // Implementation
  
      template <typename T,typename... Args>   
      T& Singleton<T,Args...>::getSingleton() {
          if ( !instance ) {
            throw SingletonException("Singleton has not been created");
          } 
          char strAddress[20];
          snprintf(strAddress,sizeof(strAddress) ,"%p",std::addressof(instance) );
          SPDLOG_TRACE("Singleton class : {0}.", typeid(instance.get()).name() );
          SPDLOG_TRACE("Singleton address: {0}", strAddress);
          return *instance;
      };
  
      template <typename T, typename... Args>
      T& Singleton<T,Args...>::create(Args...args ){
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
      }
          
  
  }
