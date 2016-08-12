#ifndef FPRINTPP_H
#define FPRINTPP_H

/** \brief Classe singleton de acesso a biblioteca.
 *
*/
#include <iostream>
#include <fprint.h>
#include <memory>
#include <vector>
#include <string>

// Device (baseado em struct fp_driver:
class CFpDevice
{
private:
    using fp_driver_t = struct fp_driver*;
    fp_driver_t _fp_driver;

public:
    CFpDevice(fp_driver_t drv)
    {
        _fp_driver = drv;
    }

    std::string getFullName(void)
    {
        if(_fp_driver == nullptr)
            return std::string();
        else
            return std::string(fp_driver_get_full_name(_fp_driver));
    }

    std::string getName(void)
    {
        if(_fp_driver == nullptr)
            return std::string();
        else
            return std::string(fp_driver_get_name(_fp_driver));
    }

    uint16_t getId(void)
    {
        if(_fp_driver == nullptr)
            return (uint16_t) -1;
        else
            return fp_driver_get_driver_id(_fp_driver);
    }
};

using vec_fpdevice_t = std::vector<CFpDevice>;


#if 0
struct fp_dscv_dev** pdevs = fp_discover_devs();
cout << "pdevs: " << pdevs << endl;
struct fp_dscv_dev** pCurrent = pdevs;
while(*pCurrent != nullptr)
{
    struct fp_driver* pDriver = fp_dscv_dev_get_driver(*pCurrent);
    pCurrent++;

    _vec_devices.push_back(string(fp_driver_get_full_name(pDriver)));
}
// Liberamos:
fp_dscv_devs_free(pdevs);
#endif


class CFPrint
{
public:
    size_t discoverDevices(vec_fpdevice_t& target);

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
