#if !defined ELLE_WINDOWS and !defined ELLE_ANDROID
#  ifdef __has_include
#    if __has_include(<execinfo.h>)
#      include <execinfo.h>
#    else
#      define NO_EXECINFO
#    endif
#  else
#    include <execinfo.h>
#  endif
#endif

#include <elle/log.hh>

namespace elle
{
  inline
  Backtrace
  Backtrace::current(unsigned skip)
  {
    ELLE_LOG_COMPONENT("elle.Backtrace");
#if defined ELLE_WINDOWS
    return {};
#elif defined ELLE_ANDROID || defined NO_EXECINFO
    // FIXME: implement with
    // https://android.googlesource.com/platform/frameworks/native/+/jb-dev/include/utils/CallStack.h
    return {};
#else
    auto res = Backtrace{};
    res._frame_count = ::backtrace(res._callstack.data(), res._callstack.size());
    ELLE_DEBUG("backtrace returned %s frames", res._frame_count);
    res._skip = skip;
    return res;
#endif
  }
}
