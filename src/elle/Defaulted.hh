#pragma once

#include <elle/attribute.hh>
#include <elle/print.hh>

namespace elle
{
  /// An optional that knows its default value.
  ///
  /// The value returned is read-only, to keep consistency.
  ///
  /// @code{.cc}
  ///
  /// auto d = Defaulted<bool>(true);
  /// assert(!d); // d was not set.
  /// assert(d.get()); // d's value is true.
  /// d = false;
  /// assert(d); // d was set.
  /// assert(!d.get()); // d's value is now false.
  ///
  /// @endcode
  template <typename T>
  class Defaulted
  {
  public:
    using Value = T;

    Defaulted(Value def, bool set = true)
      : _value{std::move(def)}
      , _set{set}
    {}

    Defaulted(Defaulted const&) = default;

    Defaulted(Defaulted&&) = default;

    Defaulted&
    operator=(Defaulted const&) = default;

    /// Override the default value.
    template <typename U>
    Defaulted&
    operator=(U&& u)
    {
      this->_value = std::forward<U>(u);
      this->_set = true;
      return *this;
    }

    /// Whether explicitly defined by the user.
    operator bool() const
    {
      return this->_set;
    }

    /// The value, readonly.
    Value const&
    get() const
    {
      return this->_value;
    }

    /// The value, readonly.
    Value const&
    operator*() const
    {
      return this->get();
    }

    /// A pointer to the value, readonly.
    Value const*
    operator->() const
    {
      return &this->_value;
    }

  private:
    /// The value.
    ELLE_ATTRIBUTE(Value, value);
    /// Whether a value was specified (as opposed to remaining equal
    /// to the initial value).
    bool _set = false;
  };

  /// Whether T is a Defaulted.
  template <typename T>
  struct is_defaulted : std::false_type{};

  template <typename T>
  struct is_defaulted<Defaulted<T>> : std::true_type{};


  template <typename T>
  std::ostream&
  operator <<(std::ostream& out, Defaulted<T> const& t)
  {
    elle::fprintf(out, "%s", t.get());
    if (repr(out) && !t)
      elle::fprintf(out, " (default)");
    return out;
  }

  template <typename T>
  Defaulted<T>
  defaulted(T t)
  {
    return Defaulted<T>(std::move(t), false);
  }
}
