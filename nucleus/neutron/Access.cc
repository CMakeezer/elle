//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// author        julien quintard   [wed mar 11 16:55:36 2009]
//

//
// ---------- includes --------------------------------------------------------
//

#include <nucleus/neutron/Access.hh>

#include <nucleus/proton/Address.hh>

namespace nucleus
{
  namespace neutron
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// the null access.
    ///
    const Access                        Access::Null;

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// default constructor.
    ///
    Access::Access():
      proton::ContentHashBlock(ComponentAccess)
    {
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method adds the given record to the ACL.
    ///
    elle::Status        Access::Add(Record*                     record)
    {
      ;

      // add the record in the range.
      if (this->range.Add(record) == elle::StatusError)
        escape("unable to add the record in the range");

      // set the block as dirty.
      this->_state = proton::StateDirty;

      return elle::StatusOk;
    }

    ///
    /// this method tests if the given subject exists.
    ///
    elle::Status        Access::Exist(const Subject&            subject) const
    {
      ;

      // test.
      if (this->range.Exist(subject) == false)
        return elle::StatusFalse;

      return elle::StatusTrue;
    }

    ///
    /// this method returns the record corresponding to the given subject.
    ///
    elle::Status        Access::Lookup(const Subject&           subject,
                                       Record*&                 record) const
    {
      ;

      // look in the range.
      if (this->range.Lookup(subject, record) == elle::StatusError)
        escape("unable to retrieve the record");

      return elle::StatusOk;
    }

    ///
    /// this method returns the index location of the given subject.
    ///
    elle::Status        Access::Lookup(const Subject&           subject,
                                       Index&                   index) const
    {
      Range<Record>::Scoutor    scoutor;

      ;

      // go through the range.
      for (scoutor = this->range.container.begin(), index = 0;
           scoutor != this->range.container.end();
           scoutor++, index++)
        {
          Record*       record = *scoutor;

          // if found, stop.
          if (record->subject == subject)
            return elle::StatusOk;
        }

      escape("unable to locate the given subject");
    }

    ///
    /// this method returns the access record located at the given index.
    ///
    elle::Status        Access::Lookup(const Index&             index,
                                       Record*&                 record) const
    {
      Range<Record>::Scoutor    scoutor;
      Index                     i;

      ;

      // set the record to null.
      record = NULL;

      // go through the range.
      for (scoutor = this->range.container.begin(), i = 0;
           scoutor != this->range.container.end();
           scoutor++, i++)
        {
          // if found, stop.
          if (i == index)
            {
              // return the record.
              record = *scoutor;

              return elle::StatusOk;
            }
        }

      escape("unable to locate the record at the given index");
    }

    ///
    /// this method returns a range representing a subset of the access
    /// control list delimited by the given index and size.
    ///
    elle::Status        Access::Consult(const Index&            index,
                                        const Size&             size,
                                        Range<Record>&          range) const
    {
      Range<Record>::Scoutor    scoutor;
      Index                     i;

      ;

      // first detach the data from the range.
      if (range.Detach() == elle::StatusError)
        escape("unable to detach the data from the range");

      // go through the records.
      for (scoutor = this->range.container.begin(), i = 0;
           scoutor != this->range.container.end();
           scoutor++, i++)
        {
          Record*       record = *scoutor;

          // if this record lies in the selected range [index, index + size[
          if ((i >= index) && (i < (index + size)))
            {
              // add the record to the range.
              if (range.Add(record) == elle::StatusError)
                escape("unable to add the record to the given range");
            }
        }

      return elle::StatusOk;
    }

    ///
    /// this method updates the records with the given secret key by
    /// encrypted the given key with every subject's public key.
    ///
    elle::Status        Access::Upgrade(const elle::SecretKey&  key)
    {
      Range<Record>::Iterator   iterator;

      ;

      // go through the range.
      for (iterator = this->range.container.begin();
           iterator != this->range.container.end();
           iterator++)
        {
          Record*       record = *iterator;

          // check if the subject has the proper permissions.
          if ((record->permissions & PermissionRead) != PermissionRead)
            continue;

          // depending on the subject's type.
          switch (record->subject.type)
            {
            case Subject::TypeUser:
              {
                //
                // if the subject is a user, encrypt the key with the
                // user's public key so that she will be the only one
                // capable of decrypting it.
                //

                elle::PublicKey*        K = record->subject.user;

                // update the token.
                if (record->token.Update(key, *K) == elle::StatusError)
                  escape("unable to update the token");

                break;
              }
            case Subject::TypeGroup:
              {
                //
                // if the subject is a group, the key is made available
                // to the group's owner. this is especially useful in
                // order to increase the number of delegates available to
                // respond to vassal's requests.
                //

                // XXX to implement.

                break;
              }
            default:
              {
                escape("the access block contains unknown entries");
              }
            }

          // set the access block as being dirty.
          this->_state = proton::StateDirty;
        }

      return elle::StatusOk;
    }

    ///
    /// this method updates the records with a null token.
    ///
    elle::Status        Access::Downgrade()
    {
      Range<Record>::Iterator   iterator;

      ;

      // go through the range.
      for (iterator = this->range.container.begin();
           iterator != this->range.container.end();
           iterator++)
        {
          Record*       record = *iterator;

          // check if the subject has the proper permissions.
          if ((record->permissions & PermissionRead) != PermissionRead)
            continue;

          // reset the token.
          record->token = Token::Null;

          // set the access block as being dirty.
          this->_state = proton::StateDirty;
        }

      return elle::StatusOk;
    }

    ///
    /// this method removes the given record.
    ///
    elle::Status        Access::Remove(const Subject&           subject)
    {
      ;

      // remove the record from the range.
      if (this->range.Remove(subject) == elle::StatusError)
        escape("unable to remove the record");

      // set the block as dirty.
      this->_state = proton::StateDirty;

      return elle::StatusOk;
    }

    ///
    /// this method returns the size of the access control list.
    ///
    elle::Status        Access::Capacity(Size&                  size) const
    {
      ;

      // look at the size of the range.
      if (this->range.Capacity(size) == elle::StatusError)
        escape("unable to retrieve the range size");

      return elle::StatusOk;
    }

    ///
    /// this method computes a hash of the (subject, permissions) tuples
    /// composing the access object.
    ///
    /// this is required by the object class for access control purposes.
    ///
    elle::Status        Access::Fingerprint(elle::Digest&       digest) const
    {
      elle::Archive             archive;
      Range<Record>::Scoutor    scoutor;

      ;

      // create the archive.
      if (archive.Create() == elle::StatusError)
        escape("unable to create an archive");

      // go through the range and serialize every tuple (subject, permissions).
      for (scoutor = this->range.container.begin();
           scoutor != this->range.container.end();
           scoutor++)
        {
          Record*       record = *scoutor;

          // serialize the subject and permissions.
          if (archive.Serialize(record->subject,
                                record->permissions) == elle::StatusError)
            escape("unable to serialize the (subject, permissions) tuple");
        }

      // hash the archive.
      if (elle::OneWay::Hash(archive, digest) == elle::StatusError)
        escape("unable to hash the set of archived tuples");

      return elle::StatusOk;
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this operator compares two objects.
    ///
    elle::Boolean       Access::operator==(const Access&        element) const
    {
      ;

      // check the address as this may actually be the same object.
      if (this == &element)
        return elle::StatusTrue;

      // compare the ranges.
      if (this->range != element.range)
        return elle::StatusFalse;

      return elle::StatusTrue;
    }

    ///
    /// this macro-function call generates the object.
    ///
    embed(Access, _());

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this function dumps the access.
    ///
    elle::Status        Access::Dump(elle::Natural32            margin) const
    {
      elle::String      alignment(margin, ' ');

      ;

      std::cout << alignment << "[Access]" << std::endl;

      // dump the range.
      if (this->range.Dump(margin + 2) == elle::StatusError)
        escape("unable to dump the range");

      return elle::StatusOk;
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes the access object.
    ///
    elle::Status        Access::Serialize(elle::Archive&        archive) const
    {
      ;

      // call the parent class.
      if (proton::ContentHashBlock::Serialize(archive) == elle::StatusError)
        escape("unable to serialize the underlying CHB");

      // serialize the range.
      if (archive.Serialize(this->range) == elle::StatusError)
        escape("unable to serialize the range");

      return elle::StatusOk;
    }

    ///
    /// this method extracts the access object.
    ///
    elle::Status        Access::Extract(elle::Archive&          archive)
    {
      ;

      // call the parent class.
      if (proton::ContentHashBlock::Extract(archive) == elle::StatusError)
        escape("unable to extract the underlying CHB");

      // compare the component.
      if (this->component != ComponentAccess)
        escape("the archive does not seem to contain an access");

      // extract the range.
      if (archive.Extract(this->range) == elle::StatusError)
        escape("unable to extract the range");

      return elle::StatusOk;
    }

  }
}
