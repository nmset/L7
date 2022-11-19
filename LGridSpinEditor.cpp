/* 
 * File:   LGridSpinEditor.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 23 septembre 2014, 18:02
 */

#include "LGridSpinEditor.h"
#include "LBoundGrid.h"

LGridSpinEditor::LGridSpinEditor(const wxString& newColName,
                                 int newMin,
                                 int newMax,
                                 int newInitial)
{
    m_min = newMin;
    m_max = newMax;
    m_initial = newInitial;
    m_colName = newColName;
    m_type = LGridColEditor::SPIN;
    m_formEditor = NULL;
    m_BoundControl = NULL;
    m_BoundSpinCtrl = NULL;
}

LGridSpinEditor::~LGridSpinEditor()
{
    wxDELETE(m_control);
}

void LGridSpinEditor::Create(wxWindow* parent, wxWindowID id, wxEvtHandler* evtHandler)
{
    m_control = new LBoundSpinCtrl(parent, id, wxDefaultPosition,
                                   wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT,
                                   m_min, m_max, m_initial);
    m_BoundSpinCtrl = (static_cast<LBoundSpinCtrl*> (m_control));
    m_BoundSpinCtrl->SetColumnName(m_colName);
    m_BoundControl = m_BoundSpinCtrl;
    m_control->Show(false);
}

void LGridSpinEditor::BeginEdit(int row, int col, wxGrid* grid)
{
    if (m_control == NULL)
    {
        Create(grid->GetGridWindow(), wxID_ANY, NULL);
    }
    m_BoundSpinCtrl->SetResultSet(((LBoundGrid *) grid)->GetResultSet());
    const wxAny sText = grid->GetTable()->GetValue(row, col);
    m_BoundSpinCtrl->SetData(sText);
    m_control->Show(true);
}

wxGridCellEditor* LGridSpinEditor::Clone() const
{
    return new LGridSpinEditor(m_colName, m_min, m_max, m_initial);
}

bool LGridSpinEditor::EndEdit(int row, int col, const wxGrid* grid, const wxString& oldval, wxString* newval)
{
    // What do we do here ?
    return true;
}

void LGridSpinEditor::ApplyEdit(int row, int col, wxGrid* grid)
{
    grid->GetTable()->SetValue(row, col, m_BoundSpinCtrl->GetData().As<wxString>());
}

void LGridSpinEditor::Reset()
{
    wxDELETE(m_control);
    m_BoundControl = NULL;
    m_BoundSpinCtrl = NULL;
}

wxString LGridSpinEditor::GetValue() const
{
    if (m_control == NULL) return wxAny(m_initial).As<wxString>();
    return m_BoundSpinCtrl->GetData().As<wxString>();
}

wxControl* LGridSpinEditor::ProvideFormEditor(wxWindow* parent)
{
    if (!m_formEditor) m_formEditor = new wxSpinCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, m_min, m_max, m_initial);
    m_formEditor->SetValue(GetValue());
    m_formEditor->SetName(m_BoundSpinCtrl->GetName());
    if (m_BoundSpinCtrl->GetValidator()) m_formEditor->SetValidator(*(m_BoundSpinCtrl->GetValidator()));
    return m_formEditor;
}

void LGridSpinEditor::SyncBack(const int row, const int col, wxGrid* grid)
{
    if (!m_formEditor) return;
    m_BoundSpinCtrl->SetValue(m_formEditor->GetValue());
    ApplyEdit(row, col, grid);
    wxDELETE(m_formEditor);
}
