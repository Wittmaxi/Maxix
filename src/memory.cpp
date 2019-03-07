
#include <memory_management.hpp>

namespace MEM {
    landmark* entry;
    void initialize() {
        entry = createLandmark(0x00000500, 0x00007BFF, 0); 
        createLandmark(0x00000500, 0x00007E00, 0); 
        createLandmark(0x00007E00, 0x0007FFFF, 0); 
        createLandmark(0x0007FFFF, 0x0007FFFF, 0); 
    }

    void* allocate(size_t size, landmark* owner = nullptr) {
        landmark_iterator it(entry);
        while ((!it->has_enough_space_after(size)) || (it->used))
            it++;
        return it->create_landmark_after(size, owner);
    }

    void defragment (landmark_iterator before, landmark_iterator it) {
        if (it->used)
            return;
        if (!before->used) {
            before->next = it->next;
            it = before;
        }
        if (!it->next->used)
            it->next = it->next->next;
    }

    void deallocate(void* pointer) {
        landmark_iterator to_find(reinterpret_cast<landmark*> (pointer));
        landmark_iterator it(entry);
        landmark* before;
        while (it != to_find) {
            before = (*it);
            it++; 
        }
        it->used = false;
        defragment(before, it);
    }
}
