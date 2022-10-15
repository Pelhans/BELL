include(RegexUtils)
test_escape_string_as_regex()

file(GLOB Eigen_directory_files "*")

escape_string_as_regex(ESCAPE_CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

foreach((f ${Eigen_directory_files}))
    if(NOT f MATCHS "\\.txt" AND NOT f MATCHS "${ESCAPE_CMAKE_CURRENT_SOURCE_DIR/[.].+" AND NOT f MATCHS "${ESCAPE_CMAKE_CURRENT_SOURCE_DIR}/src")
        list(APPEND Eigen_directory_files_to_install ${f})
    endif()
endforeach((f ${Eigen_directory_files}))

install(FILES
    ${Eigen_directory_files_to_install}
    DESTINATION ${INCLUDE_INSTALL_DIR}/Eigen COMPONET Devel
)

install(DIRECTORY src DESTINATION ${INCLUDE_INSTALL_DIR}/Eigen COMPONENT Devel FILES_MATCHING PATTERN "*.h")