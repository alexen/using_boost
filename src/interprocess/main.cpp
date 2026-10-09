/// @file main.cpp
/// @brief Демонстрация Boost.Interprocess: разделяемая память с синхронизацией
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>
#include <cstring>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/thread/thread.hpp>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/exceptions.hpp>


/// Имя разделяемой памяти
static constexpr auto shmemName = "using_boost_demo_shmem";
/// Имя мьютекса для синхронизации
static constexpr auto mutexName = "using_boost_demo_mutex";
/// Размер области данных
static constexpr auto shmemSize = 256;


/// Очистка остатков предыдущих запусков (если процесс упал без clean up)
void cleanup()
{
     boost::interprocess::shared_memory_object::remove( shmemName );
     boost::interprocess::named_mutex::remove( mutexName );
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          // Пытаемся открыть существующую разделяемую память — если её нет, создаём
          bool isFirst = false;
          boost::interprocess::shared_memory_object shmem{
               boost::interprocess::open_or_create,
               shmemName,
               boost::interprocess::read_write
          };

          // Задаём размер только при первом создании
          {
               boost::interprocess::offset_t size = 0;
               if( !shmem.get_size( size ) || size == 0 )
               {
                    shmem.truncate( shmemSize );
                    isFirst = true;
                    std::cout << "First run: created shared memory '" << shmemName << "' (" << shmemSize << " bytes)\n";
               }
               else
               {
                    std::cout << "Subsequent run: opened existing shared memory '" << shmemName << "' (" << size << " bytes)\n";
               }
          }

          // Отображаем в адресное пространство
          boost::interprocess::mapped_region region{ shmem, boost::interprocess::read_write };
          void*       addr = region.get_address();
          std::size_t size = region.get_size();

          // Открываем (или создаём) именованный мьютекс
          boost::interprocess::named_mutex mutex{
               boost::interprocess::open_or_create,
               mutexName
          };

          if( isFirst )
          {
               // Первый запуск: пишем приветствие и ждём второй процесс
               {
                    boost::interprocess::scoped_lock< boost::interprocess::named_mutex > lock{ mutex };
                    const char* msg = "Hello from first process!";
                    std::strncpy( static_cast< char* >( addr ), msg, size - 1 );
                    static_cast< char* >( addr )[ size - 1 ] = '\0';
                    std::cout << "Written: \"" << msg << "\"\n";
               }

               std::cout << "Waiting up to 5 seconds for a second process to respond...\n";
               boost::this_thread::sleep( boost::posix_time::seconds{ 5 } );

               // Читаем, что ответил второй (или своё, если второй не пришёл)
               {
                    boost::interprocess::scoped_lock< boost::interprocess::named_mutex > lock{ mutex };
                    std::cout << "Shared memory contains: \"" << static_cast< char* >( addr ) << "\"\n";
               }
          }
          else
          {
               // Второй запуск: читаем, что написал первый, отвечаем
               {
                    boost::interprocess::scoped_lock< boost::interprocess::named_mutex > lock{ mutex };
                    std::cout << "Read: \"" << static_cast< char* >( addr ) << "\"\n";
               }

               {
                    boost::interprocess::scoped_lock< boost::interprocess::named_mutex > lock{ mutex };
                    const char* msg = "Hello back from second process!";
                    std::strncpy( static_cast< char* >( addr ), msg, size - 1 );
                    static_cast< char* >( addr )[ size - 1 ] = '\0';
                    std::cout << "Written: \"" << msg << "\"\n";
               }
          }

          // Clean up
          cleanup();
          std::cout << (isFirst ? "First" : "Second") << " process done.\n";
     }
     catch( const boost::interprocess::interprocess_exception& e )
     {
          std::cerr << "interprocess exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}