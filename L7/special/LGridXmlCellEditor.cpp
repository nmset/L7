/* 
 * File:   LGridXmlCellEditor.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on December 15, 2019, 6:32 PM
 */

#include "LGridXmlCellEditor.h"
#include "../LBoundGrid.h"

LGridXmlCellEditor::LGridXmlCellEditor(const wxString& newColName,
                                       const wxString& intentLabel,
                                       const wxArrayString& types,
                                       wxSize popupSize)
: wxGridCellEditor(), LGridColEditor()
{
    m_intentLabel = intentLabel;
    m_choices = types;
    m_popupSize = popupSize;
    m_colName = newColName;
    m_type = LGridColEditor::XML_GRID;
    m_formEditor = NULL;
    m_BoundControl = NULL;
    m_BoundXmlGridPicker = NULL;
    m_editRow = wxNOT_FOUND;
    m_rsEVH = NULL;
}

LGridXmlCellEditor::~LGridXmlCellEditor()
{
    delete m_rsEVH;
    wxDELETE(m_control);
}

void LGridXmlCellEditor::Create(wxWindow *parent, wxWindowID id, wxEvtHandler *evtHandler)
{
    m_control = new LBoundXmlGridPicker(parent, id, m_choices, m_popupSize);
    m_BoundXmlGridPicker = (static_cast<LBoundXmlGridPicker*> (m_control));
    m_BoundXmlGridPicker->SetColumnName(m_colName);
    if (!m_intentLabel.IsEmpty())
        m_BoundXmlGridPicker->SetIntentLabel(m_intentLabel);
    m_BoundControl = m_BoundXmlGridPicker;
    m_rsEVH = new XmlGridEditorRsEVH(this);
    m_control->Show(false);
}

void LGridXmlCellEditor::BeginEdit(int row, int col, wxGrid *grid)
{
    if (m_editRow != row)
        m_xmlBuffer.MakeNull();
    m_editRow = row;
    if (m_control == NULL)
    {
        Create(grid->GetGridWindow(), wxID_ANY, NULL);
    }

    LResultSet * rs = ((LBoundGrid *) grid)->GetResultSet();
    m_rsEVH->SetResultSet(rs);
    rs->RegisterEventHandler(m_rsEVH);

    m_BoundXmlGridPicker->SetResultSet(rs);
    if (m_xmlBuffer.IsNull() && row < rs->GetRowCount())
        m_xmlBuffer = rs->GetData(row, col);
    m_BoundXmlGridPicker->SetData(m_xmlBuffer);
    m_control->Show(true);
}

wxGridCellEditor* LGridXmlCellEditor::Clone() const
{
    return new LGridXmlCellEditor(m_colName, m_intentLabel, m_choices, m_popupSize);
}

bool LGridXmlCellEditor::EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval)
{
    // What do we do here ?
    return true;
}

void LGridXmlCellEditor::ApplyEdit(int row, int col, wxGrid *grid)
{
    m_xmlBuffer = m_BoundXmlGridPicker->GetValue();
    grid->GetTable()->SetValue(row, col, m_xmlBuffer.As<wxString>());
}

void LGridXmlCellEditor::Reset()
{
    delete m_rsEVH;
    m_rsEVH = NULL;
    wxDELETE(m_control);
    m_BoundControl = NULL;
    m_BoundXmlGridPicker = NULL;
    m_xmlBuffer.MakeNull();
    m_editRow = wxNOT_FOUND;
}

wxString LGridXmlCellEditor::GetValue() const
{
    return m_control == NULL ? wxString(wxEmptyString) : m_BoundControl->GetData().As<wxString>();
}

wxControl* LGridXmlCellEditor::ProvideFormEditor(wxWindow * parent)
{
    if (!m_formEditor) m_formEditor = new XmlGridPickerCtrl(parent, wxID_ANY, m_choices);
    m_formEditor->SetValue(GetValue());
    m_formEditor->SetName(m_BoundXmlGridPicker->GetName());
    m_formEditor->SetIntentLabel(m_BoundXmlGridPicker->GetIntentLabel());
    if (m_BoundXmlGridPicker->GetValidator()) m_formEditor->SetValidator(*(m_BoundXmlGridPicker->GetValidator()));
    return m_formEditor;
}

void LGridXmlCellEditor::SyncBack(const int row, const int col, wxGrid * grid)
{
    if (!m_formEditor) return;
    m_BoundXmlGridPicker->SetValue(m_formEditor->GetValue());
    ApplyEdit(row, col, grid);
    wxDELETE(m_formEditor);
}

///////////////////////////////////////////////////////////////////////////////

XmlGridEditorRsEVH::XmlGridEditorRsEVH(LGridXmlCellEditor * owner)
: LResultSetEvent()
{
    m_owner = owner;
    m_rs = NULL;
}

XmlGridEditorRsEVH::~XmlGridEditorRsEVH()
{
    if (m_rs)
        m_rs->UnRegisterEventHandler(this);
}

void XmlGridEditorRsEVH::AfterAction(LResultSet * caller, ACTIONS action)
{
    m_rs = caller;
    if (action == ACTIONS::ACTION_CANCEL)
    {
        m_owner->m_xmlBuffer.MakeNull();
    }
}

void XmlGridEditorRsEVH::DataChanged(LResultSet* caller)
{
    m_rs = caller;
    m_owner->m_xmlBuffer.MakeNull();
}
