/* 
 * File:   LGridCheckRenderer.h
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 21:32
 */

#ifndef LGRIDCHECKRENDERER_H
#define LGRIDCHECKRENDERER_H

#include "wx/grid.h"

class LGridCheckRenderer : public wxGridCellStringRenderer
{
public:
    /**
     * N.B. newNullLabel must be the same in LGridCheckEditor and LGridCheckRenderer
     * @param isDualState : is the cell edited with a dual or tristate checkbox ?
     * @param newNullLabel : what to show if tristate with undetermined state.
     */
    LGridCheckRenderer(bool isDualState = false, const wxString& newNullLabel = _T("?"));
    virtual ~LGridCheckRenderer();
    /**
     * What is rendered per cell value.
     * 
     * If tristate : empty cell maps to nullLabel,
     * 0 maps to literal Non,
     * anything else is shown as literal Oui.
     * 
     * If dualstate : empty cell or 0 map to literal Non,
     * Anything else is shown as literal Oui.
     * @param grid
     * @param attr
     * @param dc
     * @param rect
     * @param row
     * @param col
     * @param isSelected
     */
    void Draw(wxGrid & grid,
              wxGridCellAttr & attr,
              wxDC & dc,
              const wxRect & rect,
              int row,
              int col,
              bool isSelected);
    wxGridCellRenderer* Clone() const;
private:
    wxString m_nullLabel;
    bool m_triState;
};

#endif /* LGRIDCHECKRENDERER_H */

