add_requires("spdlog", {configs = {header_only = true}})
add_requires("libsdl3", {configs = {shared = true}})

add_rules("mode.debug", "mode.release")

target("SunnyLand")
    set_kind("binary")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")

    -- Set C++23 standard here
    set_languages("c++23")

    -- 3rd party libss
    add_packages("spdlog")
    add_packages("libsdl3")
