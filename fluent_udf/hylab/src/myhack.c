#include "udf.h"

//FLUENT_EXPORT char* CX_Interpret_String_To_String(char *);

DEFINE_ON_DEMAND(hy_test_fluent_pointer)
{
    Pointer p;
    //char buf[1024];
    //char *str;

    p = CX_Interpret_String("(newline)");

    CX_Message("Type: %d \n", p->type);

    //str = CX_Interpret_String_To_String("(+ 1 1)");
    //CX_Message(str);


    if (SYMBOLP(p))
    {
        CX_Message("Symbol\n");
    }
    //CX_Message(" %ld . %ld\n", FIXNM(CAR(p)), FIXNM(CDR(p)));
    switch (p->type)
    {
    case 0:
        CX_Message("Type NIL: %d \n", p->type);
        break;
    case 1:
        CX_Message("Type TRUE: %d \n", p->type);
        break;
    case 2:
        CX_Message("Type cons: %d \n", p->type);
        break;
    case 3:
        CX_Message("Type symbol: %d  %d\n", p->type, p->as.symbol.ncell);
        break;
    case 5:
        CX_Message("Type fixnum : %d -- %ld \n", p->type, FIXNM(p));
        break;
    default:
        CX_Message("Type unknown: %d \n", p->type);
    }
}
