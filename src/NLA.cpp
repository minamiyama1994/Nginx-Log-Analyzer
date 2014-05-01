#include<iostream>
#include<string>
#include<functional>
#include"boost/xpressive/xpressive_static.hpp"
#include"boost/lexical_cast.hpp"
namespace bx = boost::xpressive ;
auto main ( int argc , char * * argv ) -> int
{
	std::function < void ( const bx::smatch & ) > f = [ ] ( const bx::smatch & )
	{
		std::cout << std::endl ;
	} ;
	for ( char * * iter = argv + 1 ; * iter ; ++ iter )
	{
		auto n = boost::lexical_cast < unsigned int > ( * iter ) ;
		f = [ f , n ] ( const bx::smatch & result )
		{
			f ( result ) ;
			std::cout << result [ n ] << ' ' ;
		} ;
	}
	bx::sregex regex =
		( bx::s1 = + bx::range ( '0' , '9' ) >> bx::repeat < 3 , 3 > ( '.' >> + bx::range ( '0' , '9' ) ) ) >> + bx::as_xpr ( ' ' ) >> // IP
		( bx::s2 = bx::as_xpr ( '-' ) ) >> + bx::as_xpr ( ' ' ) >> // ?
		( bx::s3 = + ~( bx::set = ' ' ) ) >> + bx::as_xpr ( ' ' ) >> // user
		( '[' >> ( bx::s4 = + ~( bx::set = '[' , ']' ) ) >> ']' ) >> + bx::as_xpr ( ' ' ) >> // Time
		( '"' >> ( bx::s5 = + ~( bx::set = '"' ) ) >> '"' ) >> + bx::as_xpr ( ' ' ) >> // HTTP
		( bx::s6 = + bx::range ( '0' , '9' ) ) >> + bx::as_xpr ( ' ' ) >> // status
		( bx::s7 = + bx::range ( '0' , '9' ) ) >> + bx::as_xpr ( ' ' ) >> // send bytes
		( '"' >> ( bx::s8 = + ~ ( bx::set = '"' ) ) >> '"' ) >> + bx::as_xpr ( ' ' ) >> // referer
		( '"' >> ( bx::s9 = + ~ ( bx::set = '"' ) ) >> '"' ) ; // User Agent
	std::string line ;
	bx::smatch result ;
	for ( ; std::getline ( std::cin , line ) ; )
	{
		if ( bx::regex_match ( line.begin ( ) , line.end ( ) , result , regex ) )
		{
			f ( result ) ;
		}
	}
}
