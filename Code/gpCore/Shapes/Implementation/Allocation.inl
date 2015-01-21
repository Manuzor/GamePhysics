
namespace gpInternal
{
    /// \brief Prevents direct allocations of gpShape
    template<>
    struct gpTypeAllocator<gpShape>
    {
    };
}
