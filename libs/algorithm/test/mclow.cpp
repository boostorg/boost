#include <stdexcept>

#include <boost/exception/exception.hpp>
#include <boost/throw_exception.hpp>

struct hex_decode_error : virtual boost::exception, virtual std::exception {};
struct not_enough_input : virtual hex_decode_error {};
struct non_hex_input    : virtual hex_decode_error {};

void toss() { BOOST_THROW_EXCEPTION(not_enough_input()); }

int main () {
	try { toss(); }
	catch ( const hex_decode_error & /*ex*/ ) {}
	}
