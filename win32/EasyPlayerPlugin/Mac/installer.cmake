set(INSTALLER_NAME "${PLUGIN_NAME} Installer")

FIREBREATH_FIND_COMMANDS()

message(STATUS "Adding DMG installer for ${PROJECT_NAME}")
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMENT "------------ CREATE DMG INSTALLER"

    #replace the copy with svn/git/whatever export if needed
    COMMAND ${CMD_CP} -r ${CMAKE_CURRENT_SOURCE_DIR}/Mac/dmg_template ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template
    COMMAND ${CMD_CP} -R ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}.plugin ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template

    #Give an icon to your bundle
    #COMMAND ${CMD_SIPS} -i ${CMAKE_CURRENT_SOURCE_DIR}/Mac/icon.png
    #COMMAND ${CMD_DEREZ} -only icns ${CMAKE_CURRENT_SOURCE_DIR}/Mac/icon.png > ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tempicns.rsrc
    #COMMAND ${CMD_REZ} -append ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tempicns.rsrc -o `printf "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template/${PLUGIN_NAME}.plugin/Icon\r"` 

    COMMAND ${CMD_SETFILE} -a C ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template/${PLUGIN_NAME}.plugin/
    COMMAND ${CMD_LN} -s /Library/Internet\ Plug-Ins ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template/
    COMMAND ${CMD_MV} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template/Internet\ Plug-Ins ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template/Plugins

    #Create the DMG
    COMMAND ${CMD_HDIUTIL} create -fs HFS+ -srcfolder ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template/ -volname "${INSTALLER_NAME}" -format UDRW ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}-temp.dmg
    COMMAND ${CMD_HDIUTIL} attach ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}-temp.dmg -noautoopen -quiet

    #Wait for the installer to mount
    COMMAND ${CMD_SLEEP} 2
    COMMAND ${CMD_OSASCRIPT} ${CMAKE_CURRENT_SOURCE_DIR}/Mac/dmgdesign.applescript ${PLUGIN_NAME}.plugin "${INSTALLER_NAME}"
    COMMAND ${CMD_SLEEP} 2
    COMMAND ${CMD_HDIUTIL} attach ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}-temp.dmg -noautoopen -quiet

    #Repeat the commands, as they are not always executed o_O
    COMMAND ${CMD_SLEEP} 2
    COMMAND ${CMD_OSASCRIPT} ${CMAKE_CURRENT_SOURCE_DIR}/Mac/dmgdesign.applescript ${PLUGIN_NAME}.plugin "${INSTALLER_NAME}"
    COMMAND ${CMD_SLEEP} 2

    COMMAND ${CMD_HDIUTIL} convert ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}-temp.dmg -format UDZO -imagekey zlib-level=9 -o ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}.dmg

    COMMAND ${CMD_RM} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PLUGIN_NAME}-temp.dmg
    COMMAND ${CMD_RM} -rf ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/dmg_template

    #COMMAND ${CMD_RM} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tempicns.rsrc
)
