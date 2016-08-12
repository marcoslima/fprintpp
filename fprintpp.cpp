#include "fprintpp.h"

using namespace std;

size_t CFPrint::discoverDevices(vec_fpdevice_t& target)
{
    struct fp_dscv_dev** pCurrent, **pDevs;
    pDevs = pCurrent = fp_discover_devs();

    // Obtemos cada um dos descobertos:
    while(*pCurrent != nullptr)
    {
        target.push_back(CFpDevice(fp_dscv_dev_get_driver(*pCurrent)));
        pCurrent++;
    }

    // Liberamos:
    fp_dscv_devs_free(pDevs);

    return target.size();
}
