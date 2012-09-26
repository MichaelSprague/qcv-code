/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */


#ifndef __DBL2DPARAMEDITOR_H
#define __DBL2DPARAMEDITOR_H

/**
 * @file dblt2DParamEditor.h
 *
 * \class CDbl2DParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CDbl2DParameter.
 *
 * The class implements a simple Qt editor with two input boxes where
 * the user enters double values.
 */

/* INCLUDES */
#include "baseParamEditorWidget.h"

#include "dbl2DParam.h"

/* PROTOTYPES */

namespace QCV
{
    /* PROTOTYPES */
    class CDoubleEditor;

    class CDbl2DParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CDbl2DParameterEditor ( CDbl2DParameter *  f_parameter_p,
                                QWidget *          f_parent_p = NULL );

        virtual ~CDbl2DParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();

        /// Protected members
    protected:

        /// Stored value.
        CDoubleEditor *           m_qdsb_p[2];

        /// Associated Dbl2D parameter
        CDbl2DParameter *         m_parameter_p;

        /// Amount of decimals to consider.
        const static unsigned int m_maxDecimals_cui;
    };
}

#endif // __DBL2DPARAMEDITOR_H
