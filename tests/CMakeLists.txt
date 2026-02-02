add_executable(obscura_tests
        test_settle_rules.cpp
        test_diff_renderer.cpp
        test_input_decode.cpp
)
target_link_libraries(obscura_tests PRIVATE obscura)
add_test(NAME obscura_tests COMMAND obscura_tests)