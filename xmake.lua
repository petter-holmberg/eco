add_rules("mode.release", "mode.debug")

set_languages("c++20")
set_warnings("all", "error")
set_optimize("fastest")

target("eco")
    set_kind("static")
    add_files("eco.ixx", "eco-memory.ixx", "eco-sequence.ixx", "eco-container.ixx")
