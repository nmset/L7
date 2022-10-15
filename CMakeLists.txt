 
cmake_minimum_required(VERSION 3.0)

project(L7) 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(wxWidgets CONFIG REQUIRED)
#[[
Pass in wxJson_DIR.
Ex : /usr/local
Where :
    /usr/local/lib/wxJson/libwxJson.so
    /usr/local/include/wxJson/wx/*.h
]]
find_package(wxJson REQUIRED)

# At least one of these must be passed in from outside.
if (${WITH_LIBPQ})
    add_definitions(-DUSE_LIBPQ)
endif()
if (${WITH_LIBSQ})
    add_definitions(-DUSE_LIBSQ)
endif()

add_library(L7 SHARED
LBoundCheckBox.cpp        LConnection.cpp         LGridDateRenderer.cpp  LLightPQResultSet.cpp  LSQConnection.cpp
LBoundComboBox.cpp        LGridCheckEditor.cpp    LGridSpinEditor.cpp    LLightResultSet.cpp    LSQresult.cpp
LBoundControl.cpp         LGridCheckRenderer.cpp  LGridSpinRenderer.cpp  LLightSQResultSet.cpp  LSQResultSet.cpp
LBoundDatePickerCtrl.cpp  LGridColEditor.cpp      LGridTextEditor.cpp    LNavigator.cpp
LBoundGrid.cpp            LGridComboEditor.cpp    LGridTextRenderer.cpp  LPQConnection.cpp
LBoundSpinCtrl.cpp        LGridComboRenderer.cpp  LInformation.cpp       LPQResultSet.cpp
LBoundTextCtrl.cpp        LGridDateEditor.cpp     LItemData.cpp          LResultSet.cpp

special/BaseGridPicker.cpp      special/LBoundJsonGridPicker.cpp   special/LGridXmlCellEditor.cpp
special/BasePicker.cpp          special/LBoundXmlGridPicker.cpp    special/LGridXmlCellRenderer.cpp
special/JsonGridPickerCtrl.cpp  special/LGridJsonCellEditor.cpp    special/XmlGridPickerCtrl.cpp
special/JsonHelper.cpp          special/LGridJsonCellRenderer.cpp  special/XmlHelper.cpp
)
    
install(TARGETS L7 DESTINATION lib/L7) 

install(FILES 
    LBoundCheckBox.h        LBoundTextCtrl.h      LGridComboRenderer.h  LGridTextRenderer.h  LNavigator.h     LSQResultSet.h
LBoundComboBox.h        LConnection.h         LGridDateEditor.h     LInformation.h       LPQConnection.h  LVersion.h
LBoundControl.h         LGridCheckEditor.h    LGridDateRenderer.h   LItemData.h          LPQResultSet.h
LBoundDatePickerCtrl.h  LGridCheckRenderer.h  LGridSpinEditor.h     LLightPQResultSet.h  LResultSet.h
LBoundGrid.h            LGridColEditor.h      LGridSpinRenderer.h   LLightResultSet.h    LSQConnection.h
LBoundSpinCtrl.h        LGridComboEditor.h    LGridTextEditor.h     LLightSQResultSet.h  LSQresult.h
    DESTINATION include/L7)
    
install(FILES 
special/BaseGridPicker.h      special/JsonHelper.h            special/LGridJsonCellEditor.h    special/LGridXmlCellRenderer.h
special/BasePicker.h          special/LBoundJsonGridPicker.h  special/LGridJsonCellRenderer.h  special/XmlGridPickerCtrl.h
special/JsonGridPickerCtrl.h  special/LBoundXmlGridPicker.h   special/LGridXmlCellEditor.h     special/XmlHelper.h
    DESTINATION include/L7/special)

target_link_libraries(L7 ${wxWidgets_LIBRARIES} ${WXJSON_LIBRARIES})

############################ Test ##############################################
# Simple 'build' directory inside the source tree.
# Here, we need both WITH_LIBPQ and WITH_LIBSQ.
include_directories(${CMAKE_BINARY_DIR}/..)
add_executable(L7Test Test/L7Test.cpp)
target_link_libraries(L7Test ${wxWidgets_LIBRARIES} ${WXJSON_LIBRARIES} L7 pq sqlite3)

