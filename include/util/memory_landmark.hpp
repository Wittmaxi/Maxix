#pragma once
#include <stddef.h>

namespace MEM {
    struct landmark {
        landmark* create_landmark_after(size_t distance, landmark* pp) {
            landmark* created = reinterpret_cast<landmark*> (this + distance);
            created->next = next;
            created->parent_pointer = pp;
            next = created;
            return created;
        }
        landmark* parent_pointer;
        landmark* next;
    }__attribute__((packed));

    struct landmark_iterator {
        landmark_iterator(landmark* lm) : position(lm) 
        {}

        landmark_iterator operator++() {
            position = position->next;
        }

        landmark_iterator operator++(int garbage) {
            landmark_iterator keep(position);
            ++(*this);
            return keep; 
        }

        landmark* operator* () {
            return position;
        }

        bool has_enough_space_after(size_t space) {
            if (space <= reinterpret_cast<int> (position->next) - reinterpret_cast<int> (position))
                return true;
            return false;
        }

        bool operator== (const landmark_iterator& other) {
            return other.position == position;
        }

        bool operator!= (const landmark_iterator& other) {
            return !((*this) != other);
        }

        private:
            landmark* position;
    };

    template<class T>
    landmark* createLandmark (T at, T next, T parent) {
        landmark* landmark_ptr = reinterpret_cast<landmark*> (at);
        landmark_ptr->next = reinterpret_cast<landmark*> (next);
        landmark_ptr->parent_pointer = reinterpret_cast<landmark*> (parent);
        return landmark_ptr;
    }
}