on run args
  set thePluginName to (item 1 of args)
  set theInstallerName to (item 2 of args)
  tell application "Finder"
    tell disk theInstallerName
      open
      set current view of container window to icon view
      set toolbar visible of container window to false
      set statusbar visible of container window to false
      set the bounds of container window to {200, 100, 712, 612}
      set opts to the icon view options of container window
      set background picture of opts to file ".background:background.png"
      set arrangement of opts to not arranged
      set icon size of opts to 80
      set position of item thePluginName of container window to {150, 275}
      set position of item "Plugins" of container window to {650, 275}
      delay 5
      eject
    end tell
  end tell
end run
