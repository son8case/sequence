#ifndef SON8_SEQUENCE_HXX
#define SON8_SEQUENCE_HXX

#include <cstddef>
#include <cstdint>

namespace son8::sequence {
    // aliases
    using Size = std::size_t;
    using Diff = std::ptrdiff_t;
    using Char = unsigned char;
    using Int1 = std::int8_t;
    using Int2 = std::int16_t;
    using Int4 = std::int32_t;

    template< typename Type > using Ptr = Type *;
    template< typename Type > using Out = Type &;
    template< typename Type > using Ref = Type const &;
    template< typename Type > using Fwd = Type &&;

    enum class Category {
        Adjacent,
        Indexing,
        Forwards,
        Streamer,
        // IMPORTANT must be last
        Size_
    };
    // read
    // -- helpers
    template< typename Type >
    auto read_diff( Type seq ) noexcept {
        static_assert( Type::category( ) == Category::Adjacent );
        return seq.end( ) - seq.beg( );
    }
    template< typename Type >
    auto read_middle( Type seq ) noexcept {
        static_assert( Type::category( ) == Category::Adjacent );
        return seq.beg( ) + ( read_diff( seq ) >> 1u );
    }
    // edit
    template< typename Type >
    struct Edit {
        using Out_ = typename Type::OutThis_;
        using Ptr_ = Ptr< typename Type::Data >;
        static_assert( Type::category( ) == Category::Adjacent
            , "son8::sequence: Edit requires sequence with adjacent category" );
        static void beg( Out_ edit,  Ptr_ value ) noexcept { edit.beg_ = value; }
        static void end( Out_ edit,  Ptr_ value ) noexcept { edit.end_ = value; }
    };
    // -- helpers
    template< typename Type >
    void edit_beg( Out< Type > edit, Ptr< typename Type::Data > value ) noexcept { Edit< Type >::beg( edit, value ); }
    template< typename Type >
    void edit_end( Out< Type > edit, Ptr< typename Type::Data > value ) noexcept { Edit< Type >::end( edit, value ); }
    // Adjacent category requires contiguous containters
    template< typename Type >
    class Adjacent final {
        // aliases
        using Ptr_ = Ptr< Type >;
        using This_ = Adjacent< Type >;
        using OutThis_ = Out< This_ >;
        // data
        Ptr_ beg_, end_;
        // setters
        friend struct Edit< This_ >;
    public:
        // aliases
        using Data = Type;
        // compile-time
        static constexpr auto category( ) -> Category { return Category::Adjacent; }
        // constructors
        Adjacent( ) = delete;
        Adjacent( Ptr_ beg_, Ptr_ end_ ) noexcept
        : beg_{ beg_ }, end_{ end_ } { }
        // range-for
        auto begin( ) const noexcept { return beg_; }
        auto end( ) const noexcept { return end_; }
        // getters
        auto beg( ) const noexcept { return beg_; }
    };

    // make_from_std( std::vector< Type > ) -> Adjacent
    // algorithms
    // -- binary search first match
    template< typename Type, typename Data = typename Type::Data >
    auto sorted_match( Type seq, Ref< Data > data ) -> Type {
        static_assert( Type::category( ) == Category::Adjacent
            , "son8::sequence sorted_match requires sequence to be adjacent (contigues)" );
        auto notFound = Type{ seq.end( ), seq.end( ) };
        while ( read_diff( seq ) ) {
            auto mid = read_middle( seq );
            if/*_*/ ( *mid < data ) edit_beg( seq, mid + 1 );
            else if ( data < *mid ) edit_end( seq, mid );
            else return Type{ mid, mid + 1 };
        }
        return notFound;
    }
}

#endif//SON8_SEQUENCE_HXX

// Ⓒ 2026 Oleg'Ease'Kharchuk ᦒ
