/* 
 * File:   BaseGridPicker.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on December 7, 2019, 9:40 PM
 */

#include "BaseGridPicker.h"

IMPLEMENT_CLASS(BaseGridPicker, BasePicker)

BaseGridPicker::BaseGridPicker(wxWindow *parent,
                               wxWindowID id,
                               const wxArrayString& types,
                               wxSize popupSize,
                               const wxString& text,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style,
                               const wxValidator& validator,
                               const wxString& name)
: BasePicker(parent, id, popupSize, text, pos, size, style, validator, name)
{
    m_grid = NULL;
    m_stringTable = NULL;
    m_editable = true;
    m_intentLabel = _T("Intent");
    m_nbInsertRows = 3;
    m_colTypeChoices = types;
    wxButton * btn = static_cast<wxButton*> (GetPickerCtrl());
    btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &BaseGridPicker::ShowPopup, this);
}

BaseGridPicker::~BaseGridPicker()
{
    delete m_grid;
}

void BaseGridPicker::UpdatePickerFromTextCtrl()
{
    // Gets called when TextCtrl is changed
}

void BaseGridPicker::UpdateTextCtrlFromPicker()
{
    // Gets called when TextCtrl loses focus
}

void BaseGridPicker::CreateGrid()
{
    wxDELETE(m_grid);
    BasePicker::CreatePopup();
    m_grid = new wxGrid(m_popup, wxID_ANY);
    m_popup->GetSizer()->Add(m_grid, 1, wxGROW | wxALL, 0);
    m_grid->SetSize(m_popup->GetSize());
    m_grid->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    m_stringTable = new wxGridStringTable();
    m_stringTable->CanHaveAttributes();
    m_grid->HideRowLabels();
    m_grid->SetUseNativeColLabels();
    m_grid->SetTable(m_stringTable);
    m_grid->SetSelectionMode(wxGrid::wxGridSelectionModes::wxGridSelectRows);

    PrepareGrid();
    FillGrid();
}

void BaseGridPicker::SetIntentLabel(const wxString& intent)
{
    m_intentLabel = intent;
    wxASSERT_MSG(m_stringTable != NULL, _T("m_stringTable IS NULL"));
    m_stringTable->SetColLabelValue(0, m_intentLabel);
}

wxString BaseGridPicker::GetIntent() const
{
    wxASSERT_MSG(m_stringTable != NULL, _T("m_stringTable IS NULL"));
    for (uint row = 0; row < m_stringTable->GetRowsCount(); row++)
    {
        if (m_stringTable->GetValue(row, 2) != _T("0")
                && !m_stringTable->GetValue(row, 2).IsEmpty()
                && !m_stringTable->GetValue(row, 0).IsEmpty())
        {
            return m_stringTable->GetValue(row, 0);
        }
    }
    return wxEmptyString;
}

void BaseGridPicker::EnableEditing(bool editable)
{
    m_editable = editable;
    m_nbInsertRows = editable ? 3 : 0;
}

void BaseGridPicker::OnPopupHidden(wxShowEvent& evt)
{
    if (evt.IsShown())
    {
        evt.Skip();
        return;
    }
    m_grid->SaveEditControlValue();
    /*
     * Workaround an unexpected behavior :
     * If cell is edited to empty, SaveEditControlValue() above does not
     * save the empty string in the string table.
     * It still contains the old non-empty value.
     * GoToCell() below effectively ends editing.
     * But this does not happen if cell is edited from empty to any non-empty
     * value.
     */
    m_grid->GoToCell(m_grid->GetGridCursorRow(), m_grid->GetGridCursorCol());
    DumpGrid();
    for (uint row = 0; row < m_stringTable->GetRowsCount(); row++)
    {
        if (m_stringTable->GetValue(row, 2) != _T("0")
                && !m_stringTable->GetValue(row, 2).IsEmpty())
        {
            const wxString intent = m_stringTable->GetValue(row, 0);
            if (!intent.IsEmpty())
            {
                GetTextCtrl()->SetValue(intent);
            }
            else
            {
                GetTextCtrl()->SetValue(INVALID_INTENT);
            }
            evt.Skip();
            return;
        }
    }
    GetTextCtrl()->SetValue(INVALID_INTENT);
    evt.Skip();
}

void BaseGridPicker::PreparePreferredCol()
{
    wxASSERT_MSG(m_stringTable != NULL, _T("m_stringTable IS NULL"));
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(m_grid->GetGridCursorRow(), 2, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    wxGridCellBoolEditor * ed = new wxGridCellBoolEditor();
    colAtt->SetEditor(ed);
    wxGridCellBoolRenderer * rn = new wxGridCellBoolRenderer();
    colAtt->SetRenderer(rn);
    colAtt->SetAlignment(wxALIGN_CENTER, wxALIGN_CENTRE);
    m_stringTable->SetColAttr(colAtt, 2);
}

void BaseGridPicker::PrepareTypeCol()
{
    wxASSERT_MSG(m_stringTable != NULL, _T("m_stringTable IS NULL"));
    wxGridCellAttr * colAtt = m_stringTable->GetAttr(m_grid->GetGridCursorRow(), 1, wxGridCellAttr::Col);
    if (colAtt == NULL) colAtt = new wxGridCellAttr();
    wxGridCellChoiceEditor* ed = new wxGridCellChoiceEditor(m_colTypeChoices);
    colAtt->SetEditor(ed);
    m_stringTable->SetColAttr(colAtt, 1);
}

void BaseGridPicker::PrepareGrid()
{
    wxASSERT_MSG(m_grid != NULL, _T("m_grid IS NULL"));
    wxASSERT_MSG(m_stringTable != NULL, _T("m_stringTable IS NULL"));
    m_grid->AppendCols(4);
    m_grid->AppendRows(m_nbInsertRows);
    PreparePreferredCol();
    PrepareTypeCol();
    m_grid->SetColSize(0, 170);
    m_grid->SetColSize(1, 100);
    m_grid->SetColSize(3, 200);
    m_stringTable->SetColLabelValue(0, m_intentLabel);
    m_stringTable->SetColLabelValue(1, _T("Type"));
    m_stringTable->SetColLabelValue(2, _T("Preferred"));
    m_stringTable->SetColLabelValue(3, _T("Notes"));
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &BaseGridPicker::OnPrefCellChanged, this);
}

void BaseGridPicker::OnPrefCellChanged(wxGridEvent& evt)
{
    if (evt.GetCol() == 2 
            && m_stringTable->GetValue(evt.GetRow(), evt.GetCol()) == _T("1"))
    {
        for (uint row = 0; row < m_stringTable->GetRowsCount(); row++)
        {
            if (row != evt.GetRow())
            {
                m_stringTable->SetValue(row, evt.GetCol(), _T("0"));
            }
        }
        m_grid->ForceRefresh();
    }
    evt.Skip();
}

void BaseGridPicker::ShowPopup(wxCommandEvent& evt)
{
    BasePicker::DoShowPopup();
    evt.Skip();
}
