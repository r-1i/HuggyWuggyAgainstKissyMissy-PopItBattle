---
config:
  theme: dark
title: Run Lifecycle Activity Diagram
---

```mermaid
flowchart TD
    A([Start in TavernState]) --> B[Prepare party in Tavern]
    B --> C[Press "Go To Battle"]
    C --> D[Enter BattleState]
    D --> E[roomIndex = 1]

    E --> F{All player characters alive?}
    F -- No --> Z[finishRun(defeat)] --> T([Return to TavernState])
    F -- Yes --> G{roomIndex <= 100?}
    G -- No --> Y[finishRun(victory)] --> T
    G -- Yes --> H[Load room/enemy templates from AssetManager]

    H --> I[Create BattleRoom for current room]
    I --> J{BattleRoom ended?}
    J -- No --> K[Player phase: all players attack]
    K --> L[Enemy phase: all enemies attack]
    L --> J

    J -- Yes --> M{Player wiped?}
    M -- Yes --> Z
    M -- No --> N[Collect room rewards: coins + loot]
    N --> O{Last room cleared?}
    O -- Yes --> Y
    O -- No --> P[roomIndex++]
    P --> E
```

