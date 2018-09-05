//
// Created by ivan on 02.05.17.
//

#ifndef VARIANT_OLD_HPP
#define VARIANT_OLD_HPP

//
// Created by ivan on 26.04.17.
//

#ifndef VARIANT_VARIANT_H
#define VARIANT_VARIANT_H

#include <iostream>
#include <limits>
#include <memory>

namespace au {

    static constexpr size_t invalid = std::numeric_limits<size_t>::max();

    namespace detail {
        template<class T, size_t index, class... Tail>
        struct is_presented {};

        template<class T, size_t index, class Head, class... Tail>
        struct is_presented<T, index, Head, Tail...> {
            static constexpr bool value = std::is_same<T, Head>::value ? true : is_presented<T, index + 1, Tail...>::value;
            static constexpr size_t index_ = std::is_same<T, Head>::value ? index : is_presented<T, index + 1, Tail...>::index_;

        };

        template<class T, size_t index, class Head>
        struct is_presented<T, index, Head> {
            static constexpr bool value = std::is_same<T, Head>::value;
            static constexpr size_t index_ = value ? index : invalid;

        };

        template<class T, size_t index, class... Tail>
        struct is_constructible_ {};

        template<class T, size_t index, class Head, class... Tail>
        struct is_constructible_<T, index, Head, Tail...> {
            static constexpr bool value = std::is_constructible<Head, T>::value ? true : is_constructible_<T, index + 1, Tail...>::value;
            static constexpr size_t index_ = std::is_constructible<Head, T>::value ? index : is_constructible_<T, index + 1, Tail...>::index_;

        };

        template<class T, size_t index, class Head>
        struct is_constructible_<T, index, Head> {
            static constexpr bool value = std::is_constructible<Head, T>::value;
            static constexpr size_t index_ = value ? index : invalid;

        };

        template<class T, class... Tail>
        struct is_convertable {
            static constexpr bool value = is_presented<T, 0, Tail...>::value ? true
                                                                             : is_constructible_<T, 0, Tail...>::value;

            static constexpr size_t index = is_presented<T, 0, Tail...>::value ? is_presented<T, 0, Tail...>::index_
                                                                               : is_constructible_<T, 0, Tail...>::index_;
        };

        template<size_t index, class... Types>
        struct get_type_by_index {};

        template<size_t index, class Head, class... Tail>
        struct get_type_by_index<index, Head, Tail...> {
            using type = typename get_type_by_index<index - 1, Tail...>::type;
        };

        template<class Head, class... Tail>
        struct get_type_by_index<0, Head, Tail...> {
            using type = Head;
        };

        template<class...>
        struct has_duplicates {};

        template<class Head>
        struct has_duplicates<Head> {
            static constexpr bool value = false;
        };

        template<class Pretender, class Head, class... Tail>
        struct has_duplicates<Pretender, Head, Tail...> {
            static constexpr bool value = std::is_same<Pretender, Head>::value
                                          || has_duplicates<Pretender, Tail...>::value;
        };

        template<class...>
        struct all_unique {};

        template<class Head>
        struct all_unique<Head> {
            static constexpr bool value = true;
        };

        template<class Head, class... Tail>
        struct all_unique<Head, Tail...> {
            static constexpr bool value = all_unique<Tail...>::value
                                          && !has_duplicates<Head, Tail...>::value;
        };
    }


    template<class... Types>
    struct variant {
        static_assert(detail::all_unique<Types...>::value, "!");

        using storage_type = std::aligned_union_t<0, Types...>;
        static const size_t invalid_index = au::invalid;

        variant() {
            index_ = invalid_index;
        }

        template<class T>
        explicit variant(T const& t) {
            construct_from_obj(t);
        }

        template<class T>
        variant& operator=(T const& t) {
            destroy(index_, storage_.get());
            construct_from_obj(t);
            return *this;
        }

        template<class T>
        void construct_from_obj(T const& t) {
            static_assert(detail::is_convertable<T, Types...>::value, "type is not presented in variant types");
            index_ = detail::is_convertable<T, Types...>::index;
            std::unique_ptr<storage_type> tmp = std::make_unique<storage_type>();

            using Type = typename detail::get_type_by_index<detail::is_convertable<T, Types...>::index, Types...>::type;
            new (tmp.get()) Type(t);
            cur_type_hash = typeid(Type).hash_code();
            std::swap(storage_, tmp);
        }

        variant(variant const& other) {
            std::unique_ptr<storage_type> tmp = std::make_unique<storage_type>();
            cur_type_hash = copy(other.index_, other.storage_.get(), tmp.get());
            index_ = other.index_;
            std::swap(storage_, tmp);
        }

        variant(variant&& other) {
            move_from_variant(std::move(other));
        }

        variant& operator=(variant other) {
            move_from_variant(std::move(other));
            return *this;
        }

        void move_from_variant(variant&& other) {
            std::unique_ptr<storage_type> tmp = std::make_unique<storage_type>();
            cur_type_hash = move(other.index_, other.storage_.get(), tmp.get());
            index_ = other.index_;

            std::swap(storage_, tmp);
        }

        ~variant() {
            if (index_ != invalid_index) {
                destroy(index_, storage_.get());
            }
        }

        static size_t copy(size_t index, const storage_type* from, storage_type* to) {
            using func_type = std::function<void(size_t& type, const storage_type*, storage_type*)>;
            size_t type;
            static const func_type copy_constructors[sizeof...(Types)] = {&variant::copy_<Types>...};

            copy_constructors[index](type, from, to);

            return type;
        }

        static size_t move(size_t index, const storage_type* from, storage_type* to) {
            size_t type;

            using func_type = std::function<void(size_t& type, const storage_type*, storage_type*)>;
            static const func_type move_constructors[sizeof...(Types)] = {&variant::copy_<Types>...};

            move_constructors[index](type, from, to);
            return type;

        }


        static void destroy(size_t index, storage_type* ptr) {
            using func_type = std::function<void(storage_type*)>;
            static const func_type destructors[sizeof...(Types)] = {&variant::destroy_<Types>...};

            destructors[index](ptr);
        }


        template<class T>
        static void copy_(size_t& type, const storage_type* from, storage_type* to) {
            T* dest = reinterpret_cast<T*>(to);
            const T* source = reinterpret_cast<const T*>(from);
            new (dest) T(*source);
            type = typeid(T).hash_code();
        }


        template<class T>
        static void move_(size_t& type, const storage_type* from, storage_type* to) {
            T* dest = reinterpret_cast<T*>(to);
            const T* source = reinterpret_cast<const T*>(from);
            new (dest) T(std::move(*source));
            type = typeid(T).hash_code();

        }

        template<class T>
        static void destroy_(storage_type* ptr) {
            T* casted_ptr = reinterpret_cast<T*>(ptr);

            casted_ptr->~T();
        }

        size_t which() const{
            return index_;
        }


        template<size_t index>
        auto& cast_variant_to() {
            using Type = typename detail::get_type_by_index<index, Types...>::type;
            return *(Type*)(storage_.get());
        }

        template<class T>
        auto& cast_variant_to() {
            return *(T*)(storage_.get());
        }

        template<size_t index>
        auto const& cast_variant_to() const {
            using Type = typename detail::get_type_by_index<index, Types...>::type;
            return *(Type*)(storage_.get());
        }

        template<class T>
        auto const& cast_variant_to() const{
            return *(T*)(storage_.get());
        }

        bool empty() {
            return index_ == invalid_index;
        }

        void clear() {
            std::unique_ptr<storage_type> tmp = std::make_unique<storage_type>();
            index_ = invalid_index;
            std::swap(storage_, tmp);
        }

        size_t cur_type_hash;


    private:


        size_t index_;
        std::unique_ptr<storage_type> storage_ = std::make_unique<storage_type>();
    };


    struct bad_get : std::exception {
        virtual const char* what() const throw()
        {
            return "bad get";
        }
    };


    template<size_t index, class... Types>
    typename detail::get_type_by_index<index, Types...>::type&
    get(variant<Types...>& var) {
        if (index != var.which()) {
            throw bad_get();
        }
        return var.template cast_variant_to<index>();
    }

    template<class T, class... Types>
    T& get(variant<Types...>& var) {
        if (typeid(T).hash_code() != var.cur_type_hash) {
            throw bad_get();
        }
        return var.template cast_variant_to<T>();
    }

    template<size_t index, class... Types>
    typename detail::get_type_by_index<index, Types...>::type const&
    get(variant<Types...> const& var) {
        if (index != var.which()) {
            throw bad_get();
        }
        return var.template cast_variant_to<index>();
    }

    template<class T, class... Types>
    T const& get(variant<Types...> const& var) {
        if (typeid(T).hash_code() != var.cur_type_hash) {
            throw bad_get();
        }
        return var.template cast_variant_to<T>();
    }

    template<class... Left, class... Right>
    void swap(variant<Left...>& lhs, variant<Right...>& rhs) {
        variant<Left...> tmp{std::move(lhs)};
        lhs = std::move(rhs);
        rhs = std::move(lhs);
    }
}

#endif //VARIANT_VARIANT_H


#endif //VARIANT_OLD_HPP
