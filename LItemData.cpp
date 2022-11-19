/* 
 * File:   ltItemData.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 24 mai 2014, 20:26
 */

#include "LItemData.h"

LItemData::LItemData(const long newData)
{
    m_data = newData;
}

LItemData::LItemData(const wxString newData)
{
    m_data = newData;
}

LItemData::LItemData(const wxAny newData)
{
    m_data = newData;
}

LItemData::~LItemData()
{
}

long LItemData::GetLong()
{
    long lVal;
    m_data.GetAs(&lVal);
    return lVal;
}

wxString LItemData::GetString()
{
    wxString sVal;
    m_data.GetAs(&sVal);
    return sVal;
}
