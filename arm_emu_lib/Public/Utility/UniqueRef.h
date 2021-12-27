#if !defined(UNIQUEREF_H_INCLUDED_431D1EC2_8104_4B9F_9F86_8655208D90FF)
    #define UNIQUEREF_H_INCLUDED_431D1EC2_8104_4B9F_9F86_8655208D90FF

    #include <API/Api.h>
    #include <functional>
    #include <memory>

namespace arm_emu {

    /// @brief std::unique_ptr with a custom deleter as std::function<void(Ptr*)>
    /// @tparam Type Pointer type
    /// @tparam Del Deleter type
    template < class Type, class Del = std::function< void(Type*) > >
    using UniqueRef = std::unique_ptr< Type, Del >;

    /// @brief Allocate a unique_ptr using an allocator similar to std::allocate_shared
    /// @tparam Type Pointer type
    /// @tparam Allocator Allocator type
    /// @tparam ...TArgs Args Types for constructing the Type
    /// @param alloc allocator to be used
    /// @param ...args Args passed to construct the type
    /// @return UniqueRef<Type, std::function<void(Type*)>>
    template < class Type, class Allocator, class... TArgs, std::enable_if_t< !std::is_array_v< Type >, int > = 0 >
    UniqueRef< Type > allocate_unique(Allocator alloc, TArgs... args) {
        using allocator_type = std::allocator_traits< Allocator >::template rebind_alloc< Type >;

        auto custom_deleter = [](Type* ptr, allocator_type m_alloc) {
            static_assert(0 < sizeof(Type), "can't delete an incomplete type");
            std::allocator_traits< allocator_type >::destroy(m_alloc, ptr);
            m_alloc.deallocate(ptr, 1);
        };

        allocator_type type_alloc { alloc };

        Type* ptr = type_alloc.allocate(1);
        std::allocator_traits< allocator_type >::construct(alloc, ptr, std::forward< TArgs >(args)...);

        return UniqueRef< Type > { ptr, std::bind(custom_deleter, std::placeholders::_1, type_alloc) };
    }
    /// @brief Allocate a unique_ptr of a polymorphic type using an allocator similar to std::allocate_shared
    /// @tparam Type Base type of the pointer
    /// @tparam ConstructedType Actual type to be allocated
    /// @tparam Allocator Allocator type
    /// @tparam ...TArgs Args Types for constructing the Type
    /// @param alloc allocator to be used
    /// @param ...args Args passed to construct the type
    /// @return UniqueRef<Type, std::function<void(Type*)>>
    template < class Type, class ConstructedType, class Allocator, class... TArgs,
               std::enable_if_t< !std::is_array_v< Type >, int > = 0 >
    UniqueRef< Type > allocate_unique(Allocator alloc, TArgs... args) {
        using constructor = std::allocator_traits< Allocator >::template rebind_alloc< ConstructedType >;

        auto custom_deleter = [](Type* ptr, constructor c_alloc) {
            static_assert(0 < sizeof(ConstructedType), "can't delete an incomplete type");
            std::allocator_traits< constructor >::destroy(c_alloc, reinterpret_cast< ConstructedType* >(ptr));
            c_alloc.deallocate(reinterpret_cast< ConstructedType* >(ptr), 1);
        };

        constructor ctor_alloc { alloc };

        ConstructedType* ptr = ctor_alloc.allocate(1);
        std::allocator_traits< constructor >::construct(ctor_alloc, ptr, std::forward< TArgs >(args)...);

        return UniqueRef< Type > { static_cast< Type* >(ptr),
                                   std::bind(custom_deleter, std::placeholders::_1, ctor_alloc) };
    }

    template < class ArrayType, class Allocator,
               std::enable_if_t< std::is_array_v< ArrayType > && std::extent_v< ArrayType > == 0, int > = 0 >
    UniqueRef< ArrayType, std::function< void(std::remove_extent_t< ArrayType >*) > >
        allocate_unique(Allocator alloc, const std::size_t size) {
        using Type           = std::remove_extent_t< ArrayType >;
        using allocator_type = std::allocator_traits< Allocator >::template rebind_alloc< Type >;

        auto custom_array_deleter = [](Type* ptr, allocator_type m_alloc, std::size_t m_size) {
            static_assert(0 < sizeof(Type), "can't delete an incomplete type");
            for (std::size_t i = 0; i < m_size; ++i) {
                std::allocator_traits< allocator_type >::destroy(m_alloc, ptr + i);
            }
            m_alloc.deallocate(ptr, m_size);
        };

        allocator_type type_alloc { alloc };

        Type* ptr = type_alloc.allocate(size);
        if constexpr (std::is_default_constructible_v< Type >) {
            for (std::size_t i = 0; i < size; ++i) {
                std::allocator_traits< allocator_type >::construct(alloc, ptr + i);
            }
        }

        return { ptr, std::bind(custom_array_deleter, std::placeholders::_1, type_alloc, size) };
    }

    template < class Type >
    using SharedRef = std::shared_ptr< Type >;

} // namespace arm_emu

#endif // !defined(UNIQUEREF_H_INCLUDED_431D1EC2_8104_4B9F_9F86_8655208D90FF)
