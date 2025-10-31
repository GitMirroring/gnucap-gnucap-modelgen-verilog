/*                        -*- C++ -*-
 * Copyright (C) 2023 Felix Salfelder
 * Author: Felix Salfelder
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *------------------------------------------------------------------
 */

#ifndef GNUCAP_VA_H
#define GNUCAP_VA_H

#include <md.h>
#include <u_parameter.h>
#include <l_stlextra.h>
#include <e_storag.h>
#include <e_base.h> // CKT_BASE
#include <u_sim_data.h> // see simparam

// attribute index
typedef int aidx; // remove?
/*--------------------------------------------------------------------------*/
namespace va {
struct ddouble_tag{};
}
/*--------------------------------------------------------------------------*/
namespace notstd {
/*--------------------------------------------------------------------------*/
template <class InputIter, class Size, class OutputIter>
void add_n(InputIter first, Size count, OutputIter result)
{ untested();
  for ( ; count > 0; --count) { untested();
    *result++ += *first++;
  }
}
/*--------------------------------------------------------------------------*/
}
/*--------------------------------------------------------------------------*/
struct Exception_OutOfRange_ :public Exception{
  std::string _key, _range;
  Exception_OutOfRange_(std::string const& key, std::string const& range)
    :Exception(key + " is not " + std::string(1,'"') + range + std::string(1,'"')),
     _key(key), _range(range) {
  }
};
/*--------------------------------------------------------------------------*/

namespace{

typedef double real;
typedef int integer;
// double inf = std::numeric_limits<double>::infinity();
#define inf std::numeric_limits<double>::infinity()
#define Inf inf

template<int... idx> class ddd;

template<int numderiv>
class ddouble_ {
  typedef PARAMETER<double> Pdbl;
  typedef PARAMETER<int> Pint;
protected:
  double _data[1+numderiv];
public:
  /*explicit*/ ddouble_(ddouble_ const& o) {
    notstd::copy_n(o._data, numderiv+1, _data);
  }
  explicit ddouble_() { clear(); }
  /*explicit*/ ddouble_(int const& d) { clear(); *_data = d; }
  /*explicit*/ ddouble_(double const& d) { clear(); *_data = d; }
  /*explicit*/ ddouble_(Pdbl const& d) { clear(); *_data = d; }
  /*explicit*/ ddouble_(Pint const& d) { clear(); *_data = d; }
  void set_all_deps() {
    std::fill_n(_data+1, numderiv, 0.);
  }
  void set_no_deps() {
    std::fill_n(_data+1, numderiv, std::numeric_limits<double>::quiet_NaN());
  }
private:
  void reset_deriv(){
    set_all_deps();
  }
public:
  virtual double const& value()const {assert(_data); return _data[0];}
  // double d(int i)const { untested();return _data[i];}
  double const& operator[](int i)const {return _data[i+1];}
  double& operator[](int i){return _data[i+1];}
  operator double()const { return *_data; }
  // ddouble_& operator=(ddouble_ o)
  ddouble_& operator=(ddouble_ const& o) {
    assert(o == o);
    notstd::copy_n(o._data, numderiv+1, _data);
    return *this;
  }
  ddouble_& operator=(const double& o) {
    assert(o == o);
    *_data = o;
    reset_deriv();
    return *this;
  }
  ddouble_& operator=(Pdbl const& o) {
    assert(o == o);
    *_data = o;
    reset_deriv();
    return *this;
  }
  ddouble_& operator=(Pint const& o) {
    assert(o == o);
    *_data = double(o);
    reset_deriv();
    return *this;
  }
#ifndef NDEBUG
  bool is_same(const ddouble_& o)const {
    for(int i=0; i<=numderiv; ++i){
      if(_data[i] != _data[i]){ itested();
	if( o._data[i] == o._data[i] ){ untested();
	  return false;
	}else{ untested();
	}
      }else if(_data[i] != o._data[i]){ untested();
	return false;
      }else{
      }
    }
    return true;
  }
#endif
  bool operator==(const ddouble_& o)const {
#if 0 // really?
    for(int i=0; i<=numderiv; ++i){ untested();
      if(_data[i] != o._data[i]){ untested();
	return false;
      }else{ untested();
      }
    }
#else
    return *_data == *o._data;
#endif
    return true;
  }
  bool operator==(const double& o)const { assert(_data); return *_data == o; }
  bool operator==(const int& o)const { double x = o; return *_data == x; }

  template<class T>
  bool operator<(const T& o)const { return o > *_data; }
  bool operator<(const ddouble_& o)const { return *_data < *o._data; }
  bool operator<(double const& o)const   { return *_data < o; }
  bool operator<(int const& o)const      { return *_data < o; }
  bool operator<(Pdbl const& o)const  { untested(); return *_data < double(o); }
  bool operator<(Pint const& o)const     { untested(); return *_data < double(o); }

  template<class T>
  bool operator<=(const T& o)const        { return o >= *_data; }
  bool operator<=(ddouble_ const& o)const { itested(); return *_data <= *o._data; }
  bool operator<=(double const& o)const   { return *_data <= o; }
  bool operator<=(int const& o)const      { itested(); return *_data <= o; }
  bool operator<=(Pdbl const& o)const     { untested(); return *_data <= double(o); }
  bool operator<=(Pint const& o)const     { untested(); return *_data <= double(o); }

  template<class T>
  bool operator>(const T& o)const        { return o < *_data; }
  bool operator>(ddouble_ const& o)const { return *_data > *o._data; }
  bool operator>(double const& o)const   { return *_data > o; }
  bool operator>(int const& o)const      { return *_data > o; }
  bool operator>(Pdbl const& o)const     { untested(); return *_data > double(o); }
  bool operator>(Pint const& o)const     { untested(); return *_data > double(o); }

  template<class T>
  bool operator>=(const T& o)const        { return o <= *_data; }
  bool operator>=(const ddouble_& o)const { return *_data >= *o._data; }
  bool operator>=(const int& o)const      { return *_data >= o; }
  bool operator>=(const double& o)const   { return *_data >= o; }
  bool operator>=(Pdbl const& o)const     { untested(); return *_data >= double(o); }
  bool operator>=(Pint const& o)const     { untested(); return *_data >= double(o); }

  ddouble_ operator-() const { return ddouble_(*this) * -1.; }
  ddouble_ const& operator+()const { return *this; }
  bool operator!() const { untested(); assert(_data); return *_data == 0.; }

public: // modify
  ddouble_& operator+=(const ddouble_& o) {
    for(int i=0; i<=numderiv; ++i){
      _data[i] += o._data[i];
    }
    return *this;
  }

  ddouble_& operator-=(const ddouble_& o) {
    for(int i=0; i<=numderiv; ++i){
      _data[i] -= o._data[i];
    }
    return *this;
  }

  ddouble_& operator*=(ddouble_ const& o) {
    assert(this!=&o);
    trace3("op*=", numderiv, _data[0], o._data[0]);
    for(int i=1; i<=numderiv; ++i){
      trace3("op*= in", i, _data[i], o._data[i]);
    }
    assert(*o._data == *o._data);
    assert(*_data == *_data);
    for(int i=1; i<=numderiv; ++i){
      // assert(_data[i] == _data[i]);
      // assert(o._data[i] == o._data[i]);
      _data[i] *= *o._data;
      _data[i] += *_data * o._data[i];
    }
    *_data *= *o._data;
    return *this;
  }
  ddouble_& operator*=(const double& o) {
    for(int i=0; i<=numderiv; ++i){
      _data[i] *= o;
    }
    return *this;
  }

  ddouble_& operator/=(ddouble_ const& o) {
    assert(this!=&o);
    if(*o._data){
      for(int i=1; i<=numderiv; ++i){
	_data[i] *= o.value();
	_data[i] -= value() * o._data[i];
	_data[i] /= o.value() * o.value();
      }
      *_data /= *o._data;
    }else{
      *this /= (o + 1e-20);
    }
    return *this;
  }
  ddouble_& operator/=(double const& o) {
    assert(_data);
    assert(o);
    for(int i=0; i<=numderiv; ++i){
      _data[i] /= o;
    }
    return *this;
  }

public: // modify. Params
  ddouble_& operator+=(Pdbl const& o) {untested(); return *this+=(double (o)); }
  ddouble_& operator*=(Pdbl const& o) {untested(); return *this*=(double (o)); }
  ddouble_& operator/=(Pdbl const& o) {untested(); return *this/=(double (o)); }
  ddouble_& operator-=(Pdbl const& o) {untested(); return *this-=(double (o)); }
  ddouble_& operator+=(Pint const& o) {untested(); return *this+=(double (o)); }
  ddouble_& operator-=(Pint const& o) {untested(); return *this-=(double (o)); }
  ddouble_& operator*=(Pint const& o) {untested(); return *this*=(double (o)); }
  ddouble_& operator/=(Pint const& o) {untested(); return *this/=(double (o)); }

public: // create
//  template<class T>
//  ddouble_ operator+(T const& o)const        { return o + *this; }
  ddouble_ operator+(ddouble_ const& o)const { return ddouble_(*this) += o; }
  ddouble_ operator+(double const& o)const   { return ddouble_(*this) += o; }
  ddouble_ operator+(int const& o)const      { return ddouble_(*this) += double(o); }
  ddouble_ operator+(Pdbl const& o)const     { return ddouble_(*this) += double(o); }
  ddouble_ operator+(Pint const& o)const     { return ddouble_(*this) += double(o); }

//  template<class T>
//  ddouble_ operator-(T const& o)const        { return (-o) + *this; }
  ddouble_ operator-(ddouble_ const& o)const { return ddouble_(*this) -= o; }
  ddouble_ operator-(double const& o)const   { return ddouble_(*this) -= o; }
  ddouble_ operator-(int const& o)const      { return ddouble_(*this) -= double(o); }
  ddouble_ operator-(Pdbl const& o)const     { return ddouble_(*this) -= double(o); }
  ddouble_ operator-(Pint const& o)const     { return ddouble_(*this) -= double(o); }

//  template<class T>
//  ddouble_ operator*(T const& o)const        { return o * *this; }
  ddouble_ operator*(const ddouble_& o)const { return ddouble_(*this) *= o; }
  ddouble_ operator*(const double& o)const   { return ddouble_(*this) *= o; }
  ddouble_ operator*(const int& o)const      { return ddouble_(*this) *= double(o); }
  ddouble_ operator*(Pdbl const& o) const    { return ddouble_(*this) *= double(o); }
  ddouble_ operator*(Pint const& o) const    { return ddouble_(*this) *= double(o); }

//  template<class T>
//  ddouble_ operator/(T const& o)const        { return (1./o) * *this; } // incomplete.
  ddouble_ operator/(ddouble_ const& o)const { return ddouble_(*this) /= o; }
  ddouble_ operator/(double const& o)const   { return ddouble_(*this) /= o; }
  ddouble_ operator/(int const& o) const     { return ddouble_(*this) /= double(o); }
  ddouble_ operator/(Pdbl const& o) const    { return ddouble_(*this) /= double(o); }
  ddouble_ operator/(Pint const& o) const    { return ddouble_(*this) /= double(o); }

public: // deriv
  ddouble_& chain(double const& d) {
    for(int i=1; i<=numderiv; ++i){
      _data[i] *= d;
    }
    return *this;
  } /*chain*/
  ddouble_& set_value(double const& v){
    *_data = v;
    return *this;
  }
  void clear() {
    std::fill_n(_data, numderiv+1, 0.);
  } /*clear*/

  template<int... idx>
  ddouble_& operator=(ddd<idx...> const&);
}; /*ddouble_*/
/*--------------------------------------------------------------------------*/
template<int... idx>
struct max_arg__{
};
/*--------------------------------------------------------------------------*/
template<int i, int... idx>
struct max_arg__<i, idx...>{
  static constexpr int value = std::max(i, max_arg__<idx...>::value);
};
/*--------------------------------------------------------------------------*/
template<>
struct max_arg__<>{
  static constexpr int value = 0;
};
/*--------------------------------------------------------------------------*/
template<int... idx>
constexpr int max_arg_()
{
  return max_arg__<idx...>::value;
}
/*--------------------------------------------------------------------------*/
template<int... idx>
class ddd {
  typedef PARAMETER<double> Pdbl;
  typedef PARAMETER<int> Pint;
  typedef ddouble_<sizeof...(idx)> base;
  base _dd{0.};
public:
  typedef va::ddouble_tag base_tag;
  explicit ddd() : _dd() {}
  template<int n>
  explicit ddd(ddouble_<n> const& d) {
    operator=(d);
  }
  /*explicit*/ ddd(int const& d) : _dd(d) {}
  /*explicit*/ ddd(double const& d) : _dd(d) {}
  /*explicit*/ ddd(PARAMETER<double> const& d) : _dd(d) {}
  ddd(ddd const& o) { operator=(o); }

public:
  void set_no_deps() {}
  void clear() {
	  _dd.clear();
   // std::fill_n(_data, sizeof...(idx)+1);
  } /*clear*/
  void flat_export(double* o) const;
  // double const& value()const override {return base::value();}
  double const& value()const {return _dd.value();}
  ddd& set_value(double const& v){ _dd.set_value(v); return *this; }
  // operator double()const { return value(); }
  operator double()const { return _dd; }
 // operator double()const { return *_data; }
  template<class T>
  bool operator==(const T& o)const {
    return operator==(double(o));
  }
  bool operator==(const double& o)const { double x = o; return value() == x; }
  ddd& operator=(ddd const& o) { _dd = o._dd; return *this; }
  ddd& operator=(Pint const& t) { return operator=(double(t)); }
  ddd& operator=(Pdbl const& t) { return operator=(double(t)); }
  template<int... jdx>
  ddd& operator=(ddd<jdx...> const& d) {
    constexpr int m = max_arg_<idx...>();
    static_assert(max_arg_<jdx...>() <= m);
    ddouble_<m+1> tmp;
    tmp = d;
    return *this = tmp;
  }
  template<int n>
  ddd& operator=(ddouble_<n> const& d) {
    set_value(d.value());
#   ifndef NDEBUG
    std::vector<bool> chk(n, false);
#   endif

    constexpr std::array<int, sizeof...(idx)> arr{idx...};
    int k=0;
    for(int i : arr) {
      assert(k<n);
      assert(i<n);
      _dd[k++] = d[i];
#     ifndef NDEBUG
      chk[i] = true;
#     endif
    }
#   ifndef NDEBUG
    for(int c=0; c<n; ++c){
      if(chk[c]){
	// OK, got it.
      }else if(!d[c]){
	// ok, zero..
      }else if(d[c]!=d[c]){ untested();
	// not used.
      }else{ untested();
	unreachable();
	trace2("missed", c, d[c]);
      }
    }
#   endif
    return *this;
  }
  ddd operator-()const { return ddd(*this) *= -1.; }
  ddd const& operator+()const { untested(); return *this; }
public: // ddouble coercion
  template<int n>
  operator ddouble_<n>()const {
    ddouble_<n> a(value());
    for(int k=0; k<n; ++k){
      assert(!a[k]);
    }
//    a.set_all_deps(); // unused slots are "0.";

    constexpr std::array<int, sizeof...(idx)> arr{idx...};
    int k=0;
    for(auto i : arr) {
      assert(i<n);
      trace3("assign", i, k, _dd[k]);
      a[i] = _dd[k++];
    }
    return a;
  }

public: // create ddd
  template<int n>
  ddd operator+(ddouble_<n> const& o)const { return ddd(*this) += o; }
  ddd operator+(double const& o)const      { return ddd(*this) += o; }
  ddd operator+(ddd const& o)const         { return ddd(*this) += o; }
  ddd operator+(int const& o)const         { return operator+(double(o)); }
  ddd operator+(Pint const& o)const        { return operator+(double(o)); }
  ddd operator+(Pdbl const& o)const        { return operator+(double(o)); }

  template<int n>
  ddd operator-(ddouble_<n> const& o)const { return ddd(*this) -= o; }
  ddd operator-(double const& o)const      { return ddd(*this) -= o; }
  ddd operator-(ddd const& o)const         { return ddd(*this) -= o; }
  ddd operator-(int const& o)const         { return operator-(double(o)); }
  ddd operator-(Pint const& o)const        { return operator-(double(o)); }
  ddd operator-(Pdbl const& o)const        { return operator-(double(o)); }

  template<int n>
  ddd operator*(ddouble_<n> const& o)const { return ddd(*this) *= o; }
  ddd operator*(const double& o)const      { return ddd(*this) *= o; }
  ddd operator*(const ddd& o)const         { return ddd(*this) *= o; }
  ddd operator*(int const& o)const         { return operator*(double(o)); }
  ddd operator*(Pint const& o)const        { untested(); return operator*(double(o)); }
  ddd operator*(Pdbl const& o)const        { return operator*(double(o)); }

  template<int n>
  ddd operator/(ddouble_<n> const& o)const { return ddd(*this) /= o; }
  ddd operator/(double const& o)const      { return ddd(*this) /= o; }
  ddd operator/(const ddd& o)const         { return ddd(*this) /= o; }
  ddd operator/(int const& o)const         { return operator/(double(o)); }
  ddd operator/(Pint const& o)const        { return operator/(double(o)); }
  ddd operator/(Pdbl const& o)const        { return operator/(double(o)); }

public: // deriv xs
  double& operator[](int pos){
    int i = 0;
    std::array<int, sizeof...(idx)> arr{idx...};
    for(int j = 0; j < int(sizeof...(idx)); ++j) {
      ++i;
      if(arr[j] == pos) {
        break;
      }else{
      }
    }
    assert(i);
    return _dd[i-1];
  }
  double const& operator[](int pos)const {
    return const_cast<ddd*>(this)->operator[](pos);
  }
  size_t size()const{
	 return sizeof...(idx);
  }
//	int a()const{
//		seq A;
//		((std::cout << x << ' '), ...);
//		(( o(x)), ...);
//		return A.size();
//	}

public: // modify, ddd
  ddd& rdiv(ddd const& o) { ddd oo(o); return operator=(oo / *this); }
  ddd& operator+=(ddd const& o) { _dd += o._dd; return *this; }
  ddd& operator-=(ddd const& o) { _dd -= o._dd; return *this; }
  ddd& operator*=(ddd const& o) { _dd *= o._dd; return *this; }
  ddd& operator/=(ddd const& o) { _dd /= o._dd; return *this; }

public: // modify, param
  ddd& operator+=(Pdbl const& o) {untested(); return *this += double(o); }
  ddd& operator-=(Pdbl const& o) {untested(); return *this -= double(o); }
  ddd& operator*=(Pdbl const& o) {untested(); return *this *= double(o); }
  ddd& operator/=(Pdbl const& o) {untested(); return *this /= double(o); }
  ddd& operator+=(Pint const& o) {untested(); return *this += double(o); }
  ddd& operator-=(Pint const& o) {untested(); return *this -= double(o); }
  ddd& operator*=(Pint const& o) {untested(); return *this *= double(o); }
  ddd& operator/=(Pint const& o) {untested(); return *this /= double(o); }

public: // modify, ddouble
  template<int n>
  ddd& rdiv(ddouble_<n> const& o) {
    ddd oo(o);
    return operator=(oo / *this);
  }
  template<int n>
  ddd& operator/=(ddouble_<n> const& o) {
    ddd oo(o);
    return operator/=(oo);
  }
  template<int n>
  ddd& operator+=(ddouble_<n> const& o) {
    ddd oo(o);
    return operator+=(oo);
  }
  template<int n>
  ddd& operator*=(ddouble_<n> const& o) {
    ddd oo(o);
    return operator*=(oo);
  }
  template<int n>
  ddd& operator-=(ddouble_<n> const& o) {
    ddd oo(o);
    return operator-=(oo);
  }

public: // modify
  ddd& operator+=(double const& o) { _dd += o; return *this; }
  ddd& operator-=(double const& o) { _dd -= o; return *this; }
  ddd& operator*=(double const& o) { _dd *= o; return *this; }
  ddd& operator/=(double const& o) { _dd /= o; return *this; }

public: // deriv
  ddd& chain(double const& d) {
    _dd.chain(d);
    return *this;
  }
};
/*--------------------------------------------------------------------------*/
template<int numderiv>
template<int... idx>
ddouble_<numderiv>& ddouble_<numderiv>::operator=(ddd<idx...> const& o)
{
  clear();
#ifndef NDEBUG
  std::array<int, sizeof...(idx)> arr{idx...};
  for(auto i : arr) {
    assert(i<numderiv);
  }
#endif
  o.flat_export(_data);
  return *this;
}
/*--------------------------------------------------------------------------*/
template<int... idx>
void ddd<idx...>::flat_export(double* o) const
{
  constexpr std::array<int, sizeof...(idx)> arr{idx...};
  int k=0;
  *o = value();
  for(auto i : arr) {
    trace3("assign", i, k, o[k]);
    o[i+1] = _dd[k++];
  }
}
/*--------------------------------------------------------------------------*/
// template<int T>
// ddouble_<T> operator/(ddouble_<T> a, ddouble_<T> const& b)  { untested(); return a /= b; }
template<int T>
ddouble_<T> operator/(int const& a, ddouble_<T> const& b)   { untested(); return ddouble_<T>(a) /= b; }
template<int T>
ddouble_<T> operator/(double const& a, ddouble_<T> const& b){ return ddouble_<T>(a) /= b; }
template<int T>
ddouble_<T> operator/(PARAMETER<double> const& a, ddouble_<T> const& b)  { return ddouble_<T>(a) /= b; }
template<int T>
ddouble_<T> operator/(PARAMETER<int> const& a, ddouble_<T> const& b)  { untested(); return ddouble_<T>(a) /= b; }
/*--------------------------------------------------------------------------*/
// template<int T>
// ddouble_<T> operator-(ddouble_<T> a, ddouble_<T> b)  { untested(); return a -= b; }
template<int T>
ddouble_<T> operator-(double const& a, ddouble_<T> b){ return ddouble_<T>(a) -= b; }
template<int T>
ddouble_<T> operator-(int const& a, ddouble_<T> b)   { return ddouble_<T>(a) -= b; }
template<int T>
ddouble_<T> operator-(PARAMETER<double> const& a, ddouble_<T> b)  { untested(); return ddouble_<T>(a) -= b; }
template<int T>
ddouble_<T> operator-(PARAMETER<int> const& a, ddouble_<T> b)  { untested(); return ddouble_<T>(a) -= b; }
/*--------------------------------------------------------------------------*/
// template<int T>
// ddouble_<T> operator*(ddouble_<T> const& a, ddouble_<T> b) { untested(); return b * a; }
template<int T>
ddouble_<T> operator*(double const& a, ddouble_<T> b)      { return b * a; }
template<int T>
ddouble_<T> operator*(int const& a, ddouble_<T> b)         { untested(); return b * a; }
template<int T>
ddouble_<T> operator*(PARAMETER<double> const& a, ddouble_<T> b) { return b * a; }
template<int T>
ddouble_<T> operator*(PARAMETER<int> const& a, ddouble_<T> b)        { return b * a; }
/*--------------------------------------------------------------------------*/
// template<int T>
// ddouble_<T> operator+(ddouble_<T> const& a, ddouble_<T> b) { untested(); return b + a; }
template<int T>
ddouble_<T> operator+(double const& a, ddouble_<T> b) { return b + a; }
template<int T>
ddouble_<T> operator+(int const& a, ddouble_<T> b)    { return b + a; }
template<int T>
ddouble_<T> operator+(PARAMETER<double> const& a, ddouble_<T> b){ return b + a; }
template<int T>
ddouble_<T> operator+(PARAMETER<int> const& a, ddouble_<T> b)   { untested(); return b + a; }
/*--------------------------------------------------------------------------*/
// template<int T>
// bool operator==(ddouble_<T> const& a, PARAMETER<int> const& b) { untested(); return double(b) == a.value(); }
template<int T>
bool operator==(PARAMETER<double> const& b, ddouble_<T> const& a) { untested(); return a == b; }
template<int T>
bool operator==(PARAMETER<int> const& b, ddouble_<T> const& a)    { untested(); return a == b; }
/*--------------------------------------------------------------------------*/
inline bool operator==(PARAMETER<int> const& b, double const& a)
{ untested();
  return double(b) == a;
}
inline bool operator==(PARAMETER<double> const& b, int const& a)
{ untested();
  return int(b) == a;
}
/*--------------------------------------------------------------------------*/
template<int... idx>
ddd<idx...> operator/(double const& a, ddd<idx...> const& b) { return ddd<idx...>(b).rdiv(a); }
template<int... idx>
ddd<idx...> operator-(double const& a, ddd<idx...> const& b) { return ddd<idx...>(-b) += a; }
template<int... idx>
ddd<idx...> operator+(double const& a, ddd<idx...> const& b) { return ddd<idx...>(b) += a; }
template<int... idx>
ddd<idx...> operator*(double const& a, ddd<idx...> const& b) { return ddd<idx...>(b) *= a; }
/*--------------------------------------------------------------------------*/
template<int... idx>
ddd<idx...> operator/(int const& a, ddd<idx...> const& b) { untested(); return ddd<idx...>(b).rdiv(a); }
template<int... idx>
ddd<idx...> operator-(int const& a, ddd<idx...> const& b) { return ddd<idx...>(-b) += a; }
template<int... idx>
ddd<idx...> operator+(int const& a, ddd<idx...> const& b) { return ddd<idx...>(b) += a; }
template<int... idx>
ddd<idx...> operator*(int const& a, ddd<idx...> const& b) { return ddd<idx...>(b) *= a; }
/*--------------------------------------------------------------------------*/
template<int n, int... idx>
ddd<idx...> operator/(ddouble_<n> const& a, ddd<idx...> const& b) { return ddd<idx...>(b).rdiv(a); }
template<int n, int... idx>
ddd<idx...> operator-(ddouble_<n> const& a, ddd<idx...> const& b) { return ddd<idx...>(-b) += a; }
template<int n, int... idx>
ddd<idx...> operator+(ddouble_<n> const& a, ddd<idx...> const& b) { return ddd<idx...>(b) += a; }
template<int n, int... idx>
ddd<idx...> operator*(ddouble_<n> const& a, ddd<idx...> const& b) { return ddd<idx...>(b) *= a; }
/*--------------------------------------------------------------------------*/
template<int... idx>
ddd<idx...> operator+(PARAMETER<double> const& a, ddd<idx...> const& b) { return b + double(a); }
template<int... idx>
ddd<idx...> operator-(PARAMETER<double> const& a, ddd<idx...> const& b) { return (-b) + double(a); }
template<int... idx>
ddd<idx...> operator*(PARAMETER<double> const& a, ddd<idx...> const& b) { return b * double(a); }
template<int... idx>
ddd<idx...> operator/(PARAMETER<double> const& a, ddd<idx...> const& b) { untested(); return b.rdiv(double(a)); }
template<int... idx>
/*--------------------------------------------------------------------------*/
ddd<idx...> operator*(PARAMETER<int> const& a, ddd<idx...> const& b) { return b * double(a); }
template<int... idx>
ddd<idx...> operator+(PARAMETER<int> const& a, ddd<idx...> const& b) { untested(); return b + double(a); }
template<int... idx>
ddd<idx...> operator-(PARAMETER<int> const& a, ddd<idx...> const& b) { untested(); return (-b) + double(a); }
template<int... idx>
ddd<idx...> operator/(PARAMETER<int> const& a, ddd<idx...> const& b) { untested(); return b.rdiv(double(a)); }
/*--------------------------------------------------------------------------*/
template<class T>
T& set_value(T& t, double const& d)
{
  t.set_value(d);
  return t;
}

template<>
inline int& set_value(int& t, double const& v)
{
  return t = int(v);
}

template<>
inline double& set_value(double& t, double const& v)
{
  return t = v;
}
  template<>
  inline PARAMETER<int>& set_value(PARAMETER<int>& t, double const& v)
{ untested();
  t = int(v);
  return t;
}
  template<>
  inline PARAMETER<double>& set_value(PARAMETER<double>& t, double const& v)
{
  t = v;
  return t;
}

template<class T>
T& chain(T& t, double const& d)
{
  t.chain(d);
  return t;
}
template<>
inline double& chain(double& t, double const&)
{
  // no-op
  return t;
}
template<>
inline int& chain(int& t, double const&)
{
  // no-op
  return t;
}
template<>
inline PARAMETER<double>& chain(PARAMETER<double>& t, double const&)
{
  // no-op
  return t;
}
template<>
inline PARAMETER<int>& chain(PARAMETER<int>& t, double const&)
{ untested();
  // no-op
  return t;
}
/*--------------------------------------------------------------------------*/
class array_{
  std::vector<double> _v;
public:
  array_() {}
  array_(double a0) {
    _v.push_back(a0);
  }
  array_(double a0, double a1) {
    _v.push_back(a0);
    _v.push_back(a1);
  }
  template<typename... Args>
  array_(Args... args) {
    push_back(args...);
  }

  size_t size() const{return _v.size();}
  double const& operator[](int x)const {return _v[x];}
private:
  template<typename T>
  void push_back(T t) {
    _v.push_back(t);
  }
  template<typename T, typename... Args>
  void push_back(T t, Args... args) {
    _v.push_back(t);
    push_back(args...);
  }
};
/*--------------------------------------------------------------------------*/
template<class A, class T>
class wrap_arg{
   A _a;
   T& _d;
public:
   wrap_arg(T& d) : _d(d) { _a = d; }
   operator A&() { return _a; }
   ~wrap_arg() { _d = std::move(T(_a)); }
};
/*--------------------------------------------------------------------------*/
template<class S, class T>
wrap_arg<S, T> io_arg(S, T& d) { return wrap_arg<S, T>(d); }
/*--------------------------------------------------------------------------*/
} // namespace
namespace va {
// some builtin numerical functions according to verilog standard
// TODO: move to plugins, like the others.
/*--------------------------------------------------------------------------*/
template<class T>
T fmod(T d, T e)
{ untested();
  return std::fmod(d, e);
}
/*--------------------------------------------------------------------------*/
inline double fmod(double d, int e)
{ untested();
  return std::fmod(d, e);
}
/*--------------------------------------------------------------------------*/
inline int fmod(int d, int e)
{ untested();
  return d % e;
}
/*--------------------------------------------------------------------------*/
inline int fmod(PARAMETER<int> const& d, int e)
{itested();
  return d % e;
}
/*--------------------------------------------------------------------------*/
template<class T, class S, class X=void>
struct ddouble_if{
  typedef T type;
};
/*--------------------------------------------------------------------------*/
template<class T, class S>
struct ddouble_if<S, T, typename std::enable_if< std::is_same<typename T::base_tag, ddouble_tag>::value >::type > {
  // typedef typename T::base type;
  typedef T type;
};
/*--------------------------------------------------------------------------*/
template<class T, class S>
struct ddouble_if<T, S, typename T::base> {
  typedef typename T::base type;
};
/*--------------------------------------------------------------------------*/
template<class T, class S>
struct ddouble_if<T, S, typename S::base> {
  typedef typename S::base type;
};
/*--------------------------------------------------------------------------*/
template<class S>
struct ddouble_if<PARAMETER<double>, S> {
  typedef double type;
};
/*--------------------------------------------------------------------------*/
template<>
struct ddouble_if<PARAMETER<double>, double> {
  typedef double type;
};
/*--------------------------------------------------------------------------*/
template<>
struct ddouble_if<PARAMETER<int>, double> {
  typedef double type;
};
/*--------------------------------------------------------------------------*/
template<>
struct ddouble_if<PARAMETER<int>, int> {
  typedef double type;
};
/*--------------------------------------------------------------------------*/
template<class T, int a>
struct ddouble_if<T, ddouble_<a>>{
  typedef ddouble_<a> type;
};
/*--------------------------------------------------------------------------*/
template<>
struct ddouble_if<double, double>{
  typedef double type;
};
/*--------------------------------------------------------------------------*/
template<class T>
struct ddouble_if<T, double>{
  typedef typename ddouble_if<double, T>::type type;
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
template<class T, class S=T>
T white_noise(T, S=0.)
{ itested();
  //incomplete();
  return T(0.);
}
/*--------------------------------------------------------------------------*/
template<class T, class T2, class S=T>
T flicker_noise(T, T2, S=0.)
{ itested();
  //incomplete();
  return T(0.);
}
/*--------------------------------------------------------------------------*/
class EVT{
public:
  virtual void operator()() const = 0;
};
/*--------------------------------------------------------------------------*/
} // va
/*--------------------------------------------------------------------------*/
class FILTER {
public:
   virtual double read_filter()const { untested();unreachable(); return 0;}
};
/*--------------------------------------------------------------------------*/
namespace va {
/*--------------------------------------------------------------------------*/
inline double PORT_FLOW(int i, BASE_SUBCKT const* m)
{
  incomplete();
  node_t const& n = m->n_(i);
  double I(0.);
  assert(m->subckt());
  for(CARD const* c : *m->subckt()){
    auto e = dynamic_cast<ELEMENT const*>(c);
    if(!e){ untested();
    }else if(e->n_(1).m_() == e->n_(0).m_()){ untested();
    }else if(e->n_(0).m_() == n.m_()){
      I+= e->tr_amps();
    }else if(e->n_(1).m_() == n.m_()){
      I-= e->tr_amps();
    }else{ untested();
    }
  }
  return I;
}
/*--------------------------------------------------------------------------*/
inline node_t& ground()
{
  assert(CARD_LIST::card_list.nodes());
 // assert(CARD_LIST::card_list.nodes()->at(0).m_()==0);
  return CARD_LIST::card_list.nodes()->at(0);
}
/*--------------------------------------------------------------------------*/
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
/*--------------------------------------------------------------------------*/
// vim:ts=8:sw=2:noet
