#include <boost/iterator/transform_iterator.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/list.hpp>
#include <boost/container/slist.hpp>
#include <boost/core/lightweight_test.hpp>

using namespace boost::container;

struct func
{
   typedef int result_type;
   int operator()(const int i) const
   {  return i*2; }
};

int main()
{
   const std::size_t N = 10u;
   func f;
   slist<int>s(N);
   list<int>l(N);
   vector<int>v(N);

   vector<int>v2;
   v2.insert(v2.end(), boost::make_transform_iterator(s.begin(), f), boost::make_transform_iterator(s.end(), f));
   v2.insert(v2.end(), boost::make_transform_iterator(l.begin(), f), boost::make_transform_iterator(l.end(), f));
   v2.insert(v2.end(), boost::make_transform_iterator(v.begin(), f), boost::make_transform_iterator(v.end(), f));

   v2.assign(boost::make_transform_iterator(s.begin(), f), boost::make_transform_iterator(s.end(), f));
   v2.assign(boost::make_transform_iterator(l.begin(), f), boost::make_transform_iterator(l.end(), f));
   v2.assign(boost::make_transform_iterator(v.begin(), f), boost::make_transform_iterator(v.end(), f));

   {
      //slist
      boost::transform_iterator<func, slist<int>::iterator>
         ti(s.begin()), ti2(s.end()), ti3(ti);
      boost::intrusive::iterator_uadvance(ti3, v.size());
      BOOST_TEST(ti3 == ti2);
   }
   {
      //list
      boost::transform_iterator<func, list<int>::iterator>
         ti(l.begin()), ti2(l.end()), ti3(ti);
      boost::intrusive::iterator_uadvance(ti3, v.size());
      BOOST_TEST(ti3 == ti2);
   }
   {
      //vector
      boost::transform_iterator<func, vector<int>::iterator>
         ti(v.begin()), ti2(v.end()), ti3(ti);
      boost::intrusive::iterator_uadvance(ti3, v.size());
      BOOST_TEST(ti3 == ti2);
   }
   return boost::report_errors();
}

