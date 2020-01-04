/* 
 * File:   LGridJsonCellEditor.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * Author: user
 * 
 * Created on December 9, 2019, 10:36 AM
 */

#include "LGridJsonCellEditor.h"
#include "../LBoundGrid.h"

LGridJsonCellEditor::LGridJsonCellEditor(const wxString& newColName,
                                         const wxString& intentLabel,
                                         const wxArrayString& types,
                                         wxSize popupSize)
: wxGridCellEditor(), LGridColEditor()
{
    m_intentLabel = intentLabel;
    m_choices = types;
    m_popupSize = popupSize;
    m_colName = newColName;
    m_type = LGridColEditor::JSON_GRID;
    m_formEditor = NULL;
    m_BoundControl = NULL;
    m_BoundJsonGridPicker = NULL;
    m_editRow = wxNOT_FOUND;
    m_rsEVH = NULL;
}

LGridJsonCellEditor::~LGridJsonCellEditor()
{
    delete m_rsEVH;
    wxDELETE(m_control);
}

void LGridJsonCellEditor::Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler)
{
    m_control = new LBoundJsonGridPicker(parent, id, m_choices, m_popupSize);
    m_BoundJsonGridPicker = (static_cast<LBoundJsonGridPicker*> (m_control));
    m_BoundJsonGridPicker->SetColumnName(m_colName);
    if (!m_intentLabel.IsEmpty())
        m_BoundJsonGridPicker->SetIntentLabel(m_intentLabel);
    m_BoundControl = m_BoundJsonGridPicker;
    m_rsEVH = new JsonGridEditorRsEVH(this);
    m_control->Show(false);
}

void LGridJsonCellEditor::BeginEdit(int row, int col, wxGrid *grid)
{
    if (m_editRow != row)
        m_jsonBuffer.MakeNull();
    m_editRow = row;
    if (m_control == NULL)
    {
        Create(grid->GetGridWindow(), wxID_ANY, NULL);
    }

    LResultSet * rs = ((LBoundGrid *) grid)->GetResultSet();
    m_rsEVH->SetResultSet(rs);
    rs->RegisterEventHandler(m_rsEVH);

    m_BoundJsonGridPicker->SetResultSet(rs);
    if (m_jsonBuffer.IsNull() && row < rs->GetRowCount())
        m_jsonBuffer = rs->GetData(row, col);
    m_BoundJsonGridPicker->SetData(m_jsonBuffer);
    m_control->Show(true);
}

wxGridCellEditor* LGridJsonCellEditor::Clone() const
{
    return new LGridJsonCellEditor(m_colName, m_intentLabel, m_choices, m_popupSize);
}

bool LGridJsonCellEditor::EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval)
{
    // What do we do here ?
    return true;
}

void LGridJsonCellEditor::ApplyEdit(int row, int col, wxGrid *grid)
{
    m_jsonBuffer = m_BoundJsonGridPicker->GetValue();
    grid->GetTable()->SetValue(row, col, m_jsonBuffer.As<wxString>());
}

void LGridJsonCellEditor::Reset()
{
    delete m_rsEVH;
    m_rsEVH = NULL;
    wxDELETE(m_control);
    m_BoundControl = NULL;
    m_BoundJsonGridPicker = NULL;
    m_jsonBuffer.MakeNull();
    m_editRow = wxNOT_FOUND;
}

wxString LGridJsonCellEditor::GetValue() const
{
    return m_control == NULL ? wxString(wxEmptyString) : m_BoundControl->GetData().As<wxString>();
}

wxControl* LGridJsonCellEditor::ProvideFormEditor(wxWindow * parent)
{
    if (!m_formEditor) m_formEditor = new JsonGridPickerCtrl(parent, wxID_ANY, m_choices);
    m_formEditor->SetValue(GetValue());
    m_formEditor->SetName(m_BoundJsonGridPicker->GetName());
    m_formEditor->SetIntentLabel(m_BoundJsonGridPicker->GetIntentLabel());
    if (m_BoundJsonGridPicker->GetValidator()) m_formEditor->SetValidator(*(m_BoundJsonGridPicker->GetValidator()));
    return m_formEditor;
}

void LGridJsonCellEditor::SyncBack(const int row, const int col, wxGrid * grid)
{
    if (!m_formEditor) return;
    m_BoundJsonGridPicker->SetValue(m_formEditor->GetValue());
    ApplyEdit(row, col, grid);
    wxDELETE(m_formEditor);
}

///////////////////////////////////////////////////////////////////////////////

JsonGridEditorRsEVH::JsonGridEditorRsEVH(LGridJsonCellEditor * owner)
: LResultSetEvent()
{
    m_owner = owner;
    m_rs = NULL;
}

JsonGridEditorRsEVH::~JsonGridEditorRsEVH()
{
    if (m_rs)
        m_rs->UnRegisterEventHandler(this);
}

void JsonGridEditorRsEVH::AfterAction(LResultSet * caller, ACTIONS action)
{
    m_rs = caller;
    if (action == ACTIONS::ACTION_CANCEL)
    {
        m_owner->m_jsonBuffer.MakeNull();
    }
}

void JsonGridEditorRsEVH::DataChanged(LResultSet* caller)
{
    m_rs = caller;
    m_owner->m_jsonBuffer.MakeNull();
}
