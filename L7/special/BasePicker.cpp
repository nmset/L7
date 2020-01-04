/* 
 * File:   BasePicker.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on December 7, 2019, 9:24 PM
 */

#include "BasePicker.h"

IMPLEMENT_CLASS(BasePicker, wxPickerBase)

BasePicker::BasePicker(wxWindow *parent,
                       wxWindowID id,
                       wxSize popupSize,
                       const wxString& text,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxValidator& validator,
                       const wxString& name)
: wxPickerBase()
{
    m_popup = NULL;
    m_popupSize = popupSize;
    wxPickerBase::CreateBase(parent, id, text, pos, size, style, validator, name);
    wxButton * btn = new wxButton(this, wxID_ANY, _T("↓"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    SetPickerCtrl(btn);
    wxPickerBase::PostCreation();
    GetTextCtrl()->SetEditable(false);
    GetTextCtrl()->SetMaxLength(0);
    SetTextCtrlGrowable(true);
    SetPickerCtrlGrowable(true);
}

BasePicker::~BasePicker()
{
    delete m_popup;
}

void BasePicker::CreatePopup()
{
    wxDELETE(m_popup);
    m_popup = new wxPopupTransientWindow(this);
    wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
    m_popup->SetSizer(sz);

    m_popup->Bind(wxEVT_SHOW, &BasePicker::OnPopupHidden, this);
}

void BasePicker::DoShowPopup()
{
    if (m_popupSize.GetWidth() < 0)
        m_popupSize.SetWidth(GetClientSize().GetWidth());
    if (m_popupSize.GetHeight() < 0)
        m_popupSize.SetHeight(300);
    m_popup->SetClientSize(m_popupSize);
    m_popup->GetSizer()->Layout();

    wxScreenDC dc;
    const int screenWidth = dc.GetSize().GetWidth();
    const int screenHeight = dc.GetSize().GetHeight();
    const wxPoint oriPos = GetScreenPosition() + GetClientRect().GetBottomLeft();
    wxPoint dest(oriPos.x, oriPos.y);
    if (oriPos.x > (screenWidth - m_popupSize.GetWidth())) dest.x = (screenWidth - m_popupSize.GetWidth());
    if (oriPos.x < 0) dest.x = 0;
    if (oriPos.y > (screenHeight - m_popupSize.GetHeight())) dest.y = (screenHeight - m_popupSize.GetHeight());
    if (oriPos.y < 0) dest.y = 0;
    if (m_popupSize.GetWidth() > screenWidth) dest.x = 0;
    if (m_popupSize.GetHeight() > screenHeight) dest.y = 0;
    m_popup->SetPosition(dest);

    m_popup->Popup();
}
