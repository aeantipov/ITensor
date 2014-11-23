//
// Distributed under the ITensor Library License, Version 1.0.
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_ITENSOR_H
#define __ITENSOR_ITENSOR_H
#include "itdata_functions.h"


namespace itensor {

//
// ITensor
//
class ITensor
    {
    public:

    using storage = ITData;
    using storage_ptr = PData;
    using IndexT = Index;
    using IndexValT = IndexVal;

    //
    // Constructors
    //

    //Construct Null ITensor, ITensor will evaluate to false in boolean context
    ITensor();

    //Construct rank 1 ITensor, all entries set to zero
    explicit
    ITensor(const Index& i1);

    //Construct rank 2 ITensor, all entries set to zero
    ITensor(const Index& i1,
            const Index& i2);

    //Construct rank n ITensor, all entries set to zero
    template <typename... Indices>
    ITensor(const Index& i1, 
            const Index& i2, 
            const Index& i3, 
            const Indices&... rest);

    //Construct rank 0 ITensor (scalar), value set to val
    explicit
    ITensor(Real val);
    explicit
    ITensor(Complex val);

    //Construct rank 1 ITensor,
    //elements given by VectorRef V
    ITensor(const Index& i1,
            const VectorRef& V);

    //Construct rank 2 ITensor,
    //diagonal given by VectorRef V
    ITensor(const Index& i1,
            const Index& i2,
            const VectorRef& V);

    //
    // Accessor Methods
    //

    //Rank of this ITensor (number of indices)
    int 
    r() const { return is_.r(); }

    //Access IndexSet<Index>
    const IndexSet<Index>&
    inds() const { return is_; }

    //evaluate to false if ITensor is default constructed
    explicit operator bool() const { return bool(store_); }

    template <typename... IndexVals>
    Real
    real(const IndexVals&... ivs) const;

    template <typename... IndexVals>
    Complex
    cplx(const IndexVals&... ivs) const;

    template<typename... IndexVals>
    void
    set(Real val, const IndexVals&... ivs);

    template<typename... IndexVals>
    void
    set(Complex val, const IndexVals&... ivs);

    //
    // Operators
    //

    //Contracting product
    //All matching Index pairs automatically contracted
    //Cji = \sum_{k,l} Akjl * Blki
    //ITensor& 
    //operator*=(const ITensor& other);

    //Multiplication and division by scalar
    ITensor& 
    operator*=(Real fac);

    ITensor& 
    operator/=(Real fac);

    ITensor& 
    operator*=(Complex z);

    ITensor& 
    operator/=(Complex z);

    ITensor
    operator-() const;

    //Tensor addition and subtraction
    //Summands must have same Indices, in any order
    //Cijk = Aijk + Bkij
    //ITensor& 
    //operator+=(const ITensor& other);

    //ITensor& 
    //operator-=(const ITensor& other);

    //
    // Index Prime Level Methods
    //

    //Set primeLevel of Indices to zero
    ITensor& 
    noprime(IndexType type = All);

    //Set primeLevel of Index I to zero
    ITensor& 
    noprime(const Index& I);

    //Increase primeLevel of Indices by 1 (or optional amount inc)
    ITensor& 
    prime(int inc = 1);

    //Increase primeLevel of Indices by 1 (or optional amount inc)
    ITensor& 
    prime(IndexType type, int inc = 1);

    //Increase primeLevel of Index I by 1 (or optional amount inc)
    ITensor& 
    prime(const Index& I, int inc = 1);

    //Change all Indices having primeLevel plevold to have primeLevel plevnew
    ITensor& 
    mapprime(int plevold, int plevnew, IndexType type = All);

    //
    // Element Transformation Methods
    //

    ITensor&
    fill(Real r);

    ITensor&
    fill(Complex z);

    template <typename Func>
    ITensor&
    generate(Func&& f);

    template <typename Func>
    ITensor&
    apply(Func&& f);

    template <typename Func>
    const ITensor&
    visit(Func&& f) const;

    private:

    //Disattach self from current ITData and create own copy instead.
    //Necessary because ITensors logically represent distinct
    //tensors even though they may share data (copy-on-write idiom)
    void 
    solo();

    void
    scaleOutNorm();

    void
    equalizeScales(ITensor& other);

    public:

    //
    // Developer / advanced methods
    //
    // The following methods should not
    // be needed for most user code.
    //

    //Construct by explicitly providing data members
    ITensor(IndexSet<Index>&& iset,
            NewData nd,
            LogNumber scale);

    //Scale factor, used internally for efficient scalar ops.
    //Mostly for developer use, not necessary to explicitly involve
    //scale factors in most ITensor operations.
    const LogNumber&
    scale() const { return scale_; }

    const ITData&
    data() const { return *store_; }

    void 
    scaleTo(const LogNumber& newscale);
    
    private:

    ////////////////////////
    IndexSet<Index> is_;
    LogNumber scale_;
    storage_ptr store_;
    ////////////////////////

    }; // class ITensor

//template <typename T_>
//class Elements
//    {
//    public:
//
//    using tensor_type = simpletensor<T_>;
//    using value_type = typename tensor_type::value_type;
//    using reference = typename tensor_type::reference;
//    using const_reference = typename tensor_type::const_reference;
//    using iterator = typename tensor_type::iterator;
//    using const_iterator = typename tensor_type::const_iterator;
//    using size_type = typename tensor_type::size_type;
//
//    template <typename... Indices>
//    Elements(const ITensor& t,
//             const Index& i0, 
//             const Index& i1, 
//             const Indices&... rest);
//
//    const IndexSet<Index>&
//    inds() const { return is_; }
//
//    size_type
//    rank() const { return d_.rank(); }
//
//    size_type
//    r() const { return d_.rank(); }
//
//    const_iterator
//    begin() const { return cbegin(); }
//    const_iterator
//    end() const { return cend(); }
//
//    const_iterator
//    cbegin() const { return d_.cbegin(); }
//    const_iterator
//    cend() const { return d_.cend(); }
//
//    iterator
//    begin() { return d_.begin(); }
//    iterator
//    end() { return d_.end(); }
//
//    size_type
//    size() const { return d_.size(); }
//
//    bool
//    empty() const { return d_.empty(); }
//
//    template<typename index0, typename... _args>
//    const_reference
//    operator()(const index0& first, const _args&... rest) const
//        {
//        return d_(first,rest...);
//        }
//
//    template<typename index0, typename... _args>
//    reference
//    operator()(const index0& first, const _args&... rest)
//        {
//        return d_(first,rest...);
//        }
//
//    template <typename IndexOrdinal>
//    const_reference
//    operator[](const IndexOrdinal& index) const
//        {
//        return d_[index];
//        }
//
//    template <typename IndexOrdinal>
//    reference
//    operator[](const IndexOrdinal& index)
//        {
//        return d_[index];
//        }
//
//    const value_type*
//    data() const { return d_.data(); }
//
//    value_type*
//    data() { return d_.data(); }
//
//    const tensor_type&
//    toTensor() const { return d_; }
//
//    private:
//
//    //////////////
//
//    IndexSet<Index> is_;
//    tensor_type d_;
//
//    //////////////
//
//    template <typename T>
//    struct GetData
//        {
//        using storage = typename ITDense<T>::storage;
//        using ptr_type = const storage*;
//
//        GetData() : p_(nullptr) { }
//
//        operator storage const&() const { return *p_; }
//
//        NewData
//        operator()(const ITDense<T>& d)
//            {
//            p_ = &(d.t_);
//            return NewData();
//            }
//
//        template <typename OtherITData>
//        NewData
//        operator()(const OtherITData& d)
//            {
//            throw ITError("ITensor data not of type ITDense<T>");
//            return NewData();
//            }
//
//        private:
//        ptr_type p_;
//        };
//
//    }; // class Elements
//
//template <typename T_>
//template <typename... Indices>
//Elements<T_>::
//Elements(const ITensor& t,
//         const Index& i0, 
//         const Index& i1, 
//         const Indices&... rest)
//    {
//    const auto size = 2 + sizeof...(rest);
//    std::array<Index,size> inds = {{ i0, i1, static_cast<Index>(rest)...}};
//    is_ = IndexSet<Index>(inds,size);
//    
//    std::array<int,size> perm;
//    detail::calc_permutation(t.inds(),inds,perm);
//
//    d_ = applyFunc<GetData<T_>>(t.data());
//    d_ = btas::permute(d_,perm);
//
//    const T_ fac = t.scale().real0(); 
//    btas::scal(fac,d_);
//    }
//
//
//template <typename value_type>
//std::ostream&
//operator<<(std::ostream& s, const Elements<value_type>& els)
//    {
//    const auto& T = els.toTensor();
//    s << "Elements r = " << T.rank() << ": ";
//    s << els.inds() << "\n";
//    for(auto x : T.range()) 
//        {
//        s << "  " << x << " ";
//        const auto val = T(x);
//        if(fabs(val) > 1E-10)
//            s << val << "\n";
//        else
//            s << format("%.8E\n",val);
//        }
//    return s;
//    }

template <typename... Indices>
ITensor::
ITensor(const Index& i1, 
        const Index& i2,
        const Index& i3,
        const Indices&... rest)
    :
    is_(i1,i2,i3,rest...),
    scale_(1.),
    store_(make_shared<ITDense<Real>>(is_))
	{ }

template <typename... IndexVals>
Complex ITensor::
cplx(const IndexVals&... ivs) const
    {
#ifdef DEBUG
    if(!*this) Error("ITensor is default constructed");
#endif
    static constexpr auto size = sizeof...(ivs);
    const std::array<IndexVal,size> vals = {{ static_cast<IndexVal>(ivs)...}};
    std::array<long,size> inds;
    detail::permute_map(is_,vals,inds,[](const IndexVal& iv) { return iv.i-1; });
    auto g = applyFunc<GetElt<Complex,size>>(store_,{inds});
	try {
	    return Complex(g)*scale_.real(); 
	    }
	catch(const TooBigForReal& e)
	    {
	    println("too big for real in cplx(...), scale = ",scale());
	    throw e;
	    }
	catch(TooSmallForReal)
	    {
        println("warning: too small for real in cplx(...)");
	    return Complex(0.,0.);
	    }
    return Complex(NAN,NAN);
    }


template <typename... IndexVals>
Real ITensor::
real(const IndexVals&... ivs) const
    {
    auto z = cplx(ivs...);
    if(fabs(z.imag()) != 0)
        {
        printfln("element = (%.5E,%.5E)",z.real(),z.imag());
        Error("ITensor is Complex-valued, use .cplx(...) method");
        }
    return z.real();
    }

template <typename... IndexVals>
void ITensor::
set(Real val, const IndexVals&... ivs)
    {
    static constexpr auto size = sizeof...(ivs);
    scaleTo(1.);
    const std::array<IndexVal,size> vals = {{ static_cast<IndexVal>(ivs)...}};
    std::array<int,size> inds;
    detail::permute_map(is_,vals,inds,[](const IndexVal& iv) { return iv.i-1; });
    applyFunc<SetEltReal<size>>(store_,{val,inds});
    }

template <typename... IndexVals>
void ITensor::
set(Complex val, const IndexVals&... ivs)
    {
    static constexpr auto size = sizeof...(ivs);
    scaleTo(1.);
    const std::array<IndexVal,size> vals = {{ static_cast<IndexVal>(ivs)...}};
    std::array<int,size> inds;
    detail::permute_map(is_,vals,inds,[](const IndexVal& iv) { return iv.i-1; });
    applyFunc<SetEltComplex<size>>(store_,{val,inds});
    }

//template <typename T, typename... inds>
//T& ITensor::
//elt(int i1, inds... rest)
//    {
//    const auto size = 1 + sizeof...(rest);
//    const std::array<int,size> inds = {{ i0, i1, static_cast<int>(rest)...}};
//    T* p = nullptr;
//    applyFunc(GetElt<T,size>(inds,p),store_);
//    return *p;
//    }

template <typename Func>
ITensor& ITensor::
generate(Func&& f)
    {
    solo();
    scaleTo(1);
    applyFunc<GenerateIT<decltype(f)>>(store_,{std::forward<Func>(f)});
    return *this;
    }

template <typename Func>
ITensor& ITensor::
apply(Func&& f)
    {
    solo();
    scaleTo(1);
    applyFunc<ApplyIT<decltype(f)>>(store_,{std::forward<Func>(f)});
    return *this;
    }

template <typename Func>
const ITensor& ITensor::
visit(Func&& f) const
    {
    applyFunc<VisitIT<decltype(f)>>(store_,{std::forward<Func>(f),scale()});
    return *this;
    }

std::ostream& 
operator<<(std::ostream & s, const ITensor& T);

//ITensor inline
//operator*(ITensor A, const ITensor& B) { A *= B; return A; }
ITensor inline
operator*(ITensor T, Real fac) { T *= fac; return T; }
ITensor inline
operator*(Real fac, ITensor T) { T *= fac; return T; }
ITensor inline
operator*(ITensor T, Complex fac) { T *= fac; return T; }
ITensor inline
operator*(Complex fac, ITensor T) { T *= fac; return T; }
ITensor inline
operator/(ITensor T, Real fac) { T /= fac; return T; }
ITensor inline
operator/(ITensor T, Complex fac) { T /= fac; return T; }
//ITensor inline
//operator+(ITensor A, const ITensor& B) { A += B; return A; }
//ITensor inline
//operator-(ITensor A, const ITensor& B) { A -= B; return A; }

//Return copy of ITensor with primeLevel of Index I increased by 1
//(or optional amount inc)
template <class Tensor, class IndexT>
Tensor
prime(Tensor A, const IndexT& I, int inc = 1)
    { 
    A.prime(I,inc); 
    return A; 
    }

//Return copy of ITensor with primeLevel of Index I set to zero
template <class Tensor, class IndexT>
Tensor
noprime(Tensor A, const IndexT& I)
    { 
    A.noprime(I); 
    return A; 
    }

template<class Tensor>
bool
hasindex(const Tensor& T, const typename Tensor::IndexT& I)
    {
    return detail::contains(T.inds(),I);
    }

ITensor
randIT(ITensor T, const OptSet& opts = Global::opts());

template <typename... Indices>
ITensor
randIT(const Index& i1, const Indices&... rest)
    {
    return randIT(ITensor(i1,rest...));
    }

template <typename... Indices>
ITensor
tieIndex(const ITensor& T,
         const Index& t0,
         const Index& t1,
         const Indices&... rest);

//Compute the norm of an ITensor.
//Thinking of elements as a vector, equivalent to sqrt(v*v).
//Result is equivalent to sqrt((T*T).real()) 
//[and similar for complex case] but computed much more efficiently.
Real 
norm(const ITensor& T);

ITensor
conj(const ITensor& T);

ITensor inline
dag(const ITensor& T) { return conj(T); }

bool
isComplex(const ITensor& T);

Real
sumels(const ITensor& T);



//
// Template Method Implementations
//

//template <typename... Indices>
//ITensor
//tieIndex(const ITensor& T,
//         const Index& t0,
//         const Index& t1,
//         const Indices&... rest)
//    {
//    static constexpr auto size = 2 + sizeof...(rest);
//    if(size > T.r()) Error("Cannot tie more indices than ITensor rank.");
//    std::array<Index,size> totie = {{ t0, t1, static_cast<Index>(rest)...}};
//    std::array<size_t,size> I;
//    NewIndexSet<Index> new_index(T.r()-size+1);
//    size_t nt = 0;
//    for(int j = 0; j < T.r(); ++j)
//        {
//        const auto& J = T.inds()[j];
//        if(detail::contains(totie,J))
//            {
//            if(J == totie.front()) new_index.add(J);
//            I[nt++] = j;
//            }
//        else
//            {
//            new_index.add(J);
//            }
//        }
//    if(nt != totie.size())
//        Error("ITensor does not have requested Index to tie");
//
//    auto nd = T.data().clone();
//    const auto f = [&I](const btas::Range& r) { return tieIndex(r,I); };
//    applyFunc<ApplyRange<decltype(f)>>(nd,{f});
//
//    return ITensor(new_index,std::move(nd),T.scale());
//    }

}; //namespace itensor


#endif
