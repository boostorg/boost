#include <boost/smart_ptr.hpp>
#include <memory> // for std::auto_ptr
#include <iostream>
#include <boost/type_traits.hpp>
#include <typeinfo>

#define CheckPoint \
std::cout << __FILE__ << "(" << __LINE__ << ")" <<  std::endl

struct PyObject {};

namespace python {

  template <class T>
  struct type {};

  PyObject* to_python(int) {
      typedef int T;
    CheckPoint;
    return 0;
  }

  template <class T>
  PyObject* to_python(const T&) {
    CheckPoint;
    return 0;
  }

  template <class T>
  PyObject* to_python(std::auto_ptr<T>& x) {
    CheckPoint;
    return 0;
  }

  template <class T>
  PyObject* to_python(boost::shared_ptr<T>& x) {
    CheckPoint;
    return 0;
  }

  template <class T> struct converter;

  // Generalized converter which retrieves a non-const reference to T.
  template <class T>
  struct converter<T&>
  {
      typedef T& value_type;
      static value_type from_python(PyObject* p)
      {
          CheckPoint;
          static T t;
          return t;
      }
  };

  // If the user requests T by value, we can hijack semantics for T const&,
  // except in the case of std::auto_ptr, for which we have a specialization.
  template <class T>
  struct converter : converter<T const&> {};

  // For T*, we use the semantics of T& and take the address of the result
  template <class T>
  struct converter<T*>
  {
      typedef T* value_type;
      static value_type from_python(PyObject* p)
      {
          CheckPoint;
          return &converter<T&>::from_python(p);
      }
  };

  // trying to get a T*& should raise a compile error, since that pointer must
  // not be modified.
  template <class T> struct non_const_reference_to_pointer_parameter_not_wrappable;
  template <class T>
  struct converter<T*&> : non_const_reference_to_pointer_parameter_not_wrappable<T*&> {};

  // T* const& can use the T* semantics: the const& will bind to the temporary
  // return value.
  template <class T>
  struct converter<T* const&> : converter<T*> {};

  // No specialization for converter<T const*> because compiler will select
  // converter<U*> where U* == T const*

  template <class T>
  struct converter<std::auto_ptr<T>&>
  {
      typedef std::auto_ptr<T>& value_type;
      static value_type from_python(PyObject* p)
      {
          CheckPoint;
          static std::auto_ptr<T> t(new T);
          return t;
      }
  };

  // Specialization for std::auto_ptr<T> because it is an exception to the rule
  // that a requeset for T can always be satisfied by the result of a request
  // for T const&.
  template <class T>
  struct converter<std::auto_ptr<T> >
  {
      typedef std::auto_ptr<T> value_type;
      static value_type from_python(PyObject* p)
      {
          CheckPoint;
          static std::auto_ptr<T> t(new T);
          return t;
      }
  };

  template <class T>
  struct converter<boost::shared_ptr<T>&>
  {
      typedef boost::shared_ptr<T>& value_type;
      static value_type from_python(PyObject* p)
      {
          CheckPoint;
          static boost::shared_ptr<T> t(new T);
          return t;
      }
  };

  // No specialization for converter<T const&> because compiler will select
  // converter<U&> where U& == T const&

  // remove top-level const for completeness
  template <class T>
  typename converter<typename boost::remove_const<T>::type>::value_type
  from_python(PyObject * p, python::type<T> t) {
    return converter<typename boost::remove_const<T>::type>::from_python(p);
  }
}

namespace application {

  struct user { user() {} };
}

template <class T>
void f(T) {};

int main()
{
  PyObject* pyobj;

  int i = 1;
  pyobj = python::to_python(i);

  application::user u;
  pyobj = python::to_python(u);

  std::auto_ptr<application::user> a(new application::user);
  pyobj = python::to_python(a);

  boost::shared_ptr<application::user> s(new application::user);
  pyobj = python::to_python(s);

  f<application::user>(python::from_python(pyobj, python::type<application::user>()));
  f<application::user const>(python::from_python(pyobj, python::type<application::user const>()));
  f<application::user&>(python::from_python(pyobj, python::type<application::user&>()));
  f<application::user const&>(python::from_python(pyobj, python::type<application::user const&>()));
  f<application::user*>(python::from_python(pyobj, python::type<application::user*>()));
  f<application::user const*>(python::from_python(pyobj, python::type<application::user const*>()));
  f<application::user*const&>(python::from_python(pyobj, python::type<application::user*const&>()));
  f<application::user const*const&>(python::from_python(pyobj, python::type<application::user const*const&>()));

  // Expected to cause an error
//  f<application::user*&>(python::from_python(pyobj, python::type<application::user*&>()));

  f<std::auto_ptr<application::user> >(python::from_python(pyobj, python::type<std::auto_ptr<application::user> >()));
  f<std::auto_ptr<application::user>&>(python::from_python(pyobj, python::type<std::auto_ptr<application::user>&>()));
  f<std::auto_ptr<application::user> const&>(python::from_python(pyobj, python::type<std::auto_ptr<application::user> const&>()));

  f<boost::shared_ptr<application::user>&>(python::from_python(pyobj, python::type<boost::shared_ptr<application::user>&>()));
  f<boost::shared_ptr<application::user> >(python::from_python(pyobj, python::type<boost::shared_ptr<application::user> >()));
  f<boost::shared_ptr<application::user> const&>(python::from_python(pyobj, python::type<boost::shared_ptr<application::user> const&>()));
}

#if 0
#include "foo.h"

foo::foo() : x(20) {}

int main()
{
    foo y;
    y.x[1] = 0;
    return 0;
}

#endif
