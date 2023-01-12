
if (onnxruntime_USE_PREINSTALLED_EIGEN)
    add_library(eigen INTERFACE)
    file(TO_CMAKE_PATH ${eigen_SOURCE_PATH} eigen_INCLUDE_DIRS)
    target_include_directories(eigen INTERFACE ${eigen_INCLUDE_DIRS})
else ()
    if (onnxruntime_USE_ACL) # Enable for ARM
        FetchContent_Declare(
        eigen
        URL https://gitlab.com/libeigen/eigen/-/archive/4d05765345e7e4a984d600039f797e2fede924f3/eigen-4d05765345e7e4a984d600039f797e2fede924f3.zip
        PATCH_COMMAND ${Patch_EXECUTABLE} --ignore-space-change --ignore-whitespace < ${PROJECT_SOURCE_DIR}/patches/eigen/Fix_Eigen_Build_Break.patch
		)
    else()
        FetchContent_Declare(
        eigen
        URL https://gitlab.com/libeigen/eigen/-/archive/4d05765345e7e4a984d600039f797e2fede924f3/eigen-4d05765345e7e4a984d600039f797e2fede924f3.zip
		)
    endif()
    FetchContent_Populate(eigen)
    set(eigen_INCLUDE_DIRS  "${eigen_SOURCE_DIR}")
endif()
