/* 
 * File:   BasePicker.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 *
 * Created on December 7, 2019, 9:24 PM
 */

#ifndef BASEPICKER_H
#define BASEPICKER_H

#include <wx/wx.h>
#include <wx/pickerbase.h>
#include <wx/popupwin.h>

/**
 * Abstract class adding a popup to wxPickerBase styled with
 * a non-editable wxTextCtrl. It manages the popup's position and size also.
 * The picker control is a wxButton that shows the popup.
 */
class BasePicker : public wxPickerBase
{
    DECLARE_CLASS(BasePicker)
public:
    BasePicker(wxWindow *parent,
               wxWindowID id,
               wxSize popupSize = wxDefaultSize,
               const wxString& text = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxPB_USE_TEXTCTRL,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxButtonNameStr);
    virtual ~BasePicker();

    virtual void UpdatePickerFromTextCtrl() = 0;
    virtual void UpdateTextCtrlFromPicker() = 0;

private:

protected:
    wxPopupTransientWindow * m_popup;
    wxSize m_popupSize;
    /**
     * A new popup is always created. Any previous popup is deleted.
     * It binds to OnPopupHidden().
     */
    virtual void CreatePopup();
    /**
     * Shows the popup managing its position and size.
     * <br/><br/>
     * If m_popupSize.GetWidth() < 0, the popup's width is set to the control's
     * width.
     * <br/>
     * If m_popupSize.GetHeight() < 0, it defaults to 300.
     */
    virtual void DoShowPopup();
    virtual void OnPopupHidden(wxShowEvent& evt) = 0;
};

#endif /* BASEPICKER_H */

