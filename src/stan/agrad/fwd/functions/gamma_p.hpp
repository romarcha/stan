#ifndef STAN__AGRAD__FWD__FUNCTIONS__GAMMA_P_HPP
#define STAN__AGRAD__FWD__FUNCTIONS__GAMMA_P_HPP

#include <stan/agrad/fwd/fvar.hpp>
#include <stan/meta/traits.hpp>
#include <stan/math/functions/gamma_p.hpp>

namespace stan{

  namespace agrad{

    template <typename T>
    inline
    fvar<T>
    gamma_p(const fvar<T>& x1, const fvar<T>& x2){
      using stan::math::gamma_p;
      using std::log;
      using std::exp;
      using std::pow;
      using std::fabs;
      using boost::math::tgamma;
      using boost::math::digamma;

      T u = gamma_p(x1.val_, x2.val_);
      
      T S = 0;
      T s = 1;
      T l = log(x2.val_);
      T g = tgamma(x1.val_);
      T dig = digamma(x1.val_);
      
      int k = 0;
      T delta = s / (x1.val_ * x1.val_);
      
      while (fabs(delta) > 1e-6) {
        S += delta;
        ++k;
        s *= -x2.val_ / k;
        delta = s / ((k + x1.val_) * (k + x1.val_));
      }
      
      T der1 = (u) * ( dig - l ) + exp( x1.val_ * l ) * S / g;
      T der2 = exp(-x2.val_) * pow(x2.val_, x1.val_ - 1.0) / g;
      
      return fvar<T>(u,x1.d_ * -der1 + x2.d_ * der2);
    }

    template <typename T>
    inline
    fvar<T>
    gamma_p(const fvar<T>& x1, const double x2){
      using stan::math::gamma_p;
      using std::log;
      using std::exp;
      using std::pow;
      using std::fabs;
      using boost::math::tgamma;
      using boost::math::digamma;

      T u = gamma_p(x1.val_, x2);
      
      T S = 0.0;
      double s = 1.0;
      double l = log(x2);
      T g = tgamma(x1.val_);
      T dig = digamma(x1.val_);
      
      int k = 0;
      T delta = s / (x1.val_ * x1.val_);
      
      while (fabs(delta) > 1e-6) {
        S += delta;
        ++k;
        s *= -x2 / k;
        delta = s / ((k + x1.val_) * (k + x1.val_));
      }
      
      T der1 = (u) * ( dig - l ) + exp( x1.val_ * l ) * S / g;

      return fvar<T>(u, x1.d_ * -der1);
    }

    template <typename T>
    inline
    fvar<T>
    gamma_p(const double x1, const fvar<T>& x2){
      using stan::math::gamma_p;
      using std::exp;
      using std::pow;
      
      T u = gamma_p(x1, x2.val_);
      
      double g = boost::math::tgamma(x1);
      
      T der2 = exp(-x2.val_) * pow(x2.val_, x1 - 1.0) / g;
      
      return fvar<T>(u, x2.d_ * der2);
    }
  }
}
#endif
