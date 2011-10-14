//
// ---------- header ----------------------------------------------------------
//
// project       nucleus
//
// license       infinit
//
// author        julien quintard   [mon sep 12 05:23:24 2011]
//

#ifndef NUCLEUS_PROTON_PORCUPINE_HH
#define NUCLEUS_PROTON_PORCUPINE_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <nucleus/proton/Address.hh>
#include <nucleus/proton/Block.hh>
#include <XXX/Nodule.hh>

namespace nucleus
{
  namespace proton
  {

//
// ---------- forward declarations --------------------------------------------
//

    ///
    /// this class needs forward declaration to prevent conflicts.
    ///
    template <typename... T>
    class Seam;

    ///
    /// this class needs forward declaration to prevent conflicts.
    ///
    template <typename... T>
    class Quill;

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this generic declaration enables the definition of an empty
    /// Porcupine<> class which contains some generic stuff.
    ///
    template <typename... T>
    class Porcupine;

    ///
    /// this class makes it easy to build a tree-based data structure of
    /// blocks which does not require all the blocks to remain in main
    /// memory.
    ///
    /// every data item is referenced in an inlet. inlets a grouped together
    /// in a quill which represents a leaf of the given tree. then, every
    /// nodule being a seam or quill is referenced by a seam which encapsulates
    /// several of these references.
    ///
    /// therefore, the tree looks a bit like this with _k_ a key, _@_
    /// the address of a block and _d_ the actual data item:
    ///
    ///                             root seam nodule
    ///                          [ k@ | k@ | ... | k@ ]
    ///                             '               `
    ///                            '                 `
    ///              internal seam nodule         internal seam nodule
    ///             [ k@ | k@ | ... | k@ ]       [ k@ | k@ | ... | k@ ]
    ///                     '
    ///                      '
    ///                leaf quill nodule
    ///              [ k@ | k@ | ... | k@ ]
    ///                                 '
    ///                                  '
    ///                               data item
    ///                                 [ d ]
    ///
    /// note that every nodule is stored in a block, very much like data
    /// items. since the size of such blocks is limited by the network
    /// descriptor's extent attribute, nodules are split when full.
    /// likewise, should a nodule become too small, a balancing or merging
    /// procedure is triggered in order to `simplify' the tree.
    ///
    /// since such blocks can be quite large, the seam and quill entries,
    /// known as inlets, are organised in a tree-based data structure such
    /// as a map.
    ///
    /// note that a mayor key refers to the key with the highest value. this
    /// is the key which is propagated through the tree in order to route
    /// the request from the top. likewise, a key is said to be the maiden
    /// key if it is the only one remaining in a nodule.
    ///
    template <typename V>
    class Porcupine<V>:
      public elle::Object
    {
    public:
      //
      // constructors & destructors
      //
      Porcupine();
      ~Porcupine();

      //
      // methods
      //
      elle::Status		Add(const typename V::K&,
				    V*);
      elle::Status		Exist(const typename V::K&);
      elle::Status		Locate(const typename V::K&,
				       V*&);
      elle::Status		Remove(const typename V::K&);

      template <typename N,
		typename W>
      elle::Status		Insert(N*,
				       const typename V::K&,
				       W*);
      template <typename N>
      elle::Status		Delete(N*,
				       const typename V::K&);

      elle::Status		Grow();
      elle::Status		Shrink();

      elle::Status		Search(const typename V::K&,
				       Quill<V>*&);

      elle::Status		Check() const;

      elle::Status		Load(const Address&,
				     Nodule<V>*&);
      elle::Status		Unload(const Address&,
				       const Nodule<V>*);

      //
      // interfaces
      //

      // dumpable
      elle::Status		Dump(const elle::Natural32 = 0) const;

      //
      // attributes
      //
      elle::Natural32		height;
      Address			root;

      Nodule<V>*		_root;
    };

    ///
    /// this class contains generic information and methods for controlling
    /// the porcupine data structure system.
    ///
    template <>
    class Porcupine<>
    {
    public:
      //
      // static methods
      //
      static elle::Status	Initialize();
      static elle::Status	Clean();
    };

  }
}

//
// ---------- templates -------------------------------------------------------
//

#include <XXX/Porcupine.hxx>

//
// ---------- includes --------------------------------------------------------
//

#include <XXX/Seam.hh>
#include <XXX/Quill.hh>

#endif
