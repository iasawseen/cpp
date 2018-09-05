//
// Created by ivan on 26.04.17.
//

#ifndef VARIANT_VARIANT_H
#define VARIANT_VARIANT_H

#include <iostream>
#include <limits>
#include <memory>
#include <typeindex>

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
        using first_type = typename detail::get_type_by_index<0, Types...>::type;

        static_assert(detail::all_unique<Types...>::value, "there are duplicate types in template parameters");

        using storage_type = std::aligned_union_t<0, Types...>;
        static const size_t invalid_index = au::invalid;

        variant() {
            index_ = invalid_index;
        }

        template<class T>
        explicit variant(T const& t) {
            variant<Types...> tmp;
            tmp.construct_from_obj(t);

            *this = std::move(tmp);
        }

        template<class T>
        explicit variant(T& t) : variant((T const&)t) {}

        template<class T>
        explicit variant(T&& t) {
            move_from_obj(std::move(t));
        }

        template<class T>
        variant& operator=(T const& t) {
            variant<Types...> tmp;
            tmp.construct_from_obj(t);

            if (index_ != invalid_index) {
                destroy(index_, &storage_);
            }

            *this = std::move(tmp);

            return *this;
        }

        template<class T>
        variant& operator=(T& t) {
            variant<Types...> tmp;
            tmp.construct_from_obj(t);

            if (index_ != invalid_index) {
                destroy(index_, &storage_);
            }

            *this = std::move(tmp);

            return *this;
        }

        template<class T>
        variant& operator=(T&& t) {
            if (index_ != invalid_index) {
                destroy(index_, &storage_);
            }

            move_from_obj(std::move(t));

            return *this;
        }

        variant(variant const& other) {
            variant<Types...> tmp;

            tmp.cur_type = copy(other.index_, &other.storage_, &tmp.storage_);
            tmp.index_ = other.index_;

            *this = std::move(tmp);
        }

        variant(variant& other) {
            variant<Types...> tmp;

            tmp.cur_type = copy(other.index_, &other.storage_, &tmp.storage_);
            tmp.index_ = other.index_;

            *this = std::move(tmp);
        }

        variant(variant&& other) {
            move_from_variant(std::move(other));
        }

        variant& operator=(variant other) {
            move_from_variant(std::move(other));
            return *this;
        }

        ~variant() {
            if (index_ != invalid_index) {
                destroy(index_, &storage_);
            }
        }

        size_t which() const{
            return index_;
        }

        bool empty() {
            return index_ == invalid_index;
        }

        void clear() {
            if (index_!= invalid_index) {
                destroy(index_, &storage_);
            }
            index_ = invalid_index;
        }

        template<class T, class... SomeTypes>
        friend T const& get(variant<SomeTypes...> const& var);

        template<size_t index, class... SomeTypes>
        friend typename detail::get_type_by_index<index, SomeTypes...>::type const&
        get(variant<SomeTypes...> const& var);

        template<class T, class... SomeTypes>
        friend T& get(variant<SomeTypes...>& var);

        template<size_t index, class... SomeTypes>
        friend typename detail::get_type_by_index<index, SomeTypes...>::type&
        get(variant<SomeTypes...>& var);

        template<class T, class... SomeTypes>
        friend T const* get(variant<SomeTypes...> const* var);

        template<class T, class... SomeTypes>
        friend T* get(variant<SomeTypes...>* var);

    private:
        template<class T>
        void construct_from_obj(T const& t) {
            static_assert(detail::is_convertable<T, Types...>::value, "type is not presented in variant types");
            size_t new_index = detail::is_convertable<T, Types...>::index;
            using Type = typename detail::get_type_by_index<detail::is_convertable<T, Types...>::index, Types...>::type;

            std::type_index new_type = std::type_index(typeid(Type));
            std::swap(cur_type, new_type);

            new ((Type*)&storage_) Type(t);

            index_ = new_index;
        }

        template<class T>
        void move_from_obj(T&& t) {
            static_assert(detail::is_convertable<T, Types...>::value, "type is not presented in variant types");
            size_t new_index = detail::is_convertable<T, Types...>::index;
            using Type = typename detail::get_type_by_index<detail::is_convertable<T, Types...>::index, Types...>::type;

            std::type_index new_type = std::type_index(typeid(Type));
            std::swap(cur_type, new_type);

            new ((Type*)&storage_) Type(std::move(t));

            index_ = new_index;
        }

        void move_from_variant(variant&& other) {
            std::type_index tmp_index(typeid(first_type));

            move(tmp_index, other.index_, &other.storage_, &storage_);

            std::swap(cur_type, tmp_index);
            std::swap(index_, other.index_);

            other.index_ = invalid_index;
        }

        static std::type_index copy(size_t index, const storage_type* from, storage_type* to) {
            std::type_index type(typeid(first_type));

            using func_type = std::function<void(std::type_index& type, const storage_type*, storage_type*)>;
            static const func_type copy_constructors[sizeof...(Types)] = {&variant::copy_<Types>...};

            copy_constructors[index](type, from, to);

            return type;
        }

        static void move(std::type_index& type, size_t index, storage_type* from, storage_type* to) {

            using func_type = std::function<void(std::type_index& type, storage_type*, storage_type*)>;
            static const func_type move_constructors[sizeof...(Types)] = {&variant::move_<Types>...};

            move_constructors[index](type, from, to);
        }

        static void destroy(size_t index, storage_type* ptr) {
            using func_type = std::function<void(storage_type*)>;
            static const func_type destructors[sizeof...(Types)] = {&variant::destroy_<Types>...};

            destructors[index](ptr);
        }

        template<class T>
        static void copy_(std::type_index& type, const storage_type* from, storage_type* to) {
            T* dest = reinterpret_cast<T*>(to);
            const T* source = reinterpret_cast<const T*>(from);
            type = std::type_index(typeid(T));
            new (dest) T(*source);
        }

        template<class T>
        static void move_(std::type_index& type, storage_type* from, storage_type* to) {
            T* dest = reinterpret_cast<T*>(to);
            T* source = reinterpret_cast<T*>(from);
            type = std::type_index(typeid(T));
            new (dest) T(std::move(*source));
        }

        template<class T>
        static void destroy_(storage_type* ptr) {
            T* casted_ptr = reinterpret_cast<T*>(ptr);
            casted_ptr->~T();
        }

        template<size_t index>
        auto& cast_variant_to() {
            using Type = typename detail::get_type_by_index<index, Types...>::type;
            return *(Type*)(&storage_);
        }

        template<class T>
        auto& cast_variant_to() {
            return *(T*)(&storage_);
        }

        template<size_t index>
        auto const& cast_variant_to() const {
            using Type = typename detail::get_type_by_index<index, Types...>::type;
            return *(Type*)(&storage_);
        }

        template<class T>
        auto const& cast_variant_to() const{
            return *(T*)(&storage_);
        }

        std::type_index cur_type = std::type_index(typeid(first_type));
        size_t index_;
        storage_type storage_;
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
        if (std::type_index(typeid(T)) != var.cur_type || var.index_ == invalid) {
            throw bad_get();
        }
        return var.template cast_variant_to<T>();
    }

    template<class T, class... Types>
    T* get(variant<Types...>* var) {
        static T* null = nullptr;
        if (std::type_index(typeid(T)) != var->cur_type || var->index_ == invalid) {
            return null;
        }
        return std::addressof(var->template cast_variant_to<T>());
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
        if (std::type_index(typeid(T)) != var.cur_type || var.index_ == invalid) {
            throw bad_get();
        }
        return var.template cast_variant_to<T>();
    }

    template<class T, class... Types>
    T const* get(variant<Types...> const* var) {
        static const T* null = nullptr;
        if (std::type_index(typeid(T)) != var->cur_type || var->index_ == invalid) {
            return null;
        }
        return std::addressof(var->template cast_variant_to<T>());
    }

    template<class... Types>
    void swap(variant<Types...>& lhs, variant<Types...>& rhs) {
        au::variant<Types...> tmp{std::move(lhs)};
        lhs = std::move(rhs);
        rhs = std::move(tmp);
    }
}


#endif //VARIANT_VARIANT_H
