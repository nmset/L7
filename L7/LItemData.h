/* 
 * File:   ltItemData.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 24 mai 2014, 20:26
 */

#ifndef LITEMDATA_H
#define LITEMDATA_H

#include <wx/wx.h>

/**
 Holds translated values for LComboBox items as wxClientData.
 * 
 */
class LItemData : public wxClientData
{
public:
    LItemData(const long newData);
    LItemData(const wxString newData);
    LItemData(const wxAny newData);
    virtual ~LItemData();

    wxAny GetData()
    {
        return m_data;
    }
    long GetLong();
    wxString GetString();

    void SetValue(wxAny newData)
    {
        m_data = newData;
    }
private:
    wxAny m_data;
};

#endif /* LITEMDATA_H */

