/* 
 * File:   LGridDateEditor.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 12 juin 2014, 17:27
 */

#include "LGridDateEditor.h"
#include "LBoundGrid.h"

LGridDateEditor::LGridDateEditor(const wxString& newColName)
{
    m_colName = newColName;
    m_type = LGridColEditor::DATE;
    m_formEditor = NULL;
    m_BoundControl = NULL;
    m_BoundDatePicker = NULL;
}

LGridDateEditor::~LGridDateEditor()
{
    wxDELETE(m_control);
}

void LGridDateEditor::Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler)
{
    m_control = new LBoundDatePickerCtrl(parent, id);
    m_BoundDatePicker = static_cast<LBoundDatePickerCtrl*> (m_control);
    m_BoundDatePicker->SetColumnName(m_colName);
    m_BoundControl = m_BoundDatePicker;
    m_control->Show(false);
}

void LGridDateEditor::BeginEdit(int row, int col, wxGrid* grid)
{
    if (m_control == NULL)
    {
        Create(grid->GetGridWindow(), wxID_ANY, NULL);
    }
    m_BoundDatePicker->SetResultSet(((LBoundGrid *) grid)->GetResultSet());
    const wxAny str_date = grid->GetTable()->GetValue(row, col);
    m_BoundDatePicker->SetData(str_date);
    m_BoundDatePicker->Show(true);
}

wxGridCellEditor* LGridDateEditor::Clone() const
{
    return new LGridDateEditor(m_colName);
}

bool LGridDateEditor::EndEdit(int row, int col, const wxGrid* grid, const wxString& oldval, wxString* newval)
{
    // What do we do here ?
    return true;
}

void LGridDateEditor::ApplyEdit(int row, int col, wxGrid* grid)
{
    const wxString sText = m_BoundDatePicker->IsNull()
            ? _T("")
            : m_BoundDatePicker->GetData().As<wxString>();
    grid->GetTable()->SetValue(row, col, sText);
}

void LGridDateEditor::Reset()
{
    wxDELETE(m_control);
    m_BoundControl = NULL;
    m_BoundDatePicker = NULL;
}

wxString LGridDateEditor::GetValue() const
{
    return m_control == NULL ? wxString(wxEmptyString) : m_BoundControl->GetData().As<wxString>();
}

wxControl* LGridDateEditor::ProvideFormEditor(wxWindow* parent)
{
    if (!m_formEditor) m_formEditor = new wxDatePickerCtrl(parent, wxID_ANY, wxInvalidDateTime, wxDefaultPosition, wxDefaultSize, m_BoundDatePicker->GetWindowStyleFlag());
    m_formEditor->SetValue(m_BoundDatePicker->GetValue());
    //formEditor->SetValue(formEditor->GetValue()); // BUG SINCE 2.9.x Still ?
    m_formEditor->SetName(m_BoundDatePicker->GetName());
    if (m_BoundDatePicker->GetValidator()) m_formEditor->SetValidator(*(m_BoundDatePicker->GetValidator()));
    return m_formEditor;
}

void LGridDateEditor::SyncBack(const int row, const int col, wxGrid* grid)
{
    if (!m_formEditor) return;
    m_BoundDatePicker->SetValue(m_formEditor->GetValue());
    //m_BoundDatePicker->SetValue(m_BoundControl->GetValue()); // BUG SINCE 2.9.x . Still ?
    ApplyEdit(row, col, grid);
    wxDELETE(m_formEditor);
}

