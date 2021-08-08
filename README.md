eco
===

eco is a generic C++ library of efficient components.

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
