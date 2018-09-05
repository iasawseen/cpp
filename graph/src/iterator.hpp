//
// Created by ivan on 10.04.17.
//

#ifndef GRAPH_ITERATOR_HPP
#define GRAPH_ITERATOR_HPP

#include <functional>

template<class value_type>
class DefaultPred {
public:
    bool operator()(value_type const&) {
        return true;
    }
};

template<class underline_iterator>
class VertexDereference {
    using value_type = typename underline_iterator::value_type::element_type;

protected:
    const value_type& deref(underline_iterator const& it_) const {
        return **it_;
    }
};

template<class underline_iterator>
class Dereference {
    using value_type = typename underline_iterator::value_type;

protected:
    const value_type& deref(underline_iterator const& it_) const {
        return *it_;
    }

    value_type& deref(underline_iterator& it_) {
        return *it_;
    }
};

template<class underline_iterator, class data_type, class vertex_iterator>
class EdgePolicy {
protected:
    using iter_value_type = typename underline_iterator::value_type;
    using value_type = data_type;
    using reference = value_type&;
    using pointer = value_type*;

    virtual iter_value_type& dereference(underline_iterator& it) = 0;
    virtual const iter_value_type& dereference(underline_iterator const& it) const = 0;

    underline_iterator it_;
    static const bool is_const = false;

public:
    EdgePolicy() : it_() {}
    EdgePolicy(underline_iterator const& it) : it_(it) {}

    data_type& operator*() { return dereference(it_).second.data(); }
    data_type* operator->() { return &dereference(it_).second.data(); }

    vertex_iterator from() const {
        return dereference(it_).second.from();
    }

    vertex_iterator to() const {
        return dereference(it_).second.to();
    }

};

template<class underline_iterator, class data_type, class vertex_iterator>
class ConstEdgePolicy {
protected:
    using iter_value_type = typename underline_iterator::value_type;
    using value_type = data_type;
    using reference = const value_type&;
    using pointer = const value_type*;

    virtual const iter_value_type& dereference(underline_iterator const& it) const = 0;
    underline_iterator it_;
    static const bool is_const = true;

public:
    ConstEdgePolicy() : it_() {}
    ConstEdgePolicy(underline_iterator const& it) : it_(it) {}


    const data_type& operator*() const { return dereference(it_).second.data(); }
    const data_type* operator->() const { return &dereference(it_).second.data(); }

    vertex_iterator from() const {
        return dereference(it_).second.from();
    }

    vertex_iterator to() const {
        return dereference(it_).second.to();
    }
};

template<class const_underline_iterator, class val_type>
class VertexPolicy {
protected:
    using value_type = val_type;

    using iter_value_type = value_type;
    using reference = const value_type&;
    using pointer = const value_type*;

    virtual const value_type& dereference(const_underline_iterator const& it) const = 0;
    const_underline_iterator it_;
    static const bool is_const = true;

public:
    VertexPolicy() : it_() {}
    VertexPolicy(const_underline_iterator const& it) : it_(it) {}

    const value_type& operator*() const { return dereference(it_); }
    const value_type* operator->() const { return &dereference(it_); }
};

template<class const_underline_iterator, class vertex_iterator>
class FilteredEdgePolicy {
protected:
    using value_type = typename const_underline_iterator::value_type;
    using iter_value_type = value_type;
    using reference = const value_type&;
    using pointer = const value_type*;

    virtual const value_type& dereference(const_underline_iterator const& it) const = 0;
    const_underline_iterator it_;
    static const bool is_const = true;

public:
    FilteredEdgePolicy() : it_() {}
    FilteredEdgePolicy(const_underline_iterator const& it) : it_(it) {}

    const value_type& operator*() const { return dereference(it_); }
    const value_type* operator->() const { return &dereference(it_); }

    vertex_iterator from() const {
        return it_.from();
    }

    vertex_iterator to() const {
        return it_.to();
    }
};

template<class underline_iterator,
        class CorePolicy,
        class WorkingPolicy,
        class Predicate>
struct advanced_iter
        : public CorePolicy, WorkingPolicy
{
    using difference_type = size_t;
    using iter_value_type = typename CorePolicy::iter_value_type;
    using value_type = typename CorePolicy::value_type;
    using reference = typename CorePolicy::reference;
    using pointer = typename CorePolicy::pointer;
    using iterator_category = std::forward_iterator_tag;

    using WorkingPolicy::deref;
    using CorePolicy::it_;
    using CorePolicy::is_const;
    using predicate = std::function<bool(underline_iterator)>;

    advanced_iter()
            : CorePolicy()
    {}

    advanced_iter(underline_iterator const& it, predicate pred = Predicate())
            : CorePolicy(it),
            pred_(pred)
    {}

    advanced_iter(advanced_iter const& other) {
        it_ = other.it_;
        pred_ = other.pred_;
    }


    template<class other_iter>
    advanced_iter(other_iter const& other,
                  typename std::enable_if<!other_iter::is_const>::type* = 0) {
        it_ = other.it_;
    }

    bool operator==(advanced_iter const& other) const {
        return it_ == other.it_;
    }

    bool operator!=(advanced_iter const& rhs) const {
        return !(*this == rhs);
    }

    advanced_iter& operator++() {
        increment(it_);
        return *this;
    }

    advanced_iter operator++(int) {
        advanced_iter tmp = *this;
        increment(it_);
        return tmp;
    }

    const iter_value_type& dereference(underline_iterator const& it) const {
        return deref(it);
    }

    iter_value_type& dereference(underline_iterator& it) {
        return deref(it);
    }

private:
    predicate pred_;

    void increment(underline_iterator& it) {
        ++it;
        while (!pred_(it)) {
            ++it;
        }
    }
};

#endif //GRAPH_ITERATOR_HPP