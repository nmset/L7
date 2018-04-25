/* 
 * File:   LGridTextEditor.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 16:09
 */

#include "LGridTextEditor.h"
#include "LBoundGrid.h"

LGridTextEditor::LGridTextEditor(const wxString& newColName, bool newMultiline)
    : wxGridCellAutoWrapStringEditor()
{
    m_multiline = newMultiline;
    m_style = 0;
    if (m_multiline) m_style = wxTE_MULTILINE;
    m_colName = newColName;
    m_type = LGridColEditor::TEXT;
    m_formEditor = NULL;
    m_BoundControl = NULL;
    m_BoundTextCtrl = NULL;
}

LGridTextEditor::~LGridTextEditor()
{
    wxDELETE(m_control);
}

void LGridTextEditor::Create (wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler) {
    m_control = new LBoundTextCtrl( parent, id, m_style);
    m_BoundTextCtrl = (static_cast<LBoundTextCtrl*> (m_control));
    m_BoundTextCtrl->SetColumnName(m_colName);
    m_BoundControl = m_BoundTextCtrl;
    m_control->Show(false);
}
void LGridTextEditor::BeginEdit (int row, int col, wxGrid *grid) {
    if (m_control == NULL) {
        Create(grid->GetGridWindow(), wxID_ANY, NULL);
    }
    m_BoundTextCtrl->SetResultSet(((LBoundGrid *) grid)->GetResultSet());
    const wxAny sText = grid->GetTable()->GetValue(row, col);
    m_BoundTextCtrl->SetData(sText);
    m_control->Show(true);
}
wxGridCellEditor* LGridTextEditor::Clone () const {
    return new LGridTextEditor(m_colName, m_multiline);
}
bool LGridTextEditor::EndEdit (int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval) {
    // What do we do here ?
    return true;
}
void LGridTextEditor::ApplyEdit (int row, int col, wxGrid *grid) {
    grid->GetTable()->SetValue(row, col, m_BoundTextCtrl->GetValue());
}
void LGridTextEditor::Reset () {
    wxDELETE(m_control);
    m_BoundControl = NULL;
    m_BoundTextCtrl = NULL;
}
wxString LGridTextEditor::GetValue() const {
    return m_control == NULL ? wxString(wxEmptyString) : m_BoundControl->GetData().As<wxString>();
}
wxControl* LGridTextEditor::ProvideFormEditor(wxWindow * parent) {
    if (!m_formEditor) m_formEditor = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_style);
    m_formEditor->SetValue(GetValue());
    m_formEditor->SetName(m_BoundTextCtrl->GetName());
    if (m_BoundTextCtrl->GetValidator()) m_formEditor->SetValidator(*(m_BoundTextCtrl->GetValidator()));
    return m_formEditor;
}
void LGridTextEditor::SyncBack(const int row, const int col, wxGrid * grid) {
    if (!m_formEditor) return;
    m_BoundTextCtrl->SetValue(m_formEditor->GetValue());
    ApplyEdit(row, col, grid);
    wxDELETE(m_formEditor);
}
