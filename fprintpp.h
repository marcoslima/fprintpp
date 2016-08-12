#ifndef FPRINTPP_H
#define FPRINTPP_H

/** \brief Classe singleton de acesso a biblioteca.
 *
*/

#include <fprint.h>

class CFPrint
{
public:
    static CFPrint& instance(void)
    {
        static CFPrint the_instance;
        return the_instance;
    }

    CFPrint(CFPrint const&) = delete;             // Copy construct
    CFPrint(CFPrint&&) = delete;                  // Move construct
    CFPrint& operator=(CFPrint const&) = delete;  // Copy assign
    CFPrint& operator=(CFPrint &&) = delete;      // Move assign

protected:
    CFPrint()
    {
        fp_init();
    }

    ~CFPrint()
    {
        fp_exit();
    }

public:

};


#endif // FPRINTPP_H
