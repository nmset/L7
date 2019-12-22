/* 
 * File:   LBoundComboBox.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 3 juin 2014, 19:00
 */

#include "LBoundComboBox.h"
#include "LItemData.h"

LBoundComboBox::LBoundComboBox(wxWindow* parent, wxWindowID id)
: wxComboBox(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY)
{
    m_sqlQuote = wxEmptyString;
    SetNull();
}

LBoundComboBox::~LBoundComboBox()
{
    if (m_rs) m_rs->UnRegisterControl(this);
}

bool LBoundComboBox::IsTranslated() const
{
    if (HasClientObjectData())
    {
        for (unsigned int i = 0; i < GetCount(); i++)
        {
            LItemData * x = static_cast<LItemData*> (GetClientObject(i));
            if (x != NULL) return true;
        }
    }
    return false;
}

bool LBoundComboBox::SelectionIsTranslated() const
{
    if (IsTranslated())
    {
        LItemData * x = static_cast<LItemData*> (GetClientObject(GetSelection()));
        return (x != NULL);
    }
    else
    {
        return false;
    }
}

const wxAny LBoundComboBox::GetDataOfIndex(unsigned int idx) const
{
    if (IsTranslated())
    {
        LItemData * x = static_cast<LItemData*> (GetClientObject(idx));
        if (x != NULL) return x->GetData();
        return wxEmptyString;
    }
    else
    {
        return GetString(idx);
    }
}

const int LBoundComboBox::GetIndexOfData(const wxAny& data) const
{
    const wxString sdata = data.As<wxString>();
    if (IsTranslated())
    {
        /* C001 Can be costly if there are thousands of items ! 
         * In such a case, the control's usefulness is doubtful.
         */
        for (unsigned int i = 0; i < GetCount(); i++)
        {
            LItemData * x = static_cast<LItemData*> (GetClientObject(i));
            if (x)
            {
                if (x->GetData().As<wxString>() == sdata)
                {
                    return i;
                }
            }
        }
        return wxNOT_FOUND;
    }
    else
    {
        return FindString(sdata);
    }
}

const wxString LBoundComboBox::GetItemOfData(const wxAny& data) const
{
    if (IsTranslated())
    {
        // See C001
        const wxString sdata = data.As<wxString>();
        for (unsigned int i = 0; i < GetCount(); i++)
        {
            LItemData * x = static_cast<LItemData*> (GetClientObject(i));
            if (x != NULL)
            {
                if (x->GetData().As<wxString>() == sdata)
                {
                    return GetString(i);
                }
            }
        }
    }
    return wxEmptyString;
}

const wxAny LBoundComboBox::GetDataOfItem(const wxString& item) const
{
    if (IsTranslated())
    {
        const int idx = FindString(item, true);
        LItemData * x = static_cast<LItemData*> (GetClientObject(idx));
        if (x != NULL) return x->GetData();
        return wxEmptyString;
    }
    else
    {
        return item;
    }
}

const wxString LBoundComboBox::GetItemOfNull() const
{
    if (IsTranslated())
    {
        // See C001
        for (unsigned int i = 0; i < GetCount(); i++)
        {
            LItemData * x = static_cast<LItemData*> (GetClientObject(i));
            if (x == NULL) return GetString(i);
        }
    }
    return wxEmptyString;
}

const int LBoundComboBox::GetIndexOfNull() const
{
    if (IsTranslated())
    {
        // See C001
        for (unsigned int i = 0; i < GetCount(); i++)
        {
            LItemData * x = static_cast<LItemData*> (GetClientObject(i));
            if (x == NULL) return i;
        }
    }
    return wxNOT_FOUND;
}

void LBoundComboBox::Clear()
{
    wxComboBox::Clear();
    const long wsflags = GetWindowStyleFlag();
    SetWindowStyleFlag(0);
    SetSelection(wxNOT_FOUND);
    SetValue(wxEmptyString);
    SetWindowStyleFlag(wsflags);
}

void LBoundComboBox::SetData(const wxAny& data)
{
    const wxString sData = data.As<wxString>();
    if (IsTranslated())
    {
        if (data.IsNull()
                || sData.IsEmpty()
                || sData == L_SQLNULL)
        {
            SetNull();
            return;
        }
        for (unsigned int i = 0; i < GetCount(); i++)
        {
            LItemData * x = static_cast<LItemData*> (GetClientObject(i));
            if (x)
            {
                if (x->GetData().As<wxString>() == sData)
                {
                    SetSelection(i);
                    return;
                }
            }
        }
    }
    else
    {
        //bool hasData = FindString(sData, true);
        SetValue(sData);
    }
}

const wxAny LBoundComboBox::GetData()
{
    if (IsNull()) return L_SQLNULL;
    if (IsTranslated())
    {
        LItemData * x = static_cast<LItemData*> (GetClientObject(GetSelection()));
        if (x != NULL)
        {
            return x->GetData();
        }
        else
        {
            return L_SQLNULL;
        }
    }
    else
    {
        return GetValue();
    }
}

void LBoundComboBox::SetResultSet(LResultSet* newResultSet)
{
    m_rs = newResultSet;
    if (m_rs == NULL) return;
    m_rs->RegisterControl(this);
}

bool LBoundComboBox::IsNull()
{
    if (IsTranslated())
    {
        if (GetSelection() != wxNOT_FOUND)
        {
            if (SelectionIsTranslated())
            {
                LItemData * x = static_cast<LItemData*> (GetClientObject(GetSelection()));
                if (x != NULL) return false;
            }
        }
        return true;
    }
    else
    {
        return (GetValue().IsEmpty());
    }
}

void LBoundComboBox::SetNull()
{
    if (IsTranslated())
    {
        for (unsigned int i = 0; i < GetCount(); i++)
        {
            wxClientData * x = GetClientObject(i);
            if (x == NULL)
            {
                SetSelection(i);
                return;
            }
        }
        return;
    }
    long wsflags = GetWindowStyleFlag();
    SetWindowStyleFlag(0);
    SetSelection(wxNOT_FOUND);
    SetValue(wxEmptyString);
    SetWindowStyleFlag(wsflags);
}

bool LBoundComboBox::IsDirty()
{
    wxASSERT_MSG(m_rs != NULL, "m_rs est NULL.");
    wxAny BEData = m_rs->GetData(m_columnName);
    if (BEData.As<wxString>().IsEmpty()) BEData = L_SQLNULL;
    return (GetData().As<wxString>() != BEData.As<wxString>());
}

const wxString LBoundComboBox::GetDisplayedData()
{
    return GetValue();
}