/* 
 * File:   LBoundCheckBox.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 2 juin 2014, 16:03
 */

#include "LBoundCheckBox.h"

LBoundCheckBox::LBoundCheckBox(wxWindow* parent, wxWindowID id, long style)
: wxCheckBox(parent, id, _T("LCheckBox"), wxDefaultPosition, wxDefaultSize, style) {
    m_sqlQuote = wxEmptyString;
    SetNull();
}

LBoundCheckBox::~LBoundCheckBox() {
    if (m_rs) m_rs->UnRegisterControl(this);
}

const wxAny LBoundCheckBox::GetData() {
    if (Is3State()) {
        if (Get3StateValue() == wxCHK_UNDETERMINED) return L_SQLNULL;
        if (Get3StateValue() == wxCHK_CHECKED) return 1;
        if (Get3StateValue() == wxCHK_UNCHECKED) return 0;
    } else {
        if (GetValue() == wxCHK_UNCHECKED) return 0;
        if (GetValue() == wxCHK_CHECKED) return 1;
    }
}

void LBoundCheckBox::SetData(const wxAny& newData) {
    /* The interpretation of literal non is not documented 
     * as it's for internal use.
     */
    wxString snewData;
    newData.GetAs<wxString>(&snewData);
    if (Is3State()) {
        if (snewData.IsEmpty()
                || snewData == L_SQLNULL) {
            Set3StateValue(wxCHK_UNDETERMINED);
            return;
        }
        if (snewData == _T("0")
                || snewData.Lower() == _T("no")) {
            Set3StateValue(wxCHK_UNCHECKED);
        } else {
            Set3StateValue(wxCHK_CHECKED);
        }
        return;
    } else {
        if (snewData == _T("0")
                || snewData.IsEmpty()
                || snewData.Lower() == _T("no")) {
            SetValue(wxCHK_UNCHECKED);
        } else {
            SetValue(wxCHK_CHECKED);
        }
    }
}

void LBoundCheckBox::SetResultSet(LResultSet* newResultSet) {
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    m_rs->RegisterControl(this);
}

bool LBoundCheckBox::IsNull() {
    if (Is3State()) {
        return (Get3StateValue() == wxCHK_UNDETERMINED);
    } else {
        return false;
    }
}

void LBoundCheckBox::SetNull() {
    SetData(wxEmptyString);
}

bool LBoundCheckBox::IsDirty() {
    wxASSERT_MSG(m_rs != NULL, "m_rs est NULL.");
    wxAny ctrlData = GetData(); // 0, 1 or L_SQLNULL
    wxAny BEData = m_rs->GetData(m_columnName);
    int iBEData;
    BEData.GetAs(&iBEData);
    if (Is3State()) {
        if (BEData.IsNull()
                || BEData.As<wxString>().IsEmpty()) {
            BEData = L_SQLNULL;
        } else if (iBEData != 0) {
            BEData = 1;
        }
    } else {
        if (BEData.IsNull()
                || iBEData == 0
                || BEData.As<wxString>().IsEmpty()) {
            BEData = 0;
        } else {
            BEData = 1;
        }
    }
    return (ctrlData.As<wxString>() != BEData.As<wxString>());
}

const wxString LBoundCheckBox::GetDisplayedData() {
    if (Is3State()) {
        if (Get3StateValue() == wxCHK_UNDETERMINED) return L_SQLNULL;
        if (Get3StateValue() == wxCHK_CHECKED) return _("Yes");
        if (Get3StateValue() == wxCHK_UNCHECKED) return _("No");
    } else {
        if (GetValue() == wxCHK_CHECKED) return _("Yes");
        if (GetValue() == wxCHK_UNCHECKED) return _("No");
    }
}
