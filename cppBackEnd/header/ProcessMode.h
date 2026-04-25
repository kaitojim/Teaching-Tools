#pragma once

#include <concepts>
#include <string>
#include <stdexcept>

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

class ProcessMode
{
public:
    void setProcessId(optionId id) {m_processId = id;}

    template <Arithmetic Tp>
    requires std::is_arithmetic_v<Tp>
    void setProcessValue(Tp value)  {*static_cast<Tp*>(m_processValue) = value;}
    void setProcessValue(std::string& value)
    {
        char* pBuffer {static_cast<char*> (m_processValue)};
        for (size_t i = 0; i < value.size(); i++)
        {
            *pBuffer = value[i];
            pBuffer++;
        }
        *pBuffer = '\0';
    }

    optionId getProcessId () {return m_processId;}

    template <Arithmetic returnTp>
    returnTp getProcessValue() {return *static_cast<returnTp*>(m_processValue);}
    std::string getProcessValue()
    {
        int i {};
        std::string temp {};
        char* cTemp {(static_cast<char*> (m_processValue))};
        while (*cTemp != '\0')
        {
            temp.push_back(*cTemp);
            cTemp++;
        }
        
        if (temp.empty())
        {
            throw std::runtime_error("String is empty at class 'ProcessMode'");
        }
        
        return temp;
    }

    template <Arithmetic Tp>
    bool processModeInit(Tp value) 
    {
        Tp* pBuffer = new Tp;
        if (pBuffer == nullptr)
        {
            return false;
        }
        m_processValue = (void*) pBuffer;
        return true;
    }

    bool processModeInit(const std::string& value) 
    {
        auto size {value.size()};
        char* pBuffer = new char[value.size() + 1];
        if (pBuffer == nullptr)
        {
            return false;
        }
        for (size_t i = 0; i < value.size() + 1; i++)
        {
            pBuffer[i] = 0;
        }
        
        m_processValue = (void*) pBuffer;
        return true;
    }

    ProcessMode() {}
    ProcessMode(const ProcessMode& other) 
    {
        this->m_processId = other.m_processId;
        *(char*)(this->m_processValue) = *(char*)other.m_processValue;
    }

    ProcessMode& operator=(const ProcessMode& source)
    {
        this->m_processId = source.m_processId;
        *(char*)(this->m_processValue) = *(char*)source.m_processValue;
        return *this;
    }

    ~ProcessMode() 
    {
        if (m_processValue == nullptr)
        {
            return;
        }
        delete[] static_cast<char*> (m_processValue);
    }
private:
    optionId m_processId {};
    void* m_processValue;
};
