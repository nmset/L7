/* 
 * File:   LBoundGrid.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 16:40
 */

#include "LBoundGrid.h"
#include "LGridCheckEditor.h"
#include "LGridCheckRenderer.h"
#include "LGridComboEditor.h"
#include "LGridComboRenderer.h"
#include "LGridDateEditor.h"
#include "LGridDateRenderer.h"
#include "LGridTextEditor.h"
#include "LGridTextRenderer.h"
#include "LGridSpinEditor.h"
#include "LGridSpinRenderer.h"
#include "special/LGridJsonCellRenderer.h"
#include "special/LGridJsonCellEditor.h"
#include "special/LGridXmlCellRenderer.h"
#include "special/LGridXmlCellEditor.h"

LBoundGrid::LBoundGrid(wxWindow* parent, wxWindowID id)
: wxGrid(parent, id, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL)
{
    m_rs = NULL;
    m_formClientSizer = NULL;
    m_gridFormEVH = NULL;
    m_scaleFactor = (wxGetDisplayPPI().GetY() / 96.0);
    SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    m_stringTable = new wxGridStringTable();
    m_stringTable->CanHaveAttributes();
    HideRowLabels();
    SetUseNativeColLabels();
    SetTable(m_stringTable);
    SetSelectionMode(wxGridSelectRows);
    CreateMenu();
    // These bindings must not propagate to the grid of BaseGridPicker !!!
    Bind(wxEVT_GRID_RANGE_SELECT, &LBoundGrid::ForceSingleLineRange, this, GetId(), GetId());
    Bind(wxEVT_GRID_SELECT_CELL, &LBoundGrid::CellSelected, this, GetId(), GetId());
    Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &LBoundGrid::ShowMenu, this, GetId(), GetId());
}

LBoundGrid::~LBoundGrid()
{
    delete m_menu;
    delete m_formClientSizer;
    ClearGrid();
}

void LBoundGrid::SetResultSet(LResultSet* newResultSet)
{
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    wxASSERT_MSG(m_rs->GetConnection() != NULL, _T("GetConnection() = NULL."));
    if (!m_menu) return;
    if (m_rs->GetConnection()->IsReadOnly() || m_rs->IsReadOnly())
    {
        m_menu->FindItemByPosition(0)->Enable(false);
        m_menu->FindItemByPosition(2)->Enable(false);
    }
    else
    {
        m_menu->FindItemByPosition(0)->Enable(true);
        m_menu->FindItemByPosition(2)->Enable(true);
    }
}

void LBoundGrid::ClearGrid()
{
    Unbind(wxEVT_GRID_RANGE_SELECT, &LBoundGrid::ForceSingleLineRange, this, GetId(), GetId());
    Unbind(wxEVT_GRID_SELECT_CELL, &LBoundGrid::CellSelected, this, GetId(), GetId());
    if (GetNumberRows()) DeleteRows(0, GetNumberRows());
    if (GetNumberCols()) DeleteCols(0, GetNumberCols());
}

void LBoundGrid::FillGrid()
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    wxASSERT_MSG(m_rs->GetConnection() != NULL, _T("GetConnection() = NULL."));
    wxGridUpdateLocker locker(this);
    // Starting again, the grid's resultset must free itself from any registered controls.
    RestoreEditorControls();
    Unbind(wxEVT_GRID_RANGE_SELECT, &LBoundGrid::ForceSingleLineRange, this, GetId(), GetId());
    Unbind(wxEVT_GRID_SELECT_CELL, &LBoundGrid::CellSelected, this, GetId(), GetId());
    // Remember some states
    const wxGridSizesInfo colSizes = GetColSizes();
    const int col = GetGridCursorCol() > -1 ? GetGridCursorCol() : 0;
    // Create enough cells
    if (GetNumberRows()) DeleteRows(0, GetNumberRows());
    const bool firstTime = (m_stringTable->GetNumberCols() == 0);
    const int nbCols = m_rs->GetColumnCount();
    if (firstTime) m_stringTable->AppendCols(nbCols);
    const int nbRows = m_rs->GetRowCount();
    m_stringTable->AppendRows(nbRows);
    // Add an insert row
    if (!m_rs->GetConnection()->IsReadOnly() && !m_rs->IsReadOnly())
    {
        m_stringTable->AppendRows(1);
    }
    // Grid header labels default to database table column names.
    if (firstTime)
    {
        for (unsigned int c = 0; c < nbCols; c++)
        {
            m_stringTable->SetColLabelValue(c, m_rs->GetColumnName(c));
        }
    }
    // Set each cell's value.
    for (unsigned int r = 0; r < nbRows; r++)
    {
        for (unsigned int c = 0; c < nbCols; c++)
        {
            m_stringTable->SetValue(r, c, (m_rs->GetData(r, c)).As<wxString>());
        }
    }
    Bind(wxEVT_GRID_SELECT_CELL, &LBoundGrid::CellSelected, this, GetId(), GetId());
    Bind(wxEVT_GRID_RANGE_SELECT, &LBoundGrid::ForceSingleLineRange, this, GetId(), GetId());
    // Restore
    SetColSizes(colSizes);
    // synchronize with the resultset
    const int targetRow = m_rs->GetRow() > -1 ? m_rs->GetRow() : 0;
    if (GetNumberRows())
    {
        // Triggers CellSelected event. If no data, we are on insert row, we must call LResultSet::AddNew()
        GoToCell(targetRow, col);
        SelectRow(targetRow);
    }
}

void LBoundGrid::CreateCheckBoxColumn(const wxString& newColName,
                                      const wxString& newLabel,
                                      unsigned int width,
                                      bool readOnly,
                                      bool isDualstate,
                                      wxString newNullLabel)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    const int col = m_rs->GetColumnIndex(newColName);
    wxASSERT_MSG(col > -1, _("Invalid column name : ") + newColName);
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    LGridCheckEditor * ed = new LGridCheckEditor(newColName, isDualstate, newNullLabel);
    colAtt->SetEditor(ed);
    LGridCheckRenderer * rn = new LGridCheckRenderer(isDualstate, newNullLabel);
    colAtt->SetRenderer(rn);
    colAtt->SetReadOnly(readOnly);
    m_stringTable->SetColAttr(colAtt, col);
    m_stringTable->SetColLabelValue(col, newLabel);
    SetColSize(col, width);
}

void LBoundGrid::CreateComboBoxColumn(const wxString& newColName,
                                      const wxString& newLabel,
                                      unsigned int width,
                                      bool readOnly)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    const int col = m_rs->GetColumnIndex(newColName);
    wxASSERT_MSG(col > -1, _("Invalid column name : ") + newColName);
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    LGridComboEditor * ed = new LGridComboEditor(newColName);
    colAtt->SetEditor(ed);
    LGridComboRenderer * rn = new LGridComboRenderer;
    colAtt->SetRenderer(rn);
    colAtt->SetReadOnly(readOnly);
    m_stringTable->SetColAttr(colAtt, col);
    m_stringTable->SetColLabelValue(col, newLabel);
    SetColSize(col, width);
}

void LBoundGrid::CreateDateColumn(const wxString& newColName,
                                  const wxString& newLabel,
                                  unsigned int width,
                                  bool readOnly)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    const int col = m_rs->GetColumnIndex(newColName);
    wxASSERT_MSG(col > -1, _("Invalid column name : ") + newColName);
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    LGridDateEditor * ed = new LGridDateEditor(newColName);
    colAtt->SetEditor(ed);
    LGridDateRenderer * rn = new LGridDateRenderer();
    colAtt->SetRenderer(rn);
    colAtt->SetReadOnly(readOnly);
    colAtt->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_stringTable->SetColAttr(colAtt, col);
    m_stringTable->SetColLabelValue(col, newLabel);
    SetColSize(col, width);
}

void LBoundGrid::CreateTextColumn(const wxString& newColName,
                                  const wxString& newLabel,
                                  unsigned int width,
                                  bool newMultiline,
                                  bool readOnly)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    const int col = m_rs->GetColumnIndex(newColName);
    wxASSERT_MSG(col > -1, _("Invalid column name : ") + newColName);
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    LGridTextEditor * ed = new LGridTextEditor(newColName, newMultiline);
    colAtt->SetEditor(ed);
    LGridTextRenderer * rn = new LGridTextRenderer();
    colAtt->SetRenderer(rn);
    colAtt->SetReadOnly(readOnly);
    m_stringTable->SetColAttr(colAtt, col);
    m_stringTable->SetColLabelValue(col, newLabel);
    SetColSize(col, width);
}

void LBoundGrid::CreateSpinColumn(const wxString& newColName,
                                  const wxString& newLabel,
                                  unsigned int width,
                                  int newMin,
                                  int newMax,
                                  int newInitial,
                                  bool readOnly)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    const int col = m_rs->GetColumnIndex(newColName);
    wxASSERT_MSG(col > -1, _("Invalid column name : ") + newColName);
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    LGridSpinEditor * ed = new LGridSpinEditor(newColName, newMin, newMax, newInitial);
    colAtt->SetEditor(ed);
    LGridSpinRenderer * rn = new LGridSpinRenderer();
    colAtt->SetRenderer(rn);
    colAtt->SetReadOnly(readOnly);
    colAtt->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_stringTable->SetColAttr(colAtt, col);
    m_stringTable->SetColLabelValue(col, newLabel);
    SetColSize(col, width);
}

void LBoundGrid::CreateJsonGridColumn(const wxString& newColName,
                                      const wxString& newLabel,
                                      unsigned int width,
                                      const wxString& intentLabel,
                                      const wxArrayString& types,
                                      wxSize popupSize,
                                      bool readOnly)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    const int col = m_rs->GetColumnIndex(newColName);
    wxASSERT_MSG(col > -1, _("Invalid column name : ") + newColName);
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    LGridJsonCellEditor * ed = new LGridJsonCellEditor(newColName, intentLabel, types, popupSize);
    colAtt->SetEditor(ed);
    LGridJsonCellRenderer * rn = new LGridJsonCellRenderer();
    colAtt->SetRenderer(rn);
    colAtt->SetReadOnly(readOnly);
    colAtt->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_stringTable->SetColAttr(colAtt, col);
    m_stringTable->SetColLabelValue(col, newLabel);
    SetColSize(col, width);
}

void LBoundGrid::CreateXmlGridColumn(const wxString& newColName,
                                     const wxString& newLabel,
                                     unsigned int width,
                                     const wxString& intentLabel,
                                     const wxArrayString& types,
                                     wxSize popupSize,
                                     bool readOnly)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    const int col = m_rs->GetColumnIndex(newColName);
    wxASSERT_MSG(col > -1, _("Invalid column name : ") + newColName);
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    LGridXmlCellEditor * ed = new LGridXmlCellEditor(newColName, intentLabel, types, popupSize);
    colAtt->SetEditor(ed);
    LGridXmlCellRenderer * rn = new LGridXmlCellRenderer();
    colAtt->SetRenderer(rn);
    colAtt->SetReadOnly(readOnly);
    colAtt->SetAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_stringTable->SetColAttr(colAtt, col);
    m_stringTable->SetColLabelValue(col, newLabel);
    SetColSize(col, width);
}

const wxString LBoundGrid::GetColName(const unsigned int col)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    InitEditor(GetGridCursorRow(), col, false);
    LGridColEditor * gce = GetColEditor(GetGridCursorRow(), col);
    if (gce == NULL) return wxEmptyString;
    return gce->GetColumnName();
}

int LBoundGrid::GetColIndex(const wxString& colName)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    for (unsigned int col = 0; col < GetNumberCols(); col++)
    {
        LGridColEditor * gce = GetColEditor(GetGridCursorRow(), col);
        if (gce == NULL) continue;
        if (gce->GetColumnName().Lower() == colName.Lower()) return col;
    }
    return -1;
}

LBoundControl* LBoundGrid::GetBoundControl(const unsigned int row,
                                           const unsigned int col,
                                           bool keepRegistered)
{
    wxASSERT_MSG(row >= 0 && row < GetNumberRows(), _("Invalid row parameter"));
    wxASSERT_MSG(col >= 0 && col < GetNumberCols(), _("Invalid col parameter"));
    wxObjectDataPtr<wxGridCellAttr> cellAttr;
    cellAttr = m_stringTable->GetAttr(row, col, wxGridCellAttr::Col);
    if (cellAttr.get() == NULL) return NULL;
    wxObjectDataPtr<wxGridCellEditor> ed;
    ed = cellAttr->GetEditor(this, row, col);
    if (ed.get() == NULL) return NULL;
    ed->BeginEdit(row, col, this);
    ed->Show(false);
    LGridColEditor* gce = dynamic_cast<LGridColEditor*> (ed.get()); // CROSS CASTING
    if (!gce) return NULL; // APP CAN ADD AN UNBOUND COLUMN
    if (!keepRegistered)
    {
        wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
        m_rs->UnRegisterControl(gce->GetBoundControl());
    }
    return gce->GetBoundControl();
}

LBoundControl* LBoundGrid::GetBoundControl(const unsigned int row,
                                           const wxString& colName,
                                           bool keepRegistered)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    wxASSERT_MSG(row >= 0 && row < GetNumberRows(), _("Invalid row parameter"));
    for (int col = 0; col < GetNumberCols(); col++)
    {
        LGridColEditor * gce = GetColEditor(row, col);
        if (gce == NULL) continue;
        if (gce->GetColumnName().Lower() == colName.Lower())
        {
            InitEditor(row, col, keepRegistered);
            return gce->GetBoundControl();
        }
    }
    return NULL;
}

LGridColEditor* LBoundGrid::GetColEditor(const unsigned int row, const unsigned int col) const
{
    wxASSERT_MSG(row >= 0 && row < GetNumberRows(), _("Invalid row parameter"));
    wxASSERT_MSG(col >= 0 && col < GetNumberCols(), _("Invalid col parameter"));
    wxObjectDataPtr<wxGridCellAttr> cellAttr;
    cellAttr = m_stringTable->GetAttr(row, col, wxGridCellAttr::Col);
    if (cellAttr.get() == NULL) return NULL;
    wxObjectDataPtr<wxGridCellEditor> ed;
    ed = cellAttr->GetEditor(this, row, col);
    if (ed.get() == NULL) return NULL;
    return dynamic_cast<LGridColEditor*> (ed.get());
}

wxControl* LBoundGrid::GetFormEditor(const unsigned int col) const
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    wxASSERT_MSG(col >= 0 && col < GetNumberCols(), _("Invalid col parameter"));
    int row = m_rs->GetRow();
    if (m_rs->IsInserting()) row = m_rs->GetRowCount();
    LGridColEditor* gce = GetColEditor(row, col);
    if (!gce) return NULL;
    return gce->GetFormEditor();
}

wxControl* LBoundGrid::GetFormEditor(const wxString& colName)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    int row = m_rs->GetRow();
    if (m_rs->IsInserting()) row = m_rs->GetRowCount();
    for (int col = 0; col < GetNumberCols(); col++)
    {
        LGridColEditor * gce = GetColEditor(row, col);
        if (gce == NULL) continue;
        if (gce->GetColumnName().Lower() == colName.Lower())
            return gce->GetFormEditor();
    }
    return NULL;
}

bool LBoundGrid::InitEditor(const unsigned int row,
                            const unsigned int col,
                            bool keepRegistered)
{
    wxASSERT_MSG(row >= 0 && row < GetNumberRows(), _("Invalid row parameter"));
    wxASSERT_MSG(col >= 0 && col < GetNumberCols(), _("Invalid col parameter"));
    wxObjectDataPtr<wxGridCellAttr> cellAttr;
    cellAttr = m_stringTable->GetAttr(row, col, wxGridCellAttr::Col);
    if (cellAttr.get() == NULL) return false;
    wxObjectDataPtr<wxGridCellEditor> ed;
    ed = cellAttr->GetEditor(this, row, col);
    if (ed.get() == NULL) return false;
    // That's what we want to do.
    ed->BeginEdit(row, col, this);
    ed->Show(false);
    if (!keepRegistered)
    {
        wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
        LGridColEditor* gce = dynamic_cast<LGridColEditor*> (ed.get());
        if (!gce) return false;
        m_rs->UnRegisterControl(gce->GetBoundControl());
    }
    return true;
}

void LBoundGrid::InitAllEditors(const unsigned int row, bool keepRegistered)
{
    wxASSERT_MSG(row >= 0 && row < GetNumberRows(), _("Invalid row parameter"));
    if (m_stringTable == NULL) return;
    if (GetNumberCols() > 0)
    {
        for (int col = 0; col < GetNumberCols(); col++)
        {
            InitEditor(row, col, keepRegistered);
        }
    }
}

void LBoundGrid::AddMenuSeparator()
{
    m_menu->AppendSeparator();
}

void LBoundGrid::AddMenuItem(wxMenuItem* newItem)
{
    wxASSERT_MSG(newItem != NULL, _("Menu item is NULL"));
    if (m_menu->FindItem(newItem->GetItemLabelText()) != wxNOT_FOUND) return;
    m_menu->Append(newItem);
}

void LBoundGrid::AddSubMenu(wxMenu* newSubMenu, const wxString& label)
{
    wxASSERT_MSG(newSubMenu != NULL, _("Submenu is NULL"));
    if (m_menu->FindItem(newSubMenu->GetTitle()) != wxNOT_FOUND) return;
    m_menu->AppendSubMenu(newSubMenu, label);
}

void LBoundGrid::RemoveMenuItem(const wxString& newItemLabelText)
{
    if (m_menu->FindItem(newItemLabelText) == wxNOT_FOUND) return;
    m_menu->Remove(m_menu->FindChildItem(m_menu->FindItem(newItemLabelText)));
}

void LBoundGrid::RemoveAllExternalMenuItems()
{
    if (m_menu->GetMenuItemCount() < 8) return;
    do
    {
        m_menu->Remove(m_menu->GetMenuItems()[7]);
    }
    while (m_menu->GetMenuItemCount() > 7);
}

void LBoundGrid::EnableMenuItem(int id, bool enable)
{
    if (m_menu->FindChildItem(id)) m_menu->Enable(id, enable);
}

/*
 * Only one single row must be selected at a time.
 * Even if multiple selections do not affect the grid row and col cursor values,
 * selecting only one row may avoid confusion.
 * And the library is not designed to account for edits on multiple rows.
 */
void LBoundGrid::ForceSingleLineRange(wxGridRangeSelectEvent& evt)
{
    if ((evt.Selecting()))
    {
        wxASSERT_MSG(GetSelectionMode() == wxGridSelectRows, "Selection mode is not wxGridSelectRows.");
        if (evt.ControlDown() == true
                || evt.GetTopRow() != evt.GetBottomRow())
        {
            SelectRow(GetGridCursorRow());
            evt.Veto();
        }
    }
    evt.Skip();
}

void LBoundGrid::CellSelected(wxGridEvent& evt)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    // The insert row is somehow un-natural, it exists in the grid but not in the resultset, we manage it separately.
    if (!m_rs->IsInserting())
    {
        // The user wants to change row.
        if ((evt.GetRow() != m_rs->GetRow()))
        {
            // If there have been no edits in therow we are leaving, we must unregister all controls in the resultset.
            if (!m_rs->IsDirty() && m_rs->HasData()) RestoreEditorControls();
            // The user doesn't want to go on insert row.
            if (evt.GetRow() < m_rs->GetRowCount())
            {
                // Absolute() will fail if there are edits
                if (!m_rs->Absolute(evt.GetRow()))
                {
                    evt.Veto(); // Doesn't seem to do anything
                    // Absolute() failed, get back to where we were.
                    GoToCell(m_rs->GetRow(), GetGridCursorCol());
                    SelectRow(m_rs->GetRow());
                }
            }
            else
            {
                // The user wants to go on insert row, call AddNe(). Will fail if there are edits.
                if (!m_rs->AddNew())
                {
                    evt.Veto();
                    // AddNew() failed, get back to where we were.
                    GoToCell(m_rs->GetRow(), GetGridCursorCol());
                    SelectRow(m_rs->GetRow());
                }
            }
        }
    }
    else
        // We are inserting, on insert row.
    {
        // The user want to go above (if there is saved data).
        if (evt.GetRow() < m_rs->GetRowCount())
        {
            // There are no edits on the insert row.
            if (!m_rs->IsDirty())
            {
                RestoreEditorControls();
                m_rs->Cancel();
                // Synchronize the resultset with the grid.
                m_rs->Absolute(evt.GetRow());
            }
            else
            {
                // There are edits on the insert row.
                m_rs->InformInserting();
                evt.Veto();
                // Get back.
                GoToCell(m_rs->GetRowCount(), GetGridCursorCol());
                SelectRow(m_rs->GetRowCount());
            }
        }
    }
    evt.Skip();
}

void LBoundGrid::CreateMenu()
{
    m_menu = new wxMenu(0);
    wxMenuItem * item = new wxMenuItem(m_menu, ID_MNU_SAVE, _("Save"));
    m_menu->Append(item);
    item = new wxMenuItem(m_menu, ID_MNU_CANCEL, _("Cancel"));
    m_menu->Append(item);
    item = new wxMenuItem(m_menu, ID_MNU_DELETE, _("Delete"));
    m_menu->Append(item);
    item = new wxMenuItem(m_menu);
    m_menu->Append(item);
    item = new wxMenuItem(m_menu, ID_MNU_REFRESH, _("Refresh"));
    m_menu->Append(item);
    item = new wxMenuItem(m_menu);
    m_menu->Append(item);
    item = new wxMenuItem(m_menu, ID_MNU_FORMVIEW, _("Form view"));
    m_menu->Append(item);
    m_menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &LBoundGrid::MenuAction, this);
}

void LBoundGrid::ShowMenu(wxGridEvent& evt)
{
    GetGridWindow()->PopupMenu(m_menu);
}

void LBoundGrid::MenuAction(wxCommandEvent& evt)
{
    switch (evt.GetId())
    {
    case ID_MNU_SAVE:
        if (m_rs)
        {
            int row = m_rs->GetRow();
            if (m_rs->IsInserting()) row = m_rs->GetRowCount();
            InitPKEditor(row);
            if (m_rs->Save())
            {
                RestoreEditorControls();
                FillGrid();
            }
        }
        break;
    case ID_MNU_CANCEL:
        if (m_rs)
        {
            HideCellEditControl();
            const unsigned int nbCols = m_rs->GetColumnCount();
            int row = m_rs->GetRow();
            if (m_rs->IsInserting()) row = m_rs->GetRowCount();
            for (int c = 0; c < nbCols; c++)
            {
                wxAny data = wxEmptyString;
                if (!m_rs->IsInserting())
                    data = (m_rs->GetData(row, c));
                m_stringTable->SetValue(row, c, data.As<wxString>());
            }
            m_rs->Cancel();
            RestoreEditorControls();
            row = m_rs->HasData() ? m_rs->GetRow() : 0;
            GoToCell(row, GetGridCursorCol());
            SelectRow(row);
            ForceRefresh();
        }
        break;
    case ID_MNU_REFRESH:
        // If there's no data, the grid is on insert row and AddNew() has been called.
        if (!m_rs->HasData()) m_rs->Cancel();
        RestoreEditorControls();
        m_rs->Refresh();
        FillGrid();
        break;
    case ID_MNU_DELETE:
        if (m_rs)
        {
            if (m_rs->GetRowCount() < 1) return;
            InitPKEditor(m_rs->GetRow());
            if (m_rs->Delete())
            {
                RestoreEditorControls();
                FillGrid();
            }
        }
        break;
    case ID_MNU_FORMVIEW:
        ShowFormView();
        break;
    }
    GetGridWindow()->SetFocus();
}

bool LBoundGrid::InitPKEditor(const int row)
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    for (int c = 0; c < GetNumberCols(); c++)
    {
        wxObjectDataPtr<wxGridCellAttr> cellAttr;
        cellAttr = m_stringTable->GetAttr(row, c, wxGridCellAttr::Col);
        if (cellAttr.get() == NULL) continue;
        wxObjectDataPtr<wxGridCellEditor> ed;
        ed = cellAttr->GetEditor(this, row, c);
        if (ed.get() == NULL) continue;
        LGridColEditor * gce = dynamic_cast<LGridColEditor*> (ed.get());
        if (gce == NULL) continue;
        if (gce->GetColumnName().Lower() == m_rs->GetPKColumnName().Lower())
        {
            ed->BeginEdit(row, c, this);
            ed->Show(false);
            return true;
        }
    }
    return false;
}

void LBoundGrid::ShowFormView()
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    wxDialog * dlg = new wxDialog(this, wxID_ANY, _("Form view"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxRESIZE_BORDER);
    unsigned int row = m_rs->GetRow();
    if (m_rs->IsInserting()) row = m_rs->GetRowCount();
    // We want to ignore hidden columns.
    unsigned int visibleCol = 0;
    // Multiline wxTextCtrl must be able to expand vertically.
    bool hasMultiline = false;

    // ScrolledWindow/VerticalSizer/Panel/VerticalSizer/FlexGridSizer
    //                                                 /ClientSizer
    wxScrolledWindow * swMain = new wxScrolledWindow(dlg, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxHSCROLL | wxVSCROLL);
    swMain->SetScrollbars(1, 1, 0, 0);
    wxBoxSizer* vsz0 = new wxBoxSizer(wxVERTICAL);
    swMain->SetSizer(vsz0);
    wxPanel * pan0 = new wxPanel(swMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxHSCROLL | wxVSCROLL | wxTAB_TRAVERSAL);
    vsz0->Add(pan0, 1, wxEXPAND);
    wxBoxSizer * vsz = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer * flxsz = new wxFlexGridSizer(0, 2, 0, 0);
    pan0->SetSizer(vsz);
    vsz->Add(flxsz, 1, wxGROW | wxGROW | wxALL);
    flxsz->AddGrowableCol(1);
    m_formClientSizer = new wxBoxSizer(wxHORIZONTAL);
    vsz->Add(m_formClientSizer, 0, wxGROW, 1);

    for (unsigned int col = 0; col < GetNumberCols(); col++)
    {
        if (GetColSize(col) == 0) continue;
        InitEditor(row, col, true);
        LGridColEditor * gce = GetColEditor(row, col);
        if (gce == NULL) continue;
        int type = gce->GetType();
        // Receive the form editor from LGridColEditor::ProvideFormEditor
        wxControl * fEditor = NULL;
        // One more pointer to a text control
        wxTextCtrl * txtCtrl = NULL;
        switch (type)
        {
        case LGridColEditor::TEXT:
            fEditor = static_cast<wxTextCtrl*> (gce->ProvideFormEditor(pan0));
            txtCtrl = static_cast<wxTextCtrl*> (fEditor);
            break;
        case LGridColEditor::CHECK:
            fEditor = static_cast<wxCheckBox*> (gce->ProvideFormEditor(pan0));
            break;
        case LGridColEditor::COMBO:
            fEditor = static_cast<wxComboBox*> (gce->ProvideFormEditor(pan0));
            break;
        case LGridColEditor::DATE:
            fEditor = static_cast<wxDatePickerCtrl*> (gce->ProvideFormEditor(pan0));
            break;
        case LGridColEditor::SPIN:
            fEditor = static_cast<wxSpinCtrl*> (gce->ProvideFormEditor(pan0));
            break;
        case LGridColEditor::JSON_GRID:
            fEditor = static_cast<JsonGridPickerCtrl*> (gce->ProvideFormEditor(pan0));
            break;
        case LGridColEditor::XML_GRID:
            fEditor = static_cast<XmlGridPickerCtrl*> (gce->ProvideFormEditor(pan0));
            break;
        }
        // A label corresponding to the grid column header
        wxStaticText * lbl = new wxStaticText(pan0, wxID_ANY, m_stringTable->GetColLabelValue(col), wxDefaultPosition, wxDefaultSize, 0);
        flxsz->Add(lbl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 0);
        if (txtCtrl && txtCtrl->IsMultiLine())
        {
            flxsz->Add(fEditor, 0, wxGROW | wxGROW | wxALL, 0);
            txtCtrl = NULL;
            // The multiline text control must expand.
            flxsz->AddGrowableRow(visibleCol);
            hasMultiline = true;
        }
        else
        {
            flxsz->Add(fEditor, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 0);
        }
        wxObjectDataPtr<wxGridCellAttr> cellAttr;
        cellAttr = m_stringTable->GetAttr(row, col, wxGridCellAttr::Col);
        if (cellAttr.get() != NULL)
        {
            // read only columns must not be edited
            fEditor->Enable(!cellAttr->IsReadOnly());
        }
        fEditor->Show(true);
        // visibleCol <> col
        visibleCol++;
    }

    wxSize dSize = wxDefaultSize;
    if (hasMultiline)
    {
        // Some arbitrary values
        wxSize screen = wxGetDisplaySize();
        dSize.SetWidth(screen.GetWidth() * 0.4);
        dSize.SetHeight(screen.GetHeight() * 0.75);
    }
    // Let the application do something with the dialog before we show it
    if (m_gridFormEVH) m_gridFormEVH->BeforeFormShown(this, m_formClientSizer);
    vsz->SetSizeHints(pan0); // here
    dlg->SetSize(dSize);
    dlg->CentreOnScreen();
    swMain->FitInside();
    int res = dlg->ShowModal();
    // Sync back any edits. Here we don't care about visible columns.
    for (int col = 0; col < GetNumberCols(); col++)
    {
        LGridColEditor * dce = GetColEditor(row, col);
        if (dce == NULL) continue;
        dce->SyncBack(row, col, this);
    }
    // Let the application complete whatever.
    if (m_gridFormEVH) m_gridFormEVH->AfterFormHidden(this, m_formClientSizer);
    SetFocus();
    dlg->Destroy();
    wxDELETE(dlg); // ??
    m_formClientSizer = NULL;
    // Last information. But it's not idle time yet; this may be a source of unexpected trouble.
    if (m_gridFormEVH) m_gridFormEVH->AfterFormDestroyed(this);
}

void LBoundGrid::RestoreEditorControls()
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    while (m_rs->GetRegisteredControls().GetCount())
    {
        LBoundControl * ctrl = static_cast<LBoundControl*> (m_rs->GetRegisteredControls().Item(0));
        m_rs->UnRegisterControl(ctrl);
        ctrl->SetNull();
    }
}

void LBoundGrid::ResetColEditor(const int col)
{
    wxObjectDataPtr<wxGridCellAttr> cellAttr;
    cellAttr = m_stringTable->GetAttr(GetGridCursorRow(), col, wxGridCellAttr::Col);
    if (cellAttr == NULL) return;
    wxObjectDataPtr<wxGridCellEditor> ed;
    ed = cellAttr->GetEditor(this, GetGridCursorRow(), col);
    // This deletes the editor (m_control).
    if (ed.get()) ed->Reset();
}

void LBoundGrid::SetDefaultRowSize(int height, bool resizeExistingRows)
{
    wxGrid::SetDefaultRowSize(((wxDouble) height) * m_scaleFactor, resizeExistingRows);
}

void LBoundGrid::SetColMinimalAcceptableWidth(int width)
{
    wxGrid::SetColMinimalAcceptableWidth(((wxDouble) width) * m_scaleFactor);
}

void LBoundGrid::SetColMinimalWidth(int col, int width)
{
    wxGrid::SetColMinimalWidth(col, ((wxDouble) width) * m_scaleFactor);
}

void LBoundGrid::SetColSize(int col, int width)
{
    wxGrid::SetColSize(col, ((wxDouble) width) * m_scaleFactor);
}

void LBoundGrid::SetDefaultColSize(int width, bool resizeExistingCols)
{
    wxGrid::SetDefaultColSize(((wxDouble) width) * m_scaleFactor, resizeExistingCols);
}

void LBoundGrid::SetRowLabelSize(int width)
{
    wxGrid::SetRowLabelSize(((wxDouble) width) * m_scaleFactor);
}

void LBoundGrid::SetRowMinimalAcceptableHeight(int height)
{
    wxGrid::SetRowMinimalAcceptableHeight(((wxDouble) height) * m_scaleFactor);
}

void LBoundGrid::SetRowMinimalHeight(int row, int height)
{
    wxGrid::SetRowMinimalHeight(row, ((wxDouble) height) * m_scaleFactor);
}

void LBoundGrid::SetRowSize(int row, int height)
{
    wxGrid::SetRowSize(row, ((wxDouble) height) * m_scaleFactor);
}

///////////////////////////////////////////////////////////////////////////

LBoundGridFormEvent::LBoundGridFormEvent()
{
}

LBoundGridFormEvent::~LBoundGridFormEvent()
{
}

void LBoundGridFormEvent::BeforeFormShown(const LBoundGrid * origin, wxBoxSizer* clientSizer)
{

}

void LBoundGridFormEvent::AfterFormHidden(const LBoundGrid * origin, wxBoxSizer* clientSizer)
{

}

void LBoundGridFormEvent::AfterFormDestroyed(const LBoundGrid * origin)
{

}
