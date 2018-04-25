#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/LBoundCheckBox.o \
	${OBJECTDIR}/LBoundComboBox.o \
	${OBJECTDIR}/LBoundControl.o \
	${OBJECTDIR}/LBoundDatePickerCtrl.o \
	${OBJECTDIR}/LBoundGrid.o \
	${OBJECTDIR}/LBoundSpinCtrl.o \
	${OBJECTDIR}/LBoundTextCtrl.o \
	${OBJECTDIR}/LConnection.o \
	${OBJECTDIR}/LGridCheckEditor.o \
	${OBJECTDIR}/LGridCheckRenderer.o \
	${OBJECTDIR}/LGridColEditor.o \
	${OBJECTDIR}/LGridComboEditor.o \
	${OBJECTDIR}/LGridComboRenderer.o \
	${OBJECTDIR}/LGridDateEditor.o \
	${OBJECTDIR}/LGridDateRenderer.o \
	${OBJECTDIR}/LGridSpinEditor.o \
	${OBJECTDIR}/LGridSpinRenderer.o \
	${OBJECTDIR}/LGridTextEditor.o \
	${OBJECTDIR}/LGridTextRenderer.o \
	${OBJECTDIR}/LInformation.o \
	${OBJECTDIR}/LItemData.o \
	${OBJECTDIR}/LLightPQResultSet.o \
	${OBJECTDIR}/LLightResultSet.o \
	${OBJECTDIR}/LLightSQResultSet.o \
	${OBJECTDIR}/LNavigator.o \
	${OBJECTDIR}/LPQConnection.o \
	${OBJECTDIR}/LPQResultSet.o \
	${OBJECTDIR}/LResultSet.o \
	${OBJECTDIR}/LSQConnection.o \
	${OBJECTDIR}/LSQResultSet.o \
	${OBJECTDIR}/LSQresult.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/wxWidgets/lib -lwx_baseu-3.1 -lwx_gtk2u_core-3.1

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libL7.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libL7.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libL7.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/LBoundCheckBox.o: LBoundCheckBox.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LBoundCheckBox.o LBoundCheckBox.cpp

${OBJECTDIR}/LBoundComboBox.o: LBoundComboBox.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LBoundComboBox.o LBoundComboBox.cpp

${OBJECTDIR}/LBoundControl.o: LBoundControl.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LBoundControl.o LBoundControl.cpp

${OBJECTDIR}/LBoundDatePickerCtrl.o: LBoundDatePickerCtrl.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LBoundDatePickerCtrl.o LBoundDatePickerCtrl.cpp

${OBJECTDIR}/LBoundGrid.o: LBoundGrid.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LBoundGrid.o LBoundGrid.cpp

${OBJECTDIR}/LBoundSpinCtrl.o: LBoundSpinCtrl.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LBoundSpinCtrl.o LBoundSpinCtrl.cpp

${OBJECTDIR}/LBoundTextCtrl.o: LBoundTextCtrl.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LBoundTextCtrl.o LBoundTextCtrl.cpp

${OBJECTDIR}/LConnection.o: LConnection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LConnection.o LConnection.cpp

${OBJECTDIR}/LGridCheckEditor.o: LGridCheckEditor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridCheckEditor.o LGridCheckEditor.cpp

${OBJECTDIR}/LGridCheckRenderer.o: LGridCheckRenderer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridCheckRenderer.o LGridCheckRenderer.cpp

${OBJECTDIR}/LGridColEditor.o: LGridColEditor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridColEditor.o LGridColEditor.cpp

${OBJECTDIR}/LGridComboEditor.o: LGridComboEditor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridComboEditor.o LGridComboEditor.cpp

${OBJECTDIR}/LGridComboRenderer.o: LGridComboRenderer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridComboRenderer.o LGridComboRenderer.cpp

${OBJECTDIR}/LGridDateEditor.o: LGridDateEditor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridDateEditor.o LGridDateEditor.cpp

${OBJECTDIR}/LGridDateRenderer.o: LGridDateRenderer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridDateRenderer.o LGridDateRenderer.cpp

${OBJECTDIR}/LGridSpinEditor.o: LGridSpinEditor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridSpinEditor.o LGridSpinEditor.cpp

${OBJECTDIR}/LGridSpinRenderer.o: LGridSpinRenderer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridSpinRenderer.o LGridSpinRenderer.cpp

${OBJECTDIR}/LGridTextEditor.o: LGridTextEditor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridTextEditor.o LGridTextEditor.cpp

${OBJECTDIR}/LGridTextRenderer.o: LGridTextRenderer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LGridTextRenderer.o LGridTextRenderer.cpp

${OBJECTDIR}/LInformation.o: LInformation.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LInformation.o LInformation.cpp

${OBJECTDIR}/LItemData.o: LItemData.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LItemData.o LItemData.cpp

${OBJECTDIR}/LLightPQResultSet.o: LLightPQResultSet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LLightPQResultSet.o LLightPQResultSet.cpp

${OBJECTDIR}/LLightResultSet.o: LLightResultSet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LLightResultSet.o LLightResultSet.cpp

${OBJECTDIR}/LLightSQResultSet.o: LLightSQResultSet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LLightSQResultSet.o LLightSQResultSet.cpp

${OBJECTDIR}/LNavigator.o: LNavigator.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LNavigator.o LNavigator.cpp

${OBJECTDIR}/LPQConnection.o: LPQConnection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LPQConnection.o LPQConnection.cpp

${OBJECTDIR}/LPQResultSet.o: LPQResultSet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LPQResultSet.o LPQResultSet.cpp

${OBJECTDIR}/LResultSet.o: LResultSet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LResultSet.o LResultSet.cpp

${OBJECTDIR}/LSQConnection.o: LSQConnection.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LSQConnection.o LSQConnection.cpp

${OBJECTDIR}/LSQResultSet.o: LSQResultSet.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LSQResultSet.o LSQResultSet.cpp

${OBJECTDIR}/LSQresult.o: LSQresult.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I/usr/local/wxWidgets/include/wx-3.1 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LSQresult.o LSQresult.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
