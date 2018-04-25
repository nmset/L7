/* 
 * File:   LGridCheckRenderer.cpp
 * Author: SET - nmset@netcourrier.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on 9 juin 2014, 21:33
 */

#include "LGridCheckRenderer.h"

LGridCheckRenderer::LGridCheckRenderer(bool isDualState, const wxString& newNullLabel)
: wxGridCellStringRenderer()
{
    m_triState = !isDualState;
    if (newNullLabel.IsEmpty())
    {
        m_nullLabel = _T("?");
    }
    else
    {
        m_nullLabel = newNullLabel;
    }
}

LGridCheckRenderer::~LGridCheckRenderer()
{
}

void LGridCheckRenderer::Draw(wxGrid & grid,
                              wxGridCellAttr & attr,
                              wxDC & dc,
                              const wxRect & rect,
                              int row,
                              int col,
                              bool isSelected)
{
    /* The interpretation of literal non/oui is not documented 
     * as it's for internal use.
     */
    wxString val = grid.GetCellValue(row, col);
    if (m_triState)
    {
        if (val.IsEmpty()
            || val == m_nullLabel)
        {
            grid.GetTable()->SetValue(row, col, m_nullLabel);
            wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
            return;
        }
        if (val == _T("0")
            || val.Lower() == _("no"))
        {
            grid.GetTable()->SetValue(row, col, _T("No"));
        }
        else
        {
            grid.GetTable()->SetValue(row, col, _T("Yes"));
        }
    }
    else
    {
        if (val == _T("0")
            || val.IsEmpty()
            || val.Lower() == _("no"))
        {
            grid.GetTable()->SetValue(row, col, _T("No"));
        }
        else
        {
            grid.GetTable()->SetValue(row, col, _T("Yes"));
        }
    }
    wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
}

wxGridCellRenderer* LGridCheckRenderer::Clone() const
{
    return new LGridCheckRenderer(!m_triState, m_nullLabel);
}

