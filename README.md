eco
===

eco is a generic C++ library of efficient components.

# Algorithms

## Combinatorial algorithms

`dlx` is a type constructor implementing Donald Knuth's "Algorithm X" for
solving exact cover problems using dancing links.

The constructor takes a `std::forward_range` of item values and a
`std::forward_range` of options containing item indexes.

The function call operator takes a `dlx_visitor` that is invoked once for every
found solution, and a `dlx_item_choice_heuristic` for choosing the items to
cover during the execution of the algorithm. `dlx_mrv_heuristic` is the default
heurisic.

`dlx_visitor` describes a `std::invocable` type that is invoked once for every
found solution.

`dlx_visit_all` is a type constructor implementing a `dlx_visitor` that stores
all matching choices in a member `array` `solutions`.

`dlx_item_choice_heuristic` describes a `std::invocable` type that is invoked to
choose the next remaining item to cover during a run of the algorithm.

`dlx_mrv_heuristic` is a type constructor implementing a
`dlx_item_choice_heuristic` that chooses the first remaining item selected by
the minimum number of options.

Example: For 7 items, choices
`[[2, 4], [0, 3, 6], [1, 2, 5], [0, 3, 5], [1, 6], [3, 4, 6]]` has one solution
`[0, 3, 4]`, i.e. the options choosing items `[[2, 4], [0, 3, 5], [1, 6]]`.

## List algorithms

`reverse_append` takes a forward range, a `std::forward_iterator` to the head of
a list, and a `forward_linker`.
It appends the forward range in reversed element order to the head, effectively
making the last element of the original range the new head.

Example: `[1 2 3 4] <reverse_append> [5 6 7 8] -> [4 3 2 1 5 6 7 8]`

# Data structures

## Array

`array` is a type constructor for dynamic arrays, similar to `std::vector`,
but with some key differences:

- The local storage of `array` is a single pointer, making storage of nested
arrays where many of the inner arrays are empty more efficient.
- Fewer member types and member functions, leveraging the capabilities of the
C++20 ranges library instead.
- Only designed to store value types and free of specializations with custom
semantics, like `std::vector<bool>`.
- The growth algorithm is configurable and by default grows the reserve area by
a factor of 1.5, which saves memory and when used with many allocators prevents
it from reusing previously allocated memory blocks.
- Support for custom allocators (as defined by concepts in this library) but
without support for storing stateful allocators in the `array` itself, which
makes it easier to reason about ownership semantics.

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

The member type `size_type` is a signed type large enough to represent the
maximum possible size of the `array`.

## Bitvectors

`bitvector` describes a `std::regular` type with the following operations:

- `size()` returns the number of bits stored.
- `bitread(i)` reads bit `i`.
- `bitset(i)` sets bit `i` to `1`.
- `bitclear(i)` sets bit `i` to `0`.
- `init()` perform post-construction initialization for faster access.
- `rank_0(i)` returns the number of 0-bits in the range `[0, i)`.
- `rank_1(i)` returns the number of 1-bits in the range `[0, i)`.
- `select_0(i)` returns the position of the `i`:th 0-bit.
- `select_1(i)` returns the position of the `i`:th 1-bit.
- `init` preprocesses the `bitvector` for efficient `rank` and `select` queries.

`basic_bitvector` is a type constructor for a `bitvector` that stores bits in an
uncompressed form.

# Trees

`louds` (Level-Order Unary Degree Sequence) is a compact representation of an
ordinal tree. It can be constructed from a tree of a known size with a pair of
`linked_bicursor`s, where the `left_branch` points to the first child of a node
and `right branch` points to the next sibling.

- `root()` returns the root node of the tree.
- `first_child(v)` returns the first child of node `v`.
- `last_child(v)` returns the last child of node `v`.
- `next_sibling(v)` returns the next sibling of node `v`.
- `prev_sibling(v)` returns the previous sibling of node `v`.
- `parent(v)` returns the parent of node `v`.
- `is_leaf(v)` returns `true` iff node `v` is a leaf.
- `nodemap(v)` returns a unique identifier of node `v`, called its _index_.
- `nodeselect(i)` returns the node with index `i`.
- `children(v)` returns the number of children of node `v`.
- `child(v, n)` returns the `n`:th child of node `v`.
- `child_rank(v)` returns the `n` such that node `v` is the `n`:th child of
its parent.
- `lca(t, u, v)` returns the lowest common ancestor of nodes `u` and `v` in `t`.

`binary_louds` (Level-Order Unary Degree Sequence) is a compact representation
of a binary tree. It can be constructed from a binary tree of a known size with
a pair of `bidirectional_bicursor`s.

- `root()` returns the root node of the tree.
- `parent(v)` returns the parent of node `v`.
- `has_left_child(v)` returns `true` iff node `v` has a left child.
- `has_right_child(v)` returns `true` iff node `v` has a right child.
- `is_leaf(v)` returns `true` iff node `v` is a leaf.
- `left_child(v)` returns the left child of node `v`.
- `right_child(v)` returns the right child of node `v`.
- `child_label(v)` returns the label of the edge leading to node `v`.

# Sequences

Components for implemention of sequence types, such as containers.

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

The value parameter `ga` is the growth algorithm used when more space is needed.
By default it is `default_array_growth`.

The value parameter `alloc` is the allocator object used to manage the owned
memory. `default_array_alloc` uses `malloc_allocator`.

## List pools

`forward_list_pool` is a type constructor for a pool of singly linked list nodes,
stored in a contiguous memory region. It uses `extent` to store the list nodes.
The lifetime of elements in the list nodes is bound to the lifetime of the pool,
not individual lists in it.

`forward_list_pool` can be customized for many different use cases.

The type parameter `T` is the type of objects stored in the elements of the pool.

The type parameter `Size` is the type used to store links in the list nodes.
Using a type not larger than required to store the maximum number of nodes in
the pool minimizes the memory footprint and makes the pool more cache-friendly.

`list_pool` is a type constructor for a pool of doubly linked list nodes,
stored in a contiguous memory region. It uses `extent` to store the list nodes.
The lifetime of elements in the list nodes is bound to the lifetime of the pool,
not individual lists in it.

`list_pool` can be customized for many different use cases.

The type parameter `T` is the type of objects stored in the elements of the pool.

The type parameter `Size` is the type used to store links in the list nodes.
Using a type not larger than required to store the maximum number of nodes in
the pool minimizes the memory footprint and makes the pool more cache-friendly.

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
number of bytes on construction, using a given `deallocatable_allocator`. It
models `deallocatable_allocator`.
`allocate` allocates from the beginning of previously unused memory in this
arena, and `deallocate` does nothing. The allocated arena will be deallocated
on destruction.
This allocator is useful for situations where allocation and deallocation must
be as fast as possible and the total amount of memory that will be used is
known beforehand.
