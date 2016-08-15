#include "fprintpp.h"

using namespace std;

CFpDevices CFPrint::discoverDevices()
{
    struct fp_dscv_dev** pCurrent, **pDevs;
    pDevs = pCurrent = fp_discover_devs();

    // Obtemos cada um dos descobertos:
    CFpDevices devs;
    while(*pCurrent != nullptr)
    {
        devs.push_back(CFpDevice(pCurrent));
        pCurrent++;
    }

    // Liberamos:
    fp_dscv_devs_free(pDevs);

    return target.size();
}

CFpDevice::CFpDevice(fp_dev *pDev)
    : _pDev(pDev, )
{
}
