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
#include "countedbody.h"

#ifdef QT_CORE_LIB
#include <QImage>
#endif


namespace fprintpp
{

/** *********************************************
 * \class CFpDriver
 *  Driver (baseado em struct fp_driver
 ************************************************/
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
using vec_fpdrivers_t = std::vector<CFpDriver>;


/** ***********************************************
 * \class CFpPrintData
 * Classe em torno de struct fp_print_data, usada
 * para conter os dados de uma impressão digital.
 **************************************************/

/** \typedef enum que especifica cada dedo de cada mão */
using finger_t = enum fp_finger;

class CFpPrintData
{
private:
    using print_data_t = struct fp_print_data*;
    print_data_t _print_data;

public:
    // TODO: Implementar leitura do user dir
    //int fp_print_data_load(struct fp_dev *dev, enum fp_finger finger, struct fp_print_data **data);

    // TODO: Implementar isso
    //int fp_print_data_from_dscv_print(struct fp_dscv_print *print, struct fp_print_data **data);

    // TODO: Implementar
    int fp_print_data_save(struct fp_print_data *data, enum fp_finger finger);
    int fp_print_data_delete(struct fp_dev *dev, enum fp_finger finger);
    void fp_print_data_free(struct fp_print_data *data);
    size_t fp_print_data_get_data(struct fp_print_data *data, unsigned char **ret);
    struct fp_print_data *fp_print_data_from_data(unsigned char *buf, size_t buflen);
    uint16_t fp_print_data_get_driver_id(struct fp_print_data *data);
    uint32_t fp_print_data_get_devtype(struct fp_print_data *data);
};


/** ************************************************
 * \class CFpDevice
 * \brief Classe que encapsula struct fp_dev
 ***************************************************/

using dev_t = fp_dev*;

class CFpDscDev;
class CFpImage;

dev_t fnDevOpen(CFpDscDev dsc_dev, void*, void*);
void fnDevClose(dev_t dev);

class CFpDevice : public CCountedBody<dev_t, nullptr, CFpDscDev, void*, void*, fnDevOpen, fnDevClose>
{
public:
    CFpDevice(CFpDscDev dsc_dev);

    /// Gets the number of enroll stages required to enroll a fingerprint with the device.
    int 	get_nr_enroll_stages()
    { return fp_dev_get_nr_enroll_stages(*this);}

    /// Gets the devtype for a device.
    uint32_t 	get_devtype()
    { return fp_dev_get_devtype(*this);}

    /// Determines if a stored print is compatible with a certain device.
    int 	supports_print_data (struct fp_print_data *data)
    { return fp_dev_supports_print_data(*this, data);}

    /// Determines if a discovered print appears to be compatible with a certain device.
    int 	supports_dscv_print(struct fp_dscv_print *data)
    { return fp_dev_supports_dscv_print(*this, data);}

    /// Determines if a device has imaging capabilities.
    int 	supports_imaging()
    { return fp_dev_supports_imaging(*this);}

    /// Determines if a device is capable of identification through fp_identify_finger() and similar.
    int 	supports_identification()
    { return fp_dev_supports_identification(*this);}

    /// Captures an image from a device.
    CFpImage img_capture(int unconditional, int& result);

    /// Gets the expected width of images that will be captured from the device.
    int 	get_img_width()
    { return fp_dev_get_img_width(*this);}

    /// Gets the expected height of images that will be captured from the device.
    int 	get_img_height()
    { return fp_dev_get_img_height(*this);}

    /// Performs an enroll stage.
    int 	enroll_finger_img(struct fp_print_data **print_data, struct fp_img **img)
    { return fp_enroll_finger_img(*this, print_data, img);}

    /// Performs a new scan and verify it against a previously enrolled print.
    int 	verify_finger_img(struct fp_print_data *enrolled_print, struct fp_img **img)
    { return fp_verify_finger_img(*this, enrolled_print, img);}

    /// Performs a new scan and attempts to identify the scanned finger against a collection of previously enrolled fingerprints.
    int 	identify_finger_img(struct fp_print_data **print_gallery, size_t *match_offset, struct fp_img **img)
    { return fp_identify_finger_img(*this, print_gallery, match_offset, img);}

    /// Performs an enroll stage.
    int 	enroll_finger(struct fp_print_data **print_data)
    { return fp_enroll_finger(*this, print_data);}

    /// Performs a new scan and verify it against a previously enrolled print.
    int 	verify_finger(struct fp_print_data *enrolled_print)
    { return fp_verify_finger(*this, enrolled_print);}


    /// Performs a new scan and attempts to identify the scanned finger against a collection of previously enrolled fingerprints.
    int 	identify_finger(struct fp_print_data **print_gallery, size_t *match_offset)
    { return fp_identify_finger(*this, print_gallery, match_offset);}
};

using CFpDevices = std::vector<CFpDevice>;

/** *******************************************************************
 * \class CFpImage
 * \brief Wrapper da struct fp_image*, uma imagem da libfprint (RAII)
 *
 **********************************************************************/
using image_t = fp_img*;

image_t fn_dummy_open(void*, void*, void*);

class CFpImage : public CCountedBody<image_t, nullptr, void*, void*, void*, fn_dummy_open, fp_img_free>
{
    /** ***************************************************
     * \class CFpMinutiae
     * \brief Wrapper para struct fp_minutia
     ******************************************************/
    class CFpMinutia : public struct fp_minutia
    {

    };

public:
    /** \brief ctor: inicialização com handle existente */
    CFpImage(image_t hImg) : CCountedBody(hImg){}

    /** \brief retorna a altura da imagem */
    int height(){return fp_img_get_height(*this);}

    /** \brief retorna a largura da imagem */
    int width(){return fp_img_get_width(*this);}

    /** \brief retorna os dados da imagem (grayscale) */
    unsigned char *data(){return fp_img_get_data(*this);}

    /** \brief Grava a imagem no arquivo especificado */
    int save_to_file(char *path){return fp_img_save_to_file(*this, path);}

    /** Standardizes an image by normalizing its orientation, colors, etc */
    void standardize(){ fp_img_standardize(*this);}

    /** Get a binarized form of a standardized scanned image. */
    CFpImage binarize(){ return CFpImage(fp_img_binarize(*this));}

    // TODO: Implementar depois da implementação de CFpMinutia
    //struct fp_minutia **fp_img_get_minutiae(struct fp_img *img, int *nr_minutiae);

#ifdef QT_CORE_LIB
    /** /brief Converte para QImage */
    operator QImage();
#endif
};



/** ****************************************************
 * \class CFpDscDev
 * \brief Discovered device (wrapping fp_dscv_dev*)
 *
 *******************************************************/
using dscv_dev_t = fp_dscv_dev*;
class CFpDscDev
{
public:
    friend class CFpDscDevs;
    friend dev_t fnDevOpen(CFpDscDev dsc_dev, void*, void*);
private:
    dscv_dev_t _dsc_dev;
    operator dscv_dev_t(){return _dsc_dev;}

public:
    CFpDscDev(dscv_dev_t dsc_dev) : _dsc_dev(dsc_dev){}
};
using vec_fpdscdevs_t = std::vector<CFpDscDev>;


/** ******************************************************************
 * \class CFpDscDevs
 * \brief Classe que contém dispositivos descobertos
 * Classe RAII para struct fp_dscv_dev** obtida de fp_discover_devs
 * Ao ser liberada a função fp_dscv_devs_free é usada.
 *********************************************************************/

using dsc_devs_t = struct fp_dscv_dev**;
dsc_devs_t fnDiscoverDevices(void*, void*, void*);

class CFpDscDevs : public CCountedBody<dsc_devs_t, nullptr, void*, void*, void*, fnDiscoverDevices, fp_dscv_devs_free>
{
public:
    friend dev_t fnDevOpen(CFpDscDevs dsc_dev, size_t, void *);

private:
    vec_fpdscdevs_t _vec_dscdevs;

public:
    CFpDscDevs();

    size_t size(void){return _vec_dscdevs.size();}
    bool   empty(void){return _vec_dscdevs.empty();}

    dscv_dev_t operator[](size_t nIndex){return _vec_dscdevs[nIndex];}

    /** Gets the driver for a discovered device. */
    CFpDriver get_driver(size_t nIndex)
    { return CFpDriver(fp_dscv_dev_get_driver(_vec_dscdevs[nIndex]));}

    /** Open and initialises a device (RAII) */
    CFpDevice get_device(size_t nIndex);

    /** Gets the devtype for a discovered device. */
    uint32_t 	get_devtype (size_t nIndex)
    { return fp_dscv_dev_get_devtype(_vec_dscdevs[nIndex]);}

    /** Determines if a specific stored print appears to be compatible with a discovered device. */
    int 	supports_print_data (size_t nIndex, struct fp_print_data *data)
    { return fp_dscv_dev_supports_print_data(_vec_dscdevs[nIndex], data);}

    /** Determines if a specific discovered print appears to be compatible with a discovered device. */
    int 	supports_dscv_print (size_t nIndex, struct fp_dscv_print *data)
    { return fp_dscv_dev_supports_dscv_print(_vec_dscdevs[nIndex], data);}
};



/** ************************************************************************************************
 * \class CFPrint
 * \brief Classe principal, singleton, pela qual fazemos todo o acesso à libfprint
 *
 * Para usar a biblioteca obtemos a lista de dispositivos do sistema usando a função
 * discoverDevices:
 *
 * ~~~~{.c}
 *
 * CFpDscDevsPtr devs = CFprint::instance().discoverDevices();
 *
 * ~~~~
 *
 * Obtemos o device de um dos dispositivos descobertos para usar:
 *
 * ~~~~{.c}
 *
 * CFpDevice dev = devs->get_device(0);
 *
 * ~~~~
 *
 * Com o dispositivo podemos obter uma imagem:
 *
 * ~~~~{.c}
 *
 * int res;
 *
 * CFpImage fpImg = dev.img_capture(0,res);
 *
 * if(0 != res)
 * {
 *     QMessageBox::information(this, "Mensagem", "Erro obtendo imagem");
 *     return;
 * }
 *
 * ~~~~
 *
 ***************************************************************************************************/
using CFpDscDevsPtr = std::shared_ptr<CFpDscDevs>;

class CFPrint
{
public:
    /** \brief Acesso à instância singleton da biblioteca */
    static CFPrint& instance(void)
    {
        static CFPrint the_instance;
        return the_instance;
    }

    CFPrint(CFPrint const&) = delete;             //!< Copy construct removida (Scott Meyers Singleton)
    CFPrint(CFPrint&&) = delete;                  //!< Move construct removida (Scott Meyers Singleton)
    CFPrint& operator=(CFPrint const&) = delete;  //!< Copy assign removida (Scott Meyers Singleton)
    CFPrint& operator=(CFPrint &&) = delete;      //!< Move assign removida (Scott Meyers Singleton)

protected:
    /** \brief ctor: inicializa a biblioteca com fp_init() */
    CFPrint()
    {
        fp_init();
    }

    /** \brief dtor: libera a biblioteca com fp_exit() */
    ~CFPrint()
    {
        fp_exit();
    }

public:

    /** \brief Descobre quais dispositivos estão instalados no sistema
     * \return shared_ptr com CFpDscDevs
     *
     * Exemplo:
     *
     * ~~~~{.c}
     *
     * CFpDscDevsPtr devs = CFPrint::instance().discoverDevices();
     * for(size_t i = 0; i < devs->size(); i++)
     * {
     *      cout << devs->get_driver(i).getFullName() << endl;
     * }
     *
     * ~~~~
     *
     */

    CFpDscDevsPtr discoverDevices(void);

    /** \brief Descobre os dispositivos instalados compativeis com o fmt especificado. */
    CFpDscDevsPtr devicesForPrint(struct fp_print_data *data);

    /** \brief Descobre os dispositivos instalados compatíveis com o dscv_print especificado. */
    CFpDscDevsPtr devicesForDscPrint(struct fp_dscv_print *print);
};

} // namespace fprintpp

#endif // FPRINTPP_H
