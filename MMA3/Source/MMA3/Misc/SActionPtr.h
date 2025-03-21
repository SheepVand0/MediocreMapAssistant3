// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define METHOD(Object, Method) SMethodPtr(Object, Method);

/**
 * 
 */
template<class t_returnType, class... t_parameters>
class SActionPtrBase {

public:
    virtual t_returnType Invoke(t_parameters...) {
        throw this;
    }

    virtual bool CanInvoke() {
        return false;
    }

    virtual SActionPtrBase<t_returnType, t_parameters...>* GetPointer() {
        return nullptr;
    }

    /*virtual bool Equals(SActionPtrBase<t_returnType, t_parameters...> x) = 0;

    virtual bool operator == (SActionPtrBase<t_returnType, t_parameters...> x) {
        return Equals(x);
    }*/
};


template<class t_classType, class t_returnType, class... t_parameters>
class SMethodPtr : SActionPtrBase<t_returnType, t_parameters...> {


public:

    typedef t_returnType(t_classType::* MethodType)(t_parameters...);

    SMethodPtr() {

    }

    SMethodPtr(t_classType* p_object, MethodType p_method) {
        Bind(p_object, p_method);
    }

    void Bind(t_classType* p_object, MethodType p_method) {
        m_object = p_object;
        m_method = p_method;
    }

    //////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    virtual t_returnType Invoke(t_parameters... x) override {
        if (!CanInvoke()) return t_returnType();

        return (m_object->*m_method)(x...);
    }

    virtual bool CanInvoke() override {
        return m_object != nullptr && m_method != nullptr;
    }

    virtual SActionPtrBase<t_returnType, t_parameters...>* GetPointer() override {
        return (SActionPtrBase<t_returnType, t_parameters...>*)this;
    }

    /*virtual bool Equals(SActionPtrBase<t_returnType, t_parameters...> x) override {
        auto l_X = (SMethodPtr<t_classType, t_returnType, t_parameters...>)x;
        return l_X.m_method == m_method && l_X.m_object == m_object;
    }*/

protected:

    t_classType* m_object = nullptr;

    MethodType m_method = nullptr;

};

template<class t_returnType, class... t_parameters>
class SActionPtr : SActionPtrBase<t_returnType, t_parameters...> {

public:

    typedef t_returnType(*ActionType)(t_parameters...);

    SActionPtr() {

    }

    SActionPtr(ActionType p_method) {
        m_action = p_method;
    }

    virtual t_returnType Invoke(t_parameters... x) {
        if (!CanInvoke()) return t_returnType();

        return m_action(x...);
    }

    virtual bool CanInvoke() override {
        return m_action != nullptr;
    }

    /* virtual bool Equals(SActionPtrBase<t_returnType, t_parameters...> x) override {
         auto l_X = (SActionPtr<t_returnType, t_parameters...>)x;
         return l_X.m_action == m_action;
     }*/

protected:

    ActionType m_action = nullptr;

};
