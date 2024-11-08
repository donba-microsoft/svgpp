// Copyright Oleg Maximenko 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://github.com/svgpp/svgpp for library home page.

#pragma once

#include <svgpp/parser/detail/common.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace svgpp 
{

namespace qi = boost::spirit::qi;

namespace detail
{
  typedef boost::tuple<unsigned char, unsigned char, unsigned char, unsigned char> rgba_t;

  namespace
  {
    struct color_keywords
    {
      typedef qi::symbols<char, rgba_t> symbols;
      static bool initialized_;
      static symbols symbols_;
    };

    bool color_keywords::initialized_ = false;
    color_keywords::symbols color_keywords::symbols_;
  }
}

template <class PropertySource, class Iterator, class ColorFactory>
class color_grammar: public qi::grammar<Iterator, typename ColorFactory::color_type()>
{
  typedef color_grammar<PropertySource, Iterator, ColorFactory> this_type;
public:
  typedef typename ColorFactory::color_type color_type;

  color_grammar()
    : this_type::grammar(color)
  {
    namespace phx = boost::phoenix;
    using qi::_1;
    using qi::_2;
    using qi::_3;
    using qi::_4;
    using qi::_a;
    using qi::_b;
    using qi::_c;
    using qi::_d;
    using qi::_val;
    using qi::lit;
    using detail::character_encoding_namespace::space;

    comma 
        =   *space 
            >> ',' 
            >> *space;
    color 
        =   hex_rule [_val = _1]
        |   (
                 detail::no_case_if_css(PropertySource())[ lit("rgb(") ]
              >> *space 
              >> (  components_rule [_val = _1]
                 |  percentage_rule [_val = _1]
                 )
              >> *space 
              >> ')' 
            )
#ifdef SVGPP_ACCEPT_RGBA_COLOR
        |   (
                 detail::no_case_if_css(PropertySource())[ lit("rgba(") ]
              >> *space
              >> (  components_rgba_rule [_val = _1]
                 |  percentage_rgba_rule [_val = _1]
                 )
              >> *space
              >> ')'
            )
#endif
        |   qi::no_case[detail::color_keywords::get_symbols()[_val = phx::bind(&color_grammar::color_keyword, _1)]];

    hex_rule 
        =   lit('#') 
            >> hex3 [_a = _1] 
            >> ( hex3 [_val = phx::bind(&color_grammar::six_hex_digits, _a, _1)] 
               | qi::eps [_val = phx::bind(&color_grammar::three_hex_digits, _a)] 
               );

    components_rule 
        =   (  integer
            >> comma 
            >> integer
            >> comma 
            >> integer
            ) [_val = phx::bind(&color_grammar::absolute_components, _1, _2, _3)];

    percentage_rule 
        =   (  number 
            >> '%'
            >> comma 
            >> number 
            >> '%' 
            >> comma 
            >> number 
            >> '%'
            ) [_val = phx::bind(&color_grammar::percent_components, _1, _2, _3)];

#ifdef SVGPP_ACCEPT_RGBA_COLOR
    components_rgba_rule 
        =   (  integer
            >> comma 
            >> integer
            >> comma 
            >> integer
            >> comma
            >> number
            ) [_val = phx::bind(&color_grammar::absolute_rgba_components, _1, _2, _3, _4)];

    percentage_rgba_rule 
        =   (  number 
            >> '%'
            >> comma 
            >> number 
            >> '%' 
            >> comma 
            >> number 
            >> '%'
            >> comma
            >> number
            ) [_val = phx::bind(&color_grammar::percent_rgba_components, _1, _2, _3, _4)];
#endif
  }

private:
  typedef typename ColorFactory::percentage_type number_type;

  typename this_type::start_type color;
  qi::rule<Iterator> comma;
  qi::rule<Iterator, color_type (), qi::locals<unsigned int> > hex_rule;
  qi::rule<Iterator, color_type ()> components_rule;
  qi::rule<Iterator, color_type ()> percentage_rule;
#ifdef SVGPP_ACCEPT_RGBA_COLOR
  qi::rule<Iterator, color_type ()> components_rgba_rule;
  qi::rule<Iterator, color_type ()> percentage_rgba_rule;
#endif
  qi::uint_parser<unsigned char, 10, 1, 3> integer;
  // There was mistake in SVG 1.1 that limits percentage to integer values, while CSS permits floating point numbers.
  // Till fixed version is not released, we will use CSS version of percentage definition.
  qi::real_parser<number_type, detail::number_policies<number_type, tag::source::css> > number;
  qi::uint_parser<unsigned int, 16, 3, 3> hex3;

  static color_type three_hex_digits(unsigned int h)
  {
    return ColorFactory::create( 
      static_cast<unsigned char>( ( (h & 0xf00) >> 8 ) | ( (h & 0xf00) >> 4 ) ),
      static_cast<unsigned char>( ( (h & 0x0f0) >> 4 ) |   (h & 0x0f0) ),
      static_cast<unsigned char>( ( (h & 0x00f) << 4 ) |   (h & 0x00f) ) );
  }

  static color_type six_hex_digits(unsigned int h1, unsigned int h2)
  {
    return ColorFactory::create( 
      static_cast<unsigned char>( h1 >> 4 ), 
      static_cast<unsigned char>( ( ( h1 & 0x0f ) << 4 ) | ( h2 >> 8 ) ),
      static_cast<unsigned char>( h2 & 0xff ) );
  }

  static color_type absolute_components(unsigned char r, unsigned char g, unsigned char b)
  {
    return ColorFactory::create(r, g, b);
  }

  static color_type percent_components(number_type r, number_type g, number_type b)
  {
    return ColorFactory::create_from_percent(r, g, b);
  }

#ifdef SVGPP_ACCEPT_RGBA_COLOR
  static color_type absolute_rgba_components(unsigned char r, unsigned char g, unsigned char b, double a)
  {
    return ColorFactory::create(r, g, b, a);
  }

  static color_type percent_rgba_components(number_type r, number_type g, number_type b, double a)
  {
    return ColorFactory::create_from_percent(r, g, b, a);
  }
#endif

  static color_type color_keyword(detail::rgba_t const & rgba)
  {
    return ColorFactory::create(rgba.get<0>(), rgba.get<1>(), rgba.get<2>(), rgba.get<3>());
  }
};

namespace detail
{
  namespace
  {
    struct color_keywords_initializer
    {
      color_keywords_initializer()
      {
        if (!color_keywords::initialized_)
        {
          color_keywords::symbols_.add
#define SVGPP_ON(name, r, g, b, a) (#name, rgba_t(r, g, b, a))
#include <svgpp/detail/dict/enumerate_colors.inc>
#undef SVGPP_ON
            ;
          color_keywords::initialized_ = true;
        }
      }
    };

    color_keywords_initializer color_keywords_initializer_instance;
  }
}

}