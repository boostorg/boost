Boost 1.34.0, the latest version of peer-reviewed, portable C++ 
libraries, has been released. This release brings five new libraries
and extensive updates to existing libraries.

New Libraries

    * Foreach Library: 
        BOOST_FOREACH macro for easily iterating over the elements of a
        sequence, from Eric Niebler.
    * Statechart Library: 
        Arbitrarily complex finite state machines can be implemented in
        easily readable and maintainable C++ code, from Andreas Huber.
    * TR1 Library: 
        An implementation of the C++ Technical Report on Standard Library
        Extensions, from John Maddock.
        This library does not itself implement the TR1 components, rather
        it's a thin wrapper that will include your standard library's TR1
        implementation (if it has one), otherwise it will include the Boost
        Library equivalents, and import them into namespace std::tr1. Highlights
        include: Reference Wrappers, Smart Pointers, result_of,
        Function Object Binders, Polymorphic function wrappers, Type Traits,
        Random Number Generators and Distributions, Tuples, Fixed Size Array,
        Hash Function Objects, Regular Expressions and
        Complex Number Additional Algorithms.
    * Typeof Library: 
        Typeof operator emulation, from Arkadiy Vertleyb and Peder Holt.
    * Xpressive Library: 
        Regular expressions that can be written as strings or as expression
        templates, and that can refer to each other and themselves recursively
        with the power of context-free grammars, from Eric Niebler.

Updated Libraries

    * Assign Library:
          o Support for ptr_map<key,T> via the new function ptr_map_insert()
          o Support for initialization of Pointer Containers when the containers
            hold pointers to an abstract base class.
    * Date_time library:
          o Support for new US/Canada timezone rules and other bug fixes.
            See Change History for details.
    * Filesystem Library: 
        Major upgrade in preparation for submission to the C++ Standards Committee
        for TR2. Changes include:
          o Internationalization, provided by class templates basic_path,
            basic_filesystem_error, basic_directory_iterator
            and basic_directory_entry.
          o Simplification of the path interface by eliminating special constructors
            to identify native formats.
          o Rationalization of predicate function design, including the addition of
            several new functions.
          o Clearer specification by reference to POSIX, the ISO/IEEE Single Unix
            Standard, with provisions for Windows and other operating systems.
          o Preservation of existing user code whenever possible.
          o More efficient directory iteration.
          o Addition of a recursive directory iterator.
    * Function Library: 
        Boost.Function now implements a small buffer optimization, which can
        drastically improve the performance when copying or constructing 
        Boost.Function objects storing small function objects. For instance,
        bind(&X:foo, &x, _1, _2) requires no heap allocation when placed into
        a Boost.Function object.
    * Functional/Hash Library
          o Use declarations for standard classes, so that the library doesn't
            need to include all of their headers
          o Deprecated the <boost/functional/hash/*.hpp> headers.
          o Add support for the BOOST_HASH_NO_EXTENSIONS macro, which disables
            the extensions to TR1
          o Minor improvements to the hash functions for floating point numbers.
    * Graph Library:
          o edmonds_maximum_cardinality_matching, from Aaron Windsor.
          o lengauer_tarjan_dominator_tree, from JongSoo Park.
          o compressed_sparse_row_graph, from Jeremiah Willcock and Douglas Gregor
            of Indiana University.
          o sorted_erdos_renyi_iterator, from Jeremiah Willcock
            of Indiana University.
          o biconnected_components now supports a visitor and named parameters,
            from Janusz Piwowarski.
          o adjacency_matrix now models the Bidirectional Graph concept.
          o dijkstra_shortest_paths now calls vis.initialize_vertex for each
            vertex during initialization.
          o Note: the name of the compiled library for the GraphViz reader has
            changed to boost_graph (from bgl-viz) to match Boost conventions.
          o See the complete revision history for more information.
    * MultiArray Library: 
        Boost.MultiArray now by default provides range-checking for operator[].
        Range checking can be disabled by defining the macro BOOST_DISABLE_ASSERTS
        before including multi_array.hpp. A bug in multi_array::resize() related
        to storage orders was fixed.
    * Multi-index Containers Library:
          o New random access indices.
          o Non key-based indices feature new rearrange facilities.
          o This version also includes a number of optimizations and usage
            improvements. For a complete list of changes,
            see the library release notes.
    * Optional Library:
          o boost::none_t and boost::none now added to Optional's documentation
          o Relational operators now directly support arguments of type 'T'
            and 'none_t'
          o operator->() now also works with reference types.
          o Helper functions make_optional(val), make_optional(cond,val)
            and get_optional_value_or(opt,alternative_value) added.
          o Constructor taking a boolean condition (as well as a value) added.
          o Member function get_value_or(alternative_value) added.
          o Incompatbility bug with mpl::apply<> fixed.
          o Converting assignment bug with uninitialized lvalues fixed.
    * Parameter Library:
          o Every ArgumentPack is now a valid MPL Forward Sequence.
          o Support for unnamed arguments (those whose keyword is deduced from
            their types) is added.
          o Support for named and unnamed template arguments is added.
          o New overload generation macros solve the forwarding problem directly.
          o See also the Python library changes, below.
    * Pointer Container Library:
          o Support for serialization via Boost.Serialization.
          o Exceptions can be disabled by defining the macro 
            BOOST_PTR_CONTAINER_NO_EXCEPTIONS before including any header.
            This macro is defined by default if BOOST_NO_EXCEPTIONS is defined.
          o Additional std::auto_ptr<T> overloads added s.t. one can also pass
            std::auto_ptr<T> instead of only T* arguments to member functions.
          o transfer() now has weaker requirements s.t. one can transfer objects
            from ptr_container<Derived> to ptr_container<Base>,
    * Python Library:
          o Boost.Python now automatically appends C++ signatures to docstrings.
            The new docstring_options.hpp header is available to control the
            content of docstrings.
          o stl_input_iterator, for turning a Python iterable object into an STL
            input iterator, from Eric Niebler.
          o Support for void* conversions is added.
          o Integrated support for wrapping C++ functions built with the
            parameter library; keyword names are automatically known to
            docsstrings.
          o Enhancements to the API for better embedding support
            (boost::python::import(), boost::python::exec()
             and boost::python::exec_file()).
    * Signals Library: 
        More improvements to signal invocation performance from Robert Zeh.
    * Smart Pointers Library:
          o Allocator support as proposed in N1851 (162 Kb PDF).
          o pointer_cast and pointer_to_other utilities to allow
            pointer-independent code, from Ion Gaztanaga.
    * String Algorithm Library:
          o lexicographical_compare
          o join
          o New comparison predicates is_less, is_not_greater.
          o Negative indexes support (like Perl) in various algorihtms
            (*_head/tail, *_nth).
    * Wave Library:
          o Wave now correctly recognizes pp-number tokens as mandated by the
            C++ Standard, which are converted to C++ tokens right before they are
            returned from the library.
          o Several new preprocessing hooks have been added. For a complete
            description please refer to the related documentation page:
            The Context Policy.
          o Shared library (dll) support has been added for the generated Wave
            libraries.
          o The overall error handling has been improved. It is now possible to
            recover and continue after an error or a warning was issued.
          o Support for optional comment and/or full whitespace preservation
            in the generated output stream has been added.
          o The Wave library now performs automatic include guard detection to
            avoid accessing header files more than once, if appropriate.
          o Full interactive mode has been added to the Wave tool. Now the Wave
            tool can be used just like Python or Perl for instance to
            interactively try out your BOOST_PP macros. Additionally it is now
            possible to load and save the current state of an interactive
            session (macro tables et.al.).
          o The overall performance has been improved by upto 40-60%, depending
            on the concrete files to process.
          o Support for new pragmas has been added allowing to control certain
            library features from inside the preprocessed sources (partial
            output redirection, control of generated whitespace and #line
            directives).
          o Optional support for #pragma message "..." has been added.
          o This version also includes a number of bug fixes and usage
            improvements. For a complete list of changes, see the libraries
            change log.

Supported Compilers

  Boost is tested on a wide range of compilers and platforms. Since Boost libraries
  rely on modern C++ features not available in all compilers, not all Boost
  libraries will work with every compiler. The following compilers and platforms
  have been extensively tested with Boost, although many other compilers and
  platforms will work as well. For more information, see the regression test
  results.

    * Apple GCC 4.0.1 on Mac OS X.
    * Borland C++ 5.8.2 on Windows.
    * GNU C++
          o 3.2.x., 3.3.x, 3.4.x, 4.0.x, 4.1.x on Linux
          o 4.1.x on Solaris
          o 3.4.x on Windows
    * HP aC++ A.06.14.
    * Intel C++ 9.1 on Windows, 9.0 on Linux.
    * Metrowerks CodeWarrior 9.4 on Windows.
    * Microsoft Visual C++ 6.0 (sp5, with and without STLport), 7.0, 7.1, 8.0.
      Note: Boost does not support the non-standard "Safe" C++ Library shipping
      with Visual C++ 8.0, which may result in many spurious warnings from Boost
      headers and other standards-conforming C++ code. To suppress these warnings,
      define the macro _SCL_SECURE_NO_DEPRECATE.
    * Sun Studio 11 on Solaris.

Acknowledgements

  Thomas Witt managed this release.

  A great number of people contributed their time and expertise to make this release
  possible. Special thanks go to Vladimir Prus for making Boost.Build version 2 a
  reality, David Abrahams for authoring a new Getting Started guide, Rene Rivera for
  general build and installation support and Greg D. for answering countless questions.