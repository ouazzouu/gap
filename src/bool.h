/****************************************************************************
**
*W  bool.h                      GAP source                   Martin Schönert
**
**
*Y  Copyright (C)  1996,  Lehrstuhl D für Mathematik,  RWTH Aachen,  Germany
*Y  (C) 1998 School Math and Comp. Sci., University of St Andrews, Scotland
*Y  Copyright (C) 2002 The GAP Group
**
**  This file declares the functions for the boolean package.
*/

#ifndef GAP_BOOL_H
#define GAP_BOOL_H

#include "objects.h"

/****************************************************************************
**
*V  True  . . . . . . . . . . . . . . . . . . . . . . . . . . . .  true value
**
**   'True' is the value 'true'.
*/
extern Obj True;


/****************************************************************************
**
*V  False . . . . . . . . . . . . . . . . . . . . . . . . . . . . false value
**
**  'False' is the value 'false'.
*/
extern Obj False;


/****************************************************************************
**
*V  Fail  . . . . . . . . . . . . . . . . . . . . . . . . . . . .  fail value
**
**  'Fail' is the value 'fail'.
*/
extern Obj Fail;

/****************************************************************************
**
*V  Undefined  . . . . . . . . . . . . . . . . . . . . . . . undefined value
**
**  'Undefined' is a special object that is used in lieu of (Obj) 0 in places
**  where the kernel cannot handle a null reference easily. This object is
**  never exposed to GAP code and only used within the kernel.
*/
extern Obj Undefined;


/****************************************************************************
**
*F * * * * * * * * * * * * * initialize module * * * * * * * * * * * * * * *
*/

/****************************************************************************
**
*F  InitInfoBool()  . . . . . . . . . . . . . . . . . table of init functions
*/
StructInitInfo * InitInfoBool ( void );


#endif // GAP_BOOL_H
