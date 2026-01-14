# BrickWorlds

**MMO Minecraft Clone** - Ein massiv-mehrspieler Voxel-/Block-Spiel

![Build Status](https://github.com/UbivisMedia/BrickWorlds/actions/workflows/build.yml/badge.svg)

## 🎯 Projektstatus

Das Projekt befindet sich in **Phase 0 - Planung & Architektur**.

### ✅ Abgeschlossen

- Entwicklungsplan erstellt (`Entwicklungsplan.md`)
- Projekt-Grundstruktur mit CMake aufgesetzt
- GitHub Actions CI/CD Workflow für Windows und Linux
- Shared Library Grundstruktur
- `.gitignore` für C++/CMake Projekte

### 🚧 In Arbeit

- Client, Server und Master Module müssen noch erstellt werden

## 📁 Projektstruktur

```
BrickWorlds/
├── .github/workflows/    # CI/CD GitHub Actions
├── shared/               # Gemeinsame Library
│   ├── include/
│   │   └── BrickWorlds/
│   │       └── Version.h
│   ├── src/
│   └── CMakeLists.txt
├── client/               # TODO: Client-Anwendung
├── server/               # TODO: Server-Anwendung
├── master/               # TODO: Masterserver
├── CMakeLists.txt        # Root CMake-Konfiguration
├── .gitignore
├── README.md
└── Entwicklungsplan.md   # Detaillierter Entwicklungsplan
```

## 🔧 Nächste Schritte

Um das Projekt kompilierbar zu machen, müssen folgende Dateien erstellt werden:

### Client Module
```bash
client/CMakeLists.txt
client/src/main.cpp
```

### Server Module
```bash
server/CMakeLists.txt
server/src/main.cpp
```

### Master Module
```bash
master/CMakeLists.txt
master/src/main.cpp
```

## 🏗️ Build-Anleitung

### Voraussetzungen
- CMake 3.20 oder höher
- C++17 kompatible Compiler:
  - Windows: Visual Studio 2019/2022 oder MinGW
  - Linux: GCC 7+ oder Clang 5+

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
- `build/bin/BrickWorlds_Client[.exe]`
- `build/bin/BrickWorlds_Server[.exe]`
- `build/bin/BrickWorlds_Master[.exe]`

## 📖 Dokumentation

Der vollständige Entwicklungsplan mit allen Details zur Architektur, Technologie-Stack und Phasenplanung findet sich in [`Entwicklungsplan.md`](Entwicklungsplan.md).

## 🤝 Beitragen

Das Projekt ist in aktiver Entwicklung. Siehe `Entwicklungsplan.md` für die Roadmap.

## 📝 Lizenz

TBD

## 🔗 Links

- [Entwicklungsplan](Entwicklungsplan.md) - Detaillierte Projektplanung
- [GitHub Actions](.github/workflows/build.yml) - CI/CD Pipeline
