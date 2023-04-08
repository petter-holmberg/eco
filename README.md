eco
===

eco is a generic C++ library of efficient components.

# Containers

The `eco.container` module contains containers.

`array` is a type constructor for dynamic arrays, similar to `std::vector`, but with some key differences:

- The local storage of `array` is a single pointer, making storage of nested arrays where many of the inner arrays are empty more efficient.
- Fewer member types and member functions, leveraging the capabilities of the C++20 ranges library instead.
- Only designed to store value types and free of specializations with custom semantics, like `std::vector<bool>`.
- The growth algorithm is configurable and by default grows the reserve area by a factor of 1.5, which saves memory and when used with many allocators prevents it from reusing previously allocated memory blocks.
- Support for custom allocators (as defined by concepts in this library) but without support for storing stateful allocators in the `array` itself, which makes it easier to reason about ownership semantics.

`array<T>` models `std::ranges::contiguous_range<T>`.

The type parameter `T` is the type of objects stored in the `array`.

The value parameter `ga` is the growth algorithm used when more space is needed.
`default_array_growth` is a function object that takes a non-negative capacity
and a lower bound. It returns `capacity + max(capacity / 2, lower_bound)`, which
gives an allocator-friendly growth factor that guarantees amortized constant
time stack operations.

The value parameter `alloc` is the allocator object used to manage the owned
memory. `default_array_alloc` uses `malloc_allocator`.

The member type `value_type` is the type of objects stored in the `array`.

The member type `size_type` is a signed type large enough to represent the maximum possible size of the `array`.

# Sequence

The `eco.sequence` module contains components for implemention of sequence
types, such as containers.

## Extents

`extent` is a type constructor for storage of arrays of objects in a contiguous
memory region, where elements are prefixed by a header that keeps track of the
number of elements stored and the number of elements that can be stored without
a reallocation. Additional metadata can also be stored in the header.
`extent` models `std::contiguous_range` and owns the elements it stores. It
supports resizing of the memory region and provides automatic growth when
inserting elements.

`extent` can be customized for many different use cases.

The type parameter `T` is the type of objects stored in the memory region.

The type parameter `Size` is the type
used to store size and capacity in the header. By using a smaller type when the
maximum number of elements is small the header size can be reduced. By default
it is equivalent to `std::ptrdiff_t`.

The type parameter `Metadata` is the type of an optional extra field in the
header. By default it is `std::monostate` and no bytes are used for it in the
header. It can be used for optimizing element operations when the data encoding
requires it.

The value parameter `copier` is an object with member functions `copy`, `move`,
`move_backward`, and `destroy` that is used to copy, move and delete
elements when necessary. By default it is `default_array_copy`, which provides
efficient and exception-safe range operations on (possibly) uninitialized arrays
of its value type.

The value parameter `ga` is the growth algorithm used when more space is needed. By default it is
`default_array_growth`.

The value parameter `alloc` is the allocator object used to manage the owned
memory. `default_array_alloc` uses `malloc_allocator`.

# Memory

The `eco.memory` module contains components for raw memory management. These are
low-level components used in the implementation of dynamic data structures.

## Memory regions

`memory_view` is a view of a (logically) contiguous memory region beginning at
`first` and containing `size` bytes of data. It models `std::ranges::view` and
is the type used throughout the library to describe memory regions with
arbitrary contents.
`memory_view` models `std::totally_ordered` and is ordered first by memory
address and second by size. A boolean test on a `memory_view` checks if the
contained memory address is not equal to `nullptr`.

## Allocators

Allocators are objects responsible for dynamically allocating contiguous memory
regions. Unlike standard library allocators, eco allocators are not concerned
with the types of the data the memory is allocated for. eco allocators are not
necessarily capable of multiple allocations, deallocations or ownership
detection.

`allocator` describes a type with a member function `allocate` that is used to
allocate raw memory and returns a `memory_view` of that memory. It returns
a `memory_view` testable as `false` if allocation fails.

`deallocatable_allocator` describes an `allocator` type with a member function
`deallocate` that is used to deallocate previously allocated memory by the same
allocator object. It returns `false` if deallocation fails.

`reallocatable_allocator` describes an `allocator` type with a member function
`reallocate` that can reallocate memory previously allocated by the same object.
It returns a memory view testable as `false` if reallocation fails.

`ownership_aware_allocator` describes an `allocator` type with a member function
`is_owner` that takes a `memory_view` and tells if the associated memory was
allocated with this object or not.

`malloc_allocator` is a type that uses `std::malloc` and associated functions
to manage dynamic memory. It models `deallocatable_allocator` and
`reallocatable_allocator`. It is the type of the default allocators used in eco
containers.

`arena_allocator` is a type constuctor for allocators that pre-allocate a given
number of bytes on construction, using a given `deallocatable_allocator`.
`allocate` allocates from the beginning of previously unused memory in this
arena, and `deallocate` does nothing. The allocated arena will be deallocated
on destruction.
This allocator is useful for situations where allocation and deallocation must
be as fast as possible and the total amount of memory that will be used is
known beforehand.
