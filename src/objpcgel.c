/****************************************************************************
**
*W  objpcgel.c                  GAP source                       Frank Celler
**
**
*Y  Copyright (C)  1996,  Lehrstuhl D für Mathematik,  RWTH Aachen,  Germany
*Y  (C) 1998 School Math and Comp. Sci., University of St Andrews, Scotland
*Y  Copyright (C) 2002 The GAP Group
*/

#include "objpcgel.h"

#include "bool.h"
#include "gvars.h"
#include "lists.h"
#include "modules.h"
#include "objscoll.h"
#include "plist.h"


/****************************************************************************
**
*F * * * * * * * * * * * * * * * pc word aspect * * * * * * * * * * * * * * *
*/

/****************************************************************************
**
*F  FuncNBitsPcWord_Comm( <self>, <left>, <right> )
*/
Obj FuncNBitsPcWord_Comm ( Obj self, Obj left, Obj right )
{
    return FuncFinPowConjCol_ReducedComm(
        self, COLLECTOR_PCWORD(left), left, right );
}


/****************************************************************************
**
*F  FuncNBitsPcWord_Conjugate( <self>, <left>, <right> )
*/
Obj FuncNBitsPcWord_Conjugate ( Obj self, Obj left, Obj right )
{
    left = FuncFinPowConjCol_ReducedProduct(
                self, COLLECTOR_PCWORD(left), left, right );
    return FuncFinPowConjCol_ReducedLeftQuotient(
                self, COLLECTOR_PCWORD(left), right, left );
}


/****************************************************************************
**
*F  FuncNBitsPcWord_LeftQuotient( <self>, <left>, <right> )
*/
Obj FuncNBitsPcWord_LeftQuotient ( Obj self, Obj left, Obj right )
{
    return FuncFinPowConjCol_ReducedLeftQuotient(
        self, COLLECTOR_PCWORD(left), left, right );
}


/****************************************************************************
**
*F  FuncNBitsPcWord_PowerSmallInt( <self>, <left>, <right> )
*/
Obj FuncNBitsPcWord_PowerSmallInt ( Obj self, Obj left, Obj right )
{
    return FuncFinPowConjCol_ReducedPowerSmallInt(
        self, COLLECTOR_PCWORD(left), left, right );
}


/****************************************************************************
**
*F  FuncNBitsPcWord_Product( <self>, <left>, <right> )
*/
Obj FuncNBitsPcWord_Product ( Obj self, Obj left, Obj right )
{
    return FuncFinPowConjCol_ReducedProduct(
        self, COLLECTOR_PCWORD(left), left, right );
}


/****************************************************************************
**
*F  FuncNBitsPcWord_Quotient( <self>, <left>, <right> )
*/
Obj FuncNBitsPcWord_Quotient ( Obj self, Obj left, Obj right )
{
    return FuncFinPowConjCol_ReducedQuotient(
        self, COLLECTOR_PCWORD(left), left, right );
}


/****************************************************************************
**
*F * * * * * * * * * * * * * * free word aspect * * * * * * * * * * * * * * *
*/

/****************************************************************************
**
*F  Func8Bits_DepthOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func8Bits_DepthOfPcElement ( Obj self, Obj pcgs, Obj w )
{
    Int         ebits;          /* number of bits in the exponent          */

    /* if the pc element is the identity we have to ask the pcgs           */
    if ( NPAIRS_WORD(w) == 0 )
        return INTOBJ_INT( LEN_LIST(pcgs) + 1 );

    /* otherwise it is the generators number of the first syllable         */
    else {
        ebits = EBITS_WORD(w);
        return INTOBJ_INT(((((UInt1*)DATA_WORD(w))[0]) >> ebits)+1);
    }
}


/****************************************************************************
**
*F  Func8Bits_ExponentOfPcElement( <self>, <pcgs>, <w>, <pos> )
*/
Obj Func8Bits_ExponentOfPcElement ( Obj self, Obj pcgs, Obj w, Obj pos )
{
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt        ebits;          /* number of exponent bits                 */
    UInt        npos;           /* the wanted generator number             */
    UInt        num;            /* number of syllables in <w>              */
    UInt1 *     ptr;            /* pointer to the syllables of <w>         */
    UInt        i;              /* loop                                    */
    UInt        gen;            /* current generator number                */

    /* all exponents are zero if the pc element if the identity            */
    num = NPAIRS_WORD(w);
    if ( num == 0 )
        return INTOBJ_INT(0);

    /* otherwise find the syllable belonging to <exp>                      */
    else {
        ebits = EBITS_WORD(w);
        exps  = 1UL << (ebits-1);
        expm  = exps - 1;
        npos  = INT_INTOBJ(pos);
        ptr   = ((UInt1*)DATA_WORD(w));
        for ( i = 1;  i <= num;  i++, ptr++ ) {
            gen = ((*ptr) >> ebits) + 1;
            if ( gen == npos ) {
                if ( (*ptr) & exps )
                    return INTOBJ_INT(((*ptr)&expm)-exps);
                else
                    return INTOBJ_INT((*ptr)&expm);
            }
            if ( npos < gen )
                return INTOBJ_INT(0);
        }
        return INTOBJ_INT(0);
    }
}


/****************************************************************************
**
*F  Func8Bits_LeadingExponentOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func8Bits_LeadingExponentOfPcElement ( Obj self, Obj pcgs, Obj w )
{
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt1       p;              /* first syllable                          */

    /* the leading exponent is zero iff the pc element if the identity     */
    if ( NPAIRS_WORD(w) == 0 )
        return Fail;

    /* otherwise it is the exponent of the first syllable                  */
    else {
        exps = 1UL << (EBITS_WORD(w)-1);
        expm = exps - 1;
        p = ((UInt1*)DATA_WORD(w))[0];
        if ( p & exps )
            return INTOBJ_INT((p&expm)-exps);
        else
            return INTOBJ_INT(p&expm);
    }
}

/****************************************************************************
**
*F  Func8Bits_ExponentsOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func8Bits_ExponentsOfPcElement ( Obj self, Obj pcgs, Obj w)
{
    UInt	len;		/* length of pcgs */
    Obj		el;		/* exponents list */
    UInt        le;
    UInt	indx;
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt        ebits;          /* number of exponent bits                 */
    UInt        num;            /* number of syllables in <w>              */
    UInt1 *     ptr;            /* pointer to the syllables of <w>         */
    UInt        i,j;            /* loop                                    */
    UInt        gen;            /* current generator number                */

    len=LEN_LIST(pcgs);
    el=NEW_PLIST(T_PLIST_CYC,len);
    SET_LEN_PLIST(el,len);

    /* Check if the exponent vector is the empty list. */
    if( len == 0 ) { RetypeBag( el, T_PLIST_EMPTY ); return el; }

    indx=1; /* current index in el we assign to */
    num = NPAIRS_WORD(w);

    le=1; /* last exponent which has been assigned+1 */

    ebits = EBITS_WORD(w);
    exps  = 1UL << (ebits-1);
    expm  = exps - 1;

    ptr   = ((UInt1*)DATA_WORD(w));
    for ( i = 1;  i <= num;  i++, ptr++ ) {
      gen = ((*ptr) >> ebits) + 1;
      for (j=le; j< gen;j++) {
        /* zero out intermediate entries */
	SET_ELM_PLIST(el,indx,INTOBJ_INT(0));
	indx++;
      }

      if ( (*ptr) & exps )
	  SET_ELM_PLIST(el,indx,INTOBJ_INT(((*ptr)&expm)-exps));
      else
	  SET_ELM_PLIST(el,indx,INTOBJ_INT((*ptr)&expm));
      indx++;
      le=gen+1;
    }

    /* zeroes at the end */
    for (j=le; j<=len;j++) {
      /* zero out  */
      SET_ELM_PLIST(el,indx,INTOBJ_INT(0));
      indx++;
    }

    CHANGED_BAG(el);
    return el;
}


/****************************************************************************
**
*F  Func16Bits_DepthOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func16Bits_DepthOfPcElement ( Obj self, Obj pcgs, Obj w )
{
    Int         ebits;          /* number of bits in the exponent          */

    /* if the pc element is the identity we have to ask the pcgs           */
    if ( NPAIRS_WORD(w) == 0 )
        return INTOBJ_INT( LEN_LIST(pcgs) + 1 );

    /* otherwise it is the generators number of the first syllable         */
    else {
        ebits = EBITS_WORD(w);
        return INTOBJ_INT(((((UInt2*)DATA_WORD(w))[0]) >> ebits)+1);
    }
}


/****************************************************************************
**
*F  Func16Bits_ExponentOfPcElement( <self>, <pcgs>, <w>, <pos> )
*/
Obj Func16Bits_ExponentOfPcElement ( Obj self, Obj pcgs, Obj w, Obj pos )
{
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt        ebits;          /* number of exponent bits                 */
    UInt        npos;           /* the wanted generator number             */
    UInt        num;            /* number of syllables in <w>              */
    UInt2 *     ptr;            /* pointer to the syllables of <w>         */
    UInt        i;              /* loop                                    */
    UInt        gen;            /* current generator number                */

    /* all exponents are zero if the pc element if the identity            */
    num = NPAIRS_WORD(w);
    if ( num == 0 )
        return INTOBJ_INT(0);

    /* otherwise find the syllable belonging to <exp>                      */
    else {
        ebits = EBITS_WORD(w);
        exps  = 1UL << (ebits-1);
        expm  = exps - 1;
        npos  = INT_INTOBJ(pos);
        ptr   = ((UInt2*)DATA_WORD(w));
        for ( i = 1;  i <= num;  i++, ptr++ ) {
            gen = ((*ptr) >> ebits) + 1;
            if ( gen == npos ) {
                if ( (*ptr) & exps )
                    return INTOBJ_INT(((*ptr)&expm)-exps);
                else
                    return INTOBJ_INT((*ptr)&expm);
            }
            if ( npos < gen )
                return INTOBJ_INT(0);
        }
        return INTOBJ_INT(0);
    }
}


/****************************************************************************
**
*F  Func16Bits_LeadingExponentOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func16Bits_LeadingExponentOfPcElement ( Obj self, Obj pcgs, Obj w )
{
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt2       p;              /* first syllable                          */

    /* the leading exponent is zero iff the pc element if the identity     */
    if ( NPAIRS_WORD(w) == 0 )
        return Fail;

    /* otherwise it is the exponent of the first syllable                  */
    else {
        exps = 1UL << (EBITS_WORD(w)-1);
        expm = exps - 1;
        p = ((UInt2*)DATA_WORD(w))[0];
        if ( p & exps )
            return INTOBJ_INT((p&expm)-exps);
        else
            return INTOBJ_INT(p&expm);
    }
}

/****************************************************************************
**
*F  Func16Bits_ExponentsOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func16Bits_ExponentsOfPcElement ( Obj self, Obj pcgs, Obj w)
{
    UInt	len;		/* length of pcgs */
    Obj		el;		/* exponents list */
    UInt        le;
    UInt	indx;
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt        ebits;          /* number of exponent bits                 */
    UInt        num;            /* number of syllables in <w>              */
    UInt2 *     ptr;            /* pointer to the syllables of <w>         */
    UInt        i,j;            /* loop                                    */
    UInt        gen;            /* current generator number                */

    len=LEN_LIST(pcgs);
    el=NEW_PLIST(T_PLIST_CYC,len);
    SET_LEN_PLIST(el,len);

    /* Check if the exponent vector is the empty list. */
    if( len == 0 ) { RetypeBag( el, T_PLIST_EMPTY ); return el; }

    indx=1; /* current index in el we assign to */
    num = NPAIRS_WORD(w);

    le=1; /* last exponent which has been assigned+1 */

    ebits = EBITS_WORD(w);
    exps  = 1UL << (ebits-1);
    expm  = exps - 1;

    ptr   = ((UInt2*)DATA_WORD(w));
    for ( i = 1;  i <= num;  i++, ptr++ ) {
      gen = ((*ptr) >> ebits) + 1;
      for (j=le; j< gen;j++) {
        /* zero out intermediate entries */
	SET_ELM_PLIST(el,indx,INTOBJ_INT(0));
	indx++;
      }

      if ( (*ptr) & exps )
	  SET_ELM_PLIST(el,indx,INTOBJ_INT(((*ptr)&expm)-exps));
      else
	  SET_ELM_PLIST(el,indx,INTOBJ_INT((*ptr)&expm));
      indx++;
      le=gen+1;
    }

    /* zeroes at the end */
    for (j=le; j<=len;j++) {
      /* zero out  */
      SET_ELM_PLIST(el,indx,INTOBJ_INT(0));
      indx++;
    }

    CHANGED_BAG(el);
    return el;
}


/****************************************************************************
**
*F  Func32Bits_DepthOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func32Bits_DepthOfPcElement ( Obj self, Obj pcgs, Obj w )
{
    Int         ebits;          /* number of bits in the exponent          */

    /* if the pc element is the identity we have to ask the pcgs           */
    if ( NPAIRS_WORD(w) == 0 )
        return INTOBJ_INT( LEN_LIST(pcgs) + 1 );

    /* otherwise it is the generators number of the first syllable         */
    else {
        ebits = EBITS_WORD(w);
        return INTOBJ_INT(((((UInt4*)DATA_WORD(w))[0]) >> ebits)+1);
    }
}


/****************************************************************************
**
*F  Func32Bits_ExponentOfPcElement( <self>, <pcgs>, <w>, <pos> )
*/
Obj Func32Bits_ExponentOfPcElement ( Obj self, Obj pcgs, Obj w, Obj pos )
{
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt        ebits;          /* number of exponent bits                 */
    UInt        npos;           /* the wanted generator number             */
    UInt        num;            /* number of syllables in <w>              */
    UInt4 *     ptr;            /* pointer to the syllables of <w>         */
    UInt        i;              /* loop                                    */
    UInt        gen;            /* current generator number                */

    /* all exponents are zero if the pc element if the identity            */
    num = NPAIRS_WORD(w);
    if ( num == 0 )
        return INTOBJ_INT(0);

    /* otherwise find the syllable belonging to <exp>                      */
    else {
        ebits = EBITS_WORD(w);
        exps  = 1UL << (ebits-1);
        expm  = exps - 1;
        npos  = INT_INTOBJ(pos);
        ptr   = ((UInt4*)DATA_WORD(w));
        for ( i = 1;  i <= num;  i++, ptr++ ) {
            gen = ((*ptr) >> ebits) + 1;
            if ( gen == npos ) {
                if ( (*ptr) & exps )
                    return INTOBJ_INT(((*ptr)&expm)-exps);
                else
                    return INTOBJ_INT((*ptr)&expm);
            }
            if ( npos < gen )
                return INTOBJ_INT(0);
        }
        return INTOBJ_INT(0);
    }
}


/****************************************************************************
**
*F  Func32Bits_LeadingExponentOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func32Bits_LeadingExponentOfPcElement ( Obj self, Obj pcgs, Obj w )
{
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt4       p;              /* first syllable                          */

    /* the leading exponent is zero iff the pc element if the identity     */
    if ( NPAIRS_WORD(w) == 0 )
        return Fail;

    /* otherwise it is the exponent of the first syllable                  */
    else {
        exps = 1UL << (EBITS_WORD(w)-1);
        expm = exps - 1;
        p = ((UInt4*)DATA_WORD(w))[0];
        if ( p & exps )
            return INTOBJ_INT((p&expm)-exps);
        else
            return INTOBJ_INT(p&expm);
    }
}

/****************************************************************************
**
*F  Func32Bits_ExponentsOfPcElement( <self>, <pcgs>, <w> )
*/
Obj Func32Bits_ExponentsOfPcElement ( Obj self, Obj pcgs, Obj w)
{
    UInt	len;		/* length of pcgs */
    Obj		el;		/* exponents list */
    UInt        le;
    UInt	indx;
    UInt        expm;           /* signed exponent mask                    */
    UInt        exps;           /* sign exponent mask                      */
    UInt        ebits;          /* number of exponent bits                 */
    UInt        num;            /* number of syllables in <w>              */
    UInt4 *     ptr;            /* pointer to the syllables of <w>         */
    UInt        i,j;            /* loop                                    */
    UInt        gen;            /* current generator number                */

    len=LEN_LIST(pcgs);
    el=NEW_PLIST(T_PLIST_CYC,len);
    SET_LEN_PLIST(el,len);

    /* Check if the exponent vector is the empty list. */
    if( len == 0 ) { RetypeBag( el, T_PLIST_EMPTY ); return el; }

    indx=1; /* current index in el we assign to */
    num = NPAIRS_WORD(w);

    le=1; /* last exponent which has been assigned+1 */

    ebits = EBITS_WORD(w);
    exps  = 1UL << (ebits-1);
    expm  = exps - 1;

    ptr   = ((UInt4*)DATA_WORD(w));
    for ( i = 1;  i <= num;  i++, ptr++ ) {
      gen = ((*ptr) >> ebits) + 1;
      for (j=le; j< gen;j++) {
        /* zero out intermediate entries */
	SET_ELM_PLIST(el,indx,INTOBJ_INT(0));
	indx++;
      }

      if ( (*ptr) & exps )
	  SET_ELM_PLIST(el,indx,INTOBJ_INT(((*ptr)&expm)-exps));
      else
	  SET_ELM_PLIST(el,indx,INTOBJ_INT((*ptr)&expm));
      indx++;
      le=gen+1;
    }

    /* zeroes at the end */
    for (j=le; j<=len;j++) {
      /* zero out  */
      SET_ELM_PLIST(el,indx,INTOBJ_INT(0));
      indx++;
    }

    CHANGED_BAG(el);
    return el;
}


/****************************************************************************
**
*F * * * * * * * * * * * * * initialize module * * * * * * * * * * * * * * *
*/


/****************************************************************************
**
*V  GVarFuncs . . . . . . . . . . . . . . . . . . list of functions to export
*/
static StructGVarFunc GVarFuncs [] = {

    GVAR_FUNC(NBitsPcWord_Comm, 2, "n_bits_pcword, n_bits_pcword"),
    GVAR_FUNC(NBitsPcWord_Conjugate, 2, "n_bits_pcword, n_bits_pcword"),
    GVAR_FUNC(NBitsPcWord_LeftQuotient, 2, "n_bits_pcword, n_bits_pcword"),
    GVAR_FUNC(NBitsPcWord_PowerSmallInt, 2, "n_bits_pcword, small_integer"),
    GVAR_FUNC(NBitsPcWord_Product, 2, "n_bits_pcword, n_bits_pcword"),
    GVAR_FUNC(NBitsPcWord_Quotient, 2, "n_bits_pcword, n_bits_pcword"),
    GVAR_FUNC(8Bits_DepthOfPcElement, 2, "8_bits_pcgs, 8_bits_pcword"),
    GVAR_FUNC(8Bits_ExponentOfPcElement, 3, "8_bits_pcgs, 8_bits_pcword, int"),
    GVAR_FUNC(8Bits_LeadingExponentOfPcElement, 2, "8_bits_pcgs, 8_bits_word"),
    GVAR_FUNC(8Bits_ExponentsOfPcElement, 2, "8_bits_pcgs, 8_bits_pcword"),
    GVAR_FUNC(16Bits_DepthOfPcElement, 2, "16_bits_pcgs, 16_bits_pcword"),
    GVAR_FUNC(16Bits_ExponentOfPcElement, 3, "16_bits_pcgs, 16_bits_pcword, int"),
    GVAR_FUNC(16Bits_LeadingExponentOfPcElement, 2, "16_bits_pcgs, 16_bits_word"),
    GVAR_FUNC(16Bits_ExponentsOfPcElement, 2, "16_bits_pcgs, 16_bits_pcword"),
    GVAR_FUNC(32Bits_DepthOfPcElement, 2, "32_bits_pcgs, 32_bits_pcword"),
    GVAR_FUNC(32Bits_ExponentOfPcElement, 3, "32_bits_pcgs, 32_bits_pcword, int"),
    GVAR_FUNC(32Bits_LeadingExponentOfPcElement, 2, "32_bits_pcgs, 32_bits_word"),
    GVAR_FUNC(32Bits_ExponentsOfPcElement, 2, "32_bits_pcgs, 32_bits_pcword"),
    { 0, 0, 0, 0, 0 }

};


/****************************************************************************
**
*F  InitKernel( <module> )  . . . . . . . . initialise kernel data structures
*/
static Int InitKernel (
    StructInitInfo *    module )
{
    /* init filters and functions                                          */
    InitHdlrFuncsFromTable( GVarFuncs );

    /* return success                                                      */
    return 0;
}


/****************************************************************************
**
*F  InitLibrary( <module> ) . . . . . . .  initialise library data structures
*/
static Int InitLibrary (
    StructInitInfo *    module )
{
    /* export position numbers 'PCWP_SOMETHING'                            */
    ExportAsConstantGVar(PCWP_FIRST_ENTRY);
    ExportAsConstantGVar(PCWP_NAMES);
    ExportAsConstantGVar(PCWP_COLLECTOR);
    ExportAsConstantGVar(PCWP_FIRST_FREE);

    /* init filters and functions                                          */
    InitGVarFuncsFromTable( GVarFuncs );

    /* return success                                                      */
    return 0;
}


/****************************************************************************
**
*F  InitInfoPcElements()  . . . . . . . . . . . . . . table of init functions
*/
static StructInitInfo module = {
    // init struct using C99 designated initializers; for a full list of
    // fields, please refer to the definition of StructInitInfo
    .type = MODULE_BUILTIN,
    .name = "objpcgel",
    .initKernel = InitKernel,
    .initLibrary = InitLibrary,
};

StructInitInfo * InitInfoPcElements ( void )
{
    return &module;
}
