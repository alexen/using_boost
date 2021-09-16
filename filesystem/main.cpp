/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>


struct TmpPath
{
     explicit TmpPath( const boost::filesystem::path& p = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path( "pfr-service-tmp-%%%%%%%%%%%%%%%%" ) ) : path{ p }
     {
          std::cout << "Created temp path " << path << ".\n";
     }
     TmpPath( const TmpPath& ) = delete;
     TmpPath( TmpPath&& ) = default;
     ~TmpPath()
     {
          std::cout << "Let's remove file " << path << "!\n";
          boost::system::error_code err;
          boost::filesystem::remove( path, err );
     }
     const boost::filesystem::path path;
     operator boost::filesystem::path() const
     {
          return path;
     }
};


std::ostream& operator<<( std::ostream& os, const TmpPath& tp )
{
     return os << tp.path;
}


TmpPath getTmpPath()
{
     TmpPath tmp;
     boost::filesystem::ofstream{ tmp } << "Important data...";
     return tmp;
}


void doSmth( TmpPath&& tp )
{
     std::cout << "Do something with " << tp << '\n';
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          std::cout << "Start!\n";
          auto tmp = getTmpPath();
          doSmth( std::move( tmp ) );
          std::cout << "Done.\n";
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
