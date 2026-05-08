# War_Engine — AI Context & Project Guide

## Project Aim
A WW2-era 3D GIS wargaming simulation set in Normandy, France. Built in Unreal Engine 5.7 C++ with Cesium for Unreal providing real-world terrain and satellite imagery. The long-term goal is a top-down operational wargame where players command WW2 units across accurate Normandy terrain, with real GIS data (elevation, roads, towns, coastline) driving gameplay.

## Tech Stack
- **Engine**: Unreal Engine 5.7 — installed at `E:\Dev\UE_5.7\`
- **Plugin**: Cesium for Unreal v2.26.0 — local copy at `Plugins/CesiumForUnreal/`
- **Terrain**: Cesium World Terrain (streamed via Cesium ion)
- **Imagery**: IGN Géoportail WMTS — 20cm/px orthophotos over Normandy
- **Buildings**: Cesium OSM Buildings tileset
- **Rendering**: Lumen GI, Virtual Shadow Maps, Nanite, TSR (configured in DefaultEngine.ini)
- **GPU**: GTX 1070 Ti (SM5, software Lumen — no hardware RT)
- **IDE**: Visual Studio 2026 Community (14.50) — non-preferred version warning is harmless
- **Git**: GitHub Desktop bundled git at `C:\Users\matth\AppData\Local\GitHubDesktop\app-3.5.4\resources\app\git\cmd\git.exe`

## Normandy Bounding Box
- **Min Lat/Lon**: 48.454709, -2.084656
- **Max Lat/Lon**: 49.768848, 1.408997
- **Center**: 49.114332°N, 0.339203°W
- **Cesium Georeference Origin**: 49.35°N, 1.30°W, 0m

---

## What Has Been Built

### Phase 1 — UE5 C++ Project Bootstrap
Created the full project structure manually (no Epic Launcher GUI needed):
- `War_Engine.uproject` — project descriptor with Cesium plugin entry
- `Source/WarEngine/WarEngine.Build.cs` — module deps: Core, CoreUObject, Engine, InputCore, CesiumRuntime
- `Source/WarEngine/WarEngine.h/.cpp` — module entry point
- `Source/WarEngine.Target.cs` / `WarEngineEditor.Target.cs` — game + editor targets
- `Config/DefaultEngine.ini` — Lumen, VSM, Nanite, TSR, 2GB texture pool, GlobalDefaultGameMode
- `Config/DefaultGame.ini` — GlobalDefaultGameMode=/Script/WarEngine.WarEngineGameMode
- `Config/DefaultInput.ini` — Enhanced Input component

### Phase 2 — Cesium Terrain & Imagery
- Cesium for Unreal plugin installed to `Plugins/CesiumForUnreal/`
- Level `Content/Maps/NormandyBaseMap` created with:
  - CesiumGeoreference (origin: 49.35°N, 1.30°W)
  - Cesium World Terrain tileset
  - CesiumSunSky for lighting
  - OSM Buildings tileset
  - PostProcessVolume (Manual Exposure, Compensation=-2.0 to avoid overexposure)
  - IGN Géoportail WMTS overlay (see settings below)

### Phase 3 — RTS Camera Pawn (C++)
`Source/WarEngine/RTSPawn.h/.cpp` — top-down locked camera:
- Fixed -90° pitch (true top-down, no rotation)
- WASD + arrow keys to pan
- Scroll wheel to zoom (altitude change)
- Mouse edge scroll (hover near viewport edge)
- Pan speed scales with altitude for consistent feel
- Altitude clamped: 500m–15,000m
- Lat/lon clamped to Normandy bbox
- All parameters are UPROPERTY — editable in Details panel

`Source/WarEngine/WarEngineGameMode.cpp` — sets DefaultPawnClass = ARTSPawn::StaticClass()

---

## Known Issues & Workarounds

### .NET version mismatch
UBT requires .NET 8 but machine has .NET 9. Fixed by adding `"rollForward": "latestMajor"` to all `runtimeconfig.json` files under `E:\Dev\UE_5.7\Engine\Binaries\DotNET\`.

### Cesium ThirdParty libs too large for GitHub
Files like `s2.lib` (144MB) exceed GitHub's 100MB limit. Excluded via `.gitignore`:
```
Plugins/CesiumForUnreal/Source/ThirdParty/lib/
```
These are rebuilt locally — do not commit them.

### VS 2026 "not preferred" warning
UBT prefers VS 2022 (14.44). VS 2026 (14.50) works fine — the warning is non-blocking.

---

## Build Instructions

**Always close the UE5 editor before building** (Live Coding blocks external builds).

```powershell
# Build the editor (run from any PowerShell window)
& "E:\Dev\UE_5.7\Engine\Build\BatchFiles\Build.bat" WarEngineEditor Win64 Development "E:\Dev\War_Engine\War_Engine.uproject" -WaitMutex
```

First build: ~5–15 min. Incremental (after small code changes): ~1–3 min.

---

## Editor Setup Steps (do after every fresh clone or engine reinstall)

### 1. Generate VS solution
```powershell
& "E:\Dev\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="E:\Dev\War_Engine\War_Engine.uproject" -game -rocket -progress
```

### 2. Open the project
Double-click `War_Engine.uproject` — UE5 editor opens. If it asks to rebuild, click Yes.

### 3. Set Game Mode in World Settings (REQUIRED each session if not saved to level)
1. Open level `Content/Maps/NormandyBaseMap`
2. In the top menu: **Window → World Settings**
3. Under **Game Mode**, set **GameMode Override** to **WarEngineGameMode**
4. Save the level (**Ctrl+S**)

> This only needs doing once per level — it saves into the .umap file.

### 4. Verify RTSPawn is spawning
1. Press **Play** (green triangle in toolbar)
2. Open **Window → Outliner**
3. Confirm **RTSPawn0** and **WarEngineGameMode0** appear in the list
4. If you see **DynamicPawn** instead: stop Play, go to World Settings, set GameMode Override to WarEngineGameMode, delete the DynamicPawn actor from the Outliner, press Play again

### 5. Keyboard input in PIE
Click inside the viewport first to give it focus, then WASD/arrow keys will pan the camera.

---

## IGN Géoportail WMTS Setup

If the imagery overlay is missing after a fresh setup:
1. In the Outliner, click **Cesium World Terrain**
2. In Details panel, scroll to **Cesium Raster Overlays**
3. Click **+ Add** → **CesiumWebMapTileServiceRasterOverlay**
4. Set:
   - **Base URL**: `https://data.geopf.fr/wmts`
   - **Layer**: `ORTHOIMAGERY.ORTHOPHOTOS`
   - **Style**: `normal`
   - **Format**: `image/jpeg`
   - **Tile Matrix Set ID**: `PM`
   - **Maximum Level**: `20`
5. Click **Refresh**

> 20cm/px resolution only loads when altitude is under ~1km. Higher altitudes show lower-res tiles — this is normal WMTS behaviour.

---

## Git Workflow

```powershell
$git = "C:\Users\matth\AppData\Local\GitHubDesktop\app-3.5.4\resources\app\git\cmd\git.exe"
cd E:\Dev\War_Engine
& $git add -A
& $git commit -m "your message"
& $git push
```

Remote: https://github.com/Matty5565/War_Engine

---

## RTSPawn Tunable Properties (edit in Details panel in-editor)

| Property | Default | Description |
|---|---|---|
| MinAltitudeMeters | 500 | Lowest zoom (metres) |
| MaxAltitudeMeters | 15000 | Highest zoom (metres) |
| PanSpeedBase | 8.0 | cm/s per metre of altitude |
| ZoomStepMeters | 600 | Altitude change per scroll tick |
| EdgeScrollMarginPx | 20 | Pixel margin from edge that triggers scroll |
| EdgeScrollSpeed | 0.3 | Edge scroll speed relative to WASD (1.0 = same) |
| MinLat/MaxLat/MinLon/MaxLon | Normandy bbox | Camera boundary |

---

## Roadmap (not yet started)
- Unit actors (infantry, armour, artillery) with WW2 OOB data
- Turn-based or real-time command system
- Line of sight / fog of war using terrain elevation
- Supply lines and logistics
- Historical scenario scripting (D-Day, Cobra, Falaise)
- UI — minimap, unit info panel, order queue
