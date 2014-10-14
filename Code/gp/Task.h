#pragma once
#include <Foundation/Threading/TaskSystem.h>

using gpSimpleTaskFunction = void(*)();

/// \brief Same as gpTask but only accepts simple void() functions and no state.
class gpSimpleTask : public ezTask
{
public:
    gpSimpleTask(gpSimpleTaskFunction pFunction) : m_pFunction(pFunction)
    {
        EZ_ASSERT(m_pFunction != nullptr, "Invalid task function.");
    }

    virtual auto Execute() -> void final { (*m_pFunction)(); }

private:
    gpSimpleTaskFunction m_pFunction;
};

template<typename Type>
using gpTaskFunction = void(*)(Type&);

/// \brief Class template to wrap a function with a bit of state which gets executed by the task system.
template<typename UserDataType, typename FunctionType = gpTaskFunction<UserDataType>>
class gpTask : public ezTask
{
public:

    gpTask(FunctionType pFunction, UserDataType Data = UserDataType()) :
        m_pFunction(pFunction),
        m_UserData(Data)
    {
        EZ_ASSERT(m_pFunction != nullptr, "Invalid task function.");
    }

    virtual auto Execute() -> void final { (*m_pFunction)(m_UserData); }

    auto GetUserData()       ->       UserDataType& { return m_UserData; }
    auto GetUserData() const -> const UserDataType& { return m_UserData; }

private:
    FunctionType m_pFunction;
    UserDataType m_UserData;
};
