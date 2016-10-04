/*******************************************************************************
* All content ?2015 DigiPen (USA) Corporation, all rights reserved.
* Reproduction or disclosure of this file or its contents without the prior
* written consent of DigiPen Institute of Technology is prohibited.
*
* File: EditorInspectorWindow.h
* Author: Jiyun Kang
* Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
******************************************************************************/
#pragma once
#include <string>
//#include<stack>
#include<list>

#include "Base\util\math\Float3.h"
#include "Physics\util\Geometry.h"
#include "Core\components\transform\CTransform.h"
#include "Base\modules\introspection\variable\Variable.h"
#include "Editor\InGameEditor\Types.h"
//namespace Editor

    class Command
    {
    public:
        virtual void Execute(void) {};
        Names name;
    };

    class UndoableCommand : public Command
    {
    public:
        virtual void Undo(void){};
        void* comp_data;
    };

    class CommandManager
    {
    private:
        std::list<UndoableCommand* > commandStack;

    public:
        void Undo(void);
        std::list<UndoableCommand* >& GetcommandStack(void);
        void ClearCommandList(void);
        void SetNames(std::string obj, std::string mem);

    };

    //////////////////////////////////////////////////////////////////////////

    class IntCommand : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        IntCommand(void);
        IntCommand(int predata, Base::Variable var);
        ~IntCommand(void);

        int m_predata;
        Base::Variable m_var;
        
    };

    class FloatCommand : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        FloatCommand(void);
        FloatCommand(float predata, Base::Variable var);
        ~FloatCommand(void);

        float m_predata;
        Base::Variable m_var;
    };

    class BoolCommand : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        BoolCommand(void);
        BoolCommand(bool predata, Base::Variable var);
        ~BoolCommand(void);

        bool m_predata;
        Base::Variable m_var;
    };
    class StringCommand :public  UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        StringCommand(void);
        StringCommand(std::string predata, Base::Variable var);
        ~StringCommand(void);


        std::string m_predata;
        Base::Variable m_var;
    };
    class Float3Command : public  UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        Float3Command(void);
        Float3Command(float* predata, Base::Variable var);
        ~Float3Command(void);


        float m_predata[3];
        Base::Variable m_var;
    };
    class Float4Command : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        Float4Command(void);
        Float4Command(float4 predata, Base::Variable var);
        ~Float4Command(void);


        float4 m_predata;
        Base::Variable m_var;
    };
    class TransformCommand : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        TransformCommand(void);
        TransformCommand(Core::Position predata, Base::Variable var, Core::GameObject* obj);
        ~TransformCommand(void);

        Core::Position m_predata;
        Base::Variable m_var;
        Core::GameObject *m_Object;
    };
    class BoxCommand : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        BoxCommand(void);
        BoxCommand(Physics::Box predata, Base::Variable var);
        ~BoxCommand(void);

        Physics::Box m_predata;
        Base::Variable m_var;
    };
    class SphereCommand : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        SphereCommand(void);
        SphereCommand(Physics::Sphere predata, Base::Variable var);
        ~SphereCommand(void);

        Physics::Sphere m_predata;
        Base::Variable m_var;
    };
    class CapsuleCommand : public UndoableCommand
    {
    public:
        void Execute(void) override;
        void Undo(void) override;

        CapsuleCommand(void);
        CapsuleCommand(Physics::Capsule predata, Base::Variable var);
        ~CapsuleCommand(void);

        Physics::Capsule m_predata;
        Base::Variable m_var;
    };

