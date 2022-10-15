/* 
 * File:   LBoundSpinCtrl.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 22 septembre 2014, 14:38
 */

#include "LBoundSpinCtrl.h"

LBoundSpinCtrl::LBoundSpinCtrl(wxWindow* parent,
                               wxWindowID id,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style,
                               int min,
                               int max,
                               int initial)
: wxSpinCtrl(parent, id, wxEmptyString, pos, size, style, min, max, initial)
{
    m_initialValue = initial;
}

LBoundSpinCtrl::~LBoundSpinCtrl()
{
    if (m_rs) m_rs->UnRegisterControl(this);
}

void LBoundSpinCtrl::SetResultSet(LResultSet* newResultSet)
{
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    m_rs->RegisterControl(this);
}

void LBoundSpinCtrl::SetData(const wxAny& newData)
{
    if (newData.IsNull() || newData.As<wxString>().IsEmpty())
    {
        SetValue(m_initialValue);
    }
    else
    {
        int iData;
        newData.GetAs<int>(&iData);
        SetValue(iData);
    }
}

bool LBoundSpinCtrl::IsDirty()
{
    wxASSERT_MSG(m_rs != NULL, _T("RS = NULL"));
    wxAny BEData = m_rs->GetData(m_columnName);
    if (BEData.IsNull() || BEData.As<wxString>().IsEmpty())
        BEData = m_initialValue;
    return (GetData().As<wxString>() != BEData.As<wxString>());

}
