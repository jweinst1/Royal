# src cmake

include_directories(${PROJECT_SOURCE_DIR}/include)

set(ROYAL_CLIB_SOURCES
   royal_ids.c
   royal_market.c
   royal_bytes.c
)

add_library(royal SHARED "${ROYAL_CLIB_SOURCES}")
add_library(royals STATIC "${ROYAL_CLIB_SOURCES}")