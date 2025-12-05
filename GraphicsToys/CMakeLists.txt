
add_executable(app src/MyApp.cpp src/Renderer/Rasterization/RasterizationRenderer.cpp ../external/tinyobjloader/tiny_obj_loader.cpp)
include_directories(
    ${CMAKE_SOURCE_DIR}/external/tinyobjloader/
)
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
target_link_libraries(app PRIVATE FluxCore)