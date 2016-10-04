/*******************************************************************************
* All content ?2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: InGameEditor.cpp
* Author: Jiyun Kang
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#include "pch/precompiled.h"
#include "Editor\InGameEditor\InspecCommand.h"

#include "Core\components\transform\CTransform.h"
#include "Core\types\object\Object.h"
//namespace Editor


    void CommandManager::Undo(void)
    {
        if (commandStack.size() > 0)
        {
            commandStack.back()->Undo();
            commandStack.pop_back();
        }
    }
    std::list<UndoableCommand* >& CommandManager::GetcommandStack(void)
    {
        return commandStack;
    }
    void CommandManager::ClearCommandList(void)
    {
        commandStack.clear();
    }

    void CommandManager::SetNames(std::string obj, std::string mem)
    {
        commandStack.back()->name = Names(obj, mem);
    }
///////////////////////////////////////////////////////////////////////////////
    void IntCommand::Execute(void)
    {
        
    }
    void IntCommand::Undo(void)
    {
        void* data = m_var.GetData();
        int* vec = reinterpret_cast<int*>(data);
        *vec = m_predata;
    }
    IntCommand::IntCommand(void)
    {

    }
    IntCommand::IntCommand(int predata, Base::Variable var)
    {
        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
    }
    IntCommand::~IntCommand(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void FloatCommand::Execute(void)
    {

    }
    void FloatCommand::Undo(void)
    {
        void* data = m_var.GetData();
        float* vec = reinterpret_cast<float*>(data);
        *vec = m_predata;
    }
    FloatCommand::FloatCommand(void)
    {

    }
    FloatCommand::FloatCommand(float predata, Base::Variable var)
    {
        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
    }
    FloatCommand::~FloatCommand(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void BoolCommand::Execute(void)
    {

    }
    void BoolCommand::Undo(void)
    {
        void* data = m_var.GetData();
        bool* vec = reinterpret_cast<bool*>(data);
        *vec = m_predata;
    }
    BoolCommand::BoolCommand(void)
    {
        
    }
    BoolCommand::BoolCommand(bool predata, Base::Variable var)
    {
        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
    }
    BoolCommand::~BoolCommand(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void StringCommand::Execute(void)
    {

    }
    void StringCommand::Undo(void)
    {
        void* data = m_var.GetData();
        std::string* vec = reinterpret_cast<std::string*>(data);
        *vec = m_predata;
    }
    StringCommand::StringCommand(void)
    {

    }
    StringCommand::StringCommand(std::string predata, Base::Variable var)
    {
        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;

    }
    StringCommand::~StringCommand(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void Float3Command::Execute(void)
    {

    }
    void Float3Command::Undo(void)
    {
        void* data = m_var.GetData();
        float* vec = reinterpret_cast<float*>(data);
        vec = m_predata;
    }
    Float3Command::Float3Command(void)
    {

    }
    Float3Command::Float3Command(float* predata, Base::Variable var)
    {
        m_predata[0] = predata[0];
        m_predata[1] = predata[1];
        m_predata[2] = predata[2];
        m_var = var;
        comp_data = &m_predata;

    }
    Float3Command::~Float3Command(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void Float4Command::Execute(void)
    {

    }
    void Float4Command::Undo(void)
    {
        void* data = m_var.GetData();
        float4* vec = reinterpret_cast<float4*>(data);
        *vec = m_predata;
    }
    Float4Command::Float4Command(void)
    {

    }
    Float4Command::Float4Command(float4 predata, Base::Variable var)
    {
        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
    }
    Float4Command::~Float4Command(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void TransformCommand::Execute(void)
    {

    }
    void TransformCommand::Undo(void)
    {
        void* data = m_var.GetData();
        Core::Position* vec = reinterpret_cast<Core::Position*>(data);
        *vec = m_predata;

        if (m_Object &&  m_Object->space)
        {
            Core::Transform* transform = dynamic_cast<Core::Transform*>(m_Object->GetComponent<Core::Transform>(eTransform));
            transform->SetTranslation(m_predata.translation);
            transform->SetRotation(m_predata.rotation);
            transform->SetScale(m_predata.scale);
        }
       
    }
    TransformCommand::TransformCommand(void)
    {

    }
    TransformCommand::TransformCommand(Core::Position predata, Base::Variable var, Core::GameObject* obj)
    {
        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
        m_Object = obj;
    }
    TransformCommand::~TransformCommand(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void BoxCommand::Execute(void)
    {

    }
    void BoxCommand::Undo(void)
    {
        void* data = m_var.GetData();
        Physics::Box* vec = reinterpret_cast<Physics::Box*>(data);
        *vec = m_predata;
    }
    BoxCommand::BoxCommand(void)
    {

    }
    BoxCommand::BoxCommand(Physics::Box predata, Base::Variable var)
    {
        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
    }
    BoxCommand::~BoxCommand(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void SphereCommand::Execute(void)
    {

    }
    void SphereCommand::Undo(void)
    {
        void* data = m_var.GetData();
        Physics::Sphere* vec = reinterpret_cast<Physics::Sphere*>(data);
        *vec = m_predata;
    }
    SphereCommand::SphereCommand(void)
    {

    }
    SphereCommand::SphereCommand(Physics::Sphere predata, Base::Variable var)
    {

        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
    }
    SphereCommand::~SphereCommand(void)
    {

    }
    ///////////////////////////////////////////////////////////////////
    void CapsuleCommand::Execute(void)
    {

    }
    void CapsuleCommand::Undo(void)
    {
        void* data = m_var.GetData();
        Physics::Capsule* vec = reinterpret_cast<Physics::Capsule*>(data);
        *vec = m_predata;
    }
    CapsuleCommand::CapsuleCommand(void)
    {

    }
    CapsuleCommand::CapsuleCommand(Physics::Capsule predata, Base::Variable var)
    {

        m_predata = predata;
        m_var = var;
        comp_data = &m_predata;
    }
    CapsuleCommand::~CapsuleCommand(void)
    {

    }
