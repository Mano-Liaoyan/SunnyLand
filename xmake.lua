add_rules("mode.debug", "mode.release")

set_project("SunnyLand")
set_version("0.1.0")

set_languages("c++20")

-- Dependencies

-- SDL3 (Git)
-- We map it to "libsdl3" so other packages (like libsdl3_image) use this version instead of downloading their own.
add_requires("libsdl3 3.4.0", {
    configs = {
        shared = true,
        cxflags = "-fPIC"
    },
    verify = false
})

-- SDL3_mixer (Git)
add_requires("git::https://github.com/libsdl-org/SDL_mixer/commit/30c1301055a35ee87b8679279b6fc88e10d28fa3", {
    alias = "libsdl3_mixer",
    configs = {
        shared = true
    }
})

-- SDL3_image (Repo)
-- Note: Disabling avif isn't standard in repo configs usually, but we check if we can pass it.
-- Repo package usually allows 'avif' config.
add_requires("libsdl3_image", {
    configs = {
        avif = false,
        shared = true
    }
})

-- SDL3_ttf (Repo)
add_requires("libsdl3_ttf", {
    configs = {
        shared = true
    }
})

-- GLM (Repo)
add_requires("glm")

-- nlohmann_json (Repo)
add_requires("nlohmann_json")

-- spdlog (Repo)
add_requires("spdlog", {
    configs = {
        shared = false,
        fmt_external = false
    }
})

target("SunnyLand")
    set_kind("binary")
    
    -- Source files
    add_files("src/main.cpp")
    add_files("src/engine/**.cpp") 
    
    -- Include directories
    add_includedirs("src")
    
    -- Add packages
    -- Note: using the package names/aliases defined above
    add_packages("libsdl3", "libsdl3_image", "libsdl3_mixer", "libsdl3_ttf", "glm", "nlohmann_json", "spdlog")
    
    -- Compiler options
    if is_plat("windows") then
        if is_cc("msvc") then
            add_cxflags("/W4", "/utf-8")
            add_defines("UNICODE", "_UNICODE") 
            add_ldflags("/SUBSYSTEM:WINDOWS", "/ENTRY:mainCRTStartup") 
        end
    elseif is_plat("linux", "macosx") then
        add_cxflags("-Wall", "-Wextra", "-Wpedantic")
    end
    
    set_languages("c++20")
