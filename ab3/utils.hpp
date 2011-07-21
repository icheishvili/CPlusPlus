#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

std::vector<std::string> tokenize(std::string, std::string);
std::vector<std::pair<std::string, std::string> > parse_qs(char*);
std::string get_param_value(std::vector<std::pair<std::string, std::string> >, std::string, std::string);

#endif
