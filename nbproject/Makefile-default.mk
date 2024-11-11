#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/sensores2023.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/sensores2023.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/main.c src/mq6.c src/template1.c src/serv_adcon.c src/serv_eeprom.c src/base_eeprom.c src/base_lcd.c src/base_adcon.c src/base_rs232.c src/base_botoes.c src/util_menu.c src/base_timer.c src/serv_menus.c src/ct_handler.c src/ct_estados.c src/ct_prot_rs232.c src/base_dht22.c src/serv_dht22.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/mq6.p1 ${OBJECTDIR}/src/template1.p1 ${OBJECTDIR}/src/serv_adcon.p1 ${OBJECTDIR}/src/serv_eeprom.p1 ${OBJECTDIR}/src/base_eeprom.p1 ${OBJECTDIR}/src/base_lcd.p1 ${OBJECTDIR}/src/base_adcon.p1 ${OBJECTDIR}/src/base_rs232.p1 ${OBJECTDIR}/src/base_botoes.p1 ${OBJECTDIR}/src/util_menu.p1 ${OBJECTDIR}/src/base_timer.p1 ${OBJECTDIR}/src/serv_menus.p1 ${OBJECTDIR}/src/ct_handler.p1 ${OBJECTDIR}/src/ct_estados.p1 ${OBJECTDIR}/src/ct_prot_rs232.p1 ${OBJECTDIR}/src/base_dht22.p1 ${OBJECTDIR}/src/serv_dht22.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/src/main.p1.d ${OBJECTDIR}/src/mq6.p1.d ${OBJECTDIR}/src/template1.p1.d ${OBJECTDIR}/src/serv_adcon.p1.d ${OBJECTDIR}/src/serv_eeprom.p1.d ${OBJECTDIR}/src/base_eeprom.p1.d ${OBJECTDIR}/src/base_lcd.p1.d ${OBJECTDIR}/src/base_adcon.p1.d ${OBJECTDIR}/src/base_rs232.p1.d ${OBJECTDIR}/src/base_botoes.p1.d ${OBJECTDIR}/src/util_menu.p1.d ${OBJECTDIR}/src/base_timer.p1.d ${OBJECTDIR}/src/serv_menus.p1.d ${OBJECTDIR}/src/ct_handler.p1.d ${OBJECTDIR}/src/ct_estados.p1.d ${OBJECTDIR}/src/ct_prot_rs232.p1.d ${OBJECTDIR}/src/base_dht22.p1.d ${OBJECTDIR}/src/serv_dht22.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/mq6.p1 ${OBJECTDIR}/src/template1.p1 ${OBJECTDIR}/src/serv_adcon.p1 ${OBJECTDIR}/src/serv_eeprom.p1 ${OBJECTDIR}/src/base_eeprom.p1 ${OBJECTDIR}/src/base_lcd.p1 ${OBJECTDIR}/src/base_adcon.p1 ${OBJECTDIR}/src/base_rs232.p1 ${OBJECTDIR}/src/base_botoes.p1 ${OBJECTDIR}/src/util_menu.p1 ${OBJECTDIR}/src/base_timer.p1 ${OBJECTDIR}/src/serv_menus.p1 ${OBJECTDIR}/src/ct_handler.p1 ${OBJECTDIR}/src/ct_estados.p1 ${OBJECTDIR}/src/ct_prot_rs232.p1 ${OBJECTDIR}/src/base_dht22.p1 ${OBJECTDIR}/src/serv_dht22.p1

# Source Files
SOURCEFILES=src/main.c src/mq6.c src/template1.c src/serv_adcon.c src/serv_eeprom.c src/base_eeprom.c src/base_lcd.c src/base_adcon.c src/base_rs232.c src/base_botoes.c src/util_menu.c src/base_timer.c src/serv_menus.c src/ct_handler.c src/ct_estados.c src/ct_prot_rs232.c src/base_dht22.c src/serv_dht22.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/sensores2023.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F886
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/mq6.p1: src/mq6.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mq6.p1.d 
	@${RM} ${OBJECTDIR}/src/mq6.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/mq6.p1 src/mq6.c 
	@-${MV} ${OBJECTDIR}/src/mq6.d ${OBJECTDIR}/src/mq6.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/mq6.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/template1.p1: src/template1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/template1.p1.d 
	@${RM} ${OBJECTDIR}/src/template1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/template1.p1 src/template1.c 
	@-${MV} ${OBJECTDIR}/src/template1.d ${OBJECTDIR}/src/template1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/template1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_adcon.p1: src/serv_adcon.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_adcon.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_adcon.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_adcon.p1 src/serv_adcon.c 
	@-${MV} ${OBJECTDIR}/src/serv_adcon.d ${OBJECTDIR}/src/serv_adcon.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_adcon.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_eeprom.p1: src/serv_eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_eeprom.p1 src/serv_eeprom.c 
	@-${MV} ${OBJECTDIR}/src/serv_eeprom.d ${OBJECTDIR}/src/serv_eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_eeprom.p1: src/base_eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/base_eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_eeprom.p1 src/base_eeprom.c 
	@-${MV} ${OBJECTDIR}/src/base_eeprom.d ${OBJECTDIR}/src/base_eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_lcd.p1: src/base_lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_lcd.p1.d 
	@${RM} ${OBJECTDIR}/src/base_lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_lcd.p1 src/base_lcd.c 
	@-${MV} ${OBJECTDIR}/src/base_lcd.d ${OBJECTDIR}/src/base_lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_adcon.p1: src/base_adcon.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_adcon.p1.d 
	@${RM} ${OBJECTDIR}/src/base_adcon.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_adcon.p1 src/base_adcon.c 
	@-${MV} ${OBJECTDIR}/src/base_adcon.d ${OBJECTDIR}/src/base_adcon.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_adcon.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_rs232.p1: src/base_rs232.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_rs232.p1.d 
	@${RM} ${OBJECTDIR}/src/base_rs232.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_rs232.p1 src/base_rs232.c 
	@-${MV} ${OBJECTDIR}/src/base_rs232.d ${OBJECTDIR}/src/base_rs232.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_rs232.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_botoes.p1: src/base_botoes.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_botoes.p1.d 
	@${RM} ${OBJECTDIR}/src/base_botoes.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_botoes.p1 src/base_botoes.c 
	@-${MV} ${OBJECTDIR}/src/base_botoes.d ${OBJECTDIR}/src/base_botoes.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_botoes.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/util_menu.p1: src/util_menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/util_menu.p1.d 
	@${RM} ${OBJECTDIR}/src/util_menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/util_menu.p1 src/util_menu.c 
	@-${MV} ${OBJECTDIR}/src/util_menu.d ${OBJECTDIR}/src/util_menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/util_menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_timer.p1: src/base_timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_timer.p1.d 
	@${RM} ${OBJECTDIR}/src/base_timer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_timer.p1 src/base_timer.c 
	@-${MV} ${OBJECTDIR}/src/base_timer.d ${OBJECTDIR}/src/base_timer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_timer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_menus.p1: src/serv_menus.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_menus.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_menus.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_menus.p1 src/serv_menus.c 
	@-${MV} ${OBJECTDIR}/src/serv_menus.d ${OBJECTDIR}/src/serv_menus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_menus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ct_handler.p1: src/ct_handler.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ct_handler.p1.d 
	@${RM} ${OBJECTDIR}/src/ct_handler.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ct_handler.p1 src/ct_handler.c 
	@-${MV} ${OBJECTDIR}/src/ct_handler.d ${OBJECTDIR}/src/ct_handler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ct_handler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ct_estados.p1: src/ct_estados.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ct_estados.p1.d 
	@${RM} ${OBJECTDIR}/src/ct_estados.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ct_estados.p1 src/ct_estados.c 
	@-${MV} ${OBJECTDIR}/src/ct_estados.d ${OBJECTDIR}/src/ct_estados.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ct_estados.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ct_prot_rs232.p1: src/ct_prot_rs232.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ct_prot_rs232.p1.d 
	@${RM} ${OBJECTDIR}/src/ct_prot_rs232.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ct_prot_rs232.p1 src/ct_prot_rs232.c 
	@-${MV} ${OBJECTDIR}/src/ct_prot_rs232.d ${OBJECTDIR}/src/ct_prot_rs232.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ct_prot_rs232.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_dht22.p1: src/base_dht22.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_dht22.p1.d 
	@${RM} ${OBJECTDIR}/src/base_dht22.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_dht22.p1 src/base_dht22.c 
	@-${MV} ${OBJECTDIR}/src/base_dht22.d ${OBJECTDIR}/src/base_dht22.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_dht22.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_dht22.p1: src/serv_dht22.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_dht22.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_dht22.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_dht22.p1 src/serv_dht22.c 
	@-${MV} ${OBJECTDIR}/src/serv_dht22.d ${OBJECTDIR}/src/serv_dht22.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_dht22.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/mq6.p1: src/mq6.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mq6.p1.d 
	@${RM} ${OBJECTDIR}/src/mq6.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/mq6.p1 src/mq6.c 
	@-${MV} ${OBJECTDIR}/src/mq6.d ${OBJECTDIR}/src/mq6.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/mq6.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/template1.p1: src/template1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/template1.p1.d 
	@${RM} ${OBJECTDIR}/src/template1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/template1.p1 src/template1.c 
	@-${MV} ${OBJECTDIR}/src/template1.d ${OBJECTDIR}/src/template1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/template1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_adcon.p1: src/serv_adcon.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_adcon.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_adcon.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_adcon.p1 src/serv_adcon.c 
	@-${MV} ${OBJECTDIR}/src/serv_adcon.d ${OBJECTDIR}/src/serv_adcon.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_adcon.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_eeprom.p1: src/serv_eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_eeprom.p1 src/serv_eeprom.c 
	@-${MV} ${OBJECTDIR}/src/serv_eeprom.d ${OBJECTDIR}/src/serv_eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_eeprom.p1: src/base_eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/base_eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_eeprom.p1 src/base_eeprom.c 
	@-${MV} ${OBJECTDIR}/src/base_eeprom.d ${OBJECTDIR}/src/base_eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_lcd.p1: src/base_lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_lcd.p1.d 
	@${RM} ${OBJECTDIR}/src/base_lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_lcd.p1 src/base_lcd.c 
	@-${MV} ${OBJECTDIR}/src/base_lcd.d ${OBJECTDIR}/src/base_lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_adcon.p1: src/base_adcon.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_adcon.p1.d 
	@${RM} ${OBJECTDIR}/src/base_adcon.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_adcon.p1 src/base_adcon.c 
	@-${MV} ${OBJECTDIR}/src/base_adcon.d ${OBJECTDIR}/src/base_adcon.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_adcon.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_rs232.p1: src/base_rs232.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_rs232.p1.d 
	@${RM} ${OBJECTDIR}/src/base_rs232.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_rs232.p1 src/base_rs232.c 
	@-${MV} ${OBJECTDIR}/src/base_rs232.d ${OBJECTDIR}/src/base_rs232.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_rs232.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_botoes.p1: src/base_botoes.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_botoes.p1.d 
	@${RM} ${OBJECTDIR}/src/base_botoes.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_botoes.p1 src/base_botoes.c 
	@-${MV} ${OBJECTDIR}/src/base_botoes.d ${OBJECTDIR}/src/base_botoes.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_botoes.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/util_menu.p1: src/util_menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/util_menu.p1.d 
	@${RM} ${OBJECTDIR}/src/util_menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/util_menu.p1 src/util_menu.c 
	@-${MV} ${OBJECTDIR}/src/util_menu.d ${OBJECTDIR}/src/util_menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/util_menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_timer.p1: src/base_timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_timer.p1.d 
	@${RM} ${OBJECTDIR}/src/base_timer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_timer.p1 src/base_timer.c 
	@-${MV} ${OBJECTDIR}/src/base_timer.d ${OBJECTDIR}/src/base_timer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_timer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_menus.p1: src/serv_menus.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_menus.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_menus.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_menus.p1 src/serv_menus.c 
	@-${MV} ${OBJECTDIR}/src/serv_menus.d ${OBJECTDIR}/src/serv_menus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_menus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ct_handler.p1: src/ct_handler.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ct_handler.p1.d 
	@${RM} ${OBJECTDIR}/src/ct_handler.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ct_handler.p1 src/ct_handler.c 
	@-${MV} ${OBJECTDIR}/src/ct_handler.d ${OBJECTDIR}/src/ct_handler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ct_handler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ct_estados.p1: src/ct_estados.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ct_estados.p1.d 
	@${RM} ${OBJECTDIR}/src/ct_estados.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ct_estados.p1 src/ct_estados.c 
	@-${MV} ${OBJECTDIR}/src/ct_estados.d ${OBJECTDIR}/src/ct_estados.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ct_estados.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ct_prot_rs232.p1: src/ct_prot_rs232.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ct_prot_rs232.p1.d 
	@${RM} ${OBJECTDIR}/src/ct_prot_rs232.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/ct_prot_rs232.p1 src/ct_prot_rs232.c 
	@-${MV} ${OBJECTDIR}/src/ct_prot_rs232.d ${OBJECTDIR}/src/ct_prot_rs232.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ct_prot_rs232.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/base_dht22.p1: src/base_dht22.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/base_dht22.p1.d 
	@${RM} ${OBJECTDIR}/src/base_dht22.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/base_dht22.p1 src/base_dht22.c 
	@-${MV} ${OBJECTDIR}/src/base_dht22.d ${OBJECTDIR}/src/base_dht22.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/base_dht22.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/serv_dht22.p1: src/serv_dht22.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/serv_dht22.p1.d 
	@${RM} ${OBJECTDIR}/src/serv_dht22.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/src/serv_dht22.p1 src/serv_dht22.c 
	@-${MV} ${OBJECTDIR}/src/serv_dht22.d ${OBJECTDIR}/src/serv_dht22.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/serv_dht22.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/sensores2023.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/sensores2023.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib -std=c99 -gdwarf-3 -mstack=compiled:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/sensores2023.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/sensores2023.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/sensores2023.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/sensores2023.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -O2 -Og -maddrqual=ignore -xassembler-with-cpp -I"/opt/microchip/xc8/v2.41/pic/include/proc" -mwarn=0 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mosccal -mno-resetbits -mno-save-resetbits -mno-download -mstackcall -mdefault-config-bits -mc90lib -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/sensores2023.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
