#include "fprintpp.h"

using namespace std;
namespace fprintpp
{

dev_t fnDevOpen(CFpDscDev dsc_dev, void*, void*)
{
    return fp_dev_open(dsc_dev);
}

void fnDevClose(dev_t dev)
{
    fp_dev_close(dev);
}

CFpDscDevsPtr CFPrint::discoverDevices()
{
    return CFpDscDevsPtr(new CFpDscDevs);
}

CFpDevice::CFpDevice(CFpDscDev dsc_dev)
    : CCountedBody(dsc_dev, nullptr, nullptr)
{
}

CFpImage CFpDevice::img_capture(int unconditional, int &result)
{
    image_t hImg;
    result = fp_dev_img_capture (*this, unconditional, &hImg);
    return CFpImage(hImg);
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

CFpDevice CFpDscDevs::get_device(size_t nIndex)
{
    return CFpDevice(_vec_dscdevs[nIndex]);
}

CFpImage::operator QImage()
{
    if(!ok())
        return QImage();

    int cx = width();
    int cy = height();

    return QImage(data(), cx,cy,QImage::Format_Grayscale8);
}

image_t fn_get_image(dev_t dev, int unconditional, void*)
{
    image_t img;
    fp_dev_img_capture(dev, unconditional, &img);
    return img;
}





























}// namespace fprintpp
