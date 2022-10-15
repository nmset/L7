/* 
 * File:   LBoundTextCtrl.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 2 juin 2014, 14:53
 */

#include "LBoundTextCtrl.h"

LBoundTextCtrl::LBoundTextCtrl(wxWindow* parent, wxWindowID id, long style)
: wxTextCtrl(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, style)
{
    m_sqlQuote = _T("'");
}

LBoundTextCtrl::~LBoundTextCtrl()
{
    if (m_rs) m_rs->UnRegisterControl(this);
}

void LBoundTextCtrl::SetResultSet(LResultSet* newResultSet)
{
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    m_rs->RegisterControl(this);
}

const wxAny LBoundTextCtrl::GetData()
{
    if (GetValue().IsEmpty()) return L_SQLNULL;
    return GetValue();
}

void LBoundTextCtrl::SetData(const wxAny& newData)
{
    ChangeValue(newData.As<wxString>());
}

bool LBoundTextCtrl::IsDirty()
{
    wxASSERT_MSG(m_rs != NULL, _("RS = NULL"));
    wxAny BEData = m_rs->GetData(m_columnName);
    return (GetValue() != BEData.As<wxString>());
}
