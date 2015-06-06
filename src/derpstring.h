#pragma once

namespace DerpScript
{
    /// Return true if the character is whitespace. False otherwise.
    bool isWhiteSpace(char c);

    /// Simple function to see if a string starts with another string.
    /// (needle is the string we're looking for, haystack is the
    /// string we're looking in).
    bool strStartsWith(const std::string &needle, const std::string &haystack);

    /// Return true if one string ends with another string. (needle is
    /// the string we're looking for, haystack is the string we're
    /// looking in).
    bool strEndsWith(const std::string &needle, const std::string &haystack);

    /// Generate tokens from a string. Stores saved tokens into the
    /// passed-in tokens parameter.
    void stringTokenize(
        const std::string &str,
        const std::string &delims,
        std::vector<std::string> &tokens,
        bool allowEmpty = false);

    // TODO: Advanced string tokenization, escape/unescape, etc.

    /// Escape a string. newLineReplace will be used to replace new
    /// lines.
    std::string stringEscape(const std::string &str, const std::string &newLineReplace = "\\n");

    /// Unescape a string.
    std::string stringUnescape(const std::string &str);

};
