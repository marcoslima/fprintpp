#ifndef CCOUNTEDBODY_H
#define CCOUNTEDBODY_H
#include <iostream>

/** \brief Implementação do RAII.

    Usamos esta classe para implementar o RAII no gerenciamento do dispositivo Digital Persona (DP).
    A classe foi criada de forma mais genérica possível para viabilizar seu uso em outros casos.
    Futuramente ela deve sair daque e ganhar um contexto próprio (seus próprio .h).

    @param RT Resource Type
    @param NV Null Value
    @param P?T Tipo dos parâmetros da função de criação (? = 1 - 3)
    @param CF Create Function
    @param DF Deleter Function

    Exemplo de uso:

    Um dispositivo que e acessado atravez da funçao:

~~~~{.cpp}

        DPFPDD_DEV OpenDevice(char* szDeviceName, DPFPDD_PRIORITY pri);

~~~~

Uma vez aberto, deve-se fechá-lo com a função:

~~~~{.cpp}

        CloseDevice(DPFPDD_DEV dev).

~~~~

Criamos uma função para abertura do dispositivo compatível com a CCountedBody (3 parametros):

~~~~{.cpp}

        DPFPDD_DEV fnOpenDevice(char* szDeviceName, DPFPDD_PRIORITY pri, void* dummy)
        {
            return OpenDevice(szDeviceName, pri);
        }

~~~~

Criamos então a classe que será, automaticamente, RAII:

~~~~{.cpp}

        class CDevice : public CCountedBody<DPFPDD_DEV, NULL, char*, DPFPDD_PRIORITY, void*, fnOpenDevice, CloseDevice>
        {
        private:
            /// Quando a classe tiver variáveis, deve-se implementar a função swap (veja abaixo).
            std::string _sContext; //!< Exemplo de variável da classe derivada.

        public:
            //! Constructor: abre o dispositivo informado em szDeviceName
            CDevice(char* szDeviceName, DPFPDD_PRIORITY pri = DPFPDD_PRIORITY_COOPERATIVE)
                : CCountedBody(szDeviceName, pri, NULL)
            {
            }
        protected:
            virtual void swap(CDevice& s) throw()
            {
                CCountedBody::swap(s);
                std::swap(_sContext, s._sContext);
            }
        };
~~~~


*/


template<typename RT, RT NV, typename P1T, typename P2T, typename P3T, RT (*CF)(P1T, P2T, P3T), void(*DF)(RT)>
class CCountedBody
{
private:
    //! Classe que mantém use-count de um body
    class BodyRep
    {
        friend class CCountedBody<RT, NV, P1T, P2T, P3T, CF, DF>;
    private:
        RT _device;
        int _count;

    public:
        BodyRep(RT hDevice)
            : _device(hDevice)
            , _count(1)
        {}

        ~BodyRep()
        {
            DF(_device);
        }
    };
private:
    BodyRep* _pDev;

public:
    //! Ctor com parâmetros para criação do recurso
    CCountedBody(P1T p1, P2T p2, P3T p3)
        : _pDev(nullptr)
    {
        Open(p1,p2,p3);
    }

    //! Copy Constructor
    CCountedBody(const CCountedBody& s)
        : _pDev(s._pDev)
    {
        if(_pDev)
        {
            // Incrementamos o uso:
            _pDev->_count++;
        }
    }

    //! Ctor com objeto já criado:
    CCountedBody(RT hRes) : _pDev(new BodyRep(hRes)) {}

    //! Default constructor
    CCountedBody()
        : _pDev(nullptr)
    {
    }

    //! Abre o dispositivo. Em caso de falha, mantém o handle como nulo.
    bool Open(P1T p1, P2T p2, P3T p3)
    {
        // Se já há um body aberto conosco, liberamos:
        if(_pDev)
        {
            Close();
        }

        RT hDev = CF(p1, p2, p3);
        if(hDev != NV)
        {
            _pDev = new BodyRep(hDev);
            return true;
        }

        return false;
    }

    //! Fecha o body, se houver algum conosco.
    void Close(void) throw()
    {
        // Se o recurso não estiver com mais ningúem:
        if(_pDev && --_pDev->_count <= 0)
        {
            // Removemos
            delete _pDev;
        }

        // Estando ou não com alguém, não está mais conosco:
        _pDev = nullptr;
    }

    CCountedBody &operator=(const CCountedBody& s)
    {
        CCountedBody(s).swap(*this);

        return *this;
    }

    ~CCountedBody() throw()
    {
        Close();
    }

    /// Essa classe pode comportar-se como o próprio handle:
    /// Se o handle for usado para ser fechado, sem que seja chamado
    /// o destructor da classe, o destructor pode crashar ao tentar
    /// fechar um handle fechado. Resumo: não feche o handle externamente.
    operator RT()
    {
        return (_pDev)?(_pDev->_device):(NV);
    }

    /// Para saber se estamos apontando para
    /// um device de fato. Se o objeto for criado
    /// com o default constructor, ele não aponta para
    /// ninguém.
    bool ok(void)
    {
        return _pDev != nullptr;
    }

///////////////////////////////////
protected:
    virtual void swap(CCountedBody& s) throw()
    {
        std::swap(_pDev, s._pDev);
    }
};

#endif // CCOUNTEDBODY_H
