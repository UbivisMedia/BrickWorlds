# BrickWorlds

**MMO Minecraft Clone** - Ein massiv-mehrspieler Voxel-/Block-Spiel

![Build Status](https://github.com/UbivisMedia/BrickWorlds/actions/workflows/build.yml/badge.svg)

## 🎯 Projektstatus

Das Projekt befindet sich in **Phase 1 – Lokaler MVP**.

### ✅ Abgeschlossen

**Infrastruktur & Build-System**
- Entwicklungsplan erstellt (`Entwicklungsplan.md`)
- Projekt-Grundstruktur mit CMake aufgesetzt
- GitHub Actions CI/CD Workflow für Windows und Linux
- `.gitignore` für C++/CMake Projekte

**Shared Library**
- Shared Library Grundstruktur
- Core-Typen und -Strukturen (`Types.h`)
- Chunk-System (`Chunk.h`) mit Voxel-Verwaltung
- World-Manager (`World.h`) mit Chunk-Management und Flat-World-Generierung
- Version-Header

**Client-Modul**
- Client lauffähig unter Windows und Linux
- Basis-Rendering-System mit OpenGL
- Kamera-System mit freier Bewegung
- Chunk-Meshing für Voxel-Rendering
- Shader-System (Vertex & Fragment Shaders)
- Einfaches Lighting-System
- Maus- und Tastatur-Input

**Content-System**
- JSON-basiertes Content-Verzeichnis (`content/blocks/`)
- Block-Definitionen für Dirt und Stone

**Server & Master**
- Master-Server Modul mit Grundstruktur
- Server-Modul mit Grundstruktur

### 🚧 In Arbeit

**Phase 1 – Lokaler MVP (laufend)**
- Verbesserung des Rendering-Systems
- Erweiterung der Block-Typen
- Optimierung des Chunk-Systems
- Block-Platzierung und -Abbau
- JSON-Content-Loader-Integration

**Geplante nächste Schritte:**
- Spieler-Interaktion (Block platzieren/abbauen)
- Inventar-System
- Physik und Kollisionserkennung
- Erweiterte Welt-Generierung (Terrain, Höhen)

## 📁 Projektstruktur

```
BrickWorlds/
├── .github/workflows/    # CI/CD GitHub Actions
├── shared/               # Gemeinsame Library
│   ├── include/
│   │   └── BrickWorlds/
│   │       ├── Core/
│   │       │   ├── Types.h       # Basis-Typen
│   │       │   ├── Chunk.h       # Chunk-System
│   │       │   └── World.h       # World-Manager
│   │       └── Version.h
│   ├── src/
│   └── CMakeLists.txt
├── client/               # Client-Anwendung
│   ├── src/
│   │   ├── Camera.cpp/h      # Kamera-System
│   │   ├── ChunkMesh.cpp/h   # Chunk-Meshing
│   │   ├── Renderer.cpp/h    # Rendering-Engine
│   │   ├── Shader.cpp/h      # Shader-Verwaltung
│   │   └── main.cpp          # Client Entry Point
│   ├── shaders/              # GLSL Shaders
│   └── CMakeLists.txt
├── server/               # Server-Anwendung
│   ├── src/
│   │   └── main.cpp
│   └── CMakeLists.txt
├── master/               # Masterserver
│   ├── src/
│   │   └── main.cpp
│   └── CMakeLists.txt
├── content/              # JSON Content
│   └── blocks/
│       ├── dirt.json         # Erde-Block
│       └── stone.json        # Stein-Block
├── CMakeLists.txt        # Root CMake-Konfiguration
├── .gitignore
├── README.md
└── Entwicklungsplan.md   # Detaillierter Entwicklungsplan
```

## 🏗️ Build-Anleitung

### Voraussetzungen

- CMake 3.20 oder höher
- C++17 kompatible Compiler:
  - Windows: Visual Studio 2019/2022 oder MinGW
  - Linux: GCC 7+ oder Clang 5+
- OpenGL 3.3+ kompatible Grafikkarte
- GLFW3 (wird automatisch heruntergeladen)
- GLEW (wird automatisch heruntergeladen)
- GLM (wird automatisch heruntergeladen)

### Kompilieren

```bash
# Repository klonen
git clone https://github.com/UbivisMedia/BrickWorlds.git
cd BrickWorlds

# Build-Verzeichnis erstellen
mkdir build
cd build

# CMake konfigurieren
cmake ..

# Kompilieren
cmake --build . --config Release
```

### Ausgabe

Die kompilierten Binaries befinden sich in:

- `build/bin/BrickWorlds_Client[.exe]` - Der Voxel-Client
- `build/bin/BrickWorlds_Server[.exe]` - Der Game-Server
- `build/bin/BrickWorlds_Master[.exe]` - Der Master-Server

### Client starten

```bash
# Aus dem build-Verzeichnis
./bin/BrickWorlds_Client

# Oder unter Windows
.\bin\BrickWorlds_Client.exe
```

**Steuerung:**
- `W/A/S/D` - Bewegung
- `Leertaste` - Nach oben
- `Shift` - Nach unten
- `Maus` - Kamera drehen
- `ESC` - Beenden

## 🎮 Aktueller Feature-Stand

### Implementiert ✅

- ✅ 3D-Voxel-Rendering mit OpenGL
- ✅ Chunk-basiertes Weltsystem (16×16×256 Blöcke pro Chunk)
- ✅ Flat-World-Generierung
- ✅ Freie Kamera mit WASD-Steuerung
- ✅ Einfaches Lighting
- ✅ JSON-basierte Block-Definitionen
- ✅ Cross-Platform (Windows & Linux)

### In Entwicklung 🚧

- 🚧 Block-Platzierung und -Abbau
- 🚧 Erweiterte Welt-Generierung (Terrain)
- 🚧 Spieler-Physik und Kollision

### Geplant 📋

- 📋 Inventar-System
- 📋 Netzwerk-Multiplayer
- 📋 Rezept-System
- 📋 Dynamisches Wasser
- 📋 Items & Werkzeuge

## 📖 Dokumentation

Der vollständige Entwicklungsplan mit allen Details zur Architektur, Technologie-Stack und Phasenplanung findet sich in [`Entwicklungsplan.md`](Entwicklungsplan.md).

### Wichtige Dokumente

- **[Entwicklungsplan](Entwicklungsplan.md)** - Komplette Roadmap mit 8 Entwicklungsphasen
- **[GitHub Actions](.github/workflows/build.yml)** - CI/CD Pipeline für automatisierte Builds

### Entwicklungsphasen (Übersicht)

Laut Entwicklungsplan sind folgende Phasen vorgesehen:

1. **Phase 1 – Lokaler MVP** ← *Aktuell hier*
2. **Phase 2 – Netzwerkfähiger Server**
3. **Phase 3 – Masterserver Basis**
4. **Phase 4 – Anti-Cheat Stufe 1**
5. **Phase 5 – Anti-Cheat Stufe 2**
6. **Phase 6 – Dedizierter Server**
7. **Phase 7 – Weltverknüpfung**
8. **Phase 8 – Mod-Ökosystem**

## 🤝 Beitragen

Das Projekt ist in aktiver Entwicklung. Siehe `Entwicklungsplan.md` für die Roadmap.

## 📝 Lizenz

TBD

## 🔗 Links

- [Entwicklungsplan](Entwicklungsplan.md) - Detaillierte Projektplanung
- [GitHub Actions](.github/workflows/build.yml) - CI/CD Pipeline
- [Issues](https://github.com/UbivisMedia/BrickWorlds/issues) - Bug-Reports und Feature-Requests
