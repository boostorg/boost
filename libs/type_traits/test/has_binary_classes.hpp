//  (C) Copyright Frederic Bron 2009-2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef TT_HAS_BINARY_CLASSES_HPP
#define TT_HAS_BINARY_CLASSES_HPP

struct ret { };
BOOST_TT_PROC ret ret_val;

class C000 { BOOST_TT_PROC C000(); public: BOOST_TT_PROC C000(int) { } };
BOOST_TT_PROC void operator+(C000, C000) { }

class C001 { BOOST_TT_PROC C001(); public: BOOST_TT_PROC C001(int) { } };
BOOST_TT_PROC ret  operator+(C001, C001) { return ret_val; }

class C002 { BOOST_TT_PROC C002(); public: BOOST_TT_PROC C002(int) { } };
BOOST_TT_PROC ret  const operator+(C002, C002) { return ret_val; }

class C005 { BOOST_TT_PROC C005(); public: BOOST_TT_PROC C005(int) { } };
BOOST_TT_PROC ret  & operator+(C005, C005) { return ret_val; }

class C006 { BOOST_TT_PROC C006(); public: BOOST_TT_PROC C006(int) { } };
BOOST_TT_PROC ret  const & operator+(C006, C006) { return ret_val; }

class C009 { BOOST_TT_PROC C009(); public: BOOST_TT_PROC C009(int) { } };
BOOST_TT_PROC void operator+(C009, C009 const) { }

class C010 { BOOST_TT_PROC C010(); public: BOOST_TT_PROC C010(int) { } };
BOOST_TT_PROC ret  operator+(C010, C010 const) { return ret_val; }

class C011 { BOOST_TT_PROC C011(); public: BOOST_TT_PROC C011(int) { } };
BOOST_TT_PROC ret  const operator+(C011, C011 const) { return ret_val; }

class C014 { BOOST_TT_PROC C014(); public: BOOST_TT_PROC C014(int) { } };
BOOST_TT_PROC ret  & operator+(C014, C014 const) { return ret_val; }

class C015 { BOOST_TT_PROC C015(); public: BOOST_TT_PROC C015(int) { } };
BOOST_TT_PROC ret  const & operator+(C015, C015 const) { return ret_val; }

class C036 { BOOST_TT_PROC C036(); public: BOOST_TT_PROC C036(int) { } };
BOOST_TT_PROC void operator+(C036, C036 &) { }

class C037 { BOOST_TT_PROC C037(); public: BOOST_TT_PROC C037(int) { } };
BOOST_TT_PROC ret  operator+(C037, C037 &) { return ret_val; }

class C038 { BOOST_TT_PROC C038(); public: BOOST_TT_PROC C038(int) { } };
BOOST_TT_PROC ret  const operator+(C038, C038 &) { return ret_val; }

class C041 { BOOST_TT_PROC C041(); public: BOOST_TT_PROC C041(int) { } };
BOOST_TT_PROC ret  & operator+(C041, C041 &) { return ret_val; }

class C042 { BOOST_TT_PROC C042(); public: BOOST_TT_PROC C042(int) { } };
BOOST_TT_PROC ret  const & operator+(C042, C042 &) { return ret_val; }

class C045 { BOOST_TT_PROC C045(); public: BOOST_TT_PROC C045(int) { } };
BOOST_TT_PROC void operator+(C045, C045 const &) { }

class C046 { BOOST_TT_PROC C046(); public: BOOST_TT_PROC C046(int) { } };
BOOST_TT_PROC ret  operator+(C046, C046 const &) { return ret_val; }

class C047 { BOOST_TT_PROC C047(); public: BOOST_TT_PROC C047(int) { } };
BOOST_TT_PROC ret  const operator+(C047, C047 const &) { return ret_val; }

class C050 { BOOST_TT_PROC C050(); public: BOOST_TT_PROC C050(int) { } };
BOOST_TT_PROC ret  & operator+(C050, C050 const &) { return ret_val; }

class C051 { BOOST_TT_PROC C051(); public: BOOST_TT_PROC C051(int) { } };
BOOST_TT_PROC ret  const & operator+(C051, C051 const &) { return ret_val; }

class C072 { BOOST_TT_PROC C072(); public: BOOST_TT_PROC C072(int) { } };
BOOST_TT_PROC void operator+(C072 const, C072) { }

class C073 { BOOST_TT_PROC C073(); public: BOOST_TT_PROC C073(int) { } };
BOOST_TT_PROC ret  operator+(C073 const, C073) { return ret_val; }

class C074 { BOOST_TT_PROC C074(); public: BOOST_TT_PROC C074(int) { } };
BOOST_TT_PROC ret  const operator+(C074 const, C074) { return ret_val; }

class C077 { BOOST_TT_PROC C077(); public: BOOST_TT_PROC C077(int) { } };
BOOST_TT_PROC ret  & operator+(C077 const, C077) { return ret_val; }

class C078 { BOOST_TT_PROC C078(); public: BOOST_TT_PROC C078(int) { } };
BOOST_TT_PROC ret  const & operator+(C078 const, C078) { return ret_val; }

class C081 { BOOST_TT_PROC C081(); public: BOOST_TT_PROC C081(int) { } };
BOOST_TT_PROC void operator+(C081 const, C081 const) { }

class C082 { BOOST_TT_PROC C082(); public: BOOST_TT_PROC C082(int) { } };
BOOST_TT_PROC ret  operator+(C082 const, C082 const) { return ret_val; }

class C083 { BOOST_TT_PROC C083(); public: BOOST_TT_PROC C083(int) { } };
BOOST_TT_PROC ret  const operator+(C083 const, C083 const) { return ret_val; }

class C086 { BOOST_TT_PROC C086(); public: BOOST_TT_PROC C086(int) { } };
BOOST_TT_PROC ret  & operator+(C086 const, C086 const) { return ret_val; }

class C087 { BOOST_TT_PROC C087(); public: BOOST_TT_PROC C087(int) { } };
BOOST_TT_PROC ret  const & operator+(C087 const, C087 const) { return ret_val; }

class C108 { BOOST_TT_PROC C108(); public: BOOST_TT_PROC C108(int) { } };
BOOST_TT_PROC void operator+(C108 const, C108 &) { }

class C109 { BOOST_TT_PROC C109(); public: BOOST_TT_PROC C109(int) { } };
BOOST_TT_PROC ret  operator+(C109 const, C109 &) { return ret_val; }

class C110 { BOOST_TT_PROC C110(); public: BOOST_TT_PROC C110(int) { } };
BOOST_TT_PROC ret  const operator+(C110 const, C110 &) { return ret_val; }

class C113 { BOOST_TT_PROC C113(); public: BOOST_TT_PROC C113(int) { } };
BOOST_TT_PROC ret  & operator+(C113 const, C113 &) { return ret_val; }

class C114 { BOOST_TT_PROC C114(); public: BOOST_TT_PROC C114(int) { } };
BOOST_TT_PROC ret  const & operator+(C114 const, C114 &) { return ret_val; }

class C117 { BOOST_TT_PROC C117(); public: BOOST_TT_PROC C117(int) { } };
BOOST_TT_PROC void operator+(C117 const, C117 const &) { }

class C118 { BOOST_TT_PROC C118(); public: BOOST_TT_PROC C118(int) { } };
BOOST_TT_PROC ret  operator+(C118 const, C118 const &) { return ret_val; }

class C119 { BOOST_TT_PROC C119(); public: BOOST_TT_PROC C119(int) { } };
BOOST_TT_PROC ret  const operator+(C119 const, C119 const &) { return ret_val; }

class C122 { BOOST_TT_PROC C122(); public: BOOST_TT_PROC C122(int) { } };
BOOST_TT_PROC ret  & operator+(C122 const, C122 const &) { return ret_val; }

class C123 { BOOST_TT_PROC C123(); public: BOOST_TT_PROC C123(int) { } };
BOOST_TT_PROC ret  const & operator+(C123 const, C123 const &) { return ret_val; }

class C288 { BOOST_TT_PROC C288(); public: BOOST_TT_PROC C288(int) { } };
BOOST_TT_PROC void operator+(C288 &, C288) { }

class C289 { BOOST_TT_PROC C289(); public: BOOST_TT_PROC C289(int) { } };
BOOST_TT_PROC ret  operator+(C289 &, C289) { return ret_val; }

class C290 { BOOST_TT_PROC C290(); public: BOOST_TT_PROC C290(int) { } };
BOOST_TT_PROC ret  const operator+(C290 &, C290) { return ret_val; }

class C293 { BOOST_TT_PROC C293(); public: BOOST_TT_PROC C293(int) { } };
BOOST_TT_PROC ret  & operator+(C293 &, C293) { return ret_val; }

class C294 { BOOST_TT_PROC C294(); public: BOOST_TT_PROC C294(int) { } };
BOOST_TT_PROC ret  const & operator+(C294 &, C294) { return ret_val; }

class C297 { BOOST_TT_PROC C297(); public: BOOST_TT_PROC C297(int) { } };
BOOST_TT_PROC void operator+(C297 &, C297 const) { }

class C298 { BOOST_TT_PROC C298(); public: BOOST_TT_PROC C298(int) { } };
BOOST_TT_PROC ret  operator+(C298 &, C298 const) { return ret_val; }

class C299 { BOOST_TT_PROC C299(); public: BOOST_TT_PROC C299(int) { } };
BOOST_TT_PROC ret  const operator+(C299 &, C299 const) { return ret_val; }

class C302 { BOOST_TT_PROC C302(); public: BOOST_TT_PROC C302(int) { } };
BOOST_TT_PROC ret  & operator+(C302 &, C302 const) { return ret_val; }

class C303 { BOOST_TT_PROC C303(); public: BOOST_TT_PROC C303(int) { } };
BOOST_TT_PROC ret  const & operator+(C303 &, C303 const) { return ret_val; }

class C324 { BOOST_TT_PROC C324(); public: BOOST_TT_PROC C324(int) { } };
BOOST_TT_PROC void operator+(C324 &, C324 &) { }

class C325 { BOOST_TT_PROC C325(); public: BOOST_TT_PROC C325(int) { } };
BOOST_TT_PROC ret  operator+(C325 &, C325 &) { return ret_val; }

class C326 { BOOST_TT_PROC C326(); public: BOOST_TT_PROC C326(int) { } };
BOOST_TT_PROC ret  const operator+(C326 &, C326 &) { return ret_val; }

class C329 { BOOST_TT_PROC C329(); public: BOOST_TT_PROC C329(int) { } };
BOOST_TT_PROC ret  & operator+(C329 &, C329 &) { return ret_val; }

class C330 { BOOST_TT_PROC C330(); public: BOOST_TT_PROC C330(int) { } };
BOOST_TT_PROC ret  const & operator+(C330 &, C330 &) { return ret_val; }

class C333 { BOOST_TT_PROC C333(); public: BOOST_TT_PROC C333(int) { } };
BOOST_TT_PROC void operator+(C333 &, C333 const &) { }

class C334 { BOOST_TT_PROC C334(); public: BOOST_TT_PROC C334(int) { } };
BOOST_TT_PROC ret  operator+(C334 &, C334 const &) { return ret_val; }

class C335 { BOOST_TT_PROC C335(); public: BOOST_TT_PROC C335(int) { } };
BOOST_TT_PROC ret  const operator+(C335 &, C335 const &) { return ret_val; }

class C338 { BOOST_TT_PROC C338(); public: BOOST_TT_PROC C338(int) { } };
BOOST_TT_PROC ret  & operator+(C338 &, C338 const &) { return ret_val; }

class C339 { BOOST_TT_PROC C339(); public: BOOST_TT_PROC C339(int) { } };
BOOST_TT_PROC ret  const & operator+(C339 &, C339 const &) { return ret_val; }

class C360 { BOOST_TT_PROC C360(); public: BOOST_TT_PROC C360(int) { } };
BOOST_TT_PROC void operator+(C360 const &, C360) { }

class C361 { BOOST_TT_PROC C361(); public: BOOST_TT_PROC C361(int) { } };
BOOST_TT_PROC ret  operator+(C361 const &, C361) { return ret_val; }

class C362 { BOOST_TT_PROC C362(); public: BOOST_TT_PROC C362(int) { } };
BOOST_TT_PROC ret  const operator+(C362 const &, C362) { return ret_val; }

class C365 { BOOST_TT_PROC C365(); public: BOOST_TT_PROC C365(int) { } };
BOOST_TT_PROC ret  & operator+(C365 const &, C365) { return ret_val; }

class C366 { BOOST_TT_PROC C366(); public: BOOST_TT_PROC C366(int) { } };
BOOST_TT_PROC ret  const & operator+(C366 const &, C366) { return ret_val; }

class C369 { BOOST_TT_PROC C369(); public: BOOST_TT_PROC C369(int) { } };
BOOST_TT_PROC void operator+(C369 const &, C369 const) { }

class C370 { BOOST_TT_PROC C370(); public: BOOST_TT_PROC C370(int) { } };
BOOST_TT_PROC ret  operator+(C370 const &, C370 const) { return ret_val; }

class C371 { BOOST_TT_PROC C371(); public: BOOST_TT_PROC C371(int) { } };
BOOST_TT_PROC ret  const operator+(C371 const &, C371 const) { return ret_val; }

class C374 { BOOST_TT_PROC C374(); public: BOOST_TT_PROC C374(int) { } };
BOOST_TT_PROC ret  & operator+(C374 const &, C374 const) { return ret_val; }

class C375 { BOOST_TT_PROC C375(); public: BOOST_TT_PROC C375(int) { } };
BOOST_TT_PROC ret  const & operator+(C375 const &, C375 const) { return ret_val; }

class C396 { BOOST_TT_PROC C396(); public: BOOST_TT_PROC C396(int) { } };
BOOST_TT_PROC void operator+(C396 const &, C396 &) { }

class C397 { BOOST_TT_PROC C397(); public: BOOST_TT_PROC C397(int) { } };
BOOST_TT_PROC ret  operator+(C397 const &, C397 &) { return ret_val; }

class C398 { BOOST_TT_PROC C398(); public: BOOST_TT_PROC C398(int) { } };
BOOST_TT_PROC ret  const operator+(C398 const &, C398 &) { return ret_val; }

class C401 { BOOST_TT_PROC C401(); public: BOOST_TT_PROC C401(int) { } };
BOOST_TT_PROC ret  & operator+(C401 const &, C401 &) { return ret_val; }

class C402 { BOOST_TT_PROC C402(); public: BOOST_TT_PROC C402(int) { } };
BOOST_TT_PROC ret  const & operator+(C402 const &, C402 &) { return ret_val; }

class C405 { BOOST_TT_PROC C405(); public: BOOST_TT_PROC C405(int) { } };
BOOST_TT_PROC void operator+(C405 const &, C405 const &) { }

class C406 { BOOST_TT_PROC C406(); public: BOOST_TT_PROC C406(int) { } };
BOOST_TT_PROC ret  operator+(C406 const &, C406 const &) { return ret_val; }

class C407 { BOOST_TT_PROC C407(); public: BOOST_TT_PROC C407(int) { } };
BOOST_TT_PROC ret  const operator+(C407 const &, C407 const &) { return ret_val; }

class C410 { BOOST_TT_PROC C410(); public: BOOST_TT_PROC C410(int) { } };
BOOST_TT_PROC ret  & operator+(C410 const &, C410 const &) { return ret_val; }

class C411 { BOOST_TT_PROC C411(); public: BOOST_TT_PROC C411(int) { } };
BOOST_TT_PROC ret  const & operator+(C411 const &, C411 const &) { return ret_val; }

#endif
