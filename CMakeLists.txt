cmake_minimum_required(VERSION 3.26)

project(starlight_app_template
	DESCRIPTION "Template application using starlight"
	LANGUAGES C CXX
	VERSION 1.0.1
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
set(CMAKE_CXX_STANDARD 20) 
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
	"src/InteractiveApplication.cpp"
	"src/loader/LightLoaders.cpp" 
	"src/loader/CameraLoaders.cpp"
	"src/loader/SceneLoaders.cpp"
		"src/loader/RenderLoaders.cpp"
	"src/run_mode/Headless.cpp" 
	"src/run_mode/Windowed.cpp" 
	"src/renderer/headless/Orchestrator.cpp" 
	"src/renderer/headless/GiveImageToTransfer.cpp"
)
set(${PROJECT_NAME}_HEADERS 
	"include/Application.hpp"
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
	if (WIN32)
		target_link_libraries(${PROJECT_NAME}_lib
			PUBLIC
				Starlight::starlight
		)
	else()
		target_link_libraries(${PROJECT_NAME}_lib
			PUBLIC
				Starlight::starlight
				atomic
		)
	endif()
else()
	if (WIN32)
		target_link_libraries(${PROJECT_NAME}_lib
			PUBLIC
				Starlight::windowing
		)
	else()
		target_link_libraries(${PROJECT_NAME}_lib
			PUBLIC
				Starlight::windowing
				atomic
		)
	endif()

endif()

#main application executable
add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME}
	PRIVATE ${PROJECT_NAME}_lib
)
target_include_directories(${PROJECT_NAME}
	PRIVATE "include"
)


find_program(CPPCHECK_EXECUTABLE cppcheck)
if (CPPCHECK_EXECUTABLE)
	set(CPPCHECK_FILES "")

	foreach(FILE "${${PROJECT_NAME}_SOURCE}" "${PROJECT_NAME}_HEADERS")
		file(TO_CMAKE_PATH "${FILE}" UNIX_PATH)
		list(APPEND CPPCHECK_FILES "${UNIX_PATH}")
	endforeach()

	add_custom_target("${PROJECT_NAME}_cppcheck"
		COMMAND ${CPPCHECK_EXECUTABLE} 
			--enable=all 
			-q
			--inline-suppr
			--suppress=missingIncludeSystem
			--language=c++
			--std=c++20
			--template=gcc
			-I "include"
			${CPPCHECK_FILES}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Running Cppcheck"
		VERBATIM
	)

	add_dependencies("${PROJECT_NAME}_cppcheck" ${CMAKE_PROJECT_NAME})
endif()

if (NOT ${HEADLESS})
    target_compile_definitions(${PROJECT_NAME}_lib PUBLIC STAR_ENABLE_PRESENTATION)
endif()
set(STARLIGHT_APP_BUILDER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/StarlightAppBuilder/")
set(PREP_MEDIA_CMD "${STARLIGHT_APP_BUILDER_DIR}/scripts/PrepareMedia.py")

find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(FINAL_APPLICATION_DIR "")
if (MSVC)
    set(FINAL_APPLICATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
	target_compile_options(${PROJECT_NAME}_lib PUBLIC 
		$<$<CXX_COMPILER_ID:MSVC>:/MP /EHsc /Zc:__cplusplus> 
	)
else()
	set(FINAL_APPLICATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_custom_target(prepare_media ALL
		COMMAND ${Python3_EXECUTABLE} -m star_app_builder prep-media --outDir=${FINAL_APPLICATION_DIR}/media --inDir=${CMAKE_CURRENT_SOURCE_DIR}/media --depsDir=${STARLIGHT_APP_BUILDER_DIR}/deps --fastest
		COMMENT "Running media prep"
		VERBATIM
	)
else()
	add_custom_target(prepare_media ALL
		COMMAND ${Python3_EXECUTABLE} -m star_app_builder prep-media --outDir=${FINAL_APPLICATION_DIR}/media --inDir=${CMAKE_CURRENT_SOURCE_DIR}/media --depsDir=${STARLIGHT_APP_BUILDER_DIR}/deps 
		COMMENT "Running media prep"
		VERBATIM
	)
endif()

add_custom_target(create_config ALL
	COMMAND ${Python3_EXECUTABLE} -m star_app_builder create-config --outDir=${FINAL_APPLICATION_DIR}
	COMMENT "Running config prep"
	VERBATIM
)

set(ALL_FILES ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
# Convert to space-separated string
string(REPLACE ";" " " FILE_LIST "${ALL_FILES}")

add_custom_target(format_files
    COMMAND ${Python3_EXECUTABLE} ${STARLIGHT_APP_BUILDER_DIR}/scripts/SourceCodeFormatter.py ${FILE_LIST} --dir ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${ALL_FILES} ${STARLIGHT_APP_BUILDER_DIR}/scripts/SourceCodeFormatter.py
    COMMENT "Running clang-format"
)