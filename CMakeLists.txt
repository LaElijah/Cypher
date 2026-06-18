cmake_minimum_required(VERSION 3.28)
project(CypherLauncher LANGUAGES CXX)

message(STATUS "Configuring Cypher Multi-Language Workspace...")

set(CSHARP_PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cypher-launcher/cypher-launcher.csproj")

add_subdirectory(cypher-engine)

find_program(DOTNET_EXE dotnet)

if(NOT DOTNET_EXE)
    message(FATAL_ERROR "Could not find the 'dotnet' CLI tool! Is the .NET SDK installed?")
endif()

add_custom_target(CypherLauncher
    ALL
    COMMAND ${DOTNET_EXE} build "${CSHARP_PROJECT_PATH}" --configuration $<IF:$<BOOL:$<CONFIG>>,$<CONFIG>,Debug>
    COMMENT "Building C# Application..."
)


set(PUBLISH_DIR "${CMAKE_CURRENT_SRC_DIR}")

add_custom_target(CypherPublish
    COMMAND ${DOTNET_EXE} publish "${CSHARP_PROJECT_PATH}" 
            --configuration Release 
            --output "${PUBLISH_DIR}"
    COMMENT "Publishing C# Application to ${PUBLISH_DIR}..."
)

add_dependencies(CypherLauncher Cypher)