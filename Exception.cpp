

#include	"Exception.hpp"


Exception::Exception( char const * msg)
  : std::runtime_error::runtime_error( msg ),
    m_msg(msg),
    m_file( NULL ), m_func( NULL ), m_line( -1 )
{
}

Exception::Exception( char const * msg, const char* file, const char* func, int line )
  : std::runtime_error::runtime_error( msg ),
    m_msg( msg ),
    m_file( file ), m_func( func ), m_line( line )
{
}

Exception::~Exception() throw()
{
}

const char*		Exception::what() const throw()
{
  return (m_msg.c_str());
}

