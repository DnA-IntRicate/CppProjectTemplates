#pragma once
#include <memory>
#include <vector>
#include <filesystem>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)


class Debug
{
public:
    static void Initialize() noexcept
    {
        spdlog::sink_ptr colorSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        colorSink->set_pattern("%^[%T] %n: %v%$");

        s_Log = std::make_shared<spdlog::logger>(spdlog::logger("GLFW-DX11", colorSink));
        spdlog::register_logger(s_Log);
        s_Log->set_level(spdlog::level::trace);
        s_Log->flush_on(spdlog::level::trace);
    }

    template<typename... _Args>
    constexpr inline static void Trace(spdlog::format_string_t<_Args...> fmt, _Args&&... args) noexcept
    {
        s_Log->trace(fmt, std::forward<_Args>(args)...);
    }

    template<typename... _Args>
    constexpr inline static void Info(spdlog::format_string_t<_Args...> fmt, _Args&&... args) noexcept
    {
        s_Log->info(fmt, std::forward<_Args>(args)...);
    }

    template<typename... _Args>
    constexpr inline static void Warn(spdlog::format_string_t<_Args...> fmt, _Args&&... args) noexcept
    {
        s_Log->warn(fmt, std::forward<_Args>(args)...);
    }

    template<typename... _Args>
    constexpr inline static void Error(spdlog::format_string_t<_Args...> fmt, _Args&&... args) noexcept
    {
        s_Log->error(fmt, std::forward<_Args>(args)...);
    }

    template<typename... _Args>
    constexpr inline static void Critical(spdlog::format_string_t<_Args...> fmt, _Args&&... args) noexcept
    {
        s_Log->critical(fmt, std::forward<_Args>(args)...);
    }

#ifdef _DEBUG
    #define DEBUGBREAK() __debugbreak()
#else
    #define DEBUGBREAK() exit(EXIT_FAILURE)
#endif 

    template<typename... _Args>
    constexpr inline static void Assert(bool check, spdlog::format_string_t<_Args...> fmt, _Args&&... args) noexcept
    {
        if (!check)
        {
            Critical(fmt, std::forward<_Args>(args)...);
            DEBUGBREAK();
        }
    }

#ifdef _DEBUG
    #define ASSERT(check, msg) Debug::Assert(check, "Assertion failed at {0}:{1} - {2}", std::filesystem::path(__FILE__).filename().string(),\
            __LINE__, msg)
#else
    #define ASSERT(check, msg)
#endif

private:
    static std::shared_ptr<spdlog::logger> s_Log;
};
