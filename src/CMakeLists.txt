

add_executable(MAIN
    main.cpp
    mainwindow.cpp
    nodewidget.cpp
    chartwidget.cpp
    analysis.cpp
    end-host.cpp
    graph.cpp
    link.cpp
    node.cpp
    packet.cpp
)
  target_compile_options(MAIN PRIVATE -fPIC -Wall -Wextra -pedantic -Wno-missing-field-initializers)
  target_link_libraries(MAIN Qt5::Widgets Qt5::Charts)

#  add_executable(MAIN2
#     main2.cpp
#     analysis.cpp
#     end-host.cpp
#     graph.cpp
#     link.cpp
#     node.cpp
#    packet.cpp
# )
# target_compile_options(MAIN2 PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
