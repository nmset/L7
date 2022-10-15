/* 
 * File:   JsonGridPickerCtrl.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on December 7, 2019, 10:01 PM
 */

/*
 * [
   {
      "Intent" : "a@b.com",
      "Type" : "Home",
      "Preferred" : 0,
      "Notes" : "Do not use"  
   },
   {
      "Intent" : "c@d.com",
      "Type" : "Work",
      "Preferred" : 1,
      "Notes" : "Neither"
   }
   ]

 */
#include "JsonGridPickerCtrl.h"
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

IMPLEMENT_CLASS(JsonGridPickerCtrl, BaseGridPicker)

#define EMPTY_ARR _T("[]")

JsonGridPickerCtrl::JsonGridPickerCtrl(wxWindow *parent,
                                       wxWindowID id,
                                       const wxArrayString& types,
                                       wxSize popupSize,
                                       const wxString& text,
                                       const wxPoint& pos,
                                       const wxSize& size,
                                       long style,
                                       const wxValidator& validator,
                                       const wxString& name)
: BaseGridPicker(parent, id, types, popupSize, text, pos, size, style, validator, name)
{
    /*
     * Don't create the grid here. Let SetValue() do it, deleting the previous
     * one every time new data is fed in.
     */
    SetValue(wxEmptyString);
}

JsonGridPickerCtrl::~JsonGridPickerCtrl()
{
}

void JsonGridPickerCtrl::SetValue(const wxString& value)
{
    if (value.IsEmpty())
    {
        m_value = EMPTY_ARR;
        BaseGridPicker::CreateGrid(); // +++
        GetTextCtrl()->SetValue(INVALID_INTENT);
        return;
    }
    m_value = value;
    BaseGridPicker::CreateGrid(); // +++
    GetTextCtrl()->SetValue(INVALID_INTENT);
    
    wxJSONReader reader(wxJSONREADER_STRICT);
    wxJSONValue root;
    uint nbErr = reader.Parse(m_value, &root);
    if (nbErr > 0)
    {
        const wxArrayString& errors = reader.GetErrors();
        for (int i = 0; i < nbErr; i++)
        {
            wxASSERT_MSG(nbErr == 0, errors[i]);
        }
        return;
    }
    if (!root.IsArray())
    {
        wxASSERT_MSG(root.IsArray(), _("JSON data is not an array"));
        return;
    }
    for (uint row = 0; row < root.Size(); row++)
    {
        if (root[row]["Preferred"].IsValid()
                && !root[row]["Preferred"].AsString().IsEmpty()
                && root[row]["Preferred"].AsInt() != 0)
        {
            GetTextCtrl()->SetValue(root[row]["Intent"].AsString());
            return;
        }
    }
    GetTextCtrl()->SetValue(INVALID_INTENT);
}

wxString JsonGridPickerCtrl::GetValue()
{
    DumpGrid();
    /*
     * wxJSONWriter adds quotes and line breaks to EMPTY_ARR !
     * m_value is set in DumpGrid(). If there is no grid data, m_value is set
     * to \"[]\"\n
     */
    wxJSONValue empty(EMPTY_ARR);
    wxJSONWriter writer;
    wxString sempty;
    writer.Write(empty, sempty);
    if (m_value == sempty)
        return wxEmptyString;
    return m_value;
}

void JsonGridPickerCtrl::FillGrid()
{
    wxASSERT_MSG(m_grid != NULL, _("m_grid IS NULL"));
    wxJSONReader reader(wxJSONREADER_STRICT);
    wxJSONValue root;
    uint nbErr = reader.Parse(m_value, &root);
    if (nbErr > 0)
    {
        const wxArrayString& errors = reader.GetErrors();
        for (int i = 0; i < nbErr; i++)
        {
            wxASSERT_MSG(nbErr == 0, errors[i]);
        }
        return;
    }
    if (!root.IsArray())
    {
        wxASSERT_MSG(root.IsArray(), _("JSON data is not an array"));
        return;
    }

    for (uint row = 0; row < root.Size(); row++)
    {
        m_grid->InsertRows(row);
        m_grid->SetCellValue(row, 0, root[row]["Intent"].AsString());
        m_grid->SetCellValue(row, 1, root[row]["Type"].AsString());
        m_grid->SetCellValue(row, 2, root[row]["Preferred"].AsString());
        m_grid->SetCellValue(row, 3, root[row]["Notes"].AsString());
    }
}

void JsonGridPickerCtrl::DumpGrid()
{
    wxASSERT_MSG(m_grid != NULL, _("m_grid IS NULL"));
    wxASSERT_MSG(m_stringTable != NULL, _("m_stringTable IS NULL"));
    if (!m_editable || !m_stringTable || !m_grid)
        return;
    wxJSONWriter writer;
    wxJSONValue root(EMPTY_ARR);
    uint row = 0;
    uint jrow = 0;
    for (row; row < m_stringTable->GetRowsCount(); row++)
    {
        if (m_stringTable->GetValue(row, 0).IsEmpty())
            continue;
        root[jrow]["Intent"] = m_stringTable->GetValue(row, 0);
        root[jrow]["Type"] = m_stringTable->GetValue(row, 1);
        root[jrow]["Preferred"] = (m_stringTable->GetValue(row, 2) == _T("0")
                || m_stringTable->GetValue(row, 2).IsEmpty()) ? 0 : 1;
        root[jrow]["Notes"] = m_stringTable->GetValue(row, 3);
        jrow++;
    }
    writer.Write(root, m_value); // May be EMPTY_ARR
}
