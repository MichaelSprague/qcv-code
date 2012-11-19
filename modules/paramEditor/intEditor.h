/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
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


#ifndef __INTEDITOR_H
#define __INTEDITOR_H

/**
 *******************************************************************************
 *
 * @file intEditor.h
 *
 * \class CIntEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CIntParameter.
 *
 * The class implements a simple Qt editor with one input box where
 * the user enters a signed integer value.
 *
 *******************************************************************************/

/* INCLUDES */
#include <QWidget>

/* PROTOTYPES */
class QLineEdit;

namespace QCV
{
    /* PROTOTYPES */

    class CIntEditor : public QWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CIntEditor ( int     f_value_i,
                     QWidget *  f_parent_p = NULL );
        
        virtual ~CIntEditor ();

        int getValue() const { return m_value_i; }
        
        bool   setValue( int f_value_i );
        
        virtual QSize sizeHint () const;

    protected slots:
        void lineEditChanged();

    signals:
        void valueChanged( int );
        
        /// Protected members
    protected:

        /// Stored value.
        QLineEdit *          m_qle_p;

        /// Int value
        int                  m_value_i;
    };
}

#endif // __INTEDITOR_H
