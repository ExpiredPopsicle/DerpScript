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

#include "derpvm.h"
#include "derpcontext.h"

#include <cassert>
#include <iostream>

namespace DerpScript {

    DerpContext::DerpContext(DerpVM *vm)
      #if __cplusplus > 199711L && DERPSCRIPT_USE_UNORDERED_MAP
        :
        variables(10),
        variablesProtected(10)
      #endif
    {
        parent = NULL;
        this->vm = vm;
    }

    DerpContext::DerpContext(DerpContext *parent) {
        assert(parent);
        this->parent = parent;
        this->vm = parent->vm;
    }

    void DerpContext::setVariable(const PooledString::Ref &name, DerpObject::Ref data) {

        // TODO: Check total visible variable count to make sure we
        // don't go over some arbitrary limit.

        variables[name] = data;
    }

    DerpObject::Ref DerpContext::getVariable(const PooledString::Ref &name) {

        if(!variables.count(name)) {
            if(parent) {
                return parent->getVariable(name);
            } else {
                return NULL;
            }
        }

        return variables[name];
    }

    DerpObject::Ref *DerpContext::getVariablePtr(const PooledString::Ref &name, bool noRecurse) {

        if(getVariableProtected(name)) {
            return NULL;
        }

        if(!variables.count(name)) {
            if(!noRecurse && parent) {
                return parent->getVariablePtr(name);
            } else {
                return NULL;
            }
        }

        return &(variables[name]);
    }

    void DerpContext::unsetVariable(const PooledString::Ref &name) {
        variables.erase(name);
    }

    void DerpContext::setVariableProtected(const PooledString::Ref &name, bool refProtected) {
        if(refProtected) {
            variablesProtected[name] = true;
        } else {
            variablesProtected.erase(name);
        }
    }

    bool DerpContext::getVariableProtected(const PooledString::Ref &name) {
        return variablesProtected.count(name);
    }

    void DerpContext::clearAllVariables(void) {
        variables.clear();
        variablesProtected.clear();
    }

    void DerpContext::getVariableNames(std::vector<std::string> &out) {

        for(MapType_DerpObjectRef::iterator i = variables.begin();
            i != variables.end();
            i++) {

            std::string name = (*i).first;

            unsigned int j;
            for(j = 0; j < out.size(); j++) {
                if(out[j] == name) break;
            }

            if(j != out.size()) {
                out.push_back(name);
            }
        }

        if(parent) {
            parent->getVariableNames(out);
        }
    }

    void DerpContext::setVariable(const std::string &name, DerpObject::Ref data)
    {
        setVariable(vm->getVariablenameRef(name), data);
    }

    void DerpContext::unsetVariable(const std::string &name)
    {
        unsetVariable(vm->getVariablenameRef(name));
    }

    DerpObject::Ref DerpContext::getVariable(const std::string &name)
    {
        return getVariable(vm->getVariablenameRef(name));
    }

    void DerpContext::setVariableProtected(const std::string &name, bool refProtected)
    {
        setVariableProtected(vm->getVariablenameRef(name), refProtected);
    }

    bool DerpContext::getVariableProtected(const std::string &name)
    {
        return getVariableProtected(vm->getVariablenameRef(name));
    }

}

