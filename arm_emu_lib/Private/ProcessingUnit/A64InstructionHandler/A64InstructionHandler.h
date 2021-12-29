#if !defined(A64INSTRUCTIONHANDLER_H_INCLUDED_DE9F73EC_96C1_4A8F_861E_098819177880)
    #define A64INSTRUCTIONHANDLER_H_INCLUDED_DE9F73EC_96C1_4A8F_861E_098819177880

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <Memory/IMemory.h>
    #include <ProcessingUnit/A64InstructionManager/A64InstructionManager.h>
    #include <Utility/UniqueRef.h>

BEGIN_NAMESPACE

class [[nodiscard]] A64InstructionHandler {
  public:
    A64InstructionHandler(Object* debugObject, IMemory* instructionMemory, IMemory* upStreamMemory);

    A64InstructionHandler(A64InstructionHandler&&);
    A64InstructionHandler& operator=(A64InstructionHandler&&);

    ~A64InstructionHandler();

  private:
    class Impl;
    UniqueRef< Impl > m_instructionHandler;

    template < class ImplDetail >
    [[nodiscard]] static UniqueRef< Impl > ConstructInstructionHandler(IMemory* instructionMemory,
                                                                       IMemory* upStreamMemory, ImplDetail detail);
};

END_NAMESPACE

#endif // !defined(A64INSTRUCTIONHANDLER_H_INCLUDED_DE9F73EC_96C1_4A8F_861E_098819177880)
