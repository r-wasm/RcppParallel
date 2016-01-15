//==============================================================================
//         Copyright 2003 - 2012 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef BOOST_SIMD_SWAR_FUNCTIONS_SIMD_COMMON_INTERLEAVE_EVEN_HPP_INCLUDED
#define BOOST_SIMD_SWAR_FUNCTIONS_SIMD_COMMON_INTERLEAVE_EVEN_HPP_INCLUDED

#include <boost/simd/swar/functions/interleave_even.hpp>
#include <boost/simd/include/functions/simd/insert.hpp>
#include <boost/simd/include/functions/simd/extract.hpp>
#include <boost/simd/include/functions/simd/bitwise_cast.hpp>
#include <boost/simd/sdk/meta/is_bitwise_logical.hpp>
#include <boost/simd/sdk/meta/as_arithmetic.hpp>
#include <boost/simd/sdk/meta/cardinal_of.hpp>

#include <boost/simd/swar/functions/details/builtin_shuffle.hpp>

namespace boost { namespace simd { namespace ext
{
  BOOST_DISPATCH_IMPLEMENT          ( interleave_even_, tag::cpu_
                                    , (A0)(A1)(X)
                                    , ((simd_< unspecified_<A0>, X>))
                                      ((simd_< unspecified_<A1>, X>))
                                    )
  {
    typedef A0 result_type;

    result_type operator()(A0 const& a0, A1 const& a1) const
    {
      result_type that;
      const std::size_t n = meta::cardinal_of<A0>::value;
      for(std::size_t i=0;i<n;i+= 2)
      {
        that[i]   = a0[i];
        that[i+1] = a1[i];
      }
      return that;
    }
  };

  BOOST_DISPATCH_IMPLEMENT_IF         ( interleave_even_, tag::cpu_
                                      , (A0)(X)
                                      , ( boost::simd::meta::is_bitwise_logical<A0> )
                                      , ((simd_< logical_<A0>, X>))
                                        ((simd_< logical_<A0>, X>))
                                      )
  {
    typedef A0 result_type;

    BOOST_FORCEINLINE BOOST_SIMD_FUNCTOR_CALL_REPEAT(2)
    {
      typedef typename meta::as_arithmetic<A0>::type type;
      return bitwise_cast<result_type>(
        interleave_even( bitwise_cast<type>(a0), bitwise_cast<type>(a1) )
      );
    }
  };

  #define M_IEVEN(z,n,t) (n%2 ? (t+n-1) : n)
  BOOST_SIMD_DEFINE_SHUFFLE2(  interleave_even_, M_IEVEN, type32_ )
  BOOST_SIMD_DEFINE_SHUFFLE2(  interleave_even_, M_IEVEN, type16_ )
  BOOST_SIMD_DEFINE_SHUFFLE2(  interleave_even_, M_IEVEN, type8_  )
  #undef M_IEVEN

} } }

#endif