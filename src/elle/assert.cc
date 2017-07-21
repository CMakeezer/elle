#include <elle/assert.hh>
#include <elle/Backtrace.hh>
#include <elle/log.hh>
#include <elle/os/environ.hh>

#include <iostream>
#include <sstream>

ELLE_LOG_COMPONENT("elle.assert");

namespace elle
{
  void
  unreachable()
  {
    _abort("unreachable code reached", "", 0);
  }

  void
  _abort(std::string const& message, char const* file, int line)
  {
    if (std::uncaught_exception())
    {
      ELLE_WARN(
        "raising an assert (%s at %s:%s) with an exception already in flight",
        message, file, line);
    }
    if (elle::os::getenv("ELLE_REAL_ASSERT", false))
    {
      ELLE_ERR("%s: (%s:%s)", message.c_str(), file, line);
      std::abort();
    }
    else
      throw elle::AssertError(message.c_str(), file, line);
  }

  AssertError::AssertError(char const* condition,
                           char const* file, int line) noexcept
  {
    try
    {
      this->_what = elle::sprintf("assertion '%s' failed at %s:%s",
                                  condition, file, line);
      ELLE_ERR("%s", this->_what)
        ELLE_ERR("%s", Backtrace::current());
    }
    catch (...)
    {
      ELLE_WARN("could not build the error string for %s at %s:%s",
                condition, file, line);
    }
  }

  const char*
  AssertError::what() const noexcept
  {
    if (_what.empty())
      return "AssertError (could not build the error string)";
    else
      return _what.c_str();
  }
}
