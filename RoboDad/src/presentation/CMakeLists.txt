file(GLOB_RECURSE PRESENTATION_SOURCES *.cpp)

add_library(roboDadPresentation OBJECT ${PRESENTATION_SOURCES})

# Presentation depends on all other layers
target_link_libraries(roboDadPresentation PRIVATE
    roboDadDomain
    roboDadApplication
    roboDadInfrastructure
)
