cmake_minimum_required(VERSION 3.27)

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

project("starlightapp")
enable_language(CXX)

add_subdirectory("extern/starlight") 

set(${PROJECT_NAME}_SOURCE 
	"src/main.cpp"
 	"src/Application.cpp"
)

set(${PROJECT_NAME}_HEADERS 
	"include/Application.hpp"
)

add_executable(${PROJECT_NAME} "${${PROJECT_NAME}_SOURCE};${${PROJECT_NAME}_HEADERS}")

target_include_directories(${PROJECT_NAME}
	PRIVATE "include"
)

target_link_libraries(${PROJECT_NAME}
PRIVATE 
	starlight
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		starlight
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
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


set(STARLIGHT_APP_BUILDER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/StarlightAppBuilder/")
set(PREP_MEDIA_CMD "${STARLIGHT_APP_BUILDER_DIR}/scripts/PrepareMedia.py")

set(FINAL_APPLICATION_DIR "")
if (MSVC)
	set(FINAL_APPLICATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
	set(FINAL_APPLICATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")
endif()

find_package(Python3 REQUIRED COMPONENTS Interpreter)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_custom_target(prepare_media ALL
		COMMAND ${Python3_EXECUTABLE} ${PREP_MEDIA_CMD} --builddir=${FINAL_APPLICATION_DIR} --mediadir=${CMAKE_CURRENT_SOURCE_DIR}/media --depsdir=${STARLIGHT_APP_BUILDER_DIR}/deps --fastest
		DEPENDS ${ALL_FILES} ${STARLIGHT_APP_BUILDER_DIR}/scripts/PrepareMedia.py
		COMMENT "Running media prep"
		VERBATIM
	)
else()
	add_custom_target(prepare_media ALL
		COMMAND ${Python3_EXECUTABLE} ${PREP_MEDIA_CMD} --builddir=${FINAL_APPLICATION_DIR} --mediadir=${CMAKE_CURRENT_SOURCE_DIR}/media --depsdir=${STARLIGHT_APP_BUILDER_DIR}/deps
		DEPENDS ${ALL_FILES} ${STARLIGHT_APP_BUILDER_DIR}/scripts/PrepareMedia.py
		COMMENT "Running media prep"
		VERBATIM
	)
endif()

set(ALL_FILES ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
# Convert to space-separated string
string(REPLACE ";" " " FILE_LIST "${ALL_FILES}")

add_custom_target(format_files
    COMMAND ${Python3_EXECUTABLE} ${STARLIGHT_APP_BUILDER_DIR}/scripts/SourceCodeFormatter.py ${FILE_LIST} --dir ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${ALL_FILES} ${STARLIGHT_APP_BUILDER_DIR}/scripts/SourceCodeFormatter.py
    COMMENT "Running clang-format"
)