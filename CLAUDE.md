# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Language Settings

**Primary Language**: Japanese
- All responses should be in Japanese unless specifically requested otherwise
- Code comments should be in Japanese when adding new comments
- Commit messages should be in Japanese

## Project Overview

AstroGolf is a C++ physics-based golf game where players navigate a golf ball through gravitational fields created by planets. The game uses the DxLib graphics library and modern C++20 features with an Entity-Component-System architecture.

## Build System

**Build Tool**: Visual Studio 2022 with MSBuild (.vcxproj project file)
**Language**: C++20 with UTF-8 encoding support
**Platforms**: Win32 and x64 (Debug/Release configurations)
**Dependencies**: Managed via vcpkg with manifest mode

### Build Commands
```bash
# Build with Visual Studio
msbuild AstroGolf.sln /p:Configuration=Debug /p:Platform=x64
msbuild AstroGolf.sln /p:Configuration=Release /p:Platform=x64

# Or use Visual Studio IDE
# Open AstroGolf.sln in Visual Studio 2022
# Build -> Build Solution (Ctrl+Shift+B)
```

### Dependencies (vcpkg.json)
- **fmt**: String formatting library
- **imgui**: Immediate mode GUI (with DirectX 11 and Win32 bindings)
- **spdlog**: High-performance logging
- **nlohmann-json**: JSON parsing for stage configuration
- **DxLib**: Japanese 2D graphics library (included in lib/dxlib/)

## Architecture Overview

### Entity-Component-System (ECS)
- **Base Class**: `Component` (src/component/Component.h)
- All game objects inherit from Component with lifecycle methods: `Init()`, `Update()`, `PostUpdate()`, `Draw()`
- Components have transforms, world references, z-indexing, and parent-child relationships
- Physics-enabled components inherit from `PhysicsComponent`

### World-Based Scene Management
- **Base Class**: `World` (src/world/World.h)
- Each game scene is a World containing components managed via ID-based map system
- **World Types**: TitleWorld, StageWorld, StageSelectWorld, DemoWorld
- Built-in camera, player, and ballistics systems per world

### Core Game Loop
- **Main Class**: `Game` (src/Game.h) - Singleton pattern for global state
- World transition system with visual effects (Circle/Slide transitions)
- Delta time management, FPS tracking, pause/resume functionality

## Key Source Structure

```
src/
├── component/              # All game components (ECS entities)
│   ├── Component.h/cpp     # Base component class
│   ├── PhysicsComponent.h  # Physics-enabled base class
│   ├── PlayerComponent.h   # Golf ball with physics/trail
│   ├── PlanetComponent.h   # Gravity-affecting celestial bodies
│   ├── misc/              # Utility components (ballistics, UI)
│   ├── select/            # Stage selection UI
│   ├── title/             # Title screen components
│   └── object/            # General game objects
├── world/                 # Scene management
│   ├── World.h/cpp        # Base world class
│   ├── TitleWorld.h       # Title screen
│   ├── StageWorld.h       # Main gameplay
│   └── StageSelectWorld.h # Stage selection
├── math/                  # Physics and collision system
│   ├── Vec2.h/cpp         # 2D vector operations
│   ├── Transform.h/cpp    # Position/rotation/scale
│   ├── *Collider.h/cpp    # Collision detection (Circle, Box, Hole, etc.)
│   └── DrawStack.h        # Rendering transformation stack
├── game/                  # Game management
│   └── StageManager.h/cpp # Stage loading and configuration
├── graph/                 # Graphics utilities
│   └── Graph.h/cpp        # Sprite/texture management
└── debug/                 # Debug tools
    └── Debug.h/cpp        # Debug rendering and ImGui integration
```

### Game-Specific Systems
- **Physics**: Gravity-based golf mechanics with planetary gravity sources
- **Collision**: Multiple collider types (Circle, Box, RotatableBox, Hole)
- **Stage System**: JSON-based stage definitions (assets/stages.json)
- **Challenge System**: HoleInOne, Excellent, Goal challenges per stage
- **Trail Rendering**: Ball trajectory visualization

## Development Patterns

### Adding New Components
1. Inherit from `Component` or `PhysicsComponent`
2. Implement lifecycle methods: `Init()`, `Update()`, `PostUpdate()`, `Draw()`
3. Add to appropriate subdirectory under src/component/
4. Update AstroGolf.vcxproj with new .h/.cpp files

### Adding New Stages
1. Modify assets/stages.json with stage configuration
2. Create stage-specific components if needed
3. Consider creating specialized World class for complex stages

### Physics Extensions
- Extend `PhysicsComponent` for new mechanics
- Use existing collision system: CircleCollider, BoxCollider, HoleCollider, etc.
- Gravity sources implement via PlanetComponent

## Debug Tools

- **F3 Key**: Toggle debug mode with collision visualization
- **ImGui Integration**: Runtime parameter tweaking and debugging
- **Logging**: spdlog throughout application (use Debug.h for debug output)

## Asset Organization

```
assets/
├── fonts/          # M+ and Outfit font files
├── sprite/         # Game graphics organized by category
│   ├── menu/       # UI elements
│   ├── preview/    # Stage preview images
│   ├── stage/      # Stage-specific graphics
│   └── title/      # Title screen graphics
├── movie/          # Video files (effects)
└── stages.json     # Stage configuration data
```

## Important Notes

- **Japanese Content**: Game contains Japanese text/assets
- **DxLib Integration**: Use Graph.h for texture loading, avoid direct DxLib calls in components
- **Memory Management**: Components auto-managed by World, use smart pointers where appropriate
- **UTF-8 Encoding**: Project configured for UTF-8, handles Japanese characters correctly