set_languages("c++23")
set_policy("build.c++.modules", true)
set_warnings("allextra", "error")
add_cxxflags("cl::/wd4324")
set_optimize("fastest")

add_rules("mode.debug", "mode.release")
if is_mode("debug") then
    add_defines("DEBUG")
elseif is_mode("release") then
    add_defines("NDEBUG")
    set_optimize("fastest")
end

target("eco")
    set_kind("static")
    add_files(
        "include/eco_bit.mpp",
        "include/eco_type_traits.mpp",
        "include/eco_concepts.mpp",
        "include/eco_algorithm.mpp",
        "include/eco_memory.mpp",
        "include/eco_allocator.mpp",
        "include/eco_extent.mpp",
        "include/eco_array.mpp",
        "include/eco_array_dict.mpp",
        "include/eco_bitvector.mpp",
        "include/eco_forward_list_pool.mpp",
        "include/eco_iterator.mpp",
        "include/eco_list_pool.mpp",
        "include/eco_fixed_array.mpp",
        "include/eco_binary_tree.mpp",
        "include/eco_parentheses.mpp",
        "include/eco_ordinal_tree.mpp",
        "eco.mpp"
    )

target("test")
    add_deps("eco")
    add_files("test/unit/main.cpp")
