cmake_minimum_required(VERSION 3.14)

# Имя вашего проекта
project(ContainerTest)
set(CMAKE_TLS_VERIFY OFF)

# Стандарт C++20, как в задании
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем предупреждения и ошибки (как в задании: -Wall -Wextra -Werror -pedantic)
if(MSVC)
    # Флаги для компилятора MSVC (если вдруг используете Visual Studio компилятор)
    add_compile_options(/W4 /WX)
else()
    # Флаги для GCC/MinGW (ваш случай)
    add_compile_options(-g -Wall -Wextra -Werror -pedantic)
endif()

# --- Подключение Google Test (FetchContent) ---
include(FetchContent)

message(STATUS "Downloading GoogleTest...")
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Настройка вашего исполняемого файла ---

# Указываем, какие файлы компилировать (только .cpp, хедеры подтянутся сами)
add_executable(test_app test_main.cpp)

# Линкуем Google Test к вашему приложению
# GTest::gtest_main содержит встроенную функцию main(), 
# поэтому свою писать НЕ ОБЯЗАТЕЛЬНО (см. Шаг 4)
target_link_libraries(test_app PRIVATE GTest::gtest_main)

# Включаем интеграцию тестов в CTest (чтобы видеть их в VS Code)
include(GoogleTest)
gtest_discover_tests(test_app)