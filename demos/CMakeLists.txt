add_executable(demo_menu_ecology demo_menu_ecology.cpp)
target_link_libraries(demo_menu_ecology PRIVATE obscura)

add_executable(demo_heatmap demo_heatmap.cpp)
target_link_libraries(demo_heatmap PRIVATE obscura)

add_executable(demo_rules_playground demo_rules_playground.cpp)
target_link_libraries(demo_rules_playground PRIVATE obscura)

add_custom_target(run_demo_menu_ecology
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/run_in_terminal.sh $<TARGET_FILE:demo_menu_ecology>
    DEPENDS demo_menu_ecology
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(run_demo_heatmap
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/run_in_terminal.sh $<TARGET_FILE:demo_heatmap>
    DEPENDS demo_heatmap
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(run_demo_rules_playground
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/run_in_terminal.sh $<TARGET_FILE:demo_rules_playground>
    DEPENDS demo_rules_playground
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
