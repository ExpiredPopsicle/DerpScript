// ---------------------------------------------------------------------------
//
//   Lily Engine alpha
//
//   Copyright (c) 2012 Clifford Jolly
//     http://expiredpopsicle.com
//     expiredpopsicle@gmail.com
//
// ---------------------------------------------------------------------------
//
//   This software is provided 'as-is', without any express or implied
//   warranty. In no event will the authors be held liable for any
//   damages arising from the use of this software.
//
//   Permission is granted to anyone to use this software for any
//   purpose, including commercial applications, and to alter it and
//   redistribute it freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must
//      not claim that you wrote the original software. If you use this
//      software in a product, an acknowledgment in the product
//      documentation would be appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must
//      not be misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//      distribution.
//
// -------------------------- END HEADER -------------------------------------

#pragma once

#if __cplusplus > 199711L
#include <unordered_map>
#endif
#include <map>
#include <string>

#include "derpobject.h"
#include "pooledstring.h"

namespace ExPop {

    class DerpVM;

    /// DerpContext represents the current scope and is where the entire
    /// VM-side concept of variables lives.
    class DerpContext {
    public:

        DerpContext(DerpVM *vm);
        DerpContext(DerpContext *parent);

        /// Set (and create if needed) a variable to some DerpObject.
        void setVariable(const PooledString::Ref &name, DerpObject::Ref data);

        /// Clear out a variable. Doesn't clear protected status.
        void unsetVariable(const PooledString::Ref &name);

        /// Get a variable by name. Will recurse if necessary to the
        /// parent context.
        DerpObject::Ref getVariable(const PooledString::Ref &name);

        /// Set a variable to protected (or unprotected). Protected means
        /// that the reference cannot be reassigned. NOTE: Protected
        /// variables can be modified. They just can't be pointed to
        /// anything else. Use with const stuff to make them unmodifyable
        /// by scripts.
        void setVariableProtected(const PooledString::Ref &name, bool refProtected);

        /// Get protected flag for a variable.
        bool getVariableProtected(const PooledString::Ref &name);

        /// std::string wrappers for all the stuff above that uses
        /// std::string.
        void setVariable(const std::string &name, DerpObject::Ref data);
        void unsetVariable(const std::string &name);
        DerpObject::Ref getVariable(const std::string &name);
        void setVariableProtected(const std::string &name, bool refProtected);
        bool getVariableProtected(const std::string &name);

        /// Clear all variables and protected status.
        void clearAllVariables(void);

        /// Get all variable names accessible from this context.
        void getVariableNames(std::vector<std::string> &out);

    private:

        // Get the pointer to a variable reference. This can be used to
        // reassign the reference to something else later. NOTE: Only
        // guaranteed to be valid immediately after the call to this
        // function!
        DerpObject::Ref *getVariablePtr(
            const PooledString::Ref &name,
            bool noRecurse = false);

        friend class DerpObject;    // For getVariablePtr().
        friend class DerpExecNode;  // For getVariablePtr().

        DerpContext *parent;

        // Some day, we should make this a hash table and speed up
        // variable lookup by a lot. (Variable lookup is the second
        // bottleneck after GC performance!) Also, make the key type
        // PooledString!

      #if __cplusplus > 199711L && DERPSCRIPT_USE_UNORDERED_MAP
        typedef std::unordered_map<PooledString::Ref, DerpObject::Ref> MapType_DerpObjectRef;
        typedef std::unordered_map<PooledString::Ref, bool> MapType_bool;
      #else
        typedef std::map<PooledString::Ref, DerpObject::Ref> MapType_DerpObjectRef;
        typedef std::map<PooledString::Ref, bool> MapType_bool;
      #endif

        MapType_DerpObjectRef variables;
        MapType_bool variablesProtected;

        DerpVM *vm;

    };
}

