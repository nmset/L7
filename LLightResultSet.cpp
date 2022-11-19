/* 
 * File:   LLightResultSet.cpp
 * Author: Saleem Edah-Tally - nmset@yandex.com
 * License : LGPL version 2.1
 * Copyright Saleem Edah-Tally, M. D. - © 2014
 * 
 * Created on 28 mai 2014, 14:41
 */

#include "LLightResultSet.h"

LLightResultSet::LLightResultSet()
{
    m_initialised = false;
    m_curSql = wxEmptyString;
    m_cursor = -1;
    m_rs = NULL;
    m_conn = NULL;
}

LLightResultSet::~LLightResultSet()
{
}

LLightResultSet::LLightResultSet(LConnection* newConnection)
{
    m_initialised = false;
    m_curSql = wxEmptyString;
    m_cursor = -1;
    m_rs = NULL;
    m_conn = newConnection;
}
