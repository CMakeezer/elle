#ifndef ELLE_RADIX_TRACE_HH
# define ELLE_RADIX_TRACE_HH

# include <elle/system/Platform.hh>

# if defined(INFINIT_LINUX)

#  include <elle/types.hh>
#  include <elle/io/Dumpable.hh>
#  include <elle/idiom/Close.hh>
#   include <cxxabi.h>
#   include <execinfo.h>
#  include <elle/idiom/Open.hh>

namespace elle
{
  namespace radix
  {

    ///
    /// this class represents a trace i.e a sequence of function calls
    /// performed within a stack.
    ///
    /// note that the traces are stored in files in order to avoid
    /// maintaining a data structure which would require allocating memory.
    ///
    /// note that this class exports fileable-like method but cannot
    /// implement the interface as this would generate too much
    /// inter-dependencies.
    ///
    /// \todo XXX for now the tracing mechanism can only work on Linux
    ///           because it makes use of /tmp/XXXXXX
    ///
    class Trace:
      public io::Dumpable
    {
    public:
      //
      // constants
      //
      static const Natural32            Capacity = 30;

      static Character                  Location[32];
      static Natural32                  Length;

      //
      // constructors & destructors
      //
      Trace(Void*);

      //
      // methods
      //
      Status            Generate();

      //
      // interfaces
      //

      // dumpable
      Status            Dump(const Natural32 = 0) const;

      //
      // attributes
      //
      Void*             address;

      Void*             frames[Capacity];
      Natural32         size;

      //
      // static methods
      //
      static Status     Store(Void*);
      static Status     Erase(Void*);
      static Status     Exist(Void*);

      static Status     Initialize();
      static Status     Clean();

      static Status     Show(const Natural32 = 0);
    };

  }
}

# endif

#endif
