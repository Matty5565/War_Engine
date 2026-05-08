# War_Engine — Project Plan & Status

## Project Overview
WW2-themed 3D GIS wargaming simulation built in Unreal Engine 5.7.
Photorealistic northern France rendered from real-world geospatial data, streamed at runtime via Cesium for Unreal.

---

## Play Area (Phase 1)

| | |
|---|---|
| **Bounding box** | 48.45°N–49.77°N, 2.08°W–1.41°E |
| **Size** | ~270 km wide × 145 km tall (~39,000 km²) |
| **Center** | 49.114332°N, -0.339203°E (Argentan/Falaise, inland Normandy) |
| **Covers** | D-Day beaches, Caen, Cherbourg, Falaise Pocket, Alençon |
| **GIS projection** | EPSG:32631 (UTM Zone 31N) for all local data processing |

---

## Environment

| | |
|---|---|
| **Engine** | Unreal Engine 5.7 — `E:\Dev\UE_5.7\` |
| **Project** | `E:\Dev\War_Engine\` |
| **GitHub** | https://github.com/Matty5565/War_Engine (branch: main) |
| **C++ module** | WarEngine |
| **GIS plugin** | Cesium for Unreal (in `Plugins/CesiumForUnreal/`) |
| **Cesium ion** | Account created, token configured in editor |

---

## Folder Structure

```
War_Engine/
  Content/
    Maps/           ← UE5 levels (.umap)
    GIS/
      Terrain/      ← DEM / DTM / DSM tiles
      Imagery/      ← satellite / aerial textures
      Vectors/      ← roads, rails, buildings (raw + processed)
      Tiles/        ← 3D Tiles / Cesium tilesets
    Blueprints/
    Characters/
    VFX/
    CesiumSettings/ ← Cesium ion token asset (auto-generated)
  Source/
    WarEngine/      ← C++ module
  Plugins/
    CesiumForUnreal/
  RawData/          ← original downloaded datasets (never modify)
    DEM/
    Imagery/
    Vectors/
  Config/
```

---

## Status

### ✅ Done

- [x] UE5 C++ project created (`War_Engine.uproject`, Source module, Target files)
- [x] Content & RawData folder structure created on disk
- [x] Cesium for Unreal plugin installed to `Plugins/CesiumForUnreal/`
- [x] Git repo initialised and pushed to GitHub
  - `.gitignore` excludes: Binaries, Intermediate, Saved, RawData, Cesium ThirdParty/lib (too large for GitHub)
- [x] .NET 8.0 roll-forward patched in all UE5 DotNET runtimeconfig.json files (engine ships needing .NET 8, only 9 was installed)
- [x] Visual Studio 2022 Community installed with "Game development with C++" workload
- [x] VS project files generated via UnrealBuildTool
- [x] Editor build succeeded (143s, warnings only — no errors)
- [x] UE5 editor opened successfully
- [x] `Content/Maps/NormandyBaseMap.umap` level created and saved
- [x] Cesium World Terrain (asset ID 1) added to level
- [x] Cesium ion token saved to project
- [x] CesiumGeoreference origin set to Normandy center (49.114332°N, -0.339203°E)
- [x] Max quality settings written to `Config/DefaultEngine.ini`:
  - Lumen GI + Lumen Reflections
  - Virtual Shadow Maps
  - Nanite enabled
  - Temporal Super Resolution (TSR)
  - Texture streaming pool: 2 GB
- [x] `IncludeOrderVersion = Unreal5_7` set in both Target.cs files

---

### 🔧 In Progress

- [ ] **Black screen on PIE** — level has no lighting actors or player pawn yet
  - Needs: Directional Light (atmosphere sun), Sky Atmosphere, Sky Light, DynamicPawn
  - Fix: add those 4 actors in editor + set Default Pawn Class to `DynamicPawn` in World Settings
- [ ] **World Bounds Checks** — Cesium warns this should be disabled
  - Fix: World Settings → World → uncheck "Enable World Bounds Checks"

---

### 📋 Next Up

#### Phase 2 — Playable Viewport
- [ ] Auto-configure lighting via C++ (so level sets itself up on load)
- [ ] Add Bing Maps / Sentinel-2 imagery raster overlay to terrain
- [ ] Verify satellite imagery streaming over Normandy bbox
- [ ] Tune Cesium tile loading radius and LOD settings
- [ ] Add Exponential Height Fog for atmosphere

#### Phase 3 — GIS Data Layers
- [ ] Download Normandy DEM from IGN France (RGE ALTI 1m or 5m)
- [ ] Download OSM road & railway network for the bbox
- [ ] Download Corine Land Cover for vegetation/land-use
- [ ] Download building footprints (OSM / cadastral)
- [ ] Process DEM → GeoTIFF → import to UE5 or upload to Cesium ion
- [ ] Process roads/rails → vector overlay or 3D Tiles

#### Phase 4 — WW2 Content
- [ ] WW2-era unit actors (infantry, vehicles, artillery)
- [ ] Basic movement and order system
- [ ] Terrain analysis (line of sight, elevation queries)
- [ ] WW2 order of battle data for Normandy (June–August 1944)

#### Phase 5 — Simulation Systems
- [ ] Turn-based or real-time simulation loop
- [ ] Combat resolution
- [ ] Supply lines and logistics layer
- [ ] Historical scenario: Operation Overlord (D-Day)

---

## Key Technical Notes

- **Coordinate system**: Cesium uses WGS84 internally. All local GIS processing in EPSG:32631 (UTM 31N).
- **Scale**: 1:1 real-world. 1 Unreal Unit = 1 cm. Terrain at real elevation.
- **Cesium ThirdParty libs**: NOT in git (too large). Re-download Cesium plugin zip to restore if needed.
- **git binary**: Not in PATH — use `C:\Users\matth\AppData\Local\GitHubDesktop\app-3.5.4\resources\app\git\cmd\git.exe`
- **UBT .NET fix**: `rollForward: latestMajor` added to all `Engine/Binaries/DotNET/**/runtimeconfig.json` files so UBT runs on .NET 9.

---

## GIS Data Sources (planned)

| Layer | Source | Format |
|---|---|---|
| Terrain (DEM) | IGN France — RGE ALTI | GeoTIFF |
| Satellite imagery | Sentinel-2 / IGN Ortho HR | GeoTIFF / WMTS |
| Roads & railways | OpenStreetMap (Overpass API) | GeoJSON / Shapefile |
| Buildings | OSM / cadastral | GeoJSON |
| Land cover | Corine Land Cover 2018 | GeoTIFF / Shapefile |
| Infrastructure | OSM (bridges, ports, airfields) | GeoJSON |
