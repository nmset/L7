/* 
 * File:   LInformation.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on 24 mai 2014, 21:23
 */

#ifndef LINFORMATION_H
#define LINFORMATION_H

#include <wx/wx.h>

/**
 * Holds messages from LConnection and LResultSet objects.
 * @param newCode 
 * @param newMsg
 */
class LInformation
{
public:
    LInformation(const wxString& newCode,
                 const wxString& newMsg);
    virtual ~LInformation();

    wxString GetCode() const
    {
        return m_code;
    }

    wxString GetMessage() const
    {
        return m_msg;
    }

    /**
     * 
     * @return Both the code and the message concatenated.
     */
    wxString GetFullInformation() const
    {
        return m_code + _T(" : ") + m_msg;
    };
private:
    wxString m_code;
    wxString m_msg;
};

#endif /* LINFORMATION_H */

