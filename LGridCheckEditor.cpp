/* 
 * File:   LGridCheckEditor.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 20:04
 */

#include "LGridCheckEditor.h"
#include "LBoundGrid.h"

LGridCheckEditor::LGridCheckEditor(const wxString& newColName, bool isDualstate, const wxString& newNullLabel)
{
    m_triState = !isDualstate;
    m_style = wxCHK_3STATE | wxCHK_ALLOW_3RD_STATE_FOR_USER;
    if (!m_triState) m_style = wxCHK_2STATE;
    m_colName = newColName;
    if (newNullLabel.IsEmpty())
    {
        m_nullLabel = _T("?");
    }
    else
    {
        m_nullLabel = newNullLabel;
    }
    m_type = LGridColEditor::CHECK;
    m_formEditor = NULL;
    m_BoundControl = NULL;
    m_BoundCheckBox = NULL;
}

LGridCheckEditor::~LGridCheckEditor()
{
    wxDELETE(m_control);
}

void LGridCheckEditor::Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler)
{
    m_control = new LBoundCheckBox(parent, id, m_style);
    m_BoundCheckBox = static_cast<LBoundCheckBox*> (m_control);
    m_BoundCheckBox->SetColumnName(m_colName);
    m_BoundCheckBox->SetLabel(wxEmptyString);
    m_BoundControl = m_BoundCheckBox;
    m_control->Show(false);
}

void LGridCheckEditor::BeginEdit(int row, int col, wxGrid *grid)
{
    if (m_control == NULL)
    {
        Create(grid->GetGridWindow(), wxID_ANY, NULL);
    }
    m_BoundCheckBox->SetResultSet(((LBoundGrid *) grid)->GetResultSet());
    const wxAny val = grid->GetTable()->GetValue(row, col);
    if (m_triState && val.As<wxString>() == m_nullLabel)
    {
        m_BoundCheckBox->SetNull();
    }
    else
    {
        m_BoundCheckBox->SetData(val);
    }
    m_control->Show(true);
}

wxGridCellEditor* LGridCheckEditor::Clone() const
{
    return new LGridCheckEditor(m_colName, !m_triState, m_nullLabel);
}

bool LGridCheckEditor::EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval)
{
    // What do we do here ?
    return true;
}

void LGridCheckEditor::ApplyEdit(int row, int col, wxGrid *grid)
{
    const wxAny val = m_BoundCheckBox->GetData();
    if (m_triState && val.As<wxString>() == L_SQLNULL)
    {
        grid->GetTable()->SetValue(row, col, m_nullLabel);
    }
    else
    {
        grid->GetTable()->SetValue(row, col, val.As<wxString>());
    }
}

void LGridCheckEditor::Reset()
{
    wxDELETE(m_control);
    m_BoundControl = NULL;
    m_BoundCheckBox = NULL;
}

wxString LGridCheckEditor::GetValue() const
{
    return m_control == NULL ? wxString(wxEmptyString) : m_BoundControl->GetData().As<wxString>();
}

wxControl* LGridCheckEditor::ProvideFormEditor(wxWindow * parent)
{
    if (!m_formEditor) m_formEditor = new wxCheckBox(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_style);
    if (m_triState)
    {
        m_formEditor->Set3StateValue(m_BoundCheckBox->Get3StateValue());
    }
    else
    {
        m_formEditor->SetValue(m_BoundCheckBox->GetValue());
    }
    m_formEditor->SetName(m_BoundCheckBox->GetName());
    if (m_BoundCheckBox->GetValidator()) m_formEditor->SetValidator(*(m_BoundCheckBox->GetValidator()));
    return m_formEditor;
}

void LGridCheckEditor::SyncBack(const int row, const int col, wxGrid * grid)
{
    if (!m_formEditor) return;
    if (m_triState)
    {
        m_BoundCheckBox->Set3StateValue(m_formEditor->Get3StateValue());
    }
    else
    {
        m_BoundCheckBox->SetValue(m_formEditor->GetValue());
    }
    ApplyEdit(row, col, grid);
    wxDELETE(m_formEditor);
}
