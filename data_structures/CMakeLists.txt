set(SOURCES 
    clist.h
    clist.c

    llist.h
    llist.c

    pool.h
    pool.c

    vector.h
    vector.c
)

list(TRANSFORM SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

target_sources(${PROJECT_NAME}
    PRIVATE 
    ${SOURCES}
)
