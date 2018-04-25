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
	${OBJECTDIR}/L7Test.o


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
LDLIBSOPTIONS=-L/usr/local/wxWidgets/lib -lwx_baseu-3.1 -lwx_gtk2u_core-3.1 -lwx_gtk2u_adv-3.1 -lpq -lsqlite3 -Wl,-rpath,'../L7/dist/Debug/GNU-Linux' -L../L7/dist/Debug/GNU-Linux -lL7

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/l7test

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/l7test: ../L7/dist/Debug/GNU-Linux/libL7.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/l7test: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/l7test ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/L7Test.o: L7Test.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUSE_LIBPQ -DUSE_LIBSQ -DWXUSINGDLL -D_FILE_OFFSET_BITS=64 -D__WXGTK__ -I/usr/local/wxWidgets/include/wx-3.1 -I/usr/local/wxWidgets/lib/wx/include/gtk2-unicode-3.1 -I../L7 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/L7Test.o L7Test.cpp

# Subprojects
.build-subprojects:
	cd ../L7 && ${MAKE} -j8 -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libL7.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/l7test

# Subprojects
.clean-subprojects:
	cd ../L7 && ${MAKE} -j8 -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
