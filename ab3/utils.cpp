#include "utils.hpp"

std::vector<std::string> tokenize(std::string str, std::string delimiters) {
    std::vector<std::string> tokens;
    size_t lastPosition = str.find_first_not_of(delimiters, 0);
    size_t position = str.find_first_of(delimiters, lastPosition);

    while (std::string::npos != position || std::string::npos != lastPosition) {
        tokens.push_back(str.substr(lastPosition, position - lastPosition));
        lastPosition = str.find_first_not_of(delimiters, position);
        position = str.find_first_of(delimiters, lastPosition);
    }

    return tokens;
}

std::vector<std::pair<std::string, std::string> > parse_qs(char* raw_query_string) {
    std::vector<std::pair<std::string, std::string> > parsed;
    if (!raw_query_string) {
        return parsed;
    }
    std::string query_string = raw_query_string;
    std::vector<std::string> params = tokenize(query_string, "&");
    std::vector<std::string> parts;
    std::string name;
    std::string value;
    for (size_t i = 0; i < params.size(); i++) {
        parts = tokenize(params.at(i), "=");
        name = parts.at(0);
        if (parts.size() > 1) {
            value = parts.at(1);
        } else {
            value = "";
        }
        parsed.push_back(std::make_pair(name, value));
    }
    return parsed;
}

std::string get_param_value(std::vector<std::pair<std::string, std::string> > params, std::string name, std::string default_value) {
    std::pair<std::string, std::string> param;
    for (size_t i = 0; i < params.size(); i++) {
        param = params.at(i);
        if (param.first == name) {
            return param.second;
        }
    }
    return default_value;
}
