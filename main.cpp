

#include "Program.hpp"

#include "Exception.hpp"

#include <iostream>


int     main()
{

  try
    {
      Program	exec;
      exec.Run();
    }

  catch (const Exception &e)
    {
      std::cerr << "Exception : " << std::endl
		<< e.what() << std::endl;

      std::cerr << "[file : " << (e.GetFile() ? e.GetFile() : "NULL") << "]" << std::endl
		<< "[func : " << (e.GetFunc() ? e.GetFunc() : "NULL") << "]" << std::endl
		<< "[line : " << e.GetLine() << "]" << std::endl;
    }

  catch (const std::bad_alloc &)
    {
      std::cerr << "Empty memory exception" << std::endl;
    }

  catch (...)
    {
      std::cerr << "Unknown exception" << std::endl;
    }

  return (0);
}

