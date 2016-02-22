#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>

#include <string>

/*  message    =  [ ":" prefix SPACE ] command [ params ] crlf
    prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
    command    =  1*letter / 3digit
    params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
               =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]

    nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
                    ; any octet except NUL, CR, LF, " " and ":"
    middle     =  nospcrlfcl *( ":" / nospcrlfcl )
    trailing   =  *( ":" / " " / nospcrlfcl )

    SPACE      =  %x20        ; space character
    crlf       =  %x0D %x0A   ; "carriage return" "linefeed"
    target     =  nickname / server
    msgtarget  =  msgto *( "," msgto )
    msgto      =  channel / ( user [ "%" host ] "@" servername )
    msgto      =/ ( user "%" host ) / targetmask
    msgto      =/ nickname / ( nickname "!" user "@" host )
    channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
                  [ ":" chanstring ]
    servername =  hostname
    host       =  hostname / hostaddr
    hostname   =  shortname *( "." shortname )
    shortname  =  ( letter / digit ) *( letter / digit / "-" )
                 *( letter / digit )
                  ; as specified in RFC 1123 [HNAME]
    hostaddr   =  ip4addr / ip6addr
    ip4addr    =  1*3digit "." 1*3digit "." 1*3digit "." 1*3digit
    ip6addr    =  1*hexdigit 7( ":" 1*hexdigit )
    ip6addr    =/ "0:0:0:0:0:" ( "0" / "FFFF" ) ":" ip4addr
    nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
    targetmask =  ( "$" / "#" ) mask
                  ; see details on allowed masks in section 3.3.1
    chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
    chanstring =/ %x2D-39 / %x3B-FF
                  ; any octet except NUL, BELL, CR, LF, " ", "," and ":"
    channelid  = 5( %x41-5A / digit )   ; 5( A-Z / 0-9 )
    user       =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
                  ; any octet except NUL, CR, LF, " " and "@"
    key        =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
                  ; any 7-bit US_ASCII character,
                  ; except NUL, CR, LF, FF, h/v TABs, and " "
    letter     =  %x41-5A / %x61-7A       ; A-Z / a-z
    digit      =  %x30-39                 ; 0-9
    hexdigit   =  digit / "A" / "B" / "C" / "D" / "E" / "F"
    special    =  %x5B-60 / %x7B-7D
                   ; "[", "]", "\", "`", "_", "^", "{", "|", "}"

*/

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct IrcMessage {
    std::string prefix;
    std::string command;
    std::string params;
};

BOOST_FUSION_ADAPT_STRUCT(
    IrcMessage,
    (std::string, prefix)
    (std::string, command)
    (std::string, params)
)

template <typename Iterator>
struct message_parser : qi::grammar<Iterator, IrcMessage()> {

    message_parser() : message_parser::base_type(message) {
        using qi::eol;
        using ascii::char_;

        message         %= -( ':' >> prefix >> ' ' ) >> command >> -( params ) >> crlf;
        prefix          %= servername | ( nickname >> -( -( char_('!') >> user ) >> char_('@') >> host ) );
        command         %= ( digit >> digit >> digit ) | +letter;
        params          %= *( ' ' >> middle ) >> -( char_(' ') >> -( char_(':') ) >> trailing );
        nospcrlfcl      %= char_('\x01', '\x09') | char_('\x0B', '\x0C') | char_('\x0E', '\x1F') | char_('!', '9') | char_(';', '~');
        middle          %= nospcrlfcl >> *( char_(':') | nospcrlfcl );
        trailing        %= *( char_(':') | char_(' ') | nospcrlfcl );
        crlf            %= eol;
        servername      %= hostname;
        host            %= hostname | hostaddr;
        hostname        %= shortname >> *( char_('.') >> shortname );
        shortname       %= ( digit | letter ) >> *( digit | letter | char_('-') ) >> *( digit | letter );
        hostaddr        %= ip4addr | ip6addr;
        ip4addr         %= ( digit >> -( digit >> -( digit ) ) ) >> char_('.') >> ( digit >> -( digit >> -( digit ) ) ) >> char_('.') >> ( digit >> -( digit >> -( digit ) ) ) >> char_('.') >> ( digit >> -( digit >> -( digit ) ) );
        ip6addr         %= +( hexdigit );   // FIXME
        user            %= +( char_('\x01', '\x09') | char_('\x0B', '\x0C') | char_('\x0E', '\x1F') | char_('\x21', '\x3F') | char_('\x41', '\xFF') );
        letter          %= char_('a', 'z') | char_('A', 'Z');
        digit           %= char_('0', '9');
        hexdigit        %= digit | char_('A') | char_('B') | char_('C') | char_('D') | char_('E') | char_('F');
        special         %= char_('[') | char_(']') | char_('\\') | char_('`') | char_('_') | char_('^') | char_('{') | char_('|') | char_('}');

    }

    qi::rule<Iterator, IrcMessage()> message;
    qi::rule<Iterator, std::string()> prefix;
    qi::rule<Iterator, std::string()> command;
    qi::rule<Iterator, std::string()> params;
    qi::rule<Iterator, char()> nospcrlfcl;
    qi::rule<Iterator, std::string()> middle;
    qi::rule<Iterator, std::string()> trailing;
    qi::rule<Iterator> crlf;
    qi::rule<Iterator, std::string()> servername;
    qi::rule<Iterator, std::string()> host;
    qi::rule<Iterator, std::string()> hostname;
    qi::rule<Iterator, std::string()> shortname;
    qi::rule<Iterator, std::string()> hostaddr;
    qi::rule<Iterator, std::string()> ip4addr;
    qi::rule<Iterator, std::string()> ip6addr;
    qi::rule<Iterator, std::string()> nickname;
    qi::rule<Iterator, std::string()> user;
    qi::rule<Iterator, char()> letter;
    qi::rule<Iterator, char()> digit;
    qi::rule<Iterator, char()> hexdigit;
    qi::rule<Iterator, char()> special;
};
