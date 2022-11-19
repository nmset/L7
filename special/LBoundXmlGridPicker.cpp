/* 
 * File:   LBoundXmlGridPicker.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on December 14, 2019, 10:12 PM
 */

#include "LBoundXmlGridPicker.h"

IMPLEMENT_CLASS(LBoundXmlGridPicker, XmlGridPickerCtrl)

LBoundXmlGridPicker::LBoundXmlGridPicker(wxWindow *parent,
                                         wxWindowID id,
                                         const wxArrayString& types,
                                         wxSize popupSize,
                                         const wxString& text,
                                         const wxPoint& pos,
                                         const wxSize& size,
                                         long style,
                                         const wxValidator& validator,
                                         const wxString& name)
: XmlGridPickerCtrl(parent, id, types, popupSize, text, pos, size, style, validator, name)
{
    m_sqlQuote = _T("'");
}

LBoundXmlGridPicker::~LBoundXmlGridPicker()
{
    if (m_rs) m_rs->UnRegisterControl(this);
}

void LBoundXmlGridPicker::SetResultSet(LResultSet* newResultSet)
{
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    m_rs->RegisterControl(this);
}

const wxAny LBoundXmlGridPicker::GetData()
{
    // If m_value is EMPTY_DOC, GetValue() returns empty string
    if (GetValue().IsEmpty()) return L_SQLNULL;
    return GetValue();
}

void LBoundXmlGridPicker::SetData(const wxAny& newData)
{
    SetValue(newData.As<wxString>());
}

void LBoundXmlGridPicker::SetNull()
{
    // m_value is set to EMPTY_DOC
    SetValue(wxEmptyString);
}

bool LBoundXmlGridPicker::IsDirty()
{
    /*
     * String compare backend and control values.
     * Database column data type is TEXT, hence data is saved verbatim.
     * On the other hand, comparing XML documents is not trivial.
     * wxWidgets does not have this functionality, not does it write
     * canonical XML, AFAIK.
     * Linking to other libraries for this would be an overkill.
     */
    wxASSERT_MSG(m_rs != NULL, _T("RS = NULL"));
    wxAny BEData = m_rs->GetData(m_columnName);
    return (GetValue() != BEData.As<wxString>());
}
