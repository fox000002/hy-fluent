#include "udf.h"

//FLUENT_EXPORT char* CX_Interpret_String_To_String(char *);

DEFINE_ON_DEMAND(hy_test_fluent_pointer)
{
    Pointer p;
    //char buf[1024];
    //char *str;
    int i;

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

    /*                */
    if (root_domain)
    {
        CX_Message("===========================\n");
        CX_Message("Root Domain \n");
        CX_Message("id: %d \n", root_domain->id);
        CX_Message("index: %d \n", root_domain->index);
        CX_Message("level: %d\n", root_domain->level);
        CX_Message("sub_domains: %d\n", root_domain->n_domains);
        CX_Message("name: %s\n", root_domain->name);
        CX_Message("material name: %s\n", root_domain->material_name);
        CX_Message("***res:\n");
        for (i=0; i<MAX_EQNS && root_domain->res[i]; ++i)
            CX_Message("equ %d  -- %s : %f\n", i, root_domain->equation_labels[i], *root_domain->res[i]);
        CX_Message("===========================\n");

        if (root_domain->f)
        {
            CX_Message("id: %d \n", root_domain->f->id);
            CX_Message("flag: %d \n", root_domain->f->flags);
            CX_Message("name: %s\n", root_domain->f->name);
            CX_Message("element_type: %d\n", root_domain->f->element_type);
        }

    }


}
