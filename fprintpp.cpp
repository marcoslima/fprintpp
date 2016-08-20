#include "fprintpp.h"

using namespace std;
namespace fprintpp
{

dev_t fnDevOpen(CFpDscDevs dsc_devs, size_t nIndex, void *)
{
    return fp_dev_open(dsc_devs[nIndex]);
}

void fnDevClose(dev_t dev)
{
    fp_dev_close(dev);
}

CFpDscDevsPtr CFPrint::discoverDevices()
{
    return CFpDscDevsPtr(new CFpDscDevs);
}

CFpDevice::CFpDevice(CFpDscDevs dsc_devs, size_t nIndex)
    : CCountedBody(dsc_devs, nIndex, nullptr)
{
}

dsc_devs_t fnDiscoverDevices(void *, void *, void *)
{
    return fp_discover_devs();
}

CFpDscDevs::CFpDscDevs()
    :  CCountedBody(nullptr, nullptr, nullptr)
{
    struct fp_dscv_dev** pCurrent = *this;

    // Obtemos cada um dos descobertos:
    while(*pCurrent != nullptr)
    {
        _vec_dscdevs.push_back(CFpDscDev(*pCurrent));
        pCurrent++;
    }
}


}// namespace fprintpp
