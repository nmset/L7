/* 
 * File:   LGridComboEditor.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 12 juin 2014, 18:34
 */

#include "LGridComboEditor.h"
#include "LBoundGrid.h"

LGridComboEditor::LGridComboEditor(const wxString& newColName)
{
    m_colName = newColName;
    m_type = LGridColEditor::COMBO;
    m_formEditor = NULL;
    m_BoundControl = NULL;
    m_BoundComboBox = NULL;
}

LGridComboEditor::~LGridComboEditor()
{
    wxDELETE(m_control);
}

void LGridComboEditor::Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler)
{
    m_control = new LBoundComboBox(parent, id);
    m_BoundComboBox = static_cast<LBoundComboBox*> (m_control);
    m_BoundComboBox->SetColumnName(m_colName);
    m_BoundControl = m_BoundComboBox;
    m_control->Show(false);
}

void LGridComboEditor::BeginEdit(int row, int col, wxGrid* grid)
{
    if (m_control == NULL)
    {
        Create(grid->GetGridWindow(), wxID_ANY, NULL);
    }
    m_BoundComboBox->SetResultSet(((LBoundGrid *) grid)->GetResultSet());
    const wxString cellVal = grid->GetTable()->GetValue(row, col);
    if (cellVal.IsEmpty())
    {
        m_BoundComboBox->SetNull();
    }
    else
    {
        m_BoundComboBox->SetStringSelection(cellVal);
    }
    m_BoundComboBox->Show(true);
}

wxGridCellEditor* LGridComboEditor::Clone() const
{
    return new LGridComboEditor(m_colName);
}

bool LGridComboEditor::EndEdit(int row, int col, const wxGrid* grid, const wxString& oldval, wxString* newval)
{
    // What do we do here ?
    return true;
}

void LGridComboEditor::ApplyEdit(int row, int col, wxGrid* grid)
{
    grid->GetTable()->SetValue(row, col, m_BoundComboBox->GetStringSelection());
}

void LGridComboEditor::Reset()
{
    wxDELETE(m_control);
    m_BoundControl = NULL;
    m_BoundComboBox = NULL;
}

wxString LGridComboEditor::GetValue() const
{
    return m_control == NULL ? wxString(wxEmptyString) : m_BoundControl->GetData().As<wxString>();
}

wxControl* LGridComboEditor::ProvideFormEditor(wxWindow* parent)
{
    if (!m_formEditor)
    {
        m_formEditor = new wxComboBox(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxArrayString(), m_BoundComboBox->GetWindowStyleFlag());
        for (unsigned int i = 0; i < m_BoundComboBox->GetCount(); i++)
        {
            m_formEditor->Append(m_BoundComboBox->GetString(i));
        }
        m_formEditor->Select(m_BoundComboBox->GetSelection());
        m_formEditor->SetName(m_BoundComboBox->GetName());
        if (m_BoundComboBox->GetValidator()) m_formEditor->SetValidator(*(m_BoundComboBox->GetValidator()));
    }
    return m_formEditor;
}

void LGridComboEditor::SyncBack(const int row, const int col, wxGrid* grid)
{
    if (!m_formEditor) return;
    m_BoundComboBox->Select(m_formEditor->GetSelection());
    ApplyEdit(row, col, grid);
    grid->ForceRefresh();
    grid->GetGridWindow()->SetFocus();
    delete m_formEditor;
    m_formEditor = NULL;
}
