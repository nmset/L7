/* 
 * File:   LBoundJsonGridPicker.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on December 8, 2019, 9:23 AM
 */

#include "LBoundJsonGridPicker.h"
#include <wx/jsonreader.h>

IMPLEMENT_CLASS(LBoundJsonGridPicker, JsonGridPickerCtrl)

LBoundJsonGridPicker::LBoundJsonGridPicker(wxWindow *parent,
                                           wxWindowID id,
                                           const wxArrayString& types,
                                           wxSize popupSize,
                                           const wxString& text,
                                           const wxPoint& pos,
                                           const wxSize& size,
                                           long style,
                                           const wxValidator& validator,
                                           const wxString& name)
: JsonGridPickerCtrl(parent, id, types, popupSize, text, pos, size, style, validator, name)
{
    m_sqlQuote = _T("'");
}

LBoundJsonGridPicker::~LBoundJsonGridPicker()
{
    if (m_rs) m_rs->UnRegisterControl(this);
}

void LBoundJsonGridPicker::SetResultSet(LResultSet* newResultSet)
{
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    m_rs->RegisterControl(this);
}

const wxAny LBoundJsonGridPicker::GetData()
{
    // If m_value == EMPTY_ARR, GetValue() returns empty string
    if (GetValue().IsEmpty()) return L_SQLNULL;
    return GetValue();
}

void LBoundJsonGridPicker::SetData(const wxAny& newData)
{
    SetValue(newData.As<wxString>());
}

void LBoundJsonGridPicker::SetNull()
{
    // m_value is set to EMPTY_ARR
    SetValue(wxEmptyString);
}

bool LBoundJsonGridPicker::IsDirty()
{
    wxASSERT_MSG(m_rs != NULL, _T("RS = NULL"));
    wxAny BEData = m_rs->GetData(m_columnName);
    const wxString ctrlValue = GetValue();
    /*
     * Make JSON objects from backend and control values, and compare them.
     * Postgresql : jsonb columns contain single line values. Can't compare strings.
     */
    const wxString beValue = BEData.As<wxString>();
    if (ctrlValue.IsEmpty() || beValue.IsEmpty())
        return (ctrlValue != beValue);

    wxJSONReader beReader(wxJSONREADER_STRICT);
    wxJSONValue beRoot;
    uint nbErr = beReader.Parse(beValue, &beRoot);
    if (nbErr > 0)
    {
        const wxArrayString& errors = beReader.GetErrors();
        for (int i = 0; i < nbErr; i++)
        {
            wxASSERT_MSG(nbErr == 0, errors[i]);
        }
        // Backend value is not what we expect. Declare dirty to overwrite it.
        return true;
    }
    if (!beRoot.IsArray())
    {
        wxASSERT_MSG(beRoot.IsArray(), _T("Back end JSON data is not an array"));
        return true;
    }

    wxJSONReader ctrlReader(wxJSONREADER_STRICT);
    wxJSONValue ctrlRoot;
    nbErr = beReader.Parse(ctrlValue, &ctrlRoot);
    // Should not happen as we wrote the data.
    if (nbErr > 0)
    {
        const wxArrayString& errors = ctrlReader.GetErrors();
        for (int i = 0; i < nbErr; i++)
        {
            wxASSERT_MSG(nbErr == 0, errors[i]);
        }
        // Control value is not what we expect. Don't overwrite in backend.
        return false;
    }
    if (!ctrlRoot.IsArray())
    {
        wxASSERT_MSG(ctrlRoot.IsArray(), _T("Control JSON data is not an array"));
        return false;
    }

    return (!ctrlRoot.IsSameAs(beRoot));
}
