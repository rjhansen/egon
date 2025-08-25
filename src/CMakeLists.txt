find_package(CURL)
find_package(PkgConfig REQUIRED)
pkg_search_module(GPGME REQUIRED IMPORTED_TARGET gpgme)
add_executable(egon egon.cpp)
target_include_directories(egon
    PRIVATE
    ${GPGME_INCLUDE_DIRS}
    ${BOOST_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIR})
target_link_libraries(egon PRIVATE CURL::libcurl PkgConfig::GPGME)