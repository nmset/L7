/* 
 * File:   LGridColEditor.h
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 15:45
 */

#include <wx/wx.h>
#include <wx/grid.h>
#include "LBoundControl.h"

#ifndef LGRIDCOLEDITOR_H
#define LGRIDCOLEDITOR_H

/**
 * Abstract class defining methods data bound grid cell editors must implement.
 */
class LGridColEditor
{
public:
    LGridColEditor();
    virtual ~LGridColEditor();

    enum COL_TYPE
    {
        TEXT, COMBO, DATE, CHECK, SPIN, JSON_GRID, XML_GRID
    };

    /**
     * 
     * @return the database table column name.
     */
    const wxString& GetColumnName() const
    {
        return m_colName;
    }

    int GetType() const
    {
        return m_type;
    }
    /**
     * 
     * @param parent : the parent of the returned wxControl.
     * @return 
     */
    virtual wxControl* ProvideFormEditor(wxWindow * parent) = 0;
    /**
     * Applies values of controls in form view of the row to their respective cells.
     * @param row
     * @param col
     * @param grid
     */
    virtual void SyncBack(const int row, const int col, wxGrid * grid) = 0;
    /**
     * 
     * @return the wxControl on the form. May be NULL if grid row is not in form view.
     */
    virtual wxControl* GetFormEditor() const = 0;

    LBoundControl * GetBoundControl() const
    {
        return m_BoundControl;
    }

protected:
    int m_type;
    wxString m_colName;
    /**
     * m_control of the wxGridCellEditor, typed as generic LBoundControl.
     */
    LBoundControl * m_BoundControl;
private:

};

#endif /* LGRIDCOLEDITOR_H */

