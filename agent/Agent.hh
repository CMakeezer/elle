//
// ---------- header ----------------------------------------------------------
//
// project       agent
//
// license       infinit
//
// file          /home/mycure/infinit/agent/Agent.hh
//
// created       julien quintard   [thu mar  4 17:20:28 2010]
// updated       julien quintard   [thu mar 25 20:46:47 2010]
//

#ifndef AGENT_AGENT_HH
#define AGENT_AGENT_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <etoile/Manifest.hh>
#include <agent/Manifest.hh>

#include <elle/idiom/Close.hh>
# include <sys/stat.h>
#include <elle/idiom/Open.hh>

namespace agent
{

//
// ---------- classes ---------------------------------------------------------
//

  ///
  /// this class implements the agent by providing an entry point
  /// to user-specific cryptographic functionalities.
  ///
  class Agent
  {
  public:
    //
    // constants
    //
    static const String		Path;
    static const String		Line;

    //
    // static methods
    //
    static Status	Initialize(const String&);
    static Status	Clean();

    //
    // callbacks
    //
    static Status	Start();

    static Status	Decrypt(const Code&);
    static Status	Sign(const Plain&);

    static Status	Error(const Report&);
    static Status	Error(const String&);

    //
    // static attributes
    //
    static KeyPair	Pair;
    static Door		Channel;
    static String	Phrase;

    static Timer	Event;
  };

}

#endif
