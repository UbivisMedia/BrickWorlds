# Entwicklungsplan – C++ Massively Multiplayer Open World Spiel

## 1. Zielbild und Leitprinzipien

### Zielbild
- Voxel-/Blockwelt ähnlich Minecraft  
- Modular erweiterbar über JSON-Dateien (Blöcke, Items, Rezepte, Regeln)
- Jeder Spieler kann gleichzeitig als **Server-Host** fungieren
- Zentrale **Masterserver-Instanz** für:
  - Weltverknüpfung
  - Spieler-Identitäten
  - Anti-Cheat- und Integritätsprüfungen
- Zusätzlich **dedizierte Server** (Windows & Linux)
- Keine große kommerzielle Engine, nur leichtgewichtige Grafik-/Render-Layer
- Initiale Inhalte:
  - Blöcke: Erde, Fels, Wasser (dynamisch), Holz
  - Items: Werkbank, Axt, Schwert

### Leitprinzipien
- Server-autoritative Simulation
- Datengetriebenes Design (JSON first)
- Plattformübergreifende Server-Software
- Modular, auditierbar, erweiterbar

---

## 2. Systemarchitektur (High-Level)

### Komponenten
- **Client**
  - Rendering, UI, Input
  - Netzwerk-Client
  - JSON-Content-Loader
- **Player-Hosted Server**
  - Autoritative Welt-Simulation
  - Serverregeln (JSON)
  - Persistence (Welt speichern/laden)
- **Dedizierter Server**
  - Headless-Version des Player-Servers
  - Admin- & Konfigurationsschnittstellen
- **Masterserver**
  - Authentifizierung
  - Server-/Weltverzeichnis
  - Anti-Cheat & Verifikation
  - Join-Tokens & Routing

---

## 3. Kernentscheidungen

### Netzwerkmodell
- Autoritative Server
- Clients senden nur Intentionen
- UDP für Gameplay, TCP/HTTP für Masterserver
- Snapshot- & Delta-Replikation für Chunks

### Anti-Cheat-Grundsatz
- Kein vollständiger Schutz bei Player-Hosts möglich
- Einführung von Trust-Leveln:
  - Unverified
  - Verified (signierte Builds)
- Event-Logs & Auditierung über Masterserver

### Modding & Erweiterbarkeit
- JSON nur für Content, kein ausführbarer Code
- Mods mit Manifest, Version & Hash

---

## 4. Content- & Datenmodell (JSON)

### Ordnerstruktur
- `content/blocks/*.json`
- `content/items/*.json`
- `content/recipes/*.json`
- `content/rules/*.json`
- `assets/textures/*`
- `mods/<modid>/manifest.json`

### Block-Attribute (Beispiel)
- `id`
- `displayName`
- `hardness`
- `breakTime`
- `drops`
- `render`
- `physics`
- `tags`

### Wasser-Mechanik (MVP)
- Flüssigkeitslevel pro Block (z. B. 0–7)
- Flussregeln:
  - Bevorzugt nach unten
  - Horizontal bei Blockierung
- Begrenzte Updates pro Tick

---

## 5. Serverregeln (JSON-basiert)

- PvP an/aus
- Inventarverlust bei Tod
- Bau-/Abbaurechte
- Max. Spieler
- Mod-Policy

Regeln:
- Werden beim Join übertragen
- Sind für Spieler einsehbar

---

## 6. Technologie-Stack

### Build & Tooling
- CMake
- CI für Windows & Linux
- Static Analysis & Sanitizer

### Rendering
- Leichtgewichtige Grafikbibliothek
- Chunk-Meshing
- Texture-Atlas
- Basis-Lighting

### Backend
- Masterserver mit HTTP/JSON API
- Datenbank: PostgreSQL (empfohlen)
- Kryptografie für Tokens & Signaturen

---

## 7. Entwicklungsphasen

### Phase 0 – Planung & Architektur
- Architekturdokumentation
- Netzwerk- & Content-Spezifikation
- Projektstruktur aufsetzen

### Phase 1 – Lokaler MVP
- Singleplayer / lokaler Host
- Chunk-System & Rendering
- Blockplatzierung & -abbau
- JSON-Content-Lader

### Phase 2 – Netzwerkfähiger Server
- Separater Serverprozess
- Chunk- & Entity-Replikation
- Grundlegende Serverregeln

### Phase 3 – Masterserver Basis
- Login & Auth
- Server-Discovery
- Join-Tokens
- Versions- & Mod-Prüfung

### Phase 4 – Anti-Cheat Stufe 1
- Serverseitige Validierungen
- Rate-Limits
- Konsistenzprüfungen

### Phase 5 – Anti-Cheat Stufe 2
- Signierte Builds
- Event-Audits
- Trust-Level-System

### Phase 6 – Dedizierter Server
- Headless-Betrieb
- Konfigurationsdateien
- Monitoring & Logging

### Phase 7 – Weltverknüpfung
- Weltwechsel über Masterserver
- Globale Spieleridentität
- Cross-World-Mechaniken

### Phase 8 – Mod-Ökosystem
- Mod-Distribution
- Abhängigkeiten & Versionierung
- Kompatibilitätsprüfungen

---

## 8. Code-Struktur (Vorschlag)

- `shared/`
  - Core, Math, Serialization
  - Content-Registries
  - Netzwerkprotokolle
- `server/`
  - Simulation
  - Persistence
  - Master-Anbindung
- `client/`
  - Rendering
  - UI
  - Netzwerk
- `master/`
  - API
  - Auth
  - Verifikation

---

## 9. Qualitätssicherung

- Unit-Tests (JSON, Simulation)
- Integrationstests (Client/Server)
- Security-Tests (Fuzzing)
- Performance-Benchmarks

---

## 10. Risiken & Gegenmaßnahmen

- **Cheating bei Player-Hosts**
  - Trust-Level & Verifikation
- **NAT/Erreichbarkeit**
  - Dedizierte Server
- **Voxel-Performance**
  - Optimiertes Meshing
- **Mod-Konflikte**
  - Namespaces & Schemas

---

## 11. MVP-Backlog

### Gameplay
- Basisblöcke & Items
- Werkbank & Rezepte
- Inventar & Hotbar

### Technik
- Chunk-System
- Netzwerk-Replikation
- Masterserver (minimal)

### Content
- JSON-Definitionen für:
  - Blöcke
  - Items
  - Rezepte
  - Serverregeln
