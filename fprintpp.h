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
class CFpDriver
{
private:
    using fp_driver_t = struct fp_driver*;
    fp_driver_t _fp_driver;

public:
    CFpDriver(fp_driver_t drv)
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
using vec_fpdevice_t = std::vector<CFpDriver>;

/** \class CFpDscDev
 * \brief Discovered device
 */
class CFpDscDev
{
private:
    std::shared_ptr<fp_dscv_dev> _dsc_dev;

public:
    /// Gets the driver for a discovered device.
    struct fp_driver * 	get_driver()
    { return fp_dscv_dev_get_driver(_dsc_dev.get());}

    /// Gets the devtype for a discovered device.
    uint32_t 	fp_dscv_dev_get_devtype (struct fp_dscv_dev *dev);

    /// Determines if a specific stored print appears to be compatible with a discovered device.
    int 	fp_dscv_dev_supports_print_data (struct fp_dscv_dev *dev, struct fp_print_data *data);

    /// Determines if a specific discovered print appears to be compatible with a discovered device.
    int 	fp_dscv_dev_supports_dscv_print (struct fp_dscv_dev *dev, struct fp_dscv_print *data);

    /// Searches a list of discovered devices for a device that appears to be compatible with a stored print.
    struct fp_dscv_dev * 	fp_dscv_dev_for_print_data (struct fp_dscv_dev **devs, struct fp_print_data *data);

    /// Searches a list of discovered devices for a device that appears to be compatible with a discovered print.
    struct fp_dscv_dev * 	fp_dscv_dev_for_dscv_print (struct fp_dscv_dev **devs, struct fp_dscv_print *print);


};


/** \class CFpDevice
 * \brief Classe que encapsula struct fp_dev
 */
class CFpDevice
{
private:
    std::shared_ptr<fp_dev> _pDev;

public:
    CFpDevice(struct fp_dev* pDev);

    /// Gets the number of enroll stages required to enroll a fingerprint with the device.
    int 	get_nr_enroll_stages()
    { return fp_dev_get_nr_enroll_stages(_pDev.get());}

    /// Gets the devtype for a device.
    uint32_t 	get_devtype()
    { return fp_dev_get_devtype(_pDev.get());}

    /// Determines if a stored print is compatible with a certain device.
    int 	supports_print_data (struct fp_print_data *data)
    { return fp_dev_supports_print_data(_pDev.get(), data);}

    /// Determines if a discovered print appears to be compatible with a certain device.
    int 	supports_dscv_print(struct fp_dscv_print *data)
    { return fp_dev_supports_dscv_print(_pDev.get(), data);}

    /// Determines if a device has imaging capabilities.
    int 	supports_imaging()
    { return fp_dev_supports_imaging(_pDev.get());}

    /// Determines if a device is capable of identification through fp_identify_finger() and similar.
    int 	supports_identification()
    { return fp_dev_supports_identification(_pDev.get());}

    /// Captures an image from a device.
    int 	img_capture(int unconditional, struct fp_img **image)
    { return fp_dev_img_capture (_pDev.get(), unconditional, image);}

    /// Gets the expected width of images that will be captured from the device.
    int 	get_img_width()
    { return fp_dev_get_img_width(_pDev.get());}

    /// Gets the expected height of images that will be captured from the device.
    int 	get_img_height()
    { return fp_dev_get_img_height(_pDev.get());}

    /// Performs an enroll stage.
    int 	enroll_finger_img(struct fp_print_data **print_data, struct fp_img **img)
    { return fp_enroll_finger_img(_pDev.get(), print_data, img);}

    /// Performs a new scan and verify it against a previously enrolled print.
    int 	verify_finger_img(struct fp_print_data *enrolled_print, struct fp_img **img)
    { return fp_verify_finger_img(_pDev.get(), enrolled_print, img);}

    /// Performs a new scan and attempts to identify the scanned finger against a collection of previously enrolled fingerprints.
    int 	identify_finger_img(struct fp_print_data **print_gallery, size_t *match_offset, struct fp_img **img)
    { return fp_identify_finger_img(_pDev.get(), print_gallery, match_offset, img);}

    /// Performs an enroll stage.
    int 	enroll_finger(struct fp_print_data **print_data)
    { return fp_enroll_finger(_pDev.get(), print_data);}

    /// Performs a new scan and verify it against a previously enrolled print.
    int 	verify_finger(struct fp_print_data *enrolled_print)
    { return fp_verify_finger(_pDev.get(), enrolled_print);}


    /// Performs a new scan and attempts to identify the scanned finger against a collection of previously enrolled fingerprints.
    int 	identify_finger(struct fp_print_data **print_gallery, size_t *match_offset)
    { return fp_identify_finger(_pDev.get(), print_gallery, match_offset);}
};

using CFpDevices = std::vector<CFpDevice>;

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
    CFpDevices discoverDevices(void);

};


#endif // FPRINTPP_H
