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

#include "derpparser_internal.h"
using namespace std;

namespace DerpScript {

    DerpExecNode *derpParser_parseValue(
        DerpVM *vm, std::vector<DerpToken*> &tokens,
        unsigned int &i, DerpErrorState &errorState,
        bool &innerError) {

        if(!TOKENS_LEFT()) return NULL;

        // Check for negative sign.
        bool isNegative = false;
        if(TOKEN_TYPE() == DERPTOKEN_MATHOP && tokens[i]->str == "-") {
            isNegative = true;
            i++;
            if(!TOKENS_LEFT()) return NULL;
        }

        DerpExecNode *ret = NULL;
        istringstream inStr(tokens[i]->str);
        innerError = false;

        DerpObject *literalData = NULL;

        switch(TOKEN_TYPE()) {

            case DERPTOKEN_INT: {

                // Literal integer.
                int intVal;
                inStr >> intVal;
                literalData = vm->makeRawObject();
                literalData->setInt(isNegative ? -intVal : intVal);

            } break;

            case DERPTOKEN_FLOAT: {

                // Literal float.
                float floatVal;
                inStr >> floatVal;
                literalData = vm->makeRawObject();
                literalData->setFloat(isNegative ? -floatVal : floatVal);

            } break;

            case DERPTOKEN_STRING: {

                // Literal string.
                literalData = vm->makeRawObject();
                literalData->setString(tokens[i]->str);

            } break;

            case DERPTOKEN_SYMBOL: {

                // Variable name.
                ret = new DerpExecNode(
                    vm,
                    derpSafeLineNumber(tokens, i),
                    derpSafeFileName(tokens, i));

                ret->setType(DERPEXEC_VARLOOKUP);

                ret->setVariableName(
                    vm->getVariablenameRef(tokens[i]->str));

                i++;

            } break;

            case DERPTOKEN_KEYWORD_FUNCTION: {

                ret = parseFunction(vm, tokens, i, errorState);
                innerError = true;

            } break;

            case DERPTOKEN_KEYWORD_VAR: {

                ret = parseVardec(vm, tokens, i, errorState);
                innerError = true;

            } break;

            default:
                // Failed to parse as value. Might just be a prefix
                // operator, though. Calling function should handle those
                // errors.
                return NULL;
                break;
        }

        if(!ret) {

            // If we've made it here, we need to construct a new
            // DerpExecNode for the literal value. And we MUST have
            // decided what the literalData is.

            // FIXME: I don't know how this comes to be, but it looks
            // like if you leave end parenthesis off a complicated
            // return expression it could show up:
            // For Example: return imageOp.swizzle(imageOp.gaussianBlur(blackMask, 4.0/float(size), 0, 0, 0)
            if(!literalData) {
                errorState.addError("Internal parser error.");
                return NULL;
            }

            assert(literalData);

            ret = new DerpExecNode(
                vm,
                derpSafeLineNumber(tokens, i),
                derpSafeFileName(tokens, i));

            ret->setType(DERPEXEC_LITERAL);
            ret->setData(literalData);
            literalData->setConst(true);
            i++;
        }

        return ret;
    }
}

