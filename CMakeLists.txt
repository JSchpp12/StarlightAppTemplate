cmake_minimum_required(VERSION 3.26)

project(starlight_app_template
	DESCRIPTION "Template application using starlight"
	LANGUAGES C CXX
	VERSION 1.1.0
)
option(HEADLESS "Build without presentation support" OFF)
option(STAR_ENABLE_UBSAN "Enable UBSan" OFF)
option(STAR_ENABLE_WARNINGS "Enable Warnings" OFF)
if (STAR_ENABLE_UBSAN)
	message("STAR - Enable UBSAN")
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
		add_link_options(-fsanitize=undefined -lubsan)
	endif()
endif()
if (STAR_ENABLE_WARNINGS)
	message("STAR - Enable WARNINGS")

	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		add_compile_options(
			-Wall
			-Wextra
			-Wpedantic
			-Wconversion
			-g
		)
	endif()

	if (MSVC)
		add_compile_options(/W4 /Zi)
	endif()
endif()

cmake_policy(SET CMP0071 NEW)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0079 NEW)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
	set(CMAKE_MACOSX_RPATH 1)
endif()

set(starlight_common_DIR "${CMAKE_BINARY_DIR}/extern/Starlight-Common" CACHE PATH "")
add_subdirectory("extern/Starlight-Common")
add_subdirectory("extern/starlight") 
if (NOT HEADLESS)
	add_subdirectory("extern/Starlight-Windowing")
endif()
set(${PROJECT_NAME}_SOURCE
	"src/Application.cpp"
	"src/WindowedMode.cpp"
	"src/WindowedApplication.cpp"
	"src/HeadlessMode.cpp"

	"src/loaders/LightLoaders.cpp"
	"src/loaders/CameraLoaders.cpp"
	"src/loaders/SceneLoaders.cpp"
)
set(${PROJECT_NAME}_HEADERS
	"include/Application.hpp"
	"include/WindowedMode.hpp"
	"include/WindowedApplication.hpp"
	"include/HeadlessMode.hpp"
	"include/HeadlessApplication.hpp"
)
#main application library
add_library(${PROJECT_NAME}_lib 
    ${${PROJECT_NAME}_SOURCE}
    ${${PROJECT_NAME}_HEADERS}
)
target_include_directories(${PROJECT_NAME}_lib
	PUBLIC "include"
)
if (HEADLESS)
	target_link_libraries(${PROJECT_NAME}_lib
		PUBLIC
			Starlight::starlight
	)
else()
	target_link_libraries(${PROJECT_NAME}_lib
		PUBLIC
			Starlight::windowing
	)
endif()

if (CMAKE_CONFIGURATION_TYPES)
	set(FINAL_APPLICATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
else()
	set(FINAL_APPLICATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")
endif()

if (NOT ${HEADLESS})
    target_compile_definitions(${PROJECT_NAME}_lib PUBLIC STAR_ENABLE_PRESENTATION "GLM_FORCE_DEPTH_ZERO_TO_ONE")
    message("STAR - Enabling Presentation")
else()
    target_compile_definitions(${PROJECT_NAME}_lib PUBLIC "GLM_FORCE_DEPTH_ZERO_TO_ONE")
    message("STAR - Disable Presentation")
endif()

#main application executable
add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME}
	PRIVATE ${PROJECT_NAME}_lib
)
target_include_directories(${PROJECT_NAME}
	PRIVATE "include"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${FINAL_APPLICATION_DIR}"
)

# app builder and media

find_package(Git REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(STARLIGHT_APP_BUILDER_DIR "${CMAKE_SOURCE_DIR}/out/extern/StarlightAppBuilder" CACHE PATH "Shared StarlightAppBuilder install directory")
set(STARLIGHT_APP_BUILDER_BIN "${STARLIGHT_APP_BUILDER_DIR}/bin")
set(STARLIGHT_APP_BUILDER_SRC "${STARLIGHT_APP_BUILDER_DIR}/src")
find_program(STAR_APP_BUILDER_COMMAND
	NAMES star_app_builder
	PATHS "${STARLIGHT_APP_BUILDER_BIN}"
)

if (STAR_APP_BUILDER_COMMAND)
	message(STATUS "Found star_app_builder: ${STAR_APP_BUILDER_COMMAND}")
else()
	message(STATUS "star_app_builder not found; fetching and installing into ${STARLIGHT_APP_BUILDER_DIR}")
	file(REMOVE_RECURSE
		"${STARLIGHT_APP_BUILDER_SRC}"
		"${STARLIGHT_APP_BUILDER_BIN}"
	)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} clone --depth 1
				"https://github.com/JSchpp12/StarlightAppBuilder.git"
				"${STARLIGHT_APP_BUILDER_SRC}"
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND_ERROR_IS_FATAL ANY
	)
	execute_process(
		COMMAND ${Python3_EXECUTABLE} -m pip install
				--upgrade
				--target "${STARLIGHT_APP_BUILDER_DIR}"
				"${STARLIGHT_APP_BUILDER_SRC}"
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND_ERROR_IS_FATAL ANY
	)
endif()

add_custom_target(starlight_app_builder)
set(STAR_APP_BUILDER_ENV_ARGS "PYTHONPATH=${STARLIGHT_APP_BUILDER_DIR}")
set(STAR_APP_BUILDER_EXEC_ARGS ${Python3_EXECUTABLE} -m star_app_builder)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_custom_target(prepare_media ALL
		COMMAND ${CMAKE_COMMAND} -E env
			${STAR_APP_BUILDER_ENV_ARGS}
			${STAR_APP_BUILDER_EXEC_ARGS} prep-media
			--outDir=${FINAL_APPLICATION_DIR}/media
			--inDir=${CMAKE_CURRENT_SOURCE_DIR}/media
			--fastest
		COMMENT "Running media prep"
		VERBATIM
	)
else()
	add_custom_target(prepare_media ALL
		COMMAND ${CMAKE_COMMAND} -E env
			${STAR_APP_BUILDER_ENV_ARGS}
			${STAR_APP_BUILDER_EXEC_ARGS} prep-media
			--outDir=${FINAL_APPLICATION_DIR}/media
			--inDir=${CMAKE_CURRENT_SOURCE_DIR}/media
		COMMENT "Running media prep"
		VERBATIM
	)
endif()
add_dependencies(prepare_media starlight_app_builder)

add_custom_target(create_config ALL
	COMMAND ${CMAKE_COMMAND} -E env
			${STAR_APP_BUILDER_ENV_ARGS}
			${STAR_APP_BUILDER_EXEC_ARGS} create-config
			--outDir=${FINAL_APPLICATION_DIR}
	COMMENT "Running config prep"
	VERBATIM
)
add_dependencies(create_config starlight_app_builder)

set(ALL_FILES ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
# Convert to space-separated string
string(REPLACE ";" " " FILE_LIST "${ALL_FILES}")

add_custom_target(format_files
    COMMAND ${Python3_EXECUTABLE} ${STARLIGHT_APP_BUILDER_DIR}/scripts/SourceCodeFormatter.py ${FILE_LIST} --dir ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${ALL_FILES} ${STARLIGHT_APP_BUILDER_DIR}/scripts/SourceCodeFormatter.py
    COMMENT "Running clang-format"
)

# ---------------------------------------------------------------------------
# Packaging (CPack)
# ---------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "starlight-app-template")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME} ${PROJECT_VERSION}")
if (WIN32)
	set(_STAR_PACKAGE_SYSTEM "win64")
else()
	set(_STAR_PACKAGE_SYSTEM "${CMAKE_SYSTEM_NAME}")
endif()
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${_STAR_PACKAGE_SYSTEM}")

# Suffix the archive with the build variant.
if (HEADLESS)
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-headless")
else()
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-windowed")
endif()

# Only the runtime component is shipped.  This keeps the vendored projects'
# static libraries, headers and CMake export files (which live in the
# "Unspecified" component) out of the distributable.
set(CPACK_COMPONENTS_ALL Runtime)
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_ARCHIVE_RUNTIME_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")
if (WIN32)
	set(CPACK_GENERATOR "ZIP")
else()
	set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)

include(CTest)
enable_testing()
find_package(GTest CONFIG REQUIRED)