#ifndef LUNE_IDENTITY_HH
# define LUNE_IDENTITY_HH

# include <elle/concept/Fileable.hh>
# include <elle/concept/Uniquable.hh>
# include <elle/fwd.hh>
# include <elle/radix/Object.hh>

# include <cryptography/fwd.hh>
# include <cryptography/KeyPair.hh>
# include <cryptography/Signature.hh>
// XXX[temporary: for cryptography]
using namespace infinit;

# include <lune/fwd.hh>

# include <elle/idiom/Open.hh>

namespace lune
{

  ///
  /// this class represents an identity issued by the Infinit authority
  /// which represents a user.
  ///
  /// note that the name attribute is supposed to be unique as it plays the
  /// role of identifier.
  ///
  class Identity:
    public elle::radix::Object,
    public elle::concept::MakeFileable<Identity>,
    public elle::concept::MakeUniquable<Identity>
  {
  public:
    //
    // enumerations
    //
    enum Mode
      {
        ModeEncrypted,
        ModeUnencrypted
      };

    //
    // constructors & destructors
    //
    Identity();
    ~Identity();

    //
    // methods
    //
    elle::Status        Create(elle::String const&,
                               const elle::String&,
                               cryptography::KeyPair const&);

    elle::Status        Encrypt(const elle::String&);
    elle::Status        Decrypt(const elle::String&);

    elle::Status        Clear();

    elle::Status        Seal(elle::Authority const&);
    elle::Status        Validate(elle::Authority const&) const;

  private:
    /// XXX
    static
    elle::io::Path
    _path(elle::String const& user);

    //
    // interfaces
    //
  public:
    // object
    declare(Identity);

    // dumpable
    elle::Status        Dump(const elle::Natural32 = 0) const;

    // fileable
    ELLE_CONCEPT_FILEABLE_METHODS();

    void
    load(elle::String const& user_id);

    void
    store() const;

    static
    void
    erase(elle::String const& user_id);

    static
    elle::Boolean
    exists(elle::String const& user_id);

    //
    // attributes
    //
  private:
    elle::String                _id;
  public: // XXX
    elle::String                name;
    cryptography::KeyPair       pair;
    cryptography::Signature     signature;

    cryptography::Cipher*       cipher;

    ELLE_SERIALIZE_FRIEND_FOR(Identity);
  };

}

#include <lune/Identity.hxx>

#endif
