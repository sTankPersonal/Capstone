# Presentation layer – Crow routes and controllers.
# Depends on the application layer (invokes use cases) and Crow/ASIO
# for HTTP primitives.  Must NOT link against roboDadInfrastructure;
# concrete implementations are injected at the composition root (main.cpp).
file(GLOB_RECURSE PRESENTATION_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(roboDadPresentation STATIC ${PRESENTATION_SOURCES})

target_link_libraries(roboDadPresentation PUBLIC
    roboDadApplication   # use cases this layer calls into
    roboDadDomain        # value objects used in route handlers
    server_headers       # controller/route header interfaces
    crow_asio            # Crow app, Request, Response types
    Threads::Threads
)