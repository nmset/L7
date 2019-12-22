/* 
 * File:   XmlGridPickerCtrl.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on December 14, 2019, 3:28 PM
 */

#include "XmlGridPickerCtrl.h"
#include <wx/sstream.h>
#include "XmlHelper.h"

/*
 * <?xml version="1.0" encoding="UTF-8"?>
<TABLE>
  <ROW Type="Work" Preferred="1" Notes="Do not use">+33 1 23 45 67 89</ROW>
  <ROW Type="Home" Preferred="0" Notes="Neither">+33 9 87 65 43 21</ROW>
</TABLE>
 */

IMPLEMENT_CLASS(XmlGridPickerCtrl, BaseGridPicker)

XmlGridPickerCtrl::XmlGridPickerCtrl(wxWindow *parent,
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

XmlGridPickerCtrl::~XmlGridPickerCtrl()
{
}

void XmlGridPickerCtrl::SetValue(const wxString& value)
{
    if (value.IsEmpty())
    {
        m_value = EMPTY_DOC;
        BaseGridPicker::CreateGrid(); // +++
        GetTextCtrl()->SetValue(INVALID_INTENT);
        return;
    }
    m_value = value;
    BaseGridPicker::CreateGrid(); // +++
    GetTextCtrl()->SetValue(INVALID_INTENT);
    wxXmlDocument doc;
    wxXmlNode * root = XmlHelper::ValidateXmlValue(doc, m_value);
    if (root == NULL)
    {
        GetTextCtrl()->SetValue(INVALID_INTENT);
        return;
    }
    // Look for preferred
    wxXmlNode * row = root->GetChildren();
    while (row)
    {
        const wxString pref = row->GetAttribute(XML_ATTR_PREF);
        if (!pref.IsEmpty()
                && pref != _T("0"))
        {
            GetTextCtrl()->SetValue(row->GetNodeContent());
            return;
        }
        row = row->GetNext();
    }
    GetTextCtrl()->SetValue(INVALID_INTENT);
}

wxString XmlGridPickerCtrl::GetValue()
{
    DumpGrid();

    wxXmlDocument doc;
    wxXmlNode * root = new wxXmlNode(wxXML_ELEMENT_NODE, XML_ROOT_NAME);
    doc.SetRoot(root);
    wxString emptyXmlData;
    wxStringOutputStream sos(&emptyXmlData);
    doc.Save(sos);

    if (m_value == emptyXmlData)
        return wxEmptyString;
    return m_value;
}

void XmlGridPickerCtrl::FillGrid()
{
    wxASSERT_MSG(m_grid != NULL, _("m_grid IS NULL"));
    wxXmlDocument doc;
    wxXmlNode * root = XmlHelper::ValidateXmlValue(doc, m_value);
    if (root == NULL)
    {
        GetTextCtrl()->SetValue(INVALID_INTENT);
        return;
    }
    wxXmlNode * row = root->GetChildren();
    int idx = 0;
    while (row)
    {
        m_grid->InsertRows(idx);
        m_grid->SetCellValue(idx, 0, row->GetNodeContent());
        m_grid->SetCellValue(idx, 1, row->GetAttribute(XML_ATTR_TYPE));
        m_grid->SetCellValue(idx, 2, row->GetAttribute(XML_ATTR_PREF));
        m_grid->SetCellValue(idx, 3, row->GetAttribute(XML_ATTR_NOTES));
        idx++;
        row = row->GetNext();
    }
}

void XmlGridPickerCtrl::DumpGrid()
{
    wxASSERT_MSG(m_grid != NULL, _("m_grid IS NULL"));
    wxASSERT_MSG(m_stringTable != NULL, _("m_stringTable IS NULL"));
    if (!m_editable || !m_stringTable || !m_grid)
        return;
    wxXmlDocument doc;
    // This is EMPTY_DOC
    wxXmlNode * root = new wxXmlNode(wxXML_ELEMENT_NODE, XML_ROOT_NAME);
    doc.SetRoot(root);
    for (uint grow = 0; grow < m_stringTable->GetRowsCount(); grow++)
    {
        if (m_stringTable->GetValue(grow, 0).IsEmpty())
            continue;
        /*
         * Don't use the other constructor :
         * wxXmlNode(wxXmlNode *parent, wxXmlNodeType type ...
         * The row order will be reversed each time.
         * Explicitly add independent nodes to root.
         */
        wxXmlNode * row = new wxXmlNode(wxXML_ELEMENT_NODE, XML_ROW_NAME);
        wxXmlAttribute * attr = new wxXmlAttribute(XML_ATTR_TYPE, m_stringTable->GetValue(grow, 1));
        row->AddAttribute(attr);
        attr = new wxXmlAttribute(XML_ATTR_PREF, (m_stringTable->GetValue(grow, 2) == _T("0")
                                  || m_stringTable->GetValue(grow, 2).IsEmpty()) ? _T("0") : _T("1"));
        row->AddAttribute(attr);
        attr = new wxXmlAttribute(XML_ATTR_NOTES, m_stringTable->GetValue(grow, 3));
        row->AddAttribute(attr);
        new wxXmlNode(row, wxXML_TEXT_NODE, _T("irrelevant_name"), m_stringTable->GetValue(grow, 0));
        root->AddChild(row);
    }
    wxString xmlData;
    wxStringOutputStream sos(&xmlData);
    bool res = doc.Save(sos);
    if (!res)
    {
        wxASSERT_MSG(res, _T("Error dumping to XML"));
        // Let's not empty m_value.
    }
    else
    {
        m_value = xmlData; // May be empty XML with XML_ROOT_NAME tag only
    }
}


