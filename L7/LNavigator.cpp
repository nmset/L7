/* 
 * File:   LNavigator.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 4 juin 2014, 14:57
 */

#include "LNavigator.h"

LNavigator::LNavigator(wxWindow* parent, wxWindowID id)
: wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxTAB_TRAVERSAL)
{
    m_vsz = new wxBoxSizer(wxVERTICAL);
    SetSizer(m_vsz);
    m_hsz1 = new wxBoxSizer(wxHORIZONTAL);
    m_vsz->Add(m_hsz1, 1, wxGROW, 0);
    m_hsz1->Add(5, 5, 0, wxGROW | wxTOP | wxBOTTOM, 8);
    m_btnNew = new wxButton(this, wxID_ANY, _("New"), wxDefaultPosition, wxSize(-1, 15), wxNO_BORDER);
    m_hsz1->Add(m_btnNew, 1, wxGROW, 0);
    m_btnSave = new wxButton(this, wxID_ANY, _("Save"), wxDefaultPosition, wxSize(-1, 15), wxNO_BORDER);
    m_hsz1->Add(m_btnSave, 1, wxGROW, 0);
    m_btnCancel = new wxButton(this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxSize(-1, 15), wxNO_BORDER);
    m_hsz1->Add(m_btnCancel, 1, wxGROW, 0);
    m_btnRefresh = new wxButton(this, wxID_ANY, _("Refresh"), wxDefaultPosition, wxSize(-1, 15), wxNO_BORDER);
    m_hsz1->Add(m_btnRefresh, 1, wxGROW | wxTOP | wxBOTTOM, 0);
    m_btnDelete = new wxButton(this, wxID_ANY, _("Delete"), wxDefaultPosition, wxSize(-1, 15), wxNO_BORDER);
    m_hsz1->Add(m_btnDelete, 1, wxGROW, 0);
    m_hsz2 = new wxBoxSizer(wxHORIZONTAL);
    m_vsz->Add(m_hsz2, 1, wxGROW, 0);
    m_btnFirst = new wxButton(this, wxID_ANY, _T("<<"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxNO_BORDER);
    m_hsz2->Add(m_btnFirst, 0, wxGROW, 0);
    m_btnPrevious = new wxButton(this, wxID_ANY, _T("<"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxNO_BORDER);
    m_hsz2->Add(m_btnPrevious, 0, wxGROW, 0);
    m_lbl = new wxStaticText(this, wxID_STATIC, _("Rec : "), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    m_hsz2->Add(m_lbl, 1, wxALIGN_CENTER_VERTICAL, 0);
    m_btnNext = new wxButton(this, wxID_ANY, _T(">"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxNO_BORDER);
    m_hsz2->Add(m_btnNext, 0, wxGROW, 0);
    btnLast = new wxButton(this, wxID_ANY, _T(">>"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxNO_BORDER);
    m_hsz2->Add(btnLast, 0, wxGROW, 0);

    m_btnNew->Bind(wxEVT_BUTTON, &LNavigator::OnButtonNew, this);
    m_btnSave->Bind(wxEVT_BUTTON, &LNavigator::OnButtonSave, this);
    m_btnCancel->Bind(wxEVT_BUTTON, &LNavigator::OnButtonCancel, this);
    m_btnRefresh->Bind(wxEVT_BUTTON, &LNavigator::OnButtonRefresh, this);
    m_btnDelete->Bind(wxEVT_BUTTON, &LNavigator::OnButtonDelete, this);
    m_btnFirst->Bind(wxEVT_BUTTON, &LNavigator::OnBtnFirst, this);
    m_btnPrevious->Bind(wxEVT_BUTTON, &LNavigator::OnBtnPrevious, this);
    m_btnNext->Bind(wxEVT_BUTTON, &LNavigator::OnBtnNext, this);
    btnLast->Bind(wxEVT_BUTTON, &LNavigator::OnBtnLast, this);

    m_vsz->Fit(this);

    m_rs = NULL;
    m_evh = NULL;
}

LNavigator::~LNavigator()
{
    delete m_btnNew;
    delete m_btnSave;
    delete m_btnCancel;
    delete m_btnRefresh;
    delete m_btnDelete;
    delete m_btnFirst;
    delete m_btnPrevious;
    delete m_lbl;
    delete m_btnNext;
    delete btnLast;
    m_rs->UnRegisterEventHandler(m_evh);
    delete m_evh;
}

void LNavigator::OnBtnNext(wxCommandEvent& evt)
{
    if (m_rs) m_rs->Next();
    evt.Skip(false);
}

void LNavigator::OnBtnFirst(wxCommandEvent& evt)
{
    if (m_rs) m_rs->First();
    evt.Skip(false);
}

void LNavigator::OnBtnLast(wxCommandEvent& evt)
{
    if (m_rs) m_rs->Last();
    evt.Skip(false);
}

void LNavigator::OnBtnPrevious(wxCommandEvent& evt)
{
    if (m_rs) m_rs->Previous();
    evt.Skip(false);
}

void LNavigator::OnButtonSave(wxCommandEvent& evt)
{
    if (m_rs) m_rs->Save();
    evt.Skip(false);
}

void LNavigator::OnButtonRefresh(wxCommandEvent& evt)
{
    if (m_rs) m_rs->Refresh();
    evt.Skip(false);
}

void LNavigator::OnButtonCancel(wxCommandEvent& evt)
{
    if (m_rs) m_rs->Cancel();
    evt.Skip(false);
}

void LNavigator::OnButtonNew(wxCommandEvent& evt)
{
    if (m_rs) m_rs->AddNew();
    evt.Skip(false);
}

void LNavigator::OnButtonDelete(wxCommandEvent& evt)
{
    if (m_rs) m_rs->Delete();
    evt.Skip(false);
}

void LNavigator::SetResultSet(LResultSet * newResultSet)
{
    m_rs = newResultSet;
    if (!m_rs) return;
    wxASSERT_MSG(m_rs->GetConnection() != NULL, _T("GetConnection() = NULL."));
    m_evh = new NavigatorRsEVH(m_lbl);
    m_rs->RegisterEventHandler(m_evh);
    ShowActionBar(!(m_rs->IsReadOnly()));
    if (m_vsz->IsShown(m_hsz1))
    {
        if (m_rs->GetConnection()) ShowActionBar(!(m_rs->GetConnection()->IsReadOnly()));
    }
}

LResultSet* LNavigator::GetResultSet() const
{
    return m_rs;
}

void LNavigator::ShowActionBar(bool show) const
{
    m_vsz->Show(m_hsz1, show, true);
    m_vsz->Layout();
}

void LNavigator::ShowNavigationBar(bool show) const
{
    m_vsz->Show(m_hsz2, show, true);
    m_vsz->Layout();
}

void LNavigator::ShowButtonAddNew(bool show) const
{
    m_hsz1->Show(m_btnNew, show, false);
    m_vsz->Layout();
}

void LNavigator::ShowButtonSave(bool show) const
{
    m_hsz1->Show(m_btnSave, show, false);
    m_vsz->Layout();
}

void LNavigator::ShowButtonCancel(bool show) const
{
    m_hsz1->Show(m_btnCancel, show, false);
    m_vsz->Layout();
}

void LNavigator::ShowButtonRefresh(bool show) const
{
    m_hsz1->Show(m_btnRefresh, show, false);
    m_vsz->Layout();
}

void LNavigator::ShowButtonDelete(bool show) const
{
    m_hsz1->Show(m_btnDelete, show, false);
    m_vsz->Layout();
}

////////////////////////////////////////////////////////////////////////////

NavigatorRsEVH::NavigatorRsEVH(wxStaticText * newLbl)
{
    m_lbl = newLbl;
}

NavigatorRsEVH::~NavigatorRsEVH()
{

}

void NavigatorRsEVH::Initialised(LResultSet* caller)
{
    UpdateLocation(caller);
}

void NavigatorRsEVH::AfterChangeRow(LResultSet* caller)
{
    UpdateLocation(caller);
}

void NavigatorRsEVH::AfterAction(LResultSet* caller, ACTIONS action)
{
    UpdateLocation(caller);
}

void NavigatorRsEVH::UpdateLocation(const LResultSet* caller)
{
    if (!caller) return;
    if (caller->IsInserting())
    {
        m_lbl->SetLabel(_("Insert"));
    }
    else
    {
        const wxAny row = (caller->GetRow() + 1);
        const wxAny count = caller->GetRowCount();
        const wxString loc = _("Rec : ")
                + row.As<wxString>()
                + _T("/")
                + count.As<wxString>();
        m_lbl->SetLabel(loc);
    }
}
