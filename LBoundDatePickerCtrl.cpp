/* 
 * File:   LBoundDatePickerCtrl.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 3 juin 2014, 16:37
 */

#include "LBoundDatePickerCtrl.h"
#include <wx/tokenzr.h>

LBoundDatePickerCtrl::LBoundDatePickerCtrl(wxWindow* parent, wxWindowID id)
: wxDatePickerCtrl(parent, id, wxInvalidDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT | wxDP_ALLOWNONE | wxDP_SHOWCENTURY)
{
    m_sqlQuote = _T("'");
    SetValue(wxInvalidDateTime);
}

LBoundDatePickerCtrl::~LBoundDatePickerCtrl()
{
    if (m_rs) m_rs->UnRegisterControl(this);
}

const wxAny LBoundDatePickerCtrl::GetData()
{
    if (IsNull()) return L_SQLNULL;
    return GetValue().GetDateOnly().FormatISODate();
}

void LBoundDatePickerCtrl::SetData(const wxAny& newData)
{
    if (newData.IsNull())
    {
        SetValue(wxInvalidDateTime);
        return;
    }
    if (newData.As<wxString>() == L_SQLNULL || newData.As<wxString>().IsEmpty())
    {
        SetValue(wxInvalidDateTime);
        return;
    }
    wxString s;
    newData.GetAs(&s);
    const wxDateTime dt = BuildDate(s);
    SetValue(dt);
}

void LBoundDatePickerCtrl::SetResultSet(LResultSet* newResultSet)
{
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    m_rs->RegisterControl(this);
}

bool LBoundDatePickerCtrl::IsNull()
{
    return !(GetValue().IsValid());
}

void LBoundDatePickerCtrl::SetNull()
{
    SetData(L_SQLNULL);
}

bool LBoundDatePickerCtrl::IsDirty()
{
    wxASSERT_MSG(m_rs != NULL, "m_rs est NULL.");
    wxAny BEData = m_rs->GetData(m_columnName);
    if (BEData.As<wxString>().IsEmpty()) BEData = L_SQLNULL;
    return (GetData().As<wxString>() != BEData.As<wxString>());
}

const wxString LBoundDatePickerCtrl::GetDisplayedData()
{
    if (IsNull()) return wxEmptyString;
    return GetValue().GetDateOnly().FormatDate();
}

wxDateTime LBoundDatePickerCtrl::BuildDate(const wxString& ISODate)
{
    wxDateTime dt = wxDateTime::Today();
    if (!dt.ParseISODate(ISODate)) return wxInvalidDateTime;
    unsigned int * res = new unsigned int[3];
    unsigned int i = 0;
    wxAny token;
    wxStringTokenizer tkz(ISODate, _T("-"));
    while (tkz.HasMoreTokens())
    {
        token = tkz.GetNextToken();
        token.GetAs(&res[i]);
        i++;
    }
    dt.Set(res[2], (wxDateTime::Month) (res[1] - 1), res[0]);
    wxDELETEA(res);
    if (!dt.IsValid()) return wxInvalidDateTime;
    dt.ResetTime();
    return dt;
}
