//
// Created by Eduard Valeyev on 8/7/18.
//

#ifndef _libint2_include_numeric_h_
#define _libint2_include_numeric_h_

#include <libint2/config.h>
#include <sstream>
#include <iomanip>

#if LIBINT_HAS_MPFR
# include <cstddef>
# include <gmpxx.h>
# include <mpfr.h>
#endif

#include <libint2/util/generated/libint2_params.h>
#include <libint2/util/type_traits.h>

#if LIBINT_HAS_MPFR
 /// implement exp for mpf_class using MPFR ... I do not claim to know what issues the rounding presents here
 inline mpf_class exp(mpf_class x) {
   mpfr_t x_r; mpfr_init(x_r);
   mpfr_set_f(x_r, x.get_mpf_t(), MPFR_RNDN);

   mpfr_t expx_r; mpfr_init(expx_r);
   mpfr_exp(expx_r, x_r, MPFR_RNDN);

   mpf_t expx;
   mpf_init(expx);
   mpfr_get_f(expx, expx_r, MPFR_RNDN);
   mpf_class result(expx);
   return result;
 }
 /// implement pow for mpf_class using MPFR ... I do not claim to know what issues the rounding presents here
 inline mpf_class pow(mpf_class x, int a) {
   mpf_t x_to_a;
   mpf_init(x_to_a);
   if (a >= 0)
     mpf_pow_ui(x_to_a, x.get_mpf_t(), (unsigned int) a);
   else
     mpf_pow_ui(x_to_a, x.get_mpf_t(), (unsigned int) (-a));
   mpf_class result(x_to_a);
   if (a < 0)
     result = 1.0 / result;
   return result;
 }
 /// implement erf for mpf_class using MPFR ... I do not claim to know what issues the rounding presents here
 inline mpf_class erf(mpf_class x) {
   mpfr_t x_r; mpfr_init(x_r);
   mpfr_set_f(x_r, x.get_mpf_t(), MPFR_RNDN);

   mpfr_t erfx_r; mpfr_init(erfx_r);
   mpfr_erf(erfx_r, x_r, MPFR_RNDN);

   mpf_t erfx;
   mpf_init(erfx);
   mpfr_get_f(erfx, erfx_r, MPFR_RNDN);
   mpf_class result(erfx);
   return result;
 }
 /// implement log for mpf_class using MPFR ... I do not claim to know what issues the rounding presents here
 inline mpf_class log(mpf_class x) {
   mpfr_t x_r; mpfr_init(x_r);
   mpfr_set_f(x_r, x.get_mpf_t(), MPFR_RNDN);

   mpfr_t logx_r; mpfr_init(logx_r);
   mpfr_log(logx_r, x_r, MPFR_RNDN);

   mpf_t logx;
   mpf_init(logx);
   mpfr_get_f(logx, logx_r, MPFR_RNDN);
   mpf_class result(logx);
   return result;
 }
#endif

#ifdef LIBINT_HAS_MPFR
using LIBINT2_REF_REALTYPE = mpf_class;
#else
using LIBINT2_REF_REALTYPE = double;
#endif

namespace libint2 {
  using value_type = LIBINT2_REALTYPE;
  using scalar_type = libint2::vector_traits<value_type>::scalar_type;

  template <typename Real>
  inline Real get_epsilon(const Real& value);

#ifdef LIBINT_HAS_MPFR
  template <>
  inline mpf_class get_epsilon(const mpf_class& value) {
    const auto nbits = value.get_prec();
    return pow(mpf_class(2, nbits), -nbits);
  };
#endif

  template <typename Real>
  inline Real get_epsilon(const Real& value) {
    return std::numeric_limits<Real>::epsilon();
  }

template <typename Real>
inline int get_max_digits10(const Real& value);

#ifdef LIBINT_HAS_MPFR
template <>
  inline int get_max_digits10(const mpf_class& value) {
    const auto nbits = value.get_prec();
    return std::ceil(nbits * std::log10(2) + 1);
  };
#endif

template <typename Real>
inline int get_max_digits10(const Real& value) {
  return std::numeric_limits<Real>::max_digits10;
}

template <typename To, typename From>
  To sstream_convert(From && from) {
    std::stringstream ss;
    ss << std::scientific << std::setprecision(get_max_digits10(from)) << from;
    To to(ss.str().c_str());
    return to;
  }

};  // namespace libint2

#endif // _libint2_include_numeric_h_