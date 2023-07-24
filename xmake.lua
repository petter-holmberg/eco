set_languages("c++20")
set_warnings("allextra", "error")
set_optimize("fastest")

add_rules("mode.debug", "mode.release")
if is_mode("debug") then
    add_defines("DEBUG")
elseif is_mode("release") then
    add_defines("NDEBUG")
    set_optimize("fastest")
end

target("eco")
    add_includedirs("include")
    set_kind("headeronly")
    add_files("eco.hpp")

target("test")
    add_includedirs("include")
    add_files("test/unit/main.cpp")
