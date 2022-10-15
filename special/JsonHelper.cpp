/* 
 * File:   JsonHelper.cpp
 * Author: SET - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright SET, M. D. - © 2014
 * 
 * Created on December 16, 2019, 8:25 PM
 */

#include "JsonHelper.h"
#include <wx/jsonreader.h>

JsonHelper::JsonHelper()
{
}

JsonHelper::~JsonHelper()
{
}

wxString JsonHelper::GetIntent(const wxString& jsonData)
{
    if (jsonData.IsEmpty())
        return wxEmptyString;
    
    wxJSONReader reader(wxJSONREADER_STRICT);
    wxJSONValue root;
    uint nbErr = reader.Parse(jsonData, &root);
    if (nbErr > 0)
    {
        const wxArrayString& errors = reader.GetErrors();
        for (int i = 0; i < nbErr; i++)
        {
            wxASSERT_MSG(nbErr == 0, errors[i]);
        }
        return wxEmptyString;
    }
    if (!root.IsArray())
    {
        wxASSERT_MSG(root.IsArray(), _("JSON data is not an array"));
        return wxEmptyString;
    }
    for (uint row = 0; row < root.Size(); row++)
    {
        if (root[row]["Preferred"].IsValid()
                && !root[row]["Preferred"].AsString().IsEmpty()
                && root[row]["Preferred"].AsInt() != 0)
        {
            return root[row]["Intent"].AsString();
        }
    }
    return wxEmptyString;
}
