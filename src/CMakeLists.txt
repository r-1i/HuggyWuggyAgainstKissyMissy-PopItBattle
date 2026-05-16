find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(huggy-pop-it-battle
    "huggy-pop-it-battle.cpp"
    "core/Game.cpp"
    "core/GameStateManager.cpp"
    "core/GameWorld.cpp"
    "states/TavernState.cpp"
    "ui/Button.cpp"
    "assets/data/JsonDataLoader.cpp"
    "states/IGameState.h"
    "entities/Hero.h"
    "entities/HeroTemplate.h"
    "entities/EnemyTemplate.h"
    "entities/RoomTemplate.h"
    "items/Item.h"
    "items/LootEntry.h"
    "items/LootTable.h"
    "items/ItemTemplate.h"
    "assets/AssetManager.h"
    "assets/data/IDataLoader.h"
    "assets/data/JsonDataLoader.h"
    "states/TavernState.h"
    "ui/HeroRecruitCard.h"
    "ui/Button.h"
)

target_link_libraries(huggy-pop-it-battle PRIVATE SFML::Graphics SFML::Window SFML::System nlohmann_json::nlohmann_json)

set_target_properties(huggy-pop-it-battle PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(huggy-pop-it-battle PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_custom_command(TARGET huggy-pop-it-battle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "C:/_Learning/CPP/SFML/SFML-3.0.2/bin"
    $<TARGET_FILE_DIR:huggy-pop-it-battle>
)

add_custom_command(TARGET huggy-pop-it-battle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/assets"
    "$<TARGET_FILE_DIR:huggy-pop-it-battle>/assets"
)