
namespace gpInternal
{
    /// \brief Prevents direct allocations of gpShapeBase
    template<>
    struct gpTypeAllocator<gpShapeBase>
    {
    };
}
