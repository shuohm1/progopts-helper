#ifndef PROGOPTS_HELPER_HPP
#define PROGOPTS_HELPER_HPP

#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

namespace progopts_helper {

namespace po = boost::program_options;

// a default argument name length
constexpr size_t ARGNAME_LENGTH = 24 - (2 + 1);
// default captions
constexpr char* POSARG_CAPTION = (char*) "positional arguments";
constexpr char* OPTARG_CAPTION = (char*) "optional arguments";

// make sure there are no `optional-style` positional arguments
template <typename charT>
void validate_parsedargs(
	const po::basic_parsed_options<charT>& parsed,
	const po::options_description& posarg_desc
) {
	for (const auto& parsed_opt: parsed.options) {
		if (parsed_opt.position_key < 0) {
			for (const auto opt: posarg_desc.options()) {
				if (parsed_opt.string_key == opt->long_name()) {
					throw po::unknown_option(parsed_opt.string_key);
				}
			}
		}
	}
}

// get a help message
std::string help_message(
	const po::options_description& posarg_desc,
	const po::options_description& optarg_desc
) {
	std::ostringstream sout;

	// get the maximum length of argument names/parameters
	size_t max_length = ARGNAME_LENGTH;
	for (const auto opt: posarg_desc.options()) {
		const size_t temp_length = opt->long_name().length();
		if (max_length < temp_length) {
			max_length = temp_length;
		}
	}
	for (const auto opt: optarg_desc.options()) {
		const size_t fname_len = opt->format_name().length();
		const size_t fparam_len = opt->format_parameter().length();
		const size_t temp_length =
			fname_len + fparam_len + ((fparam_len > 0) ? 1 : 0);
		if (max_length < temp_length) {
			max_length = temp_length;
		}
	}

	// construct a help message
	sout << POSARG_CAPTION << ":" << std::endl;
	for (const auto opt: posarg_desc.options()) {
		const auto name = opt->long_name();
		const size_t padlen = max_length - name.length();
		sout << "  " << name << std::string(padlen, ' ') << " ";
		sout << opt->description() << std::endl;
	}
	sout << optarg_desc;

	return sout.str();
}

} // end namespace progopts_helper

#endif
