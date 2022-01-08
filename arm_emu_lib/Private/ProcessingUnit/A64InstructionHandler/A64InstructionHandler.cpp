
#include <ProcessingUnit/A64InstructionHandler/A64InstructionHandler.h>

BEGIN_NAMESPACE

class A64InstructionHandler::Impl {
  public:
    Impl(Object* debugObject, IMemory* instructionMemory, IMemory* upStreamMemory) :
        m_debugObject(*debugObject), m_instructionMemory(instructionMemory), m_upStreamMemory(upStreamMemory) {
    }

    ~Impl() {
        m_debugObject.Log(LogType::Destruction, "Destroying A64InstructionHandler!");
    }

  private:
    IMemory* m_instructionMemory;
    IMemory* m_upStreamMemory;
    Object&  m_debugObject;
};

template < class ImplDetail >
[[nodiscard]] UniqueRef< A64InstructionHandler::Impl >
    A64InstructionHandler::ConstructInstructionHandler(IMemory* instructionMemory, IMemory* upStreamMemory,
                                                       ImplDetail myself) {
    myself->Log(LogType::Construction,
                "Constructing A64InstructionHandler, with upStreamMemory: {}, and instructionMemory: {}",
                static_cast< void* >(upStreamMemory), static_cast< void* >(instructionMemory));
    std::pmr::polymorphic_allocator< A64InstructionHandler::Impl > alloc {};

    return allocate_unique< A64InstructionHandler::Impl >(alloc, myself, instructionMemory, upStreamMemory);
}

A64InstructionHandler::A64InstructionHandler(Object* debugObject, IMemory* instructionMemory, IMemory* upStreamMemory) :
    m_instructionHandler(ConstructInstructionHandler(instructionMemory, upStreamMemory, debugObject)) {
}

A64InstructionHandler::A64InstructionHandler(A64InstructionHandler&&) = default;

A64InstructionHandler& A64InstructionHandler::operator=(A64InstructionHandler&&) = default;

A64InstructionHandler::~A64InstructionHandler() = default;

END_NAMESPACE
