#set(CMAKE_VERBOSE_MAKEFILE ON) 

cmake_minimum_required(VERSION 3.5)

project(game_text C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall --coverage")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/default.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/solution.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
link_directories(${CMAKE_SOURCE_DIR})

#file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# find SDL2
include(sdl2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

# compilation rules
include_directories(${SDL2_ALL_INC})
add_executable(game_sdl main.c game_sdl.c)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m)

add_executable(game_text game_text.c)
add_executable(game_test_werobert game_test_werobert.c)
add_executable(game_test_lindrano game_test_lindrano.c)
add_executable(game_solve game_solve.c)
add_library(game game.c game_aux.c game_ext.c game_tools.c queue.c)

target_link_libraries(game_text game)
target_link_libraries(game_test_werobert game)
target_link_libraries(game_test_lindrano game)
target_link_libraries(game_solve game)
target_link_libraries(game_sdl game)

add_test(test_werobert_dummy ./game_test_werobert dummy)
add_test(test_lindrano_dummy ./game_test_lindrano dummy)

add_test(test_werobert_game_default ./game_test_werobert game_default)
add_test(test_werobert_game_new_empty ./game_test_werobert game_new_empty)
add_test(test_werobert_game_delete ./game_test_werobert game_delete)
add_test(test_werobert_game_get_constraint ./game_test_werobert game_get_constraint)
add_test(test_werobert_game_won ./game_test_werobert game_won)
add_test(test_werobert_game_get_status ./game_test_werobert game_get_status)

add_test(test_werobert_game_new_ext ./game_test_werobert game_new_ext)
add_test(test_werobert_game_new_empty_ext ./game_test_werobert game_new_empty_ext)
add_test(test_werobert_game_undo ./game_test_werobert game_undo)
add_test(test_werobert_game_redo ./game_test_werobert game_redo)

#partie de Aure
add_test(test_werobert_game_default_solution ./game_test_werobert game_default_solution)
add_test(test_werobert_game_copy ./game_test_werobert game_copy)
add_test(test_werobert_game_set_constraint ./game_test_werobert game_set_constraint)
add_test(test_werobert_game_get_color ./game_test_werobert game_get_color)
add_test(test_werobert_game_nb_neighbors ./game_test_werobert game_nb_neighbors)
add_test(test_werobert_game_restart ./game_test_werobert game_restart)

add_test(test_lindrano_game_new ./game_test_lindrano game_new)
add_test(test_lindrano_game_set_color ./game_test_lindrano game_set_color)
add_test(test_lindrano_game_print ./game_test_lindrano game_print)
add_test(test_lindrano_game_equal ./game_test_lindrano game_equal)
add_test(test_lindrano_game_play_move ./game_test_lindrano game_play_move)
add_test(test_lindrano_game_get_next_square ./game_test_lindrano game_get_next_square)

add_test(test_lindrano_game_nb_cols ./game_test_lindrano game_nb_cols)
add_test(test_lindrano_game_nb_rows ./game_test_lindrano game_nb_rows)
add_test(test_lindrano_game_is_wrapping ./game_test_lindrano game_is_wrapping)
add_test(test_lindrano_game_get_neighbourhood ./game_test_lindrano game_get_neighbourhood)


add_test(test_lindrano_game_load ./game_test_lindrano game_load)
add_test(test_werobert_game_save ./game_test_werobert game_save)

add_test(test_werobert_game_nb_solutions ./game_test_werobert game_nb_solutions)
add_test(test_lindrano_game_solve ./game_test_lindrano game_solve)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})