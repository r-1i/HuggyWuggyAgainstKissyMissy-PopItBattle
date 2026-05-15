sequenceDiagram
autonumber

participant BS as BattleState
participant GW as GameWorld
participant AM as AssetManager
participant BR as BattleRoom
participant P as PlayerParty
participant E as EnemyParty

Note over BS: Start battle run

loop roomIndex = 1..100
  BS->>GW: getCurrentParty()
  GW-->>BS: party
  BS->>BS: check that all player characters are alive

  alt dead characters found
    BS-->>BS: finish run (defeat)
  else all characters alive
    BS->>GW: getAssetManager()
    GW-->>BS: AssetManager&
    BS->>AM: getRoomTemplate(roomIndex)
    AM-->>BS: RoomTemplate
    BS->>AM: getEnemyTemplatesForRoom(roomIndex)
    AM-->>BS: EnemyTemplate[]

    BS->>BR: create BattleRoom(RoomTemplate, party)
    activate BR

    loop while room is not cleared and player is not wiped
      BS->>BR: update(dt)
      BR->>P: all player characters attack (in order)
      P-->>BR: damage applied to enemies
      BR->>E: all enemies attack (in order)
      E-->>BR: damage applied to players
    end

    BR-->>BS: isCleared()/isPlayerWiped(), coins/loot
    deactivate BR

    alt room cleared
      BS->>GW: addCoins(coins)
      BS->>GW: addItem(loot...)
    else player wiped
      BS-->>BS: finish run (defeat)
    end
  end
end

BS->>GW: finishRun(totalGold, totalLoot)
BS-->>BS: transition to TavernState