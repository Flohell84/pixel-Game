# pixel-Game

Ein Pixel-Adventure für PC mit **Unreal Engine 5 (C++)**.

## Projektinhalt

- Unreal Projektdatei: `/home/runner/work/pixel-Game/pixel-Game/PixelAdventure.uproject`
- C++ Modul: `/home/runner/work/pixel-Game/pixel-Game/Source/PixelAdventure`
- Eingaben: `/home/runner/work/pixel-Game/pixel-Game/Config/DefaultInput.ini`

## Bereits umgesetzt

- Prozedural erzeugte Pixel-Map aus einem Tile-Layout (Wände, Boden, Bäume)
- Spielbarer Character mit Top-Down-Kamera und Bewegung
- NPC (Dorfältester) mit Interaktion
- Quest-Logik: Ziel ist das Gespräch mit dem Dorfältesten
- Quest-/Statusmeldungen per Ingame-Textausgabe
- Konfigurierbare Spielmusik beim Start (über den GameMode)

## Starten (Unreal Engine)

1. Unreal Engine 5.4 installieren.
2. `PixelAdventure.uproject` mit Unreal öffnen.
3. Falls abgefragt, Projektdateien generieren und C++ kompilieren.
4. Eine leere Map unter `/Game/Maps/AdventureMap` anlegen und speichern.
5. Im aktiven GameMode (`PAAdventureGameModeBase`) optional ein `Game Music`-Asset setzen.
6. Play drücken (Welt, NPC und ggf. Musik werden beim Start durch den GameMode erzeugt).

## Steuerung im Spiel

- **WASD / Pfeiltasten**: Bewegen
- **E**: Interaktion auslösen