/* 
 * File:   XmlGridPickerCtrl.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on December 14, 2019, 3:28 PM
 */

#ifndef XMLGRIDPICKERCTRL_H
#define XMLGRIDPICKERCTRL_H

#include "BaseGridPicker.h"
#include <wx/xml/xml.h>

/**
 * The grid content is stored in an XML document.
 * @return 
 */
class XmlGridPickerCtrl : public BaseGridPicker
{
    DECLARE_CLASS(XmlGridPickerCtrl)
public:
    XmlGridPickerCtrl(wxWindow *parent,
                      wxWindowID id,
                      const wxArrayString& types,
                      wxSize popupSize = wxDefaultSize,
                      const wxString& text = wxEmptyString,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = wxPB_USE_TEXTCTRL,
                      const wxValidator& validator = wxDefaultValidator,
                      const wxString& name = wxButtonNameStr);
    virtual ~XmlGridPickerCtrl();

    /**
     * Creates a new grid deleting the previous one and fills it with the XML
     * data.
     * @param value
     */
    virtual void SetValue(const wxString& value);

    /**
     * Dumps the grid content and returns it as an XML document. But returns
     * empty string if XML document is empty.
     * @return 
     */
    virtual wxString GetValue();
private:
    /**
     * Parses the XML document and fills the grid.
     */
    virtual void FillGrid();
    /**
     * Translates the grid content to an XML document.
     * If the Intent column is empty, this row is ignored.
     */
    virtual void DumpGrid();
    /**
     * Is the XML document.
     */
    wxString m_value;
};

#endif /* XMLGRIDPICKERCTRL_H */

