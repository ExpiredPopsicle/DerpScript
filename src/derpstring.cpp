

namespace ExPop
{

    bool strStartsWith(const std::string &needle, const std::string &haystack) {
        if(haystack.size() < needle.size()) return false;
        return strncmp(needle.c_str(), haystack.c_str(), needle.size()) == 0;
    }

    bool strEndsWith(const std::string &needle, const std::string &haystack) {
        if(haystack.size() < needle.size()) return false;
        return strncmp(needle.c_str(), haystack.c_str() + (haystack.size() - needle.size()), needle.size()) == 0;
    }

    bool isWhiteSpace(char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    void stringTokenize(const std::string &str, const std::string &delims, std::vector<std::string> &tokens, bool allowEmpty) {

        unsigned int i = 0;
        while(i < str.size()) {

            bool firstDelim = true;

            // Skip over delimeters.
            while(i < str.size() && strchr(delims.c_str(), str[i])) {
                i++;

                if(!firstDelim && allowEmpty) {
                    tokens.push_back("");
                }

                firstDelim = false;
            }

            // Mark the start of a token.
            int tokStart = i;

            // Skip to the next delimeter.
            while(i < str.size() && !strchr(delims.c_str(), str[i])) {
                i++;
            }

            // Mark the end.
            int tokEnd = i;

            // Copy the token.
            if(tokStart != tokEnd) {
                string token = str.substr(tokStart, (tokEnd - tokStart));
                tokens.push_back(token);
            }
        }
    }

    std::string stringEscape(const std::string &str, const std::string &newLineReplace) {

        ostringstream outStr;

        for(unsigned int i = 0; i < str.size(); i++) {

            switch(str[i]) {

                case '"':
                    outStr << "\\\"";
                    break;

                case '\\':
                    outStr << "\\\\";
                    break;

                case '\n':
                    outStr << newLineReplace;
                    break;

                case '\r':
                    outStr << "\\r";
                    break;

                default:
                    outStr << str[i];
                    break;
            }

        }

        return outStr.str();

    }

    std::string stringUnescape(const std::string &str) {

        ostringstream outStr;

        for(unsigned int i = 0; i < str.size(); i++) {

            if(str[i] == '\\') {

                i++;

                if(i >= str.size()) break;

                switch(str[i]) {

                    case '\\':
                        outStr << "\\";
                        break;

                    case '"':
                        outStr << "\"";
                        break;

                    case 'n':
                        outStr << "\n";
                        break;

                    case 'r':
                        outStr << "\r";
                        break;

                    default:
                        // What the heck is this?
                        outStr << str[i];
                        break;

                }

            } else {

                outStr << str[i];

            }
        }

        return outStr.str();
    }

};
