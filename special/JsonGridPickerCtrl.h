/* 
 * File:   JsonGridPickerCtrl.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on December 7, 2019, 10:01 PM
 */

#ifndef JSONGRIDPICKERCTRL_H
#define JSONGRIDPICKERCTRL_H

#include "BaseGridPicker.h"

/**
 * The grid content is stored in a JSON array.
 * @return 
 */
class JsonGridPickerCtrl : public BaseGridPicker
{
    DECLARE_CLASS(JsonGridPickerCtrl)
public:
    JsonGridPickerCtrl(wxWindow *parent,
                       wxWindowID id,
                       const wxArrayString& types,
                       wxSize popupSize = wxDefaultSize,
                       const wxString& text = wxEmptyString,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxPB_USE_TEXTCTRL,
                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = wxButtonNameStr);
    virtual ~JsonGridPickerCtrl();

    /**
     * Creates a new grid deleting the previous one and fills it with the JSON
     * data.
     * @param value
     */
    virtual void SetValue(const wxString& value);

    /**
     * Dumps the grid content and returns it as a JSON array. But returns empty
     * string if JSON array is empty.
     * @return 
     */
    virtual wxString GetValue();

protected:
    /**
     * Parses the JSON data and fills the grid. It should be pure JSON, without
     * any extension like comments.
     */
    virtual void FillGrid();
    /**
     * Translates the grid content to a JSON array.
     * If the Intent column is empty, this row is ignored.
     */
    virtual void DumpGrid();
    /**
     * Is the JSON array.
     */
    wxString m_value;

private:

};

#endif /* JSONGRIDPICKERCTRL_H */

