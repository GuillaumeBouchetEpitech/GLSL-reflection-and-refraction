

#ifndef	EXCEPTION_HPP
#define	EXCEPTION_HPP


#include	<stdexcept>
#include	<iostream>
#include	<exception>
#include	<string>


class Exception : public std::runtime_error
{
private :

  std::string	m_msg;

private :

  const char*	m_file;
  const char*	m_func;
  int		m_line;

public :

  Exception( char const * msg );
  Exception( char const * msg, const char* file, const char* func, int line );
  virtual ~Exception() throw();
  virtual const char * what() const throw();

public :

  inline const char*	GetFile() const throw()	{return (m_file);}
  inline const char*	GetFunc() const throw()	{return (m_func);}
  inline int		GetLine() const throw()	{return (m_line);}

};

#define	Exception_Ex(msg)	Exception( msg, __FILE__, __func__, __LINE__ )


#endif

